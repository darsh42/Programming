
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
reg  clk;   // main clock
reg  D;     // Data line 

wire Q;     // Flip Flop output

/**************************************
* Create the Design Under Test module *
*                                     *
* Allows for testing other modules by *
* monitoring and changing registers   *
**************************************/
dut u0
(
    .Q (Q),
    .D (D),
    .clk (clk)
);

/*************************************
* The initial conditions of the test *
* bench                              *
*************************************/
initial 
begin
    Q <= 0;
    D <= 0;
end

task 

endmodule
