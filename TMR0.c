#include "Main.h"
#include "Port.h"
#include "TMR0.h"
#include "LCD.h"
#include "keypad.h"
#include "Time.h"
#include "Disp.h"
#include "SW.h"
#include "MW.h"


void TMR_Init(void)
{
	TMR_SET_PRESCALER_8;
	TMR_DISABLE;
}
void TMR_Start(void)
{
	TMR_CLEAR_FLAG;
	TMR_UPDATE_REGISTER(TMR_TICK_MS);
	TMR_ENABLE_INTERRUPT;
	GLOBAL_INTERRUPT_ENABLE;
	TMR_ENABLE;
}
tByte TMR_CheckOverFlow(void)
{
	return TMR_GET_FLAG;
}
void TMR_Stop(void)
{
	TMR_DISABLE_CLOCK;
}

void __interrupt() TMR_Update(void) 
{
	TMR_CLEAR_FLAG;
	TMR_UPDATE_REGISTER(TMR_TICK_MS);

    Keypad_Update();
    SW_Update();
    MW_Update();
    Disp_Update();
    TIM_Update();
    LCD_Update();

}
