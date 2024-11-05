/* shows simple reduction operators */
module eight_input_and
(
    input  [7:0] a,
    output       y
);

assign y = &a;

// equivelent to:
// y = a[7] & a[6] & a[5] & a[4] & a[3] & a[2] & a[1] & a[0];
// works for '|', '^', '~&', '~|' operators

endmodule
