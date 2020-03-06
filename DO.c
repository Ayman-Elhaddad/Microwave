#include "Main.h"
#include "Port.h"
#include "DO.h"

#define Lamp_Toggle_PERIOD  (200)

void DO_Init(void)
{

    GPIO_InitPortPin(HEATER_PORT_CR, HEATER_PIN , GPIO_OUT);
    GPIO_InitPortPin(MOTOR_PORT_CR, MOTOR_PIN , GPIO_OUT);
    GPIO_InitPortPin(LAMP_1_PORT_CR, LAMP_1_PIN , GPIO_OUT);
    GPIO_InitPortPin(LAMP_2_PORT_CR, LAMP_2_PIN , GPIO_OUT);
    
    
    GPIO_WritePortPin(HEATER_PORT_DR, HEATER_PIN , OFF);
    GPIO_WritePortPin(MOTOR_PORT_DR, MOTOR_PIN , OFF);
    GPIO_WritePortPin(LAMP_1_PORT_DR, LAMP_1_PIN , OFF);
    GPIO_WritePortPin(LAMP_2_PORT_DR, LAMP_2_PIN , OFF);

}

void DO_SetState(tDO device ,tDO_State state)
{
    switch (device)
    {
        case Heater :
            GPIO_WritePortPin(HEATER_PORT_DR, HEATER_PIN , state);
            break;
            
        case Motor :
            GPIO_WritePortPin(MOTOR_PORT_DR, MOTOR_PIN , state);
            break;
            
        case Lamp_1 :
            GPIO_WritePortPin(LAMP_1_PORT_DR, LAMP_1_PIN , state);
            break;
   
        case Lamp_2 :
            GPIO_WritePortPin(LAMP_2_PORT_DR, LAMP_2_PIN , state);
            break;            
            
        default :
            break;
    }
            
}


void Toggle_Lamps (void)
{
    static tByte Lamp_counter = 0;
    Lamp_counter += TMR_TICK_MS;
    if(Lamp_counter == (Lamp_Toggle_PERIOD / 2))
    {
        DO_SetState(Lamp_1 , OFF);
        DO_SetState(Lamp_2 , ON);
    }
    if(Lamp_counter == Lamp_Toggle_PERIOD )
    {
        DO_SetState(Lamp_1 , ON);
        DO_SetState(Lamp_2 , OFF);   
        
        Lamp_counter = 0;
        return;
    }
}