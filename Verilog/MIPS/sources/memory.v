`include "memory.vh"

module memory 
( 
    input                      clk,                  /* clock signal            */
    input                      memory_read,          /* read flag               */
    input                      memory_write,         /* write flag              */
    input      [`WIDTH - 1: 0] memory_address,       /* address in memory       */
    input      [`WIDTH - 1: 0] memory_data_write,    /* data to write in memory */
    output     [`WIDTH - 1: 0] memory_data_read      /* data return from read   */
);
    integer i;

    reg [`WIDTH - 1:0] _memory [0:`SIZE - 1];

    /************************************
     * set initial conditions of memory *
     ************************************/
    initial
    begin
    /************************************
     * clear memory contents on powerup *
     ************************************/
        for( i = 0; i < `SIZE - 1; i++)
        begin
            _memory[i] <= `WIDTH'd0;
        end
    end

    always @(posedge clk)
    begin
    /************************************
     * if memory write is set, do write *
     ************************************/
        if (memory_write)
            _memory[memory_address] <= memory_data_write;
    end
    
    /************************************
     * if memory read is set, do read   *
     ************************************/
    assign memory_data_read = (memory_read) ? _memory[memory_address]: `WIDTH'd0;
endmodule
