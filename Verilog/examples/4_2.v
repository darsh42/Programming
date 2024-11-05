/* Illustrates basic inversion on multiple bits */
module inverter
(
    input  [3:0] a,
    output [3:0] y
);

assign y = ~a;

endmodule
