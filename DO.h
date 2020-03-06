/* 
 * File:   DO.c
 * Author: Ayman Elhaddad
 * Comments: this module is for the digital outputs
 * Revision history: 26/2/2020
 */

#ifndef __DO_H__
#define __DO_H__

#include <xc.h>  


typedef enum
{
    Heater,
    Motor,
    Lamp_1,
    Lamp_2
}tDO;

typedef enum
{
    OFF = 0,
    ON  = 1        
}tDO_State;


void DO_Init(void);
void DO_SetState(tDO device ,tDO_State state);
void Toggle_Lamps (void);


#endif	/* XC_HEADER_TEMPLATE_H */

