#include <xc.h>

#include "Time.h"
#include "Disp.h"
#include "DO.h"
#include "Main.h"
#include "Port.h"
#include "SW.h"
#include "keypad.h"
#include "TMR0.h"
#include "LCD.h"


#define MW_Update_PERIOD (50)

static tSens_State Weight_State = Released;
static tSens_State Door_State = Released;

static tMW_State Current_MW_State = WELCOME;
static tCount_State Current_Count_State = Reset_Time;



void MW_Update (void)
{
    static tByte Min_Unit_val = 0;
    static tByte Min_Ten_val = 0;
    static tByte Sec_Unit_val = 0;
    static tByte Sec_Ten_val = 0;
    static tByte Pressed_Key = Key_INIT ;
    tWord Time_Val = Min_Unit_val + Min_Ten_val + Sec_Unit_val + Sec_Ten_val;
    
    static tByte MW_Update_Counter = 10;
    MW_Update_Counter += TMR_TICK_MS;
    if(MW_Update_Counter != MW_Update_PERIOD)
    {
        return;
    }
    MW_Update_Counter = 0;
    
    Set_TIM_Current_Count_State (Current_Count_State);
    Set_Disp_Current_Count_State (Current_Count_State);
    
    if(Keypad_Get_Data_From_buffer(&Pressed_Key) == 1)
    {
        Keypad_Get_Data_From_buffer(&Pressed_Key);
    }
    
     if(Current_Count_State == Count_Time)
    {
        DO_SetState(Heater , ON);
        DO_SetState(Motor , ON);
        Toggle_Lamps();
    }else
    {
        DO_SetState(Heater , OFF);
        DO_SetState(Motor , OFF);
        DO_SetState(Lamp_1 , OFF);
        DO_SetState(Lamp_2 , OFF);
        
        if(Current_Count_State == Wait_Door)
        {
            DO_SetState(Lamp_1 , ON);
        }
    }    
    

    if(SW_GetState(WEIGHT_SW) == SW_PRESSED)
    {
        Weight_State = Pressed;
    }
    if(Weight_State == Pressed)
    {
        if(SW_GetState(DOOR_SW) == SW_PRESSED)
        {
            Door_State = Pressed;
        }
    }

   
        switch(Current_MW_State)
    {
        case WELCOME :
            
            Set_Disp_State(WELCOME);

 
            if(Pressed_Key != Key_INIT)
            {
                Current_MW_State = SCAN_MIN_TENS;
                Pressed_Key = Key_INIT; 
                LCD_Clear();
            }
            
            break;
        case SCAN_MIN_TENS:
            
            Set_Disp_State(SCAN_MIN_TENS);
            
            if((Pressed_Key == KEY_START)&&(Time_Val != 0))
            {
                Current_MW_State = START_COUNT;
                Pressed_Key = Key_INIT;
                return;
            }
            else if (Pressed_Key == KEY_CANCEL)
            {
                Current_MW_State = CANCEL;
                Pressed_Key = Key_INIT;
                return;
            }
            else if((Pressed_Key != Key_INIT)&&(Pressed_Key!= KEY_START))
            {
                if(Pressed_Key >= key_6)
                {
                    Min_Ten_val = key_5;
                }
                else
                {
                    Min_Ten_val = Pressed_Key;
                }           
                Current_MW_State = SCAN_MIN_UNITS;             
                Pressed_Key = Key_INIT;
                Set_Time( Min_Ten_val , Min_Unit_val , Sec_Ten_val , Sec_Unit_val );                 
            }    
            break;

        case SCAN_MIN_UNITS:
            
            Set_Disp_State(SCAN_MIN_UNITS);
            if((Pressed_Key == KEY_START)&&(Time_Val != 0))
            {
                Current_MW_State = START_COUNT;
                Pressed_Key = Key_INIT;
                return;
            }
            else if (Pressed_Key == KEY_CANCEL)
            {
                Current_MW_State = CANCEL;
                Pressed_Key = Key_INIT;
                return;
            }
            else if((Pressed_Key != Key_INIT)&&(Pressed_Key!= KEY_START))
            {
                Min_Unit_val = Pressed_Key;
                Current_MW_State = SCAN_SEC_TENS;           
                Pressed_Key = Key_INIT;
                Set_Time( Min_Ten_val , Min_Unit_val , Sec_Ten_val , Sec_Unit_val );                                               
            }
            break;

        case SCAN_SEC_TENS:
            Set_Disp_State(SCAN_SEC_TENS);
            if((Pressed_Key== KEY_START)&&(Time_Val != 0))
            {
                Current_MW_State = START_COUNT;
                Pressed_Key = Key_INIT;
                return;
            }
            else if (Pressed_Key == KEY_CANCEL)
            {
                Current_MW_State = CANCEL;
                Pressed_Key = Key_INIT;
                return;
            }
            else if((Pressed_Key != Key_INIT)&&(Pressed_Key!= KEY_START))
            {
                if(Pressed_Key >= key_6)
                {
                    Sec_Ten_val = key_5;
                }
                else
                {
                    Sec_Ten_val = Pressed_Key;
                }                       
                Current_MW_State = SCAN_SEC_UNITS;               
                Pressed_Key = Key_INIT;
                Set_Time( Min_Ten_val , Min_Unit_val , Sec_Ten_val , Sec_Unit_val );                        
            }
            break;

        case SCAN_SEC_UNITS:
            Set_Disp_State(SCAN_SEC_UNITS);
            if((Pressed_Key == KEY_START)&&(Time_Val != 0))
            {
                Current_MW_State = START_COUNT;
                Pressed_Key = Key_INIT;
                return;
            }
            else if (Pressed_Key == KEY_CANCEL)
            {
                Current_MW_State = CANCEL;
                Pressed_Key = Key_INIT;
                return;
            }
            else if((Pressed_Key != Key_INIT)&&(Pressed_Key!= KEY_START))
            {
                Sec_Unit_val = Pressed_Key;
                Set_Time( Min_Ten_val , Min_Unit_val , Sec_Ten_val , Sec_Unit_val ); 
            }
            
            break;

        case START_COUNT :  

            if (Pressed_Key == KEY_CANCEL)
            {
                if(Current_Count_State == Count_Time)
                {
                    Current_Count_State = Pause_Time;
                }
                else
                {
                    Current_Count_State = Reset_Time;
                }
                Current_MW_State = CANCEL;
                Pressed_Key = Key_INIT;
                return;
            }
            
            Set_Disp_State(START_COUNT);
            if(Weight_State != Pressed)
            {
                Current_Count_State = Wait_Food;
                return;
            }
            if (Door_State != Pressed)
            {  
                Current_Count_State = Wait_Door;
                return;
            }

            Current_Count_State = Count_Time;
            if(Is_Finished () == 1)
            {
                Current_Count_State = Finish_Time;
                Current_MW_State = FINISHED;
            }
            break;

        case CANCEL :

            Set_Disp_State(CANCEL);
            
             if(Current_Count_State == Reset_Time)
            {
                Min_Unit_val = 0;
                Min_Ten_val = 0;
                Sec_Unit_val = 0;
                Sec_Ten_val = 0;
                Set_Time( Min_Ten_val , Min_Unit_val , Sec_Ten_val , Sec_Unit_val ); 
                Current_MW_State = WELCOME;
                
                Weight_State = Released;
                Door_State = Released;
                Pressed_Key = Key_INIT;
            }
             else if(Current_Count_State == Pause_Time) 
            {
                if (Pressed_Key == KEY_START)
                {
                   Current_MW_State = START_COUNT;
                    Pressed_Key = Key_INIT;
                }
                if (Pressed_Key == KEY_CANCEL)
                {
                    Current_Count_State = Reset_Time;
                    Pressed_Key = Key_INIT;
                }
            }              
            break; 
            
        case FINISHED :
            
            Set_LCD_BUFFER1(" Heating         ");
            Set_LCD_BUFFER2("    Is Finished  ");   
            
            Weight_State = Released;
            Door_State = Released;
            break;

        default :
            break;
    }    

}