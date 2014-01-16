/*
 * File:   touchsense
 * Author: bg25382
 *
 * Created on November 5, 2012, 10:35 AM
 *
 * 
 * 
 *
 *
 *
 */

#include <p18f44k22.h>
#include <delays.h>
#include <stdio.h> // for uart etc
#include <stdlib.h> //for strings etc
#include <htc.h>
#include <stdint.h>
#include "mtouch.h"

//#include "inttypes.h" 
//#include <GenericTypeDefs.h>
//#include <math.h>
//#include <signal.h>
//#include <stdarg.h>
//#include <string.h>
//#include <time.h>

#pragma config FOSC = INTIO67   // Internal Oscilator block
#pragma config PLLCFG = ON      // PLLCFG Oscillator multiplied by 4
#pragma config PRICLKEN = ON    // Primary clock enabled
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF       // Oscillator Switchover mode disabled
#pragma config WDTEN = OFF      // Watchdog Timer enabled = off
#pragma config BOREN = OFF      // Brown-out Reset disabled in hardware and software
#pragma config LVP = OFF        // Single supply ICSP Enabled = OFF
#pragma config DEBUG = ON       //
#pragma config PBADEN= OFF      // Port B AD converter off,
#pragma config PWRTEN = OFF      // Power Up Timer enabled (Page34)

// configword 0x300008
#pragma config CP0 = OFF //CP0: Code Protection bit 1 = Block 0 not code-protected 0 = Block 0 code-protected
#pragma config CP1 = OFF //CP1: Code Protection bit 1 = Block 1 not code-protected 0 = Block 1 code-protected

// configword 0x300009
#pragma config CPB = OFF //CPB: Boot Block Code Protection bit
#pragma config CPD = OFF //CPD: Data EEPROM Code Protection bit

// configword 0x30000a
#pragma config WRT0 = OFF //WRT0: Write Protection bit
#pragma config WRT1 = OFF //WRT1: Write Protection bit

// configword 0x30000b
#pragma config WRTB = OFF //WRTB:Boot Block Write Protection bit
#pragma config WRTC = OFF //WRTC: Configuration Register Write Protection bit
#pragma config WRTD = OFF //WRTD: Data EEPROM Write Protection bit

// configword 0x30000c
#pragma config EBTR0 = OFF //EBTR0: Table Read Protection bit
#pragma config EBTR1 = OFF //EBTR1: Table Read Protection bit

// configword 0x30000d
#pragma config EBTRB = OFF //Boot Block Table Read Protection bit

//Capacitivt input (analog)
#define TRIS_K0        TRISAbits.TRISA0
#define TRIS_K1        TRISBbits.TRISB4
#define TRIS_K2        TRISDbits.TRISD1

//Uart 1
#define TRIS_RX  TRISCbits.TRISC7
#define TRIS_TX  TRISCbits.TRISC6
#define PORT_RX  PORTCbits.RC7
#define PORT_TX  PORTCbits.RC6
#define SPBRG1_VAL 8
//SPBRG1 = 8; //8: 115200 @ 64MHz,
//SPBRG1 = 51; //51: 19200 @ 64MHz

//Uart 2
#define TRIS_RX2 TRISDbits.TRISD7
#define TRIS_TX2 TRISDbits.TRISD6
#define PORT_RX2 PORTDbits.RD7
#define PORT_TX2 PORTDbits.RD6
#define SPBRG2_VAL 8
//SPBRG2 = 8; //8: 115200 @ 64MHz,
//SPBRG2 = 51; //51: 19200 @ 64MHz

//Leds
#define	TRIS_RED	TRISAbits.TRISA3
#define	TRIS_GREEN	TRISEbits.TRISE1
#define	TRIS_BLUE	TRISEbits.TRISE2

#define	PORT_RED	PORTAbits.RA3
#define	PORT_GREEN	PORTEbits.RE1
#define	PORT_BLUE	PORTEbits.RE2

//#####################################
typedef unsigned		u1;
//typedef unsigned short char     u4;     //nibble, semioctet
typedef unsigned char		u8;     //byte,octet
typedef unsigned short		u16;    //unsigned int,halfword,word,short
typedef unsigned short long	u24;
typedef unsigned long		u32;    //word,long
typedef unsigned long long      u64;    //long long, quad, quadword, int6
//typedef unsigned double double  u128;   //octaword, double quadword (nonstandart)

