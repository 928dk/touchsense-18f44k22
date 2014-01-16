/* 
 * File:   RdCnf.h
 * Author: bg25382
 *
 * Created on September 3, 2012, 11:41 AM
 */

#ifndef RDCNF_H
#define	RDCNF_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* RDCNF_H */

u8 RdCnf(u16 adr)
{EEADRH=(adr>>8)&0x7F;EEADR=adr&0xFF;
 EECON1bits.EEPGD=0;EECON1bits.CFGS=0;
 EECON1bits.RD=1;
 return(EEDATA);	//0xFA8 Eedata
}


