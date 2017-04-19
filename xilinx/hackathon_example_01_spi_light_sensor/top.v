//----------------------------------------------------------------------------
//
//  Пример 1 для хакатона: Датчик освещения с выводом на серию светодиодов
//
//----------------------------------------------------------------------------

module cmod_a7_35t
(
    input         CLK,  // Тактовый сигнал 12 MHz
    input  [ 1:0] BTN,  // Две кнопки
    inout  [ 7:0] ja,   // Pmod Header JA
    inout  [48:1] pio   // GPIO, General-Purpose Input/Output
);

    wire clock   =   CLK;
    wire reset_n = ! BTN [0];

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

    assign pio [8:1] = level;

endmodule
