/* 
 * File:   includes.h
 * Author: bg25382
 *
 * Created on September 3, 2012, 5:34 PM
 */

#ifndef INCLUDES_H
#define	INCLUDES_H

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* INCLUDES_H */

typedef unsigned		u1;
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned short long	u24;
typedef unsigned long		u32;

//Access ram
u8 near OpMode,State,EvTimer,TrigTmr,DeciSec;
u8 near isrTMR0, RfState,RfEdgCnt,RfOffCnt,RfTmr;
u8 near DigitPrev,DigitLast,DigitCnt;
u8 near L3Buf[5],L2Buf[12],RfBuf[8];
u8 near wCnt,wev,wod;
u32 near BinKey;
u8 near LEDstat,LEDqueSiz,LEDque[6];
u8 near LastAcs;

