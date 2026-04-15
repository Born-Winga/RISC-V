module cpu_tb;

  logic clk;
  logic rst;

  // Clock generation
  always #5 clk = ~clk;

  initial begin
    clk = 0;
    rst = 1;

    #20;
    rst = 0;

    // Placeholder for future stimulus

    #100;
    $finish;
  end

endmodule