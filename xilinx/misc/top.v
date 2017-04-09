module cmod_a7_12
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

endmodule