//typedef signed                  s1;
//typedef signed short char       s4;     //nibble, semioctet
typedef signed char		s8;     //byte,octet
typedef signed short		s16;    //signed int,halfword,word,short
typedef signed short long	s24;
typedef signed long		s32;    //word,long
typedef signed long long        s64;    //long long, quad, quadword, int6
//typedef signed double double    s128;   //octaword, double quadword (nonstandart)
/*

char                8-bit  (signed by default)
signed char         8-bit (signed)
unsigned char       8-bit (unsigned)
int                 16 bit (signed)
unsigned int        16-bit (unsigned)
short               Same as int
unsigned short      Same as unsigned int
short long          24-bit (signed)
unsigned short long 24-bit (unsigned)
long                32-bit (signed)
unsigned long       32-bit (unsigned)

 * Signed kan holde  + og - (eks -128 til 128)
 * Unsigned kan kun holde positive værdier (eks 0 til 255)

 * and bits for at clear (nul/0/lav) e.k.s. trisb & 0b01111 1111 for at clear msb til output
 * or bits for at sætte (et/1/høj) e.k.s. trisb | 0b11111 1111 for at sætte msb til input (alle i dette tilfælde)
 * xor bits for at invertere bits for at toggle trisb ^ 0b1000000 ofr at toggle msb (invertere)
 *
 */
int setpunkt = 0;

volatile char uart_buf_char = 0;
char uart_buf[128];

volatile char uart2_buf_char = 0;
char uart2_buf[128];

//SW map L1,L2,OS ##################################
void ReStartTmr1(void);
void Init44k22(void);
void InitStatic(void);
void TimerTest(void);
void AllLedsOn(void);
void AllLedsOff(void);
void SoftwarePwm(u16 on,u16 off);
void Delayx(int y);
void Delay10x(int y);

//Uart 1 comm
void PutsUart1(char *data);
void GetsUart1(char *buffer, unsigned char len);
char DataReadyUart1(void);
char BusyUart1(void);
char GetcUart1(void);
void PutcUart1(char data);

//Uart 2 comm
void PutsUart2(char *data);
void GetsUart2(char *buffer, unsigned char len);
char DataReadyUart2(void);
char BusyUart2(void);
char GetcUart2(void);
void PutcUart2(char data);

//HW map ##############################################
void InitTimer0(void);
void InitTimer1(void);
void InitTimer2(void);
void InitTimer3(void);
void InitUart1(void);
void InitUart2(void);
void Init_IO(void);

unsigned int b1,b2; //til cap input
double b1tot,b2tot;
float b1avg,b2avg,b1cal,b2cal,b1CTMUI,b2CTMUI;

#define ADSCALE 1023                       //for unsigned conversion 10 sig bits
#define ADREF 3.3                           //Vdd connected to A/D Vr+
#define RCAL .022                           //justeres hvergang der bliver ændret baneløngde/kapacitet
                                            //R value is 4200000 (4.2M) (
                                            //scaled so that result is in
                                            //1/100th of uA

