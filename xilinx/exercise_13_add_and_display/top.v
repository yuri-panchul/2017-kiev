//----------------------------------------------------------------------------
//
//  Упражнение 13: Сложение и вывод на 7-сегментный индикатор
//
//----------------------------------------------------------------------------

module top
(
    inout  [48:1] pio   // GPIO, General-Purpose Input/Output
);

    wire [3:0] a = pio [14:11];
    wire [3:0] b = pio [20:17];

    wire [3:0] result = a + b;

    // a b c d e f g  dp   Буквы с картинки
    // 7 6 4 2 1 9 10 5    Выводы 7-сегментного индикатора
    // 7 6 5 4 3 2 1       Выводы сигнала pio в ПЛИС

    //   --a--
    //  |     |
    //  f     b
    //  |     |
    //   --g--
    //  |     |
    //  e     c
    //  |     |
    //   --d-- 

    reg  [6:0] abcdefg;

    always @*
        case (result)
        4'h0: abcdefg = 7'b1111110;
        4'h1: abcdefg = 7'b0110000;
        4'h2: abcdefg = 7'b1101101;
        4'h3: abcdefg = 7'b1111001;
        4'h4: abcdefg = 7'b0110011;
        4'h5: abcdefg = 7'b1011011;
        4'h6: abcdefg = 7'b1011111;
        4'h7: abcdefg = 7'b1110000;
        4'h8: abcdefg = 7'b1111111;
        4'h9: abcdefg = 7'b1111011;
        4'ha: abcdefg = 7'b1110111;
        4'hb: abcdefg = 7'b0011111;
        4'hc: abcdefg = 7'b1001110;
        4'hd: abcdefg = 7'b0111101;
        4'he: abcdefg = 7'b1001111;
        4'hf: abcdefg = 7'b1000111;
        endcase

    assign pio [7:1] = abcdefg;

endmodule
