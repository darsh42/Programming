/* Illustrates basics of FPGA combinational logic */
module combinational_logic
(
    input  a, b, c
    output y
);

assign y = ~a & ~b & ~c |
            a & ~b & ~c |
            a & ~b &  c;

endmodule