int main(void)//p18F #org 0x0200 w.booter {
{
u16 counter = 0;
Init44k22();
//InitStatic();

SoftwarePwm(10,20);// uart kan ikke lide (store) delays

mTouchInit();/* Initialize the mTouch library */

mTouchCalibrate();/* Call the mTouch callibration function */

INTCONbits.GIE = 1;
INTCONbits.PEIE = 1;

TimerTest(); //uart2 and timer 1+3

do //mainloop
{
counter++;

mTouchCalibrate();
ClearSettings();

int j=0;
   
    for(j=0;j<10;j++) //læs 10 gange

        {
        b1 = mTouchReadButton(1);
        b2 = mTouchReadButton(2);

        b1tot += b1;  //tilføj resultat til total
        b2tot += b2;  //tilføj resultat til total
        }

b1avg = (float)(b1tot/10.000);      //lav en avg af de 10 læsninger
b2avg = (float)(b2tot/10.000);      //lav en avg af de 10 læsninger

b1cal = (float)(b1avg/ADSCALE*ADREF); //skaleringsroutine
b2cal = (float)(b2avg/ADSCALE*ADREF); //skaleringsroutine

b1CTMUI = b1cal/RCAL; //CTMUISrc is in 1/100ths of uA tager højde for indgangs modstand
b2CTMUI = b2cal/RCAL; //CTMUISrc is in 1/100ths of uA tager højde for indgangs modstand

if (b1CTMUI <= setpunkt)
{
 PORT_RED=0;
}

if (b1CTMUI > setpunkt)
{
 PORT_RED=1;
}

if (b2CTMUI <= setpunkt)
{
  PORT_BLUE=0;  
}

if (b2CTMUI > setpunkt)
{
  PORT_BLUE=0;
}

if (counter == 65535) //counter has reached max
        {
        counter = 0;
        }
}while(1);

sprintf(uart2_buf, "Button 1 (an1) %d\r\n", b1CTMUI);
PutsUart2((char *)uart2_buf);

sprintf(uart2_buf, "Button 2 (an2) %d\r\n", b2CTMUI);
PutsUart2((char *)uart2_buf);
}


void Init44k22(void)
{
//OSC int 16MHz (62.5ns/inst.)
OSCCONbits.IRCF2 = 1;
OSCCONbits.IRCF1 = 1;
OSCCONbits.IRCF0 = 1;
while(!OSCCONbits.HFIOFS); //wait until ready

//Uart1
ANSELCbits.ANSC7 = 0; //Port C7 select som digital port
ANSELCbits.ANSC6 = 0; //Port C6 select som digital port

//Uart2
ANSELDbits.ANSD7 = 0; //Port D7 select som digital port
ANSELDbits.ANSD6 = 0; //Port D6 select som digital port

//Leds
ANSELAbits.ANSA3 = 0; //Port A3 select som digital port Red
ANSELEbits.ANSE1 = 0; //Port E1 select som digital port Green
ANSELEbits.ANSE2 = 0; //Port E2 select som digital port Blue

//InitTimer0();
InitTimer1();
//InitTimer2();
InitTimer3();

InitUart1();
InitUart2();

Init_IO();
}

void interrupt Interrupt_High(void)
{
unsigned char c1;
unsigned char c2;

c1 = PRODL;
c2 = PRODH;

if(PIR1bits.TMR1IF) // timer 1 interrupt flag
 {
TMR1H = 0x54;
TMR1L = 0xDE;
PIR1bits.TMR1IF = 0; //reset flag
}

if(PIR1bits.RCIF)//UART1 interrupt flagRX
{
uart_buf_char = GetcUart1();
PutcUart1(uart_buf_char);
PIR1bits.RC1IF = 0;
}

//if (PIR2bits.TMR3IF)
if (PIR3bits.TMR3GIF)
{
TMR3H = 0x54;
TMR3L = 0xDE;
//PIR2bits.TMR3IF = 0;
PIR3bits.TMR3GIF = 0;
}

if (PIR3bits.RC2IF)
{
uart2_buf_char = GetcUart2();
PutcUart2(uart2_buf_char);
PIR3bits.RC2IF = 0;
}

PRODL = c1;
PRODH = c2;
}
//#pragma code

void Init_IO(void)
{
//Tris vælger om det er input(1) eller output (0)
//Med Port kan man skrive 0 eller 1 hvis pin er valgt som output
//hvis port er valgt som input kan man læse om der er 0 eller 1 på pin

//Leds
TRIS_RED = 0;
TRIS_GREEN = 0;
TRIS_BLUE = 0;
PORT_RED = 1;
PORT_GREEN = 1;
PORT_BLUE = 1;

//Keypad
TRIS_K0 = 1;
TRIS_K1 = 1;
TRIS_K2 = 1;

//Uart1
//PORT_RX = 0;
//PORT_TX = 0;
TRIS_RX = 1;
TRIS_TX = 0;

//Uart2
//PORT_RX2 = 0;
//PORT_TX2 = 0;
//TRIS_RX2 = 1;
//TRIS_TX2 = 1;

}

