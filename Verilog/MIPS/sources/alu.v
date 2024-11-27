`include "alu.vh"

typedef union packed {
    logic [`WIDTH - 1:  0] value;
    logic [         3:  0] funct;
    logic [         6:  4] rd;
    logic [        10:  8] rt;
    logic [        13: 11] rs;
    logic [        16: 14] op;
} opcode_t;

module alu
(
   input                  clk,
   input                  rstn,
   input  [        15: 0] opcode,
   output [`WIDTH - 1: 0] program_counter
);
    reg [`WIDTH - 1: 0]     pc; /* internal program counter */
    reg [`WIDTH - 1: 0] r[7:0]; /* internal registers */
    opcode_t               cir; /* internal current instruction register */

    integer i;

    initial
    begin
        pc        <= 'h0000;
        cir.value <= 'h0000;
        
        for (i = 0; i < 8; i++)
            r[i] <= 'h0000;
    end

    /* reset device */
    always @(posedge clk and rstn)
    begin
        pc        <= 'h0000;
        cir.value <= 'h0000;

        for (i = 0; i < 8; i++)
            r[i] <= 'h0000;
    end

    always @(posedge clk and !rstn)
    begin
        pc        <= pc + 1;
        cir.value <= opcode;

        case (cir.op)
            'b000:
                /* R-Type instructions */
                case (cir.funct)
                    'b0000: /* add */ r[cir.rd] <= r[cir.rs] + r[cir.rt];
                    'b0001: /* sub */ r[cir.rd] <= r[cir.rs] - r[cir.rt];
                    'b0010: /* and */ r[cir.rd] <= r[cir.rs] & r[cir.rt];
                    'b0011: /* or  */ r[cir.rd] <= r[cir.rs] | r[cir.rt];
                    'b0100: /* slt */ r[cir.rd] <= r[cir.rs] < r[cir.rt];
                    'b1000: /* jr  */ pc        <= r[cir.rs];
                endcase
            'b001:
            'b010:
            'b011:
            'b100:
            'b101:
            'b110:
            'b111:
        endcase

        program_counter <= pc;
    end
endmodule
