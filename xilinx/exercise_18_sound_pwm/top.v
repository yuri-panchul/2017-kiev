//----------------------------------------------------------------------------
//
//  Упражнение 18.2: Генерация звуков До-Ми-Соль
//
//----------------------------------------------------------------------------

module frequency_generator
#
(
    parameter clock_frequency          = 12000000,  // 12 MHz
              output_frequency_mul_100 = 26163      // Частота ноты До   первой октавы * 100
)
(
    input      clock,
    input      reset_n,
    output reg out
);

    parameter [31:0] period_in_cycles
        = clock_frequency * 100 / output_frequency_mul_100;

    reg [15:0] counter;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
        begin
            counter <= 16'b0;
            out     <= 1'b0;
        end
        else
        begin
            if (counter == period_in_cycles / 2 - 1)
            begin
                out     <= ! out;
                counter <= 16'b0;
            end
            else
            begin
                counter <= counter + 16'b1;
            end
        end
    end

endmodule

//----------------------------------------------------------------------------

module top
(
    input         CLK,         // Тактовый сигнал 12 MHz

    output        RGB0_Red,    // Красная часть трехцветного светодиода
    output        RGB0_Green,  // Зеленая часть трехцветного светодиода
    output        RGB0_Blue,   // Синяя часть трехцветного светодиода

    input  [ 1:0] BTN,         // Две кнопки
    inout  [48:1] pio          // GPIO, General-Purpose Input/Output
);

    parameter clock_frequency      = 12000000;

    parameter frequency_c4_mul_100 = 26163,  // Частота ноты До   первой октавы * 100
              frequency_e4_mul_100 = 32963,  // Частота ноты Ми   первой октавы * 100
              frequency_g4_mul_100 = 39200;  // Частота ноты Соль первой октавы * 100
    
    wire clock     = CLK;
    wire button_c4 = BTN [1];
    wire button_e4 = BTN [0];
    wire button_g4 = pio [9];

    frequency_generator
    # (
        .clock_frequency          ( clock_frequency      ),
        .output_frequency_mul_100 ( frequency_c4_mul_100 )
    )
    (
        .clock   ( clock     ),
        .reset_n ( button_c4 ),
        .out     ( note_c4   )
    );

    frequency_generator
    # (
        .clock_frequency          ( clock_frequency      ),
        .output_frequency_mul_100 ( frequency_e4_mul_100 )
    )
    (
        .clock   ( clock     ),
        .reset_n ( button_e4 ),
        .out     ( note_e4   )
    );

    frequency_generator
    # (
        .clock_frequency          ( clock_frequency      ),
        .output_frequency_mul_100 ( frequency_g4_mul_100 )
    )
    (
        .clock   ( clock     ),
        .reset_n ( button_g4 ),
        .out     ( note_g4   )
    );

    assign RGB0_Red   = ~ (note_c4 & button_c4);
    assign RGB0_Green = ~ (note_e4 & button_e4);
    assign RGB0_Blue  = ~ (note_g4 & button_g4);

    assign pio [48]   = note_c4 | note_e4 | note_g4;

endmodule