void ClearSettings(void)
{
    /*
 * til mtouch
 */
b1 = 0; //init static after system reboot
b2 = 0;//init static after system reboot
b1cal = 0;
b2cal = 0;
b1tot = 0;
b2tot = 0;
b1avg = 0;
b2avg = 0;
b1CTMUI = 0;
b2CTMUI = 0;
/*
 *
 */

}

void InitTimer0(void)
{
T0CON	=0xC1;  //at:1100001 em:1100010// clr(LATC,2);
}

void InitTimer1(void)
{
TMR1L = 0x00;
T1CONbits.TMR1CS = 1;   //Clk = Fosc
T1CONbits.T1CKPS = 4;   //PreScaler
T1CONbits.T1RD16 = 0;   //16 bits = 0 = 8bits
T1CONbits.TMR1ON = 1;   //timer 1 on
PIR1bits.TMR1IF = 0;    //timer 1 interrupt flag (clear = 0)
PIE1bits.TMR1IE = 1;    //timer 1 interrupt enable
}

void InitTimer2(void)
{
    //T2OUTPS<3:0>
    //T2CKPS<1:0>
    T2CON=0x1D;  //FCA 4Khz PWM (buz), 1ms TMR2IF (eco)
    T2CONbits.T2CKPS = 4;
    T2CONbits.TMR2ON = 1;
}

void InitTimer3(void)
{
TMR3L = 0x00;
T3CONbits.TMR3CS = 1;   //Clk = Fosc
T3CONbits.T3CKPS = 4;   //PreScaler
T3CONbits.T3RD16 = 0;   //16 bits = 0 = 8bits
T3CONbits.TMR3ON = 1;   //timer 3 on

//PIR2bits.TMR3IF = 0;
//PIR2bits.
PIR3bits.TMR3GIF = 0;    //timer 3 interrupt flag (clear = 0)
PIE3bits.TMR3GIE = 1;    //timer 3 interrupt enable

}

void AllLedsOn(void)
{
PORT_BLUE=0;PORT_GREEN=0;PORT_RED=0; //all led's on
}

void AllLedsOff(void)
{
PORT_BLUE=1;PORT_GREEN=1;PORT_RED=1;//all led's off
}

void TimerTest(void)
{
sprintf(uart2_buf, "Test uart 2 timer 1 value: %d\r\n", TMR1L);
PutsUart2((char *)uart2_buf);

sprintf(uart2_buf, "Test uart 2 timer 3 value: %d\r\n", TMR3L);
PutsUart2((char *)uart2_buf);

//PutsUart1((char *)uart_buf);
}

void InitUart1(void)
{
RCSTA1bits.SPEN = 0; //disable serial port
PIE1bits.TX1IE = 0; //tx interrupt enable

TXSTA1bits.TX9 = 0; //tx not 9 bit (8bits)
TXSTA1bits.TXEN = 1; //Enable transmit
TXSTA1bits.SYNC = 0; //Async
TXSTA1bits.BRGH = 1; //High speed

RCSTA1bits.RX9 = 0; //8bits rx
RCSTA1bits.CREN = 1; //Continuous reception

BAUDCON1bits.BRG16 = 0; //1 = 16bits
SPBRG1 = SPBRG1_VAL;


PIR1bits.RC1IF = 0; //interrupt flag clear
PIR1bits.TX1IF = 0; //interrupt flag clear

PIE1bits.RC1IE = 1; //rx interrupt enable
PIE1bits.TX1IE = 0; //tx interrupt enable

RCSTA1bits.SPEN = 1; //Enable serial port
}

void InitUart2(void)
{
RCSTA2bits.SPEN = 0;    //disable uart 2
PIE3bits.TX2IE = 0;     // TX interrupt disable

TXSTA2bits.TX9 = 0;     //tx not 9 bit (8bits)
TXSTA2bits.TXEN = 1;    //Enable tx
TXSTA2bits.SYNC = 0;    //Async
TXSTA2bits.BRGH = 1;    //High speed prescale /16

RCSTA2bits.RX9 = 0;     //8bits rx
RCSTA2bits.CREN = 1;    //Continuous rx enavle

BAUDCON2bits.BRG16 = 0; //1 = 16bits
SPBRG2 = SPBRG2_VAL;

IPR3bits.TX2IP = 0;// high prio
IPR3bits.RC2IP = 0;// high prio

PIR3bits.TX2IF = 0; // Clear interrupt flag
PIR3bits.RC2IF = 0; // Clear interrupt flag

PIE3bits.RC2IE = 1;     // Set RX interrupt enable
PIE3bits.TX2IE = 0;     // TX interrupt disable

RCSTA2bits.SPEN2 = 1;
}

