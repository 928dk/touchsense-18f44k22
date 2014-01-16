/* 
 * File:   TMR1psr.h
 * Author: bg25382
 *
 * Created on September 3, 2012, 11:40 AM
 */

#ifndef TMR1PSR_H
#define	TMR1PSR_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* TMR1PSR_H */

void TMR1psr() //SysTick 100ms trig:TMR1IF
{u8 M,Md;
 bTMR1IF=0;
 PIR1bits.TMR1IF=0;//Clk 8/4/4 Mhz: 500Khz Incr:2us OF:100ms
 TMR1L = 0xB4;TMR1H = 0x3C;//FFFFh-50000d=15536d=3CB0h
 JO.KBD=1;
 if(pIN1)RD.BuzEX=0;else RD.BuzEX=1;MkBuz();
 if(LEDqueSiz)//IndicTimed();
 {LEDque[0]-=0x10;
  if(0xF0&LEDque[0]);//not done
  else//next que'd indic
  {if(--LEDqueSiz)
   {LEDque[0]=LEDque[1];LEDque[1]=LEDque[2];
	LEDque[2]=LEDque[3];LEDque[3]=LEDque[4];
	LEDque[4]=LEDque[5];
   }else
   {LEDque[0] = LEDstat;
   }WrLED(LEDque[0]);
 }}
 if(1==DeciSec--)//newSec
 {DeciSec=10;EV.SEC=1;
  if(EvTimer) {if(1==EvTimer--) {
 EV.TMR=1; EV.SEC=0; }
}}}//~TMR1psr()-------------------------
