/* 
 * File:   Disp.c
 * Author: Ayman Elhaddad
 * Comments: this module to handle the process between "MW-module" and  LCD
 * Revision history: 26/2/2020
 */


#include <xc.h>
#include <stdio.h>
#include <string.h>

#include "Disp.h"
#include "LCD.h"
#include "Main.h"
#include "Port.h"
#include "Time.h"
#include "MW.h"
#include "keypad.h"
#include "SW.h"
#include "DO.h"


#define TIM_Disp_MS (50)
#define DOT_TICK_MS (50)
#define BLINK_PERIOD (1000)

static void Disp_Time (tMW_State Current_MW_State);
static void Disp_and_Blink_Time (tByte Min_Ten , tByte Min_Unit , tByte Sec_Ten ,
                                                 tByte Sec_Unit ,tBlink_State Blink_State );

static tByte Min_Unit = 0;
static tByte Min_Ten = 0;
static tByte Sec_Unit = 0;
static tByte Sec_Ten = 0;

static tCount_State Disp_Current_Count_State;

static tByte Disp_Current_State = WELCOME;

void Disp_Update (void)
{
    tTIM_Time time;   
    TIM_GetTime(&time);

    Min_Unit =  (time.minutes % 10);
    Min_Ten =  (time.minutes / 10);
    Sec_Unit =  (time.seconds % 10);
    Sec_Ten = (time.seconds / 10);

    static tByte Disp_Counter = 10;
    Disp_Counter += TMR_TICK_MS;  
    if(Disp_Counter != TIM_Disp_MS)
    {
        return;
    }
    Disp_Counter = 0;

    Disp_Time(Disp_Current_State);
  
}


static void Disp_Time (tMW_State Disp_Current_State)
{
    switch (Disp_Current_State)
    {
        case WELCOME :
            
            Set_LCD_BUFFER1("    WELCOME !   ");
            Set_LCD_BUFFER2("  Press Any Key ");
            
            break;
        
        case SCAN_MIN_TENS :
            
            Disp_and_Blink_Time (Min_Ten ,Min_Unit ,Sec_Ten ,Sec_Unit ,BLINK_MIN_TENS);
            
            break;
            
        case SCAN_MIN_UNITS :
            
            Disp_and_Blink_Time (Min_Ten ,Min_Unit ,Sec_Ten ,Sec_Unit ,BLINK_MIN_UNITS);
            
            break;

        case SCAN_SEC_TENS :
            
            Disp_and_Blink_Time (Min_Ten ,Min_Unit ,Sec_Ten ,Sec_Unit ,BLINK_SEC_TENS);
            
            break;            
        
        case SCAN_SEC_UNITS :
            
            Disp_and_Blink_Time (Min_Ten ,Min_Unit ,Sec_Ten ,Sec_Unit ,BLINK_SEC_UNITS);
            
            break;

        case START_COUNT :
        
            if(Disp_Current_Count_State == Count_Time)
            {
                Disp_and_Blink_Time (Min_Ten ,Min_Unit ,Sec_Ten ,Sec_Unit ,BLINK_DOTS);
                Set_LCD_BUFFER2("     Heating...");
                
            }
            else if(Disp_Current_Count_State == Wait_Food)
            {
                Disp_and_Blink_Time (Min_Ten ,Min_Unit ,Sec_Ten ,Sec_Unit ,BLINK_ALL);
                Set_LCD_BUFFER2("   Put Food !   ");
            }
            else if(Disp_Current_Count_State == Wait_Door)
            {
                Disp_and_Blink_Time (Min_Ten ,Min_Unit ,Sec_Ten ,Sec_Unit ,BLINK_ALL);
                Set_LCD_BUFFER2("   Close Door ! ");
            }
            break;

        case CANCEL :
            if(Disp_Current_Count_State == Pause_Time)
            {
                Disp_and_Blink_Time (Min_Ten ,Min_Unit ,Sec_Ten ,Sec_Unit ,BLINK_ALL);
                Set_LCD_BUFFER2("    Paused !   ");
            }
            else if(Disp_Current_Count_State == Reset_Time)
            {
                //reset mode
            } 
            break;                       
                                                       
        default :
            break;
            
    }

}