void InitStatic() //Sw-restart, set vars to a known value
{
FSR0=0;
FSR0H=0;
    while(FSR0<0x60) //indirekte adressering
    {
        INDF0=0xCC;
        FSR0++;
    }
//pCFE1=1;
}

//softwarepwm
 void SoftwarePwm(u16 on,u16 off)
 {

PORT_BLUE=1;PORT_GREEN=1;PORT_RED=0; //only red on
Delayx(on);

PORT_BLUE=1;PORT_GREEN=1;PORT_RED=1;//all led's off
Delayx(off);

PORT_BLUE=1;PORT_GREEN=0;PORT_RED=1;//only green on
Delayx(on);

PORT_BLUE=1;PORT_GREEN=1;PORT_RED=1;//all led's off
Delayx(off);

PORT_BLUE=0;PORT_GREEN=1;PORT_RED=1;//only blue on
Delayx(on);

PORT_BLUE=1;PORT_GREEN=1;PORT_RED=1;//all led's off
Delayx(off);

 }

void ReStartTmr1()
{
TMR1L=0xB0;
TMR1H=0x3C;
}

void ReStartTmr2()
{

}

void
putch(char c)
{

    TXREG1 = c;
}

void PutcUart1(char data)
{
        TXREG1 = data;          //Write the data byte to the USART1
}

char GetcUart1(void)
{
  return (RCREG1);              // Return the received data
}

char GetcUart2(void)
{
  return (RCREG2);              // Return the received data
}

void PutcUart2(char data)
{

//PIE3bits.TX2IE = 0;
        TXREG2 = data;          //Write the data byte to the USART2
//PIE3bits.TX2IE = 1;
}

char BusyUart1(void)
{
  if(!TXSTA1bits.TRMT)  // Is the transmit shift register empty
        return 1;       // No, return FALSE
  return 0;             // Return TRUE
}

char BusyUart2(void)
{
  if(!TXSTA2bits.TRMT)  // Is the transmit shift register empty
        return 1;       // No, return FALSE
  return 0;             // Return TRUE
}

char DataReadyUart1(void)
{
  if(PIR1bits.RCIF)    // If RCIF is set
        return 1;               // Data is available, return TRUE
  return 0;                     // Data not available, return FALSE
}

char DataReadyUart2(void)
{
  if(PIR3bits.RC2IF)    // If RCIF is set
        return 1;               // Data is available, return TRUE
  return 0;                     // Data not available, return FALSE
}

void GetsUart1(char *buffer, unsigned char len)
{
        char i;                                                 // Length counter
        unsigned char data;

        for(i=0;i<len;i++)                      // Only retrieve len characters
        {
                while(!DataReadyUart1());       // Wait for data to be received

                data = GetcUart1();           // Get a character from the USART
                                                // and save in the string
                *buffer = data;
                buffer++;                       // Increment the string pointer
        }
}

void GetsUart2(char *buffer, unsigned char len)
{
        char i;                                                 // Length counter
        unsigned char data;

        for(i=0;i<len;i++)                      // Only retrieve len characters
        {
                while(!DataReadyUart2());       // Wait for data to be received

                data = GetcUart2();           // Get a character from the USART
                                                // and save in the string
                *buffer = data;
                buffer++;                       // Increment the string pointer
        }
}

void PutsUart1(char *data)
{
        do
        {  // Transmit a byte
                while(BusyUart1());
                PutcUart1(*data);
        } while( *data++ );
}

void PutsUart2(char *data)
{
        do
        {  // Transmit a byte
                while(BusyUart2());
                PutcUart2(*data);
        } while( *data++ );
}

void Delayx(int y){
    int x=0;
        for (x=0; x<y; x=x+1) {
            asm("nop ");
     }
}

void Delay10x(int y){
    int x=0;
        for (x=0; x<y; x=x+1) {
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
    }
}