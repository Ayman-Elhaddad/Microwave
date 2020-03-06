#ifndef __TIMER_H__
#define __TIMER_H__
#include "Main.h"
#include "Port.h"


void TMR_Init(void);
void TMR_Start(void);
tByte TMR_CheckOverFlow(void);
void TMR_Stop(void);
void __interrupt() TMR_Update(void) ; /* ISR @  vector 0 */
/* zero means that this interrupt is at position 0 in the vector table*/
/*but our pic is non vectored*/

#endif

