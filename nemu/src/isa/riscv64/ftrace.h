#ifndef FTRACE_H
#define FTRACE_H

#ifdef CONFIG_FTRACE

#include<elf.h>
#include<stdbool.h>

#define FTRACE_BUF_LEN 160

extern char *elf_content; // the elf content
extern FILE *ftrace_log; // used to write ftrace logs

char ftrace_buf[FTRACE_BUF_LEN];
int ftrace_indent_space;

// symtab
// symtab_size
// strtab
Elf64_Sym *symtab;
Elf64_Xword symtab_size;
char *strtab;

void get_symtab_strtab(){
	// get strtab
	Elf64_Ehdr *elfheader = (Elf64_Ehdr *)elf_content; 
	//printf("elfheader->e_shoff = %ld\n", elfheader->e_shoff);
	Elf64_Shdr *section_headers = (Elf64_Shdr *)(elf_content + elfheader->e_shoff);
	Elf64_Shdr *strtab_sh = section_headers + elfheader->e_shstrndx - 1;
	strtab = elf_content + strtab_sh->sh_addr + strtab_sh->sh_offset;
	// get symtab	and symtab_size
	Elf64_Shdr *p = section_headers;
	for(int i = 0; i < elfheader->e_shnum; i++){
		if(p->sh_type == SHT_SYMTAB)
			break;
		p++;	
	}

	symtab_size = p->sh_size;
	symtab = (Elf64_Sym *)(elf_content + p->sh_addr + p->sh_offset);
}

// assume the addr must be a FUNC-type symbol in symtab
char *addrToFunc(Elf64_Addr addr){
	// 1. compare addr with all entries in symtab
	//printf("In addrToFunc, addr = 0x%lx\n", addr);
	//printf("In addrToFunc, symtab = 0x%lx\n", (char *)symtab - elf_content);
	//printf("In addrToFunc, symtab_size = %lx\n", symtab_size);
	 	
	Elf64_Sym *p = symtab;
	for(; (char *)p < (char *)symtab + symtab_size; p++){
		//printf("p->st_value = 0x%lx, p->st_size = %ld\n", p->st_value, p->st_size);
		if(addr >= p->st_value && addr < p->st_value + p->st_size){
			break;
		}
	}
	Assert(p != symtab, "p is just symtab");
	Assert((char *)p < (char *)symtab + symtab_size, "p is out of symtab range, the current pc is 0x%lx", cpu.pc);
	Assert(ELF64_ST_TYPE(p->st_info) == STT_FUNC, "the entry we found is not FUNC");
	return strtab + p->st_name;
}

// We assume, if and only if ddest is x1(ra), then it is call
bool isCall(word_t dest_reg){
//	//printf("dest_reg = 0x%p\n", dest_reg);
//	Assert(dest_reg, "dest_reg == NULL!");
	return dest_reg == 1;
}
// if it is jalr, and dsrc1 is x1(ra), then it is ret
bool isRet(word_t src1_reg){
//	Assert(src1_reg, "src1_reg == NULL!");
	return src1_reg == 1;	
}

#endif

static void jalr_func(Decode *s, word_t dest, word_t src1, word_t src2) {
	R(dest) = s->snpc; 
	s->dnpc = (src1 + src2)&(~1);

#ifdef CONFIG_FTRACE
	char *p = ftrace_buf;
//TODO: isFunc?   This function is to check whether the corresponding symbol type is FUNC
  uint32_t i = s->isa.inst.val;
  int rs1 = BITS(i, 19, 15);

	if(isCall(dest)){
		ftrace_indent_space += 2;
		// write into ftrace_log: pc_val [indent] call [func_name@func_addr]
		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "0x%lx", s->pc);
		memset(p, ' ', ftrace_indent_space); // fill logbuf with space
		p += ftrace_indent_space;
		Elf64_Addr func_addr = src1;
		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "call [%s@0x%lx]\n", addrToFunc(func_addr), func_addr);
		fwrite(ftrace_buf, p-ftrace_buf, 1, ftrace_log);
	} else if(isRet(rs1)){
		// write into ftrace_log: pc_val [indent] ret [func_name]
		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "0x%lx", s->pc);
		memset(p, ' ', ftrace_indent_space); // fill logbuf with space
		p += ftrace_indent_space;
		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "ret [%s]\n", addrToFunc(s->pc)); // we are here
		fwrite(ftrace_buf, p-ftrace_buf, 1, ftrace_log);
		ftrace_indent_space -= 2;
	} else {
		// skip, do nothing
//		//printf("wow we meet accidence\n");	
//		//printf("its cur_pc = 0x%lx\n", s->pc);
//		Assert(false, "There is a jalr which is neither call nor ret, its cur_pc = 0x%lx", s->pc);
//		TODO: we need to chage here lately
	}
