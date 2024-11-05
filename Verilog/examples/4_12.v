/* Illustrates bit swizzling */
module swizzle
(
    input  [3:0] c, d,
    output [8:0] y
);

/*
    * example 
    * c = 111
    * d = 100
    * 
    * y = { 11, 3 * 0, 1, 101};
    * y = 11 000 1 101   
*/

assign y = {c[2:1], {3{d[0]}}, c[0], 3'b101};

endmodule
