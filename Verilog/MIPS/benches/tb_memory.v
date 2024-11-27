`include "memory.vh"

module tb_memory;

    /* bench agnostic */
    reg clk;
    reg delay;

    /* testing parameters */
    reg                  memory_read;
    reg                  memory_write;
    reg  [`WIDTH - 1: 0] memory_address;
    reg  [`WIDTH - 1: 0] memory_data_write;
    wire [`WIDTH - 1: 0] memory_data_read;

    /* create the memory module */
    memory mem (
        .clk               (clk),
        .memory_read       (memory_read),
        .memory_write      (memory_write),
        .memory_address    (memory_address),
        .memory_data_write (memory_data_write),
        .memory_data_read  (memory_data_read)
    );
    
    /* configure clock */
    always #1 clk = ~clk;

    initial
    begin
        $dumpfile("logs/tb_memory.vcd");
        $dumpvars(0, tb_memory);

        $monitor("read_en = %d, write_en = %d, address = %d, data_write = %d, data_read = %d",
            memory_read, memory_write, memory_address, memory_data_write, memory_data_read);

        clk = 0;

        memory_read       =      1'd0;
        memory_write      =      1'd0;
        memory_address    = `WIDTH'd0;
        memory_data_write = `WIDTH'd0;

        /* set the address and write data */
        memory_address    = $urandom%255;
        memory_data_write = $urandom%255;
        
        #10

        /* write the data */
        memory_read       = 1'd0;
        memory_write      = 1'd1;

        #10
        
        /* read the data */
        memory_read       = 1'b1;
        memory_write      = 1'b0;

        #10

        $finish;
    end
endmodule
