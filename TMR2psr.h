/* 
 * File:   TMR2psr.h
 * Author: bg25382
 *
 * Created on September 3, 2012, 11:40 AM
 */

#ifndef TMR2PSR_H
#define	TMR2PSR_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* TMR2PSR_H */

//L1####################################
void TMR2psr()//s30 trim Rfid ScanRate(R&TTE)
{bTMR2IF=0;
 PIR1bits.TMR2IF=0;
 if(--TrigTmr == 0)
 {RfTrig(); TrigTmr=RdCnf(cnEcoTrim);
}}
