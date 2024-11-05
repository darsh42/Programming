module sr_latch 
(
    input  clk,
    input  S,
    input  R,
    output Q,
    output not_Q
);
always @(posedge clk)
begin
    not_Q = (
    Q = (S ^ ~not_Q);
end

endmodule
