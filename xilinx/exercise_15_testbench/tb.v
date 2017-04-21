//----------------------------------------------------------------------------
//
//  Упражнение 15: Среда тестирования
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
//  Модуль, который мы собираемся тестировать
//
//----------------------------------------------------------------------------

module top (input a, input b, output o);

    assign o = ~ a & ~ b;

endmodule

//----------------------------------------------------------------------------
//
//  Вариант 1. Без самопроверки
//
//----------------------------------------------------------------------------

module tb_1;

    reg  a, b;
    wire o;

    top top_i (a, b, o);

    initial
    begin
        $dumpvars;
        $monitor ("%t a %b b %b o %b", $time, a, b, o);

        # 10;
        a = 0; b = 0; # 10;
        a = 1; b = 0; # 10;
        a = 0; b = 1; # 10;
        a = 1; b = 1; # 10;

        $finish;
    end

endmodule

//----------------------------------------------------------------------------
//
//  Вариант 2. Добавляем самопроверку
//
//----------------------------------------------------------------------------

module tb_2;

    reg  a, b;
    wire o;

    top top_i (a, b, o);

    initial
    begin
        $dumpvars;
        $monitor ("%t a %b b %b o %b", $time, a, b, o);

        # 10;

        a = 0; b = 0;

        # 10;

        if (o != 1)
            $display ("Unexpected value: a: %b b: %b o: %b", a, b, o);

        a = 1; b = 0;

        # 10;

        if (o != 1)
            $display ("Unexpected value: a: %b b: %b o: %b", a, b, o);

        a = 0; b = 1;

        # 10;

        if (o != 1)
            $display ("Unexpected value: a: %b b: %b o: %b", a, b, o);

        a = 1; b = 1;

        # 10;

        if (o != 1)
            $display ("Unexpected value: a: %b b: %b o: %b", a, b, o);

        $finish;
    end

endmodule

//----------------------------------------------------------------------------
//
//  Вариант 3. Улучшаем организацию кода
//
//----------------------------------------------------------------------------

module tb_3;

    reg  a, b;
    wire o;

    top top_i (a, b, o);

    //------------------------------------------------------------------------

    task test (input ta, input tb);

        reg expected_o;

        begin
            expected_o = ~ ta & ~ tb;

            a = ta;
            b = tb;

            # 10;

            if (o != expected_o)
                $display ("Unexpected value: a: %b b: %b o: %b expected o: %b",
                    a, b, o, expected_o);
        end

    endtask

    //------------------------------------------------------------------------

    initial
    begin
        $dumpvars;
        $monitor ("%t a %b b %b o %b", $time, a, b, o);

        test (0, 0);
        test (0, 1);
        test (1, 0);
        test (1, 1);

        $finish;
    end

endmodule

//----------------------------------------------------------------------------
//
//  Вариант 4. Добавляем случайные тесты
//
//----------------------------------------------------------------------------

module tb_4;

    reg  a, b;
    wire o;

    top top_i (a, b, o);

    //------------------------------------------------------------------------

    task test (input ta, input tb);

        reg expected_o;

        begin
            expected_o = ~ ta & ~ tb;

            a = ta;
            b = tb;

            # 10;

            if (o != expected_o)
                $display ("Unexpected value: a: %b b: %b o: %b expected o: %b",
                    a, b, o, expected_o);
        end

    endtask

    //------------------------------------------------------------------------

    initial
    begin
        $dumpvars;
        $monitor ("%t a %b b %b o %b", $time, a, b, o);

        repeat (10)
            test ($random, $random);

        $finish;
    end

endmodule
