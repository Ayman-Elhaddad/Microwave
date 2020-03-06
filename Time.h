#ifndef __TIME_H__
#define __TIME_H__

#include "Main.h"
#include "Disp.h"

typedef struct
{
    tByte seconds;
    tByte minutes;
}tTIM_Time;

typedef enum
{
    TIM_NORMAL = 0,
    TIM_SET_HOURS,
    TIM_SET_MINUTES
}tTIM_State;

void TIM_Init(void);
void TIM_Update(void);
void TIM_GetTime(tTIM_Time * time);
void Set_Time(tByte minutes_tens , tByte minutes_units  , tByte seconds_tens , tByte seconds_units);
tByte Is_Finished (void);
void Set_TIM_Current_Count_State (tCount_State );

#endif
