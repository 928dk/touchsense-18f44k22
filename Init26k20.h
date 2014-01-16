/* 
 * File:   Init26k20.h
 * Author: bg25382
 *
 * Created on September 3, 2012, 11:41 AM
 */

#ifndef INIT26K20_H
#define	INIT26K20_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* INIT26K20_H */

void Init26k20(void)
{PORTA  =0; PORTB=0; PORTC=0;//F89,F8A,F8B
 TRISA	=TRISA_; TRISB=TRISB_; TRISC=TRISC_;//F92 F93,F94
 INTCON2=0x00;//FF1
 WDTCON	=0x00;//FD1
 T0CON	=0xC1;//at:1100001 em:1100010// clr(LATC,2);
 T1CON	=0xA1;//FCD T1CON 8Mhz/2/4+on 16bit
 T2CON	=0x1D;//FCA 4Khz PWM (buz), 1ms TMR2IF (eco)
 PR2	=0x7C;//FCB 05 PWM1,PWM2
// T3CON	=0xB1;//FB1 4uS 16bit;
 T3CON	=0x91;//FB1 4uS 16bit;
 ADCON0	=0x00;//FC2
 ADCON1	=0x0F;//FC1
 CVRCON	=0x00;//FB5
 bTMR0IE=   1;//set(INTCON,5);
 CCPR1L	=0x3E;//FBE pwm1_duty(62);
 CCPR2L	=0x3E;//FBB Pwm2:buzzer
 CCP2CON=0x0C;//0xFBA
 OSCCON	=0x62;//FD3 setup_oscillator(OSC_8MHZ|OSC_INTRC);
 ANSEL=0x00;ANSELH=0x00;//F7F=0
}

