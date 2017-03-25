
// PIC32MX150F128D Configuration Bit Settings

// 'C' source line config statements

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2 // DIV_12        // PLL Input Divider (12x Divider)
#pragma config FPLLMUL = MUL_20 // MUL_24         // PLL Multiplier (24x Multiplier)
#pragma config FPLLODIV = DIV_2 // DIV_256       // System PLL Output Clock Divider (PLL Divide by 256)

// DEVCFG1
#pragma config FNOSC = PRIPLL // FRCDIV           // Oscillator Selection Bits (Fast RC Osc w/Div-by-N (FRCDIV))
#pragma config FSOSCEN = OFF // ON             // Secondary Oscillator Enable (Enabled)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = XT // OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_8           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = ON              // Watchdog Timer Enable (WDT Enabled)
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = ON              // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


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

    RPC1R = 3;
    RPC0R = 3;
    SDI1R = 1;

    for (;;)
    {
    TRISC = ~0;
    TRISB = ~0;

        /*
    SPI1CONbits.ON      = 0;        // Disable SPI to reset any previous state
    value               = SPI1BUF;  // Clear receive buffer

    SPI1CONbits.MSTEN   = 1;        // Master Mode Enable bit
    SPI1CONbits.MSSEN   = 1;        // Slave Select pin is driven automatically
    SPI1CONbits.MODE16  = 1;        // 16-Bit Communication
    // SPI1CONbits.DISSDO  = 1;        // Disable SDOx pin bit

//  SPI1CONbits.SRXISEL = 1;        // 01 = Interrupt is generated when the buffer is not empty

    SPI1BRG             = PB_CLK_FREQUENCY / SPI_CLK_FREQUENCY / 2 - 1;

    SPI1CONbits.ON      = 1;  // Enable SPI
*/
    
    // Int
    
        SPI1CONbits.SRXISEL = 0;        // 01 = Interrupt is generated when the buffer is not empty
    SPI1CONbits.ON      = 0;        // Disable SPI to reset any previous state
    value               = SPI1BUF;  // Clear receive buffer

    SPI1CONbits.MSTEN   = 1;        // Master Mode Enable bit
    SPI1CONbits.MSSEN   = 1;        // Slave Select pin is driven automatically
    SPI1CONbits.MODE16  = 1;        // 16-Bit Communication
    SPI1CONbits.DISSDO  = 0;        // Disable SDOx pin bit
    SPI1CONbits.CKE     = 1;

    SPI1BRG             = PB_CLK_FREQUENCY / SPI_CLK_FREQUENCY / 2 - 1;

    SPI1CONbits.ON      = 1;  // Enable SPI

    
    
    SPI1BUF = 0xa5;  // send data to slave
        
    n_busy = 0;

    while (SPI1STATbits.SPIBUSY)  // wait until SPI transmission complete
        n_busy ++;
            
    value = SPI1BUF;

    SPI2CONbits.ON      = 0;        // Disable SPI to reset any previous state
//    delay (500);
    
    TRISC = 0;
    TRISB = 0;
/*
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
*/
    PORTC = (value >> 5) & 0xff;
/*    delay (500);

    PORTC = (value >> 8) & 0xff;
    delay (500);

    PORTC = (value >> 16) & 0xff;
    delay (500);

    PORTC = (value >> 24) & 0xff;
    delay (500);
  */  }
}


void main7 (void)
{
    int value, n_busy;
    
    TRISB = ~0;
    TRISC = ~0;

    RPC1R = 3;
    RPC0R = 3;
    SDI1R = 1;
    
    SPI1CONbits.SRXISEL = 0;        // 01 = Interrupt is generated when the buffer is not empty
    SPI1CONbits.ON      = 0;        // Disable SPI to reset any previous state
    value               = SPI1BUF;  // Clear receive buffer

    SPI1CONbits.MSTEN   = 1;        // Master Mode Enable bit
    SPI1CONbits.MSSEN   = 1;        // Slave Select pin is driven automatically
    SPI1CONbits.MODE16  = 0;        // 16-Bit Communication
    SPI1CONbits.DISSDO  = 0;        // Disable SDOx pin bit
    SPI1CONbits.CKE     = 1;

    SPI1BRG             = PB_CLK_FREQUENCY / SPI_CLK_FREQUENCY / 2 - 1;

    SPI1CONbits.ON      = 1;  // Enable SPI

    for (;;)
    {
        SPI1BUF = 0xA5;  // send data to slave
        
        while (SPI1STATbits.SPIBUSY)  // wait until SPI transmission complete
            ;
            
        value = SPI1BUF;
    }
}
