module tb_rising_d_flip_flop;
/**************************************
* Creating inputs and outputs         *
*                                     *
* Inputs are all created as registers *
* as it allows us to change the value *
* contained within.                   *
*                                     *
* Outputs are all created as wires as *
* this allows us to monitor them as   *
* they change during the test         *
**************************************/

reg        clk;   // main clock
reg  [2:0] del;   // delay time

reg        D;     // Data line 
wire       Q;     // Flip Flop output

/**************************************
* Create the Design Under Test module *
*                                     *
* Allows for testing other modules by *
* monitoring and changing registers   *
**************************************/

rising_d_flip_flop rdff
(
    .clk (clk)
    .D (D),
    .Q (Q),
);

/*************************************
* Generate the clock signal for the  *
* bench                              *
*************************************/

always #10 clk = ~clk;

/*************************************
* The initial conditions of the test *
* bench                              *
*************************************/

initial 
begin
    clk <= 0;
    D   <= 0;
    Q   <= 0;

    #15 D <= 1;
    for (int i = 0; i < 5; i++)
    begin
        delay = $random;
        #(delay) d <= i;
    end
end

endmodule
