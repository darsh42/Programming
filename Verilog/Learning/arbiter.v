module arbiter (
  clock,
  reset,
  req_0,
  req_1,
  gnt_0,
  gnt_1
);

  input clock;
  input reset;
  input req_0;
  input req_1;

  output gnt_0;
  output gnt_1;

  reg gnt_0;
  reg gnt_1;

  always @ (posedge clock or posedge reset)
    if (reset) begin
      gnt_0 <= 0;
      gnt_1 <= 0;
    end else if (req_0) begin
      gnt_0 <= 1;
      gnt_1 <= 0;
    end else if (req_1) begin
       gnt_0 <= 0;
       gnt_1 <= 1;
    end
endmodule

// Testbench Code
module arbiter_tb;

  reg clock, reset, req_0, req_1;
  wire gnt_0, gnt_1;


  initial begin
    $monitor ("req0=%b, req1=%b, gnt0=%b, gnt1=%b", req_0, req_1, gnt_0, gnt_1);
    clock = 0;
    reset = 0;
    req_0 = 0;
    req_1 = 0;
    #5 reset = 1;
    #15 reset = 0;
    #10 req_0 = 1;
    #10 req_0 = 0;
    #10 req_1 = 1;
    #10 req_1 = 0;
    #10 {req_0, req_1} = 2'b11;
    #10 {req_0, req_1} = 2'b00;
    #10 $finish;
  end

  always begin
    #5 clock = !clock;
  end

  arbiter U0 (
    .clock (clock),
    .reset (reset),
    .req_0 (req_0),
    .req_1 (req_1),
    .gnt_0 (gnt_0),
    .gnt_1 (gnt_1)
  );
endmodule
