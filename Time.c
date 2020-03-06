#include "Main.h"
#include "Port.h"
#include "LCD.h"
#include "TMR0.h"
#include "Time.h"
#include "Disp.h"

#define TIM_PERIOD_MS   (20)
#define TIM_SECOND_MS (1000)

static tCount_State TIM_Current_Count_State;

//initial states
static tTIM_Time TIM_CurrentTime = {0, 0};


void TIM_Init(void)
{
    TIM_CurrentTime.seconds = 0;
    TIM_CurrentTime.minutes = 0;
}

void TIM_GetTime(tTIM_Time * time)
{
    time->seconds = TIM_CurrentTime.seconds;
    time->minutes = TIM_CurrentTime.minutes;
}



 void TIM_Update(void)
{
    static tWord Tim_Counter = 20;

    Tim_Counter += TMR_TICK_MS;  
    if(Tim_Counter != TIM_SECOND_MS)
    {
        return;
    }
    Tim_Counter = 0;

    if(TIM_Current_Count_State == Count_Time)
    {
        if ( (TIM_CurrentTime.seconds == 0) && (TIM_CurrentTime.minutes > 0))
        {
            TIM_CurrentTime.seconds = 59;
            TIM_CurrentTime.minutes --;

        }
        else if ( (TIM_CurrentTime.seconds == 0) && (TIM_CurrentTime.minutes == 0))
        {
            TIM_CurrentTime.seconds = 0;
            TIM_CurrentTime.minutes = 0;
        }
        else
        {
           TIM_CurrentTime.seconds --; 
        }
    }
 }             
    

void Set_Time(tByte minutes_tens , tByte minutes_units  , tByte seconds_tens , tByte seconds_units)
{
    minutes_tens = (minutes_tens*10) + minutes_units;
    seconds_tens = (seconds_tens*10) + seconds_units;

    TIM_CurrentTime.seconds = seconds_tens;
    TIM_CurrentTime.minutes = minutes_tens;
}


 tByte Is_Finished (void)
{
    if((TIM_CurrentTime.minutes == 0)&&(TIM_CurrentTime.seconds == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }        
}
 
void Set_TIM_Current_Count_State (tCount_State  Current_Count_State)
{
    TIM_Current_Count_State = Current_Count_State ;
}