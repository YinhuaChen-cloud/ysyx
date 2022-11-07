/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "local-include/reg.h"
#include <cpu/cpu.h>
#include <cpu/ifetch.h>
#include <cpu/decode.h>

enum {
	EVENT_NULL = 0,
	EVENT_YIELD, EVENT_SYSCALL, EVENT_PAGEFAULT, EVENT_ERROR, 
	EVENT_IRQ_TIMER, EVENT_IRQ_IODEV,
} event; // define events and its values

#define R(i) gpr(i)
#define CSR(i) (*(csr(i)))
#define Mr vaddr_read
#define Mw vaddr_write
#define shift_mask MUXDEF(CONFIG_ISA64, 0x3f, 0x1f)
#define sext32to64(a) ((int64_t)(int32_t)(a))

enum {
  TYPE_I, TYPE_U, TYPE_S, TYPE_R, TYPE_B, TYPE_J, TYPE_CR,
  TYPE_N, // none
};

#define src1R(n) do { *src1 = R(n); } while (0)
#define src2R(n) do { *src2 = R(n); } while (0)
#define destR(n) do { *dest = n; } while (0)
#define src1I(i) do { *src1 = i; } while (0)
#define src2I(i) do { *src2 = i; } while (0)
#define destI(i) do { *dest = i; } while (0)

static word_t immI(uint32_t i) { return SEXT(BITS(i, 31, 20), 12); }
static word_t immU(uint32_t i) { return SEXT(BITS(i, 31, 12), 20) << 12; }
static word_t immS(uint32_t i) { return (SEXT(BITS(i, 31, 25), 7) << 5) | BITS(i, 11, 7); }
static word_t immB(uint32_t i) { return (SEXT(BITS(i, 31, 31), 1) << 12) | \
	(BITS(i, 7, 7) << 11) | \
	(BITS(i, 30, 25) << 5) | \
	(BITS(i, 11, 8) << 1) ; }
static word_t immJ(uint32_t i) { return (SEXT(BITS(i, 31, 31), 1) << 20) | \
	(BITS(i, 19, 12) << 12) | \
	(BITS(i, 20, 20) << 11) | \
	(BITS(i, 30, 21) << 1) ; }

static void decode_operand(Decode *s, word_t *dest, word_t *src1, word_t *src2, word_t *csrid, int type) {
  uint32_t i = s->isa.inst.val;
  int rd  = BITS(i, 11, 7);
  int rs1 = BITS(i, 19, 15);
  int rs2 = BITS(i, 24, 20);
	*csrid = BITS(i, 31, 20);
  switch (type) {
		// NOTE: I add "destR(rd) in TYPE_I, and sth else at other original cases
    case TYPE_I: destR(rd); src1R(rs1); src2I(immI(i)); break;
    case TYPE_U: destR(rd); src1I(immU(i)); break;
    case TYPE_S: destI(immS(i)); src1R(rs1); src2R(rs2); break;
		case TYPE_R: destR(rd); src1R(rs1); src2R(rs2); break; 
		case TYPE_B: destI(immB(i)); src1R(rs1); src2R(rs2); break; 
		case TYPE_J: destR(rd); src1I(immJ(i)); break; 
		case TYPE_CR: destR(rd); src1R(rs1); break;
  }
}

#include "ftrace.h"

