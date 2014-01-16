/* 
 * File:   InitStatic.h
 * Author: bg25382
 *
 * Created on September 3, 2012, 11:42 AM
 */

#ifndef INITSTATIC_H
#define	INITSTATIC_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* INITSTATIC_H */

void InitStatic() //Sw-restart, set vars to a known value
{ FSR0=0;FSR0H=0;
 while(FSR0<0x60){INDF0=0xCC;FSR0++;}
 JO.Flag=0;EV.Flag=0;RD.Flag=0;RfState=0;
 if(RdCnf(cnOpMode)>W42) InitCnf();
 if(RdCnf(cnRdMode)&0x80) RD.EM=1;else RD.EM=0;
 OpMode=PWR;State=0;
 ReStartTmr1();DeciSec=10;
 LEDqueSiz=0; L3BufClr(0);EvTimer=10;
 Indic(bY);Indic(0x5F);
 pCFE1=1;
}
