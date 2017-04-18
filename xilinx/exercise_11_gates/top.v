//----------------------------------------------------------------------------
//
//  Упражнение 11: Простая комбинаторная логика
//
//----------------------------------------------------------------------------

// Вариант 1. Используем операторы непрерывного присваивания
// (continuous assignments)

module top1
(
    output [1:0] LED,  // Два светодиода
    input  [1:0] BTN   // Две кнопки
);

    assign LED [0] = BTN [0] & BTN [1];
    assign LED [1] = BTN [0] | BTN [1];
    
endmodule

// Вариант 2. Используем always-блок и блокирующие присваивания
// (blocking assignments)

module top2
(
    output reg [1:0] LED,  // Два светодиода
    input      [1:0] BTN   // Две кнопки
);

    always @*
    begin
        LED [0] = BTN [0] & BTN [1];
        LED [1] = BTN [0] | BTN [1];
    end

endmodule

// Вариант 3. Используем экземляры примитивов and и or
// (instances)

module top3
(
    output [1:0] LED,  // Два светодиода
    input  [1:0] BTN   // Две кнопки
);

    and and_i (LED [0], BTN [0], BTN [1]);
    or  or_i  (LED [1], BTN [0], BTN [1]);
    
endmodule
