module vga_clock
(
    input  wire clk,
    input  wire rst,

    output wire vga_clk
);
    reg [ 1: 0] counter;
    reg         state;

    initial
        {state, counter} <= 0;
    
    always @(posedge clk or posedge rst)
    begin
        if (rst)
            {state, counter} <= 0;
        else begin
            counter <= counter + 1;

            if ( counter == 'b10 )
            begin
                counter <= 0;
                state   <= ~state;
            end
        end
    end

    assign vga_clk = state;
endmodule