static int decode_exec(Decode *s) {
  word_t dest = 0, src1 = 0, src2 = 0, csrid = 0;
  s->dnpc = s->snpc;

#define INSTPAT_INST(s) ((s)->isa.inst.val)
#define INSTPAT_MATCH(s, name, type, ... /* body */ ) { \
  decode_operand(s, &dest, &src1, &src2, &csrid, concat(TYPE_, type)); \
  __VA_ARGS__ ; \
}
	// NOTE: no sra, srl
  INSTPAT_START();
	// Control status inst
	// 000000000000 00000 000 00000 1110011
  INSTPAT("0000000 00000 00000 000 00000 11100 11", ecall  , N, isa_raise_intr(EVENT_YIELD, s->snpc)); 
	// csr rs1 001 rd 1110011
  INSTPAT("??????? ????? ????? 001 ????? 11100 11", csrrw  , CR, R(dest) = CSR(csrid); CSR(csrid) = src1;); 
	// B
  INSTPAT("??????? ????? ????? 000 ????? 11000 11", beq    , B, s->dnpc = src1 == src2 ? s->pc + dest : s->snpc);
  INSTPAT("??????? ????? ????? 001 ????? 11000 11", bne	   , B, s->dnpc = src1 != src2 ? s->pc + dest : s->snpc); 
  INSTPAT("??????? ????? ????? 110 ????? 11000 11", bltu   , B, s->dnpc = src1 < src2 ? s->pc + dest : s->snpc);
  INSTPAT("??????? ????? ????? 101 ????? 11000 11", bge    , B, s->dnpc = (sword_t)src1 >= (sword_t)src2 ? s->pc + dest : s->snpc);
  INSTPAT("??????? ????? ????? 111 ????? 11000 11", bgeu   , B, s->dnpc = src1 >= src2 ? s->pc + dest : s->snpc);
  INSTPAT("??????? ????? ????? 100 ????? 11000 11", blt    , B, s->dnpc = (sword_t)src1 < (sword_t)src2 ? s->pc + dest : s->snpc);
	// I
  INSTPAT("??????? ????? ????? 100 ????? 00100 11", xori   , I, R(dest) = src1 ^ src2;);
  INSTPAT("??????? ????? ????? 011 ????? 00100 11", sltiu  , I, R(dest) = src1 < src2;);
  INSTPAT("000000? ????? ????? 001 ????? 00100 11", slli   , I, R(dest) = ((src1) << ((src2) & shift_mask)));
  INSTPAT("000000? ????? ????? 101 ????? 00100 11", srli   , I, R(dest) = ((src1) >> ((src2) & shift_mask)));
  INSTPAT("010000? ????? ????? 101 ????? 00100 11", srai   , I, R(dest) = ((sword_t)(src1) >> ((src2) & shift_mask)));
  INSTPAT("??????? ????? ????? 111 ????? 00100 11", andi   , I, R(dest) = src1 & src2;);
  INSTPAT("??????? ????? ????? 110 ????? 00100 11", ori    , I, R(dest) = src1 | src2;);
  INSTPAT("??????? ????? ????? 000 ????? 00110 11", addiw  , I, R(dest) = sext32to64((src1) + (src2)));
  INSTPAT("0000000 ????? ????? 001 ????? 00110 11", slliw  , I, R(dest) = sext32to64((uint32_t)(src1) << ((src2) & 0x1f)));
  INSTPAT("0000000 ????? ????? 101 ????? 00110 11", srliw  , I, R(dest) = sext32to64((uint32_t)(src1) >> ((src2) & 0x1f)));
  INSTPAT("0100000 ????? ????? 101 ????? 00110 11", sraiw  , I, R(dest) = sext32to64((int32_t)(src1) >> ((src2) & 0x1f)));
  INSTPAT("??????? ????? ????? 010 ????? 00000 11", lw     , I, R(dest) = (sword_t)(int32_t)Mr(src1 + src2, 4));
  INSTPAT("??????? ????? ????? 110 ????? 00000 11", lwu    , I, R(dest) = Mr(src1 + src2, 4));
  INSTPAT("??????? ????? ????? 001 ????? 00000 11", lh     , I, R(dest) = (sword_t)(int16_t)Mr(src1 + src2, 2));
  INSTPAT("??????? ????? ????? 101 ????? 00000 11", lhu    , I, R(dest) = Mr(src1 + src2, 2));
  INSTPAT("??????? ????? ????? 000 ????? 00000 11", lb     , I, R(dest) = (sword_t)(int8_t)Mr(src1 + src2, 1));
  INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu    , I, R(dest) = Mr(src1 + src2, 1));
	// R
  INSTPAT("0000000 ????? ????? 000 ????? 01110 11", addw   , R, R(dest) = sext32to64((src1) + (src2)));
  INSTPAT("0100000 ????? ????? 000 ????? 01110 11", subw   , R, R(dest) = sext32to64((src1) - (src2)));
  INSTPAT("0000001 ????? ????? 000 ????? 01110 11", mulw   , R, R(dest) = sext32to64((src1) * (src2)));
  INSTPAT("0000001 ????? ????? 100 ????? 01110 11", divw   , R, R(dest) = sext32to64((int32_t)(src1) / ( int32_t)(src2)));
  INSTPAT("0000001 ????? ????? 101 ????? 01110 11", divuw  , R, R(dest) = sext32to64((uint32_t)(src1) / (uint32_t)(src2)));
  INSTPAT("0000000 ????? ????? 001 ????? 01110 11", sllw   , R, R(dest) = sext32to64((uint32_t)(src1) << ((src2) & 0x1f)));
  INSTPAT("0000000 ????? ????? 101 ????? 01110 11", srlw   , R, R(dest) = sext32to64((uint32_t)(src1) >> ((src2) & 0x1f)));
  INSTPAT("0100000 ????? ????? 101 ????? 01110 11", sraw   , R, R(dest) = sext32to64((int32_t)(src1) >> ((src2) & 0x1f)));
  INSTPAT("0000001 ????? ????? 110 ????? 01110 11", remw   , R, R(dest) = sext32to64((int32_t)(src1) % ( int32_t)(src2)));
  INSTPAT("0000001 ????? ????? 111 ????? 01110 11", remuw  , R, R(dest) = sext32to64((uint32_t)(src1) % (uint32_t)(src2)));
  INSTPAT("0100000 ????? ????? 000 ????? 01100 11", sub    , R, R(dest) = src1 - src2;);
  INSTPAT("0000000 ????? ????? 110 ????? 01100 11", or     , R, R(dest) = src1 | src2;);
  INSTPAT("0000000 ????? ????? 000 ????? 01100 11", add    , R, R(dest) = src1 + src2;);
  INSTPAT("0000001 ????? ????? 000 ????? 01100 11", mul    , R, R(dest) = src1 * src2;);
  INSTPAT("0000000 ????? ????? 100 ????? 01100 11", xor    , R, R(dest) = src1 ^ src2;);
  INSTPAT("0000000 ????? ????? 001 ????? 01100 11", sll    , R, R(dest) = src1 << (src2 & shift_mask));
  INSTPAT("0000000 ????? ????? 010 ????? 01100 11", slt    , R, R(dest) = (sword_t)src1 < (sword_t)src2;);
  INSTPAT("0000000 ????? ????? 011 ????? 01100 11", sltu   , R, R(dest) = src1 < src2;);
  INSTPAT("0000000 ????? ????? 111 ????? 01100 11", and    , R, R(dest) = src1 & src2;);
  INSTPAT("0000001 ????? ????? 100 ????? 01100 11", div    , R, R(dest) = (sword_t)src1 / (sword_t)src2;);
  INSTPAT("0000001 ????? ????? 101 ????? 01100 11", divu   , R, R(dest) = src1 / src2;);
  INSTPAT("0000001 ????? ????? 110 ????? 01100 11", rem    , R, R(dest) = (sword_t)(src1) % (sword_t)(src2));
  INSTPAT("0000001 ????? ????? 111 ????? 01100 11", remu   , R, R(dest) = src1 % src2;);
	// S
  INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw     , S, Mw(src1 + dest, 4, src2));
  INSTPAT("??????? ????? ????? 001 ????? 01000 11", sh     , S, Mw(src1 + dest, 2, src2));
  INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb     , S, Mw(src1 + dest, 1, src2));
	// jmp
  INSTPAT("??????? ????? ????? 000 ????? 11001 11", jalr   , I, jalr_func(s, dest, src1, src2));
  INSTPAT("??????? ????? ????? ??? ????? 11011 11", jal    , J, jal_func(s, dest, src1));

  INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi	 , I, R(dest) = src1 + src2);
  INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc  , U, R(dest) = src1 + s->pc);
  INSTPAT("??????? ????? ????? ??? ????? 01101 11", lui    , U, R(dest) = src1);
  INSTPAT("??????? ????? ????? 011 ????? 00000 11", ld     , I, R(dest) = Mr(src1 + src2, 8));
  INSTPAT("??????? ????? ????? 011 ????? 01000 11", sd     , S, Mw(src1 + dest, 8, src2));

  INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak , N, NEMUTRAP(s->pc, R(10))); // R(10) is $a0
  INSTPAT("??????? ????? ????? ??? ????? ????? ??", inv    , N, INV(s->pc));
  INSTPAT_END();

  R(0) = 0; // reset $zero to 0

  return 0;
}

int isa_exec_once(Decode *s) {
  s->isa.inst.val = inst_fetch(&s->snpc, 4);
  return decode_exec(s);
}