static void Disp_and_Blink_Time (tByte Min_Ten , tByte Min_Unit , tByte Sec_Ten , tByte Sec_Unit ,tBlink_State Blink_State )
{
    tByte i;
    static tByte LCD_Buffer_1[16];
    static tWord Blink_Counter = 0;
    Blink_Counter += DOT_TICK_MS;
    
    switch(Blink_State)
    {
        case BLINK_MIN_TENS:
            
            if(Blink_Counter <= (BLINK_PERIOD/2))
            {
                LCD_Buffer_1[0] = ' ';
                LCD_Buffer_1[1] = Min_Ten+'0';
                LCD_Buffer_1[2] = Min_Unit+'0';
                LCD_Buffer_1[3] = ':';
                LCD_Buffer_1[4] = Sec_Ten+'0';
                LCD_Buffer_1[5] = Sec_Unit+'0';
                
            }
            if(Blink_Counter > (BLINK_PERIOD/2))
            {
                LCD_Buffer_1[1] = ' ';
                if(Blink_Counter == BLINK_PERIOD)
                {
                    Blink_Counter = 0;
                }                                
            }
            
            break;

        case BLINK_MIN_UNITS:
            
            if(Blink_Counter == (BLINK_PERIOD/2))
            {
                LCD_Buffer_1[0] = ' ';
                LCD_Buffer_1[1] = Min_Ten+'0';
                LCD_Buffer_1[2] = Min_Unit+'0';
                LCD_Buffer_1[3] = ':';
                LCD_Buffer_1[4] = Sec_Ten+'0';
                LCD_Buffer_1[5] = Sec_Unit+'0';
            }
            if(Blink_Counter == BLINK_PERIOD)
            {
                LCD_Buffer_1[2] = ' ';
                Blink_Counter = 0;                
            }
            
            break;            

        case BLINK_SEC_TENS:
            
            if(Blink_Counter == (BLINK_PERIOD/2))
            {
                LCD_Buffer_1[0] = ' ';
                LCD_Buffer_1[1] = Min_Ten+'0';
                LCD_Buffer_1[2] = Min_Unit+'0';
                LCD_Buffer_1[3] = ':';
                LCD_Buffer_1[4] = Sec_Ten+'0';
                LCD_Buffer_1[5] = Sec_Unit+'0';                
            }
            if(Blink_Counter == BLINK_PERIOD)
            {
                LCD_Buffer_1[4] = ' ';
                Blink_Counter = 0;
                return;
            }
            
            break;            

        case BLINK_SEC_UNITS:
            
            if(Blink_Counter == (BLINK_PERIOD/2))
            {
                LCD_Buffer_1[0] = ' ';
                LCD_Buffer_1[1] = Min_Ten+'0';
                LCD_Buffer_1[2] = Min_Unit+'0';
                LCD_Buffer_1[3] = ':';
                LCD_Buffer_1[4] = Sec_Ten+'0';
                LCD_Buffer_1[5] = Sec_Unit+'0';                
            }
            if(Blink_Counter == BLINK_PERIOD)
            {
                LCD_Buffer_1[5] = ' ';
                Blink_Counter = 0;
                
            }
            
            break;

        case BLINK_DOTS:
            
            if(Blink_Counter == (BLINK_PERIOD/2))
            {
                LCD_Buffer_1[0] = ' ';
                LCD_Buffer_1[1] = Min_Ten+'0';
                LCD_Buffer_1[2] = Min_Unit+'0';
                LCD_Buffer_1[3] = ':';
                LCD_Buffer_1[4] = Sec_Ten+'0';
                LCD_Buffer_1[5] = Sec_Unit+'0';
               
            }
            
            if(Blink_Counter == BLINK_PERIOD)
            {
                LCD_Buffer_1[3] = ' ';
                Blink_Counter = 0;                
            }

            
            
            
            break;

            
        case BLINK_ALL:
            
            if(Blink_Counter == (BLINK_PERIOD/2))
            {
                LCD_Buffer_1[0] = ' ';
                LCD_Buffer_1[1] = Min_Ten+'0';
                LCD_Buffer_1[2] = Min_Unit+'0';
                LCD_Buffer_1[3] = ':';
                LCD_Buffer_1[4] = Sec_Ten+'0';
                LCD_Buffer_1[5] = Sec_Unit+'0';
            }
            if(Blink_Counter == BLINK_PERIOD)
            {
                LCD_Buffer_1[0] = ' ';
                LCD_Buffer_1[1] = ' ';
                LCD_Buffer_1[2] = ' ';
                LCD_Buffer_1[3] = ' ';
                LCD_Buffer_1[4] = ' ';
                LCD_Buffer_1[5] = ' ';
                Blink_Counter = 0;
            }
            
            break;
            
        case BLINK_NONE :
            
            LCD_Buffer_1[0] = ' ';
            LCD_Buffer_1[1] = Min_Ten+'0';
            LCD_Buffer_1[2] = Min_Unit+'0';
            LCD_Buffer_1[3] = ':';
            LCD_Buffer_1[4] = Sec_Ten+'0';
            LCD_Buffer_1[5] = Sec_Unit+'0';
            
            break;
            
            
        default :
            break;


    }
    
        
    for(i=6 ; i<16 ; i++)
    {
        LCD_Buffer_1[i] = ' ';
    }

    Set_LCD_BUFFER1 ( LCD_Buffer_1 );
    Set_LCD_BUFFER2 ("   Set Time !   " );

}


void Set_Disp_State (tMW_State Current_MW_State)
{
    Disp_Current_State = Current_MW_State;
}

void Set_Disp_Current_Count_State (tCount_State  Current_Count_State)
{
    Disp_Current_Count_State = Current_Count_State ;
}