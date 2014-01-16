/* 
 * File:   TagOff.h
 * Author: bg25382
 *
 * Created on September 3, 2012, 11:47 AM
 */

#ifndef TAGOFF_H
#define	TAGOFF_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* TAGOFF_H */

void TagOff()
{if(RfOffCnt<0x64) RfOffCnt++;
// if(CB.ECO) pCFE1=0;
 RfState =0; EV.OFF=1;
 INTCONbits.GIE=0;
}