#endif
}

static void jal_func(Decode *s, word_t dest, word_t src1) {
	R(dest) = s->snpc;
	s->dnpc = s->pc + src1;

#ifdef CONFIG_FTRACE
	char *p = ftrace_buf;
//TODO: isFunc?   This function is to check whether the corresponding symbol type is FUNC
	if(isCall(dest)){
		ftrace_indent_space += 2;
		// write into ftrace_log: pc_val [indent] call [func_name@func_addr]
		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "0x%lx", s->pc);
		memset(p, ' ', ftrace_indent_space); // fill logbuf with space
		p += ftrace_indent_space;
		Elf64_Addr func_addr = s->pc + src1;
		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "call [%s@0x%lx]\n", addrToFunc(func_addr), func_addr);
		fwrite(ftrace_buf, p-ftrace_buf, 1, ftrace_log);
	} else {
		// skip, do nothing
//		printf("s->pc = 0x%lx\n", s->pc);
//		assert(0);
	}
#endif
}

//def_EHelper(jal) {
//  rtl_li(s, ddest, s->snpc); // store pc+4 to rd
//  rtl_j(s, s->pc + id_src1->simm); // let pc + imm
//	
////	//printf("s->pc + id_src1->simm = 0x%lx\n", s->pc + id_src1->simm);
////	//printf("ddest = 0x%p\n", ddest);
//#ifdef CONFIG_FTRACE
//	char *p = ftrace_buf;
////TODO: isFunc?   This function is to check whether the corresponding symbol type is FUNC
//	if(isCall(ddest)){
//		ftrace_indent_space += 2;
//		// write into ftrace_log: pc_val [indent] call [func_name@func_addr]
//		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "0x%lx", s->pc);
//		memset(p, ' ', ftrace_indent_space); // fill logbuf with space
//		p += ftrace_indent_space;
//		Elf64_Addr func_addr = s->pc + id_src1->simm;
//		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "call [%s@0x%lx]\n", addrToFunc(func_addr), func_addr);
//		fwrite(ftrace_buf, p-ftrace_buf, 1, ftrace_log);
//	} else {
//		// skip, do nothing
////		printf("s->pc = 0x%lx\n", s->pc);
////		assert(0);
//	}
//#endif
//}

//def_EHelper(jalr) {
//  rtl_addi(s, dsrc1, dsrc1, id_src2->simm); // rs1 = rs1 + simm[11:0]
//  rtl_andi(s, dsrc1, dsrc1, ~1);   // set the least significant bit to zero
//  rtl_li(s, ddest, s->snpc); // store pc+4 to rd
//  rtl_jr(s, dsrc1); // jump to the address in rs1
//
//#ifdef CONFIG_FTRACE
//	char *p = ftrace_buf;
////TODO: isFunc?   This function is to check whether the corresponding symbol type is FUNC
//	if(isCall(ddest)){
//		ftrace_indent_space += 2;
//		// write into ftrace_log: pc_val [indent] call [func_name@func_addr]
//		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "0x%lx", s->pc);
//		memset(p, ' ', ftrace_indent_space); // fill logbuf with space
//		p += ftrace_indent_space;
//		Elf64_Addr func_addr = *dsrc1;
//		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "call [%s@0x%lx]\n", addrToFunc(func_addr), func_addr);
//		fwrite(ftrace_buf, p-ftrace_buf, 1, ftrace_log);
//	} else if(isRet(dsrc1)){
//		// write into ftrace_log: pc_val [indent] ret [func_name]
//		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "0x%lx", s->pc);
//		memset(p, ' ', ftrace_indent_space); // fill logbuf with space
//		p += ftrace_indent_space;
//		p += snprintf(p, ftrace_buf + sizeof(ftrace_buf) - p, "ret [%s]\n", addrToFunc(s->pc)); // we are here
//		fwrite(ftrace_buf, p-ftrace_buf, 1, ftrace_log);
//		ftrace_indent_space -= 2;
//	} else {
//		// skip, do nothing
////		//printf("wow we meet accidence\n");	
////		//printf("its cur_pc = 0x%lx\n", s->pc);
////		Assert(false, "There is a jalr which is neither call nor ret, its cur_pc = 0x%lx", s->pc);
////		TODO: we need to chage here lately
//	}
//#endif
//}

#endif
