/* 
 * File:   ReStartTmr1.h
 * Author: bg25382
 *
 * Created on September 3, 2012, 11:41 AM
 */

#ifndef RESTARTTMR1_H
#define	RESTARTTMR1_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* RESTARTTMR1_H */

void ReStartTmr1()
{TMR1L=0xB0; TMR1H=0x3C;
}
