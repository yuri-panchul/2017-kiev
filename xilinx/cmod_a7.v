module timer
(
    input  clock_12_mhz,
    input  reset_n,
    output strobe_with_period_0_35_second,
    output strobe_1_of_of_16
);

    reg [21:0] counter;

    always @(posedge clock_12_mhz or negedge reset_n)
    begin
        if (! reset_n)
            counter <= 22'b0;
        else
            counter <= counter + 22'b1;
    end
    
    assign strobe_with_period_0_35_second = (counter [21:0] == 22'b0); 
    assign strobe_1_of_of_16              = (counter [ 3:0] ==  4'b0);

endmodule

//--------------------------------------------------------------------

module cmod_a7
(
    input         CLK,         // Тактовый сигнал 12 MHz

    output [ 1:0] LED,         // Два светодиода

    output        RGB0_Red,    // Красная часть трехцветного светодиода
    output        RGB0_Green,  // Зеленая часть трехцветного светодиода
    output        RGB0_Blue,   // Синяя часть трехцветного светодиода

    input  [ 1:0] BTN,         // Две кнопки

    inout  [ 7:0] ja,          // Pmod Header JA
    inout  [48:1] pio          // GPIO, General-Purpose Input/Output
);

    wire clock   = CLK;
    wire reset_n = ! BTN [0];
    wire button  = pio [9];

    wire shift_enable;
    wire board_led_strobe;

    timer timer_i
    (
        .clock_12_mhz                   ( clock            ),
        .reset_n                        ( reset_n          ),
        .strobe_with_period_0_35_second ( shift_enable     ),
        .strobe_1_of_of_16              ( board_led_strobe )
    );

    reg [7:0] shift;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            shift <= 8'b0;
        else if (shift_enable)
            shift <= { button, shift [7:1] };
    end

    wire [15:0] value;

    pmod_als_spi_receiver pmod_als_spi_receiver_i
    (
        .clock    ( clock   ),
        .reset_n  ( reset_n ),
        .cs       ( ja [0]  ),
        .sck      ( ja [3]  ),
        .sdo      ( ja [2]  ),
        .value    ( value   )
    );

    wire [7:0] level = ~ (~ 8'b0 >> value [12:9]);

    assign pio [8:1] = shift | level;

    assign RGB0_Red   = ~ ((shift [7:4] != 4'b0) & board_led_strobe);
    assign RGB0_Green = ~ ((shift [5:2] != 4'b0) & board_led_strobe);
    assign RGB0_Blue  = ~ ((shift [3:0] != 4'b0) & board_led_strobe);

    assign LED [0] = (ja != 8'b0) & board_led_strobe;
    assign LED [1] = (pio [48:10] != 39'b0) & board_led_strobe;

endmodule
