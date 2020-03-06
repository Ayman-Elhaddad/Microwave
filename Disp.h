/* 
 * File:   Disp.c
 * Author: Ayman Elhaddad
 * Comments: this pure software module presents an intermediate STEP between the MW and the LCD outputs
 * Revision history: 26/2/2020
 */

#ifndef __DISP_H__
#define __DISP_H__

#include <xc.h>   


typedef enum
{
    WELCOME,
    SCAN_MIN_TENS,
    SCAN_MIN_UNITS,
    SCAN_SEC_TENS,
    SCAN_SEC_UNITS,
    START_COUNT,
    CANCEL,
    FINISHED        
            
}tMW_State;

typedef enum
{
    Released,
    Pressed        
}tSens_State;

typedef enum
{
    Wait_Food,
    Wait_Door,
    Count_Time,
    Pause_Time,
    Reset_Time,
    Finish_Time
    
}tCount_State;

typedef enum
{
    BLINK_MIN_TENS,
    BLINK_MIN_UNITS,
    BLINK_SEC_TENS,
    BLINK_SEC_UNITS,
    BLINK_DOTS,
    BLINK_ALL,
    BLINK_NONE        
}tBlink_State;

void Disp_Update (void);
void Set_Disp_State (tMW_State);
void Set_Disp_Current_Count_State (tCount_State);
#endif	

