module pmod_als_spi_receiver
(
    input             clock,
    input             reset_n,
    output            cs,
    output            sck,
    input             sdo,
    output reg [15:0] value
);

    reg [19:0] counter;
    reg [15:0] shift;

    always @ (posedge clock or negedge reset_n)
    begin       
        if (! reset_n)
            counter <= 20'b100;
        else
            counter <= counter + 20'b1;
    end

    assign sck = ~ counter [1];
    assign cs  =   counter [6];

    assign sample_bit = ( cs == 1'b0 && counter [1:0] == 2'b11 );
    assign value_done = ( counter [19:0] == 20'b0 );

    always @ (posedge clock or negedge reset_n)
    begin       
        if (! reset_n)
        begin       
            shift <= 16'h0000;
            value <= 16'h0000;
        end
        else if (sample_bit)
        begin       
            shift <= (shift << 1) | sdo;
        end
        else if (value_done)
        begin       
            value <= shift;
        end
    end

endmodule
