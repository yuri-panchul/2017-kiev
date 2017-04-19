//----------------------------------------------------------------------------
//
//  Упражнение 17: Сдвиговый регистр
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// Вариант 1. Все компоненты в одном модуле
//
//----------------------------------------------------------------------------

module top_1
(
    input         CLK,  // Тактовый сигнал 12 MHz
    input  [ 1:0] BTN,  // Две кнопки
    inout  [48:1] pio   // GPIO, General-Purpose Input/Output
);

    wire clock   =   CLK;
    wire reset_n = ! BTN [0];
    wire button  =   pio [9];

    //------------------------------------------------------------------------

    reg [21:0] counter;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            counter <= 22'b0;
        else
            counter <= counter + 22'b1;
    end
    
    wire shift_enable = (counter [21:0] == 22'b0); 

    //------------------------------------------------------------------------

    reg [7:0] shift_reg;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            shift_reg <= 8'b0;
        else if (shift_enable)
            shift_reg <= { button, shift_reg [7:1] };
    end

    assign pio [8:1] = shift_reg;

endmodule

//----------------------------------------------------------------------------
//
// Вариант 2. С модульной иерархией
//
//----------------------------------------------------------------------------

module timer
(
    input  clock_12_mhz,
    input  reset_n,
    output strobe_with_period_0_35_second
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

endmodule

//--------------------------------------------------------------------

module top_2
(
    input         CLK,  // Тактовый сигнал 12 MHz
    input  [ 1:0] BTN,  // Две кнопки
    inout  [48:1] pio   // GPIO, General-Purpose Input/Output
);

    wire clock   = CLK;
    wire reset_n = ! BTN [0];
    wire button  = pio [9];

    wire shift_enable;

    timer timer_i
    (
        .clock_12_mhz                   ( clock            ),
        .reset_n                        ( reset_n          ),
        .strobe_with_period_0_35_second ( shift_enable     )
    );

    reg [7:0] shift_reg;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            shift_reg <= 8'b0;
        else if (shift_enable)
            shift_reg <= { button, shift_reg [7:1] };
    end

    assign pio [8:1] = shift_reg;

endmodule
