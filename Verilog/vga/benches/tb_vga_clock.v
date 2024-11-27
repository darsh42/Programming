module tb_vga_clock;
    
    /* inputs */
    reg  clk = 0;
    reg  rst = 0;
    
    /* outputs */
    wire vga_clk = 0;
    
    vga_clock dut
    (
        .clk     (clk),
        .rst     (rst),
        .vga_clk (vga_clk)
    );
    

    always #1 clk = ~clk;

    initial
    begin
        $dumpfile("build/vga_clock.vcd");
        $dumpvars(0, tb_vga_clock);
        $monitor("clk = %d, rst = %d, vga_clk = %d", clk, rst, vga_clk);

        #10 rst = ~rst;
        #10 rst = ~rst;

        #100

        $finish();
    end
    
endmodule
