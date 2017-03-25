#include <p32xxxx.h>

void delay (int n)
{
    n *= 1000;

    while (n --)
        asm volatile ("nop");
}

void main1 (void)
{
    TRISB = 0;

    for (;;)
    {
        PORTB = ~ 0;
        delay (50);
        PORTB = 0;
        delay (50);
    }
}

//--------------------------------------------------------------------

void main2 (void)
{
    int n;
    
    TRISC = 0;
    n     = 0x11;

    for (;;)
    {
        PORTC = n = (n >> 1) | ((n & 1) << 7);
        delay (50);
    }
}

//--------------------------------------------------------------------

void main3 (void)
{
    int i, n, m;
    
    TRISC = 0;
    
    i = 0;
    n = 1;
    m = 1;

    for (;;)
    {
        i ++;
        
        n = (n >> 1) | ((n & 1) << 7);
        
        if (i % 10 == 0)
            m = ((m << 1) & 0xff) | ((m & 0x80) >> 7);

        PORTC = n | m;
        
        delay (20);
    }
}

//--------------------------------------------------------------------

void main4 (void)
{
    int n = 0;
    
    TRISC = 0x100;
    
    for (;;)
    {
        n = n >> 1;
        
        if (PORTCbits.RC8)
           n |= 0x80;

        PORTC = n;

        delay (50);
    }
}

//--------------------------------------------------------------------

#define SYS_CLK_FREQUENCY  (40 * 1000 * 1000)
#define PB_CLK_FREQUENCY   ( 5 * 1000 * 1000)
#define SPI_CLK_FREQUENCY  ( 2 * 1000 * 1000)  // From 1 to 4 MHz

void main5 (void)
{
    int value, n_busy;
    
    TRISC = 0x100;

    SPI2CONbits.ON      = 0;        // Disable SPI to reset any previous state
    value               = SPI2BUF;  // Clear receive buffer

    SPI1CONbits.MSTEN   = 1;        // Master Mode Enable bit
    SPI1CONbits.MSSEN   = 1;        // Slave Select pin is driven automatically
    SPI1CONbits.MODE16  = 1;        // 16-Bit Communication
    SPI1CONbits.DISSDO  = 1;        // Disable SDOx pin bit

//  SPI1CONbits.SRXISEL = 1;        // 01 = Interrupt is generated when the buffer is not empty

    SPI1BRG             = PB_CLK_FREQUENCY / SPI_CLK_FREQUENCY / 2 - 1;

    SPI1CONbits.ON      = 1;  // Enable SPI

    for (;;)
    {
        SPI1BUF = 0;  // send data to slave
        
        n_busy = 0;

        PORTC = 1;
        delay (500);
        
        while (SPI1STATbits.SPIBUSY)  // wait until SPI transmission complete
        {
            PORTC = 2;
            delay (500);
            n_busy ++;
        }
            
        PORTC = 3;
        delay (500);
        value = SPI1BUF;

        PORTC = (n_busy >> 8) & 0xff;
        delay (500);
/*
        PORTC = (n_busy >> 8) & 0xff;
        delay (500);

        PORTC = (n_busy >> 16) & 0xff;
        delay (500);

        PORTC = (n_busy >> 24) & 0xff;
        delay (500);
*/
//        continue;
        
//        PORTC = n_busy;
        delay (500);

        PORTC = value & 0xff;
        delay (500);

        PORTC = (value >> 8) & 0xff;
        delay (500);
    }
/*        
IFS1bits.SPI1EIF = 0;
IFS1bits.SPI1RXIF = 0;
IFS1bits.SPI1TXIF = 0;

IEC1bits.SPI1EIE = 0;
IEC1bits.SPI1RXIE = 0;
IEC1bits.SPI1TXIE = 0;

IPC7bits.SPI1IS = 0;
IPC7bits.SPI1IP = 0;
*/
}



void main (void)
{
    int value, n_busy;
    
    for (;;)
    {
    TRISC = ~0;
    TRISB = ~0;

    SPI2CONbits.ON      = 0;        // Disable SPI to reset any previous state
    value               = SPI2BUF;  // Clear receive buffer

    SPI1CONbits.MSTEN   = 1;        // Master Mode Enable bit
    SPI1CONbits.MSSEN   = 1;        // Slave Select pin is driven automatically
    SPI1CONbits.MODE16  = 1;        // 16-Bit Communication
    SPI1CONbits.DISSDO  = 1;        // Disable SDOx pin bit

//  SPI1CONbits.SRXISEL = 1;        // 01 = Interrupt is generated when the buffer is not empty

    SPI1BRG             = PB_CLK_FREQUENCY / SPI_CLK_FREQUENCY / 2 - 1;

    SPI1CONbits.ON      = 1;  // Enable SPI

    SPI1BUF = 0;  // send data to slave
        
    n_busy = 0;

    while (SPI1STATbits.SPIBUSY)  // wait until SPI transmission complete
        n_busy ++;
            
    value = SPI1BUF;

    SPI2CONbits.ON      = 0;        // Disable SPI to reset any previous state
    delay (500);
    
    TRISC = 0;
    TRISB = 0;

    PORTC = 0xff;
    delay (500);
    
    PORTC = 0x33;
    delay (500);
    
    PORTC = 0xff;
    delay (500);
    
    PORTC = (n_busy >> 0) & 0xff;
    delay (500);

    PORTC = (n_busy >> 8) & 0xff;
    delay (500);

    PORTC = (n_busy >> 16) & 0xff;
    delay (500);

    PORTC = (n_busy >> 24) & 0xff;
    delay (500);

    PORTC = (value >> 0) & 0xff;
    delay (500);

    PORTC = (value >> 8) & 0xff;
    delay (500);

    PORTC = (value >> 16) & 0xff;
    delay (500);

    PORTC = (value >> 24) & 0xff;
    delay (500);
    }
}
