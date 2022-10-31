#ifndef __REG_H__
#define __REG_H__

#include "verilated_dpi.h"

#define GPR_NR 32

extern "C" void set_gpr_ptr(const svOpenArrayHandle r);
extern const char *regs[GPR_NR];

extern "C" void set_pc(const svOpenArrayHandle r) { // TODO: we are here
  cpu_gpr = (uint64_t *)(((VerilatedDpiOpenVar*)r)->datap());
}
// 输出RTL中通用寄存器的值
void isa_reg_display();
extern uint64_t *cpu_gpr;

void cpu_gpr_to_cpu();

#endif
