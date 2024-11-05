/*************************************************************************
*                        Rising Edge D Flip Flop                         *
**************************************************************************
*
*/

module rising_d_flip_flop ( D, Q, clk );

input  clk;             // Clock input
input  D;               // Data input
output Q;               // Flip Flop output

always @(posedge clk)   // every time the clk is set to high
begin
/************************************
 * Set the Flip Flop output to High *
 * when the value is not equal to D *
 ************************************/
    Q <= D;
end

endmodule
