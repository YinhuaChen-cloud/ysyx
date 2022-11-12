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

#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

#define GPR_NR 32

extern const char *regs[];

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	bool theSame = true;

	for(int i = 0; i < GPR_NR; i++)	{
		if(cpu.gpr[i] != ref_r->gpr[i]) {
			theSame = false;
			printf("------- regs (%s) differs, cpu.%s = 0x%lx, ref.%s = 0x%lx -------\n", regs[i], regs[i], cpu.gpr[i], regs[i], ref_r->gpr[i]);
		}
	}
	if(cpu.mstatus != ref_r->mstatus) { 
		theSame = false;
		printf("------- regs (%s) differs, cpu.%s = 0x%lx, ref.%s = 0x%lx -------\n", "mstatus", "mstatus", cpu.mstatus, "mstatus", ref_r->mstatus);
	} 
	if(cpu.pc != ref_r->pc) { 
		theSame = false;
		printf("------- pc differs, cpu.pc = 0x%lx, ref.pc = 0x%lx -------\n", cpu.pc, ref_r->pc);
	} 
//	if(cpu.gpr[15] % 2 == 1) {
//		printf("------- value in $a5 is odd: pc = 0x%lx, $a5 = 0x%lx -------\n", cpu.pc, cpu.gpr[15]);
	}
	
  return theSame;
}

void isa_difftest_attach() {
}
