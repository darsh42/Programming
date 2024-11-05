/* Illustrates a mux created using the ternary operator */
module multiplexer
(
    input  [3:0] d0, d1,
    input        s,
    output [3:0] y
);

assign y = s ? d0 : d1;

endmodule
