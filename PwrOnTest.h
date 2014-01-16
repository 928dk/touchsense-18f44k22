/* 
 * File:   PwrOnTest.h
 * Author: bg25382
 *
 * Created on September 3, 2012, 11:30 AM
 */

#ifndef PWRONTEST_H
#define	PWRONTEST_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* PWRONTEST_H */


//####################################
//Boot Section
void PwrOnTest()
{u8 i;
 pYELLOW=1;pGREEN=1;pRED=0;
 for(i=3;i>0;i--)
 {pOC2=1; Delay(6000);//15
  if(1==pIN1) goto _ReTeDone;
  pOC2=0; Delay(60);	//1
  if(0==pIN1) goto _ReTeDone;
 }InitCnf();
 pCFE1=1;pRED=0;pGREEN=0;
 while(1){;}//stay to trim Hz
_ReTeDone:
 pOC1=0;pOC2=0;pRED=1;//off
}

