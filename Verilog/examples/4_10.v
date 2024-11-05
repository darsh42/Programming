module tristate
(
    input  [3:0] a,
    input        e,
    output [3:0] y
);

/* if not enable, set y to 'ZZZZ' indicating 4 floating values */
assign y = e ? a : 4'bz;

endmodule
