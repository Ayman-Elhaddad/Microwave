/*
 * File:   keypad.c
 * Author: pc
 * Created on 17  2020, 11:44 ?
 */


#include "Main.h"
#include "Port.h"
#include "keypad.h"

#define _XTAL_FREQ 8000000
#define KEYPAD_PERIOD_MS (50)
#define Keypad_Recv_Buffer_Length (6)
#define KEYPAD_NO_NEW_DATA (15)

static tByte Keypad_Scan (tByte *pKey);

static tByte Keypad_Recv_Buffer [Keypad_Recv_Buffer_Length + 1];
static tByte KEYPAD_reading_index ;
static tByte KEYPAD_writing_index ;
static tByte Last_Valid_Key = KEYPAD_NO_NEW_DATA;

void Keypad_Init (void)
{
            
    GPIO_InitPortPin(KEYPAD_OUTPUT_PORT_CR , KEYPAD_OUTPUT_1_PIN, GPIO_OUT);
    GPIO_InitPortPin(KEYPAD_OUTPUT_PORT_CR , KEYPAD_OUTPUT_2_PIN, GPIO_OUT);
    GPIO_InitPortPin(KEYPAD_OUTPUT_PORT_CR , KEYPAD_OUTPUT_3_PIN, GPIO_OUT);
    
    GPIO_InitPortPin(KEYPAD_INPUT_PORT_CR , KEYPAD_INPUT_1_PIN, GPIO_IN);
    GPIO_InitPortPin(KEYPAD_INPUT_PORT_CR , KEYPAD_INPUT_2_PIN, GPIO_IN);
    GPIO_InitPortPin(KEYPAD_INPUT_PORT_CR , KEYPAD_INPUT_3_PIN, GPIO_IN);
    GPIO_InitPortPin(KEYPAD_INPUT_PORT_CR , KEYPAD_INPUT_4_PIN, GPIO_IN);
    
    KEYPAD_reading_index = 0;
    KEYPAD_writing_index = 0;

}


void Keypad_Update(void)
{
	tByte Key;
    static tWord Keypad_Update_Counter = 0;
    
    Keypad_Update_Counter += TMR_TICK_MS;
    if(Keypad_Update_Counter != KEYPAD_PERIOD_MS)
	{
        return ;
    }
    Keypad_Update_Counter = 0;
    
	if(Keypad_Scan (&Key) == 0)
    {
        //no new key pressed
        return ;
    }
    
    if(KEYPAD_reading_index == KEYPAD_writing_index)
    {
        KEYPAD_reading_index = 0;
        KEYPAD_writing_index = 0;       
    }
    
    //write data in buffer
    Keypad_Recv_Buffer[KEYPAD_writing_index] = Key;
    
    if(KEYPAD_writing_index < Keypad_Recv_Buffer_Length )
    {
        KEYPAD_writing_index ++;
    }
}

tByte Keypad_Get_Data_From_buffer(tByte *pKey)
{
    if(KEYPAD_reading_index < KEYPAD_writing_index)
    {
        //there are data in buffer
        
        *pKey = Keypad_Recv_Buffer[KEYPAD_reading_index];
        KEYPAD_reading_index ++;
        
        return 1;
    }
    
    //there are no any data in buffer
    return 0;
}

static tByte Keypad_Scan (tByte *pKey)
{
	static tByte Old_Key ;
	
	tByte Key = KEYPAD_NO_NEW_DATA;
	
        
    //scanning the first column
	GPIO_WritePortPin(KEYPAD_OUTPUT_PORT_DR , KEYPAD_OUTPUT_1_PIN, 0);
    GPIO_WritePortPin(KEYPAD_OUTPUT_PORT_DR , KEYPAD_OUTPUT_2_PIN, 1);
    GPIO_WritePortPin(KEYPAD_OUTPUT_PORT_DR , KEYPAD_OUTPUT_3_PIN, 1);
    
    __delay_us(50);
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_1_PIN) == 0)
    {
        Key = key_1;
    }
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_2_PIN) == 0)
    {
        Key = key_4;
    }
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_3_PIN) == 0)
    {
        Key = key_7;
    }
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_4_PIN) == 0)
    {
        Key = KEY_CANCEL;
    }
	
	GPIO_WritePortPin(KEYPAD_OUTPUT_PORT_DR , KEYPAD_OUTPUT_1_PIN, 1);
    GPIO_WritePortPin(KEYPAD_OUTPUT_PORT_DR , KEYPAD_OUTPUT_2_PIN, 0);
    GPIO_WritePortPin(KEYPAD_OUTPUT_PORT_DR , KEYPAD_OUTPUT_3_PIN, 1);
    __delay_us(50);

    //scanning the second column
    
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_1_PIN) == 0)
    {
        Key = key_2;
    }
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_2_PIN) == 0)
    {
        Key = key_5;
    }
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_3_PIN) == 0)
    {
        Key = key_8;
    }
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_4_PIN) == 0)
    {
        Key = key_0;
    }
	
	GPIO_WritePortPin(KEYPAD_OUTPUT_PORT_DR , KEYPAD_OUTPUT_2_PIN, 1);
    GPIO_WritePortPin(KEYPAD_OUTPUT_PORT_DR , KEYPAD_OUTPUT_2_PIN, 1);
    GPIO_WritePortPin(KEYPAD_OUTPUT_PORT_DR , KEYPAD_OUTPUT_3_PIN, 0);

    __delay_us(50);
    //scanning the third column
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_1_PIN) == 0)
    {
        Key = key_3;
    }
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_2_PIN) == 0)
    {
        Key = key_6;
    }
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_3_PIN) == 0)
    {
        Key = key_9;
    }
    if(GPIO_ReadPortPin(KEYPAD_INPUT_PORT_DR, KEYPAD_INPUT_4_PIN) == 0)
    {
        Key = KEY_START;
    }
	
    
    if(Key == KEYPAD_NO_NEW_DATA)
    {
        Old_Key = KEYPAD_NO_NEW_DATA;
        Last_Valid_Key = KEYPAD_NO_NEW_DATA;
        return 0;
    }
    if(Key == Old_Key)
    {
        //there is a pressed key
        
        if(Key != Last_Valid_Key)
        {
            //it's a new key 
            *pKey = Key;
            Last_Valid_Key = Key;
            return 1;
        }
        
    }
    
    Old_Key = Key;
    return 0;
	
}





