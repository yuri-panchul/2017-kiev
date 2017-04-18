//----------------------------------------------------------------------------
//
//  Пример 6: Переливы цветов с помощью широтно-импульсной модуляции (ШИМ,
//  англ.  pulse-width modulation (PWM))
//
//----------------------------------------------------------------------------

module pulse_generator
(
    input         clock,
    input         reset_n,
    input  [19:0] last_cycle,
    output        strobe
);

    reg [19:0] counter;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            counter <= 0;
        else if (counter == last_cycle)
            counter <= 0;
        else
            counter <= counter + 1;
    end
    
    assign strobe = (counter == 0);

endmodule

//--------------------------------------------------------------------

module top
(
    input         CLK,         // Тактовый сигнал 12 MHz

    output        RGB0_Red,    // Красная часть трехцветного светодиода
    output        RGB0_Green,  // Зеленая часть трехцветного светодиода
    output        RGB0_Blue,   // Синяя часть трехцветного светодиода

    input  [ 1:0] BTN          // Две кнопки
);

    wire clock   = CLK;
    wire reset_n = ! BTN [0];
    wire button  = BTN [1];

    wire red_strobe;
    wire green_strobe;
    wire blue_strobe;
    
    reg [19:0] last_cycle_red;
    reg [19:0] last_cycle_green;
    reg [19:0] last_cycle_blue;

    wire check_button;

    pulse_generator pulse_generator_red_strobe
    (
        .clock      ( clock            ),
        .reset_n    ( reset_n          ),
        .last_cycle ( last_cycle_red   ),
        .strobe     ( red_strobe       )
    );

    pulse_generator pulse_generator_green_strobe
    (
        .clock      ( clock            ),
        .reset_n    ( reset_n          ),
        .last_cycle ( last_cycle_green ),
        .strobe     ( green_strobe     )
    );

    pulse_generator pulse_generator_blue_strobe
    (
        .clock      ( clock            ),
        .reset_n    ( reset_n          ),
        .last_cycle ( last_cycle_blue  ),
        .strobe     ( blue_strobe      )
    );

    // clock is 12 MHz

    pulse_generator pulse_generator_check_button
    (
        .clock      ( clock         ),
        .reset_n    ( reset_n       ),
        .last_cycle ( 2000000       ),
        .strobe     ( check_button  )
    );

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
        begin
            last_cycle_red   <= 20;
            last_cycle_green <= 20;
            last_cycle_blue  <= 20;
        end
        else if (button & check_button)
        begin
            last_cycle_red   <= last_cycle_red   + 1;
            last_cycle_green <= last_cycle_green - 1;
            last_cycle_blue  <= last_cycle_blue  - 1;
        end
    end

    assign RGB0_Red   = ~ red_strobe;
    assign RGB0_Green = ~ green_strobe;
    assign RGB0_Blue  = ~ blue_strobe;

endmodule
