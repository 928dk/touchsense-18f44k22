/* 
 * File:   InitCnf.h
 * Author: bg25382
 *
 * Created on September 3, 2012, 11:41 AM
 */

#ifndef INITCNF_H
#define	INITCNF_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* INITCNF_H */

void InitCnf()
{WrCnf(cnRdMode,7);  WrCnf(cnOpMode,W26);
 WrCnf(cnFacil,0xFF);WrCnf(cnOffAt,6);
 WrCnf(cnOffEm,7);   WrCnf(cnEcoTrim,100);
}

