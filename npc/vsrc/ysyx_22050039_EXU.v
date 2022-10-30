`include "ysyx_22050039_all_inst.v"

module ysyx_22050039_EXU #(XLEN = 64) (
	input clk,
	input rst,
	input [`ysyx_22050039_FUNC_LEN-1:0] func,
	input [XLEN-1:0] src1,
	input [XLEN-1:0] src2,
	input [XLEN-1:0] pc,
	output reg [XLEN-1:0] exec_result,
	output [XLEN-1:0] dnpc
);

	import "DPI-C" function void ebreak();
	import "DPI-C" function void invalid();

	always@(*) begin	
		exec_result = 0;
		dnpc = 0;
		inval = 0;
		case(func)
			// Rtype
			// Itype
			Addi		: exec_result = src1 + src2;
			Jalr		: begin exec_result = pc + 4; dnpc = src1 + src2; end
			// Stype
			Sd			: ; // sd empty now
			// Btype
			// Utype
			Auipc		: exec_result = src1 + pc;
			Lui			:	exec_result = src1;
			// Jtype
			Jal			: begin exec_result = pc + 4; dnpc = pc + src1; end
			Ebreak	: ebreak();
			default	: inval = 1; // invalid
		endcase
	end

	// invalid is only valid when rst = 0
	reg inval;
	always@(posedge clk)
		if(~rst && inval)
			invalid();
		// else do nothing
			
endmodule
