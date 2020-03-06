
#include <xc.h>
#include <string.h>
#include "LCD.h"
#include "Main.h"
#include "Port.h"
#include "TMR0.h"

#define _XTAL_FREQ 8000000

static void LCD_SendData(tByte character);
static void LCD_GoTo(tByte line, tByte column);
static void LCD_Port(tByte Data);
static void LCD_SendCommand(tByte command);

static tByte LCD_LINE_1_Buffer[16];
static tByte LCD_LINE_2_Buffer[16];


void LCD_Init(void)
{
    /* Initialize Pins */
    GPIO_InitPortPin(LCD_DATA_PORT_CR, GPIO_PIN_4, GPIO_OUT);
    GPIO_InitPortPin(LCD_DATA_PORT_CR, GPIO_PIN_5, GPIO_OUT);
    GPIO_InitPortPin(LCD_DATA_PORT_CR, GPIO_PIN_6, GPIO_OUT);
    GPIO_InitPortPin(LCD_DATA_PORT_CR, GPIO_PIN_7, GPIO_OUT);
  
    GPIO_WritePortPin(LCD_DATA_PORT_DR, GPIO_PIN_4, 0);
    GPIO_WritePortPin(LCD_DATA_PORT_DR, GPIO_PIN_5, 0);
    GPIO_WritePortPin(LCD_DATA_PORT_DR, GPIO_PIN_6, 0);
    GPIO_WritePortPin(LCD_DATA_PORT_DR, GPIO_PIN_7, 0);
   
    GPIO_InitPortPin(LCD_RS_CR, LCD_RS_PIN, GPIO_OUT);
    GPIO_InitPortPin(LCD_E_CR, LCD_E_PIN, GPIO_OUT);
 

    LCD_SendCommand(LCD_CONFIG_WORD);
    __delay_us(100);
    LCD_SendCommand(LCD_CONFIG_WORD);
    __delay_us(50);
    LCD_SendCommand(LCD_CONFIG_WORD);

    /* Initial LCD commands */
    /*start operation in 4 bit mode*/
    LCD_SendCommand(0x02);
    /*select number of display lines & character font*/
	LCD_SendCommand(0x02);
    LCD_SendCommand(0x08);
   /* Turn display , cursor move */
    LCD_SendCommand(0x00);
    LCD_SendCommand(0x0C);
	/*clear display*/
    LCD_SendCommand(0x00);
	/*set entry mode */
    LCD_SendCommand(0x06);
}

static void LCD_SendCommand(tByte command)
{
    //send on edge
    
    GPIO_WritePortPin(LCD_RS_DR, LCD_RS_PIN, 0);
    LCD_Port(command);
    GPIO_WritePortPin(LCD_E_DR, LCD_E_PIN, 1);
    __delay_us(10);
    GPIO_WritePortPin(LCD_E_DR, LCD_E_PIN, 0);


}

void LCD_SendChar(tByte character, tByte line, tByte column)
{

    LCD_GoTo(line, column+1 );
    LCD_SendData(character);

}
void LCD_SendString(tByte line , tByte * str  , tByte col)
{
    tByte i = 0;
    while (str[i] != '\0')
    {
        LCD_SendChar(str[i], line, i+col );
        i++;
    }    
}

static void LCD_SendData(tByte character)
{

    tByte Low_4,High_4;

    Low_4  = character & 0x0F;
    High_4 = character & 0xF0;

    GPIO_WritePortPin(LCD_RS_DR, LCD_RS_PIN,1);
    LCD_Port(High_4>>4) ;
    GPIO_WritePortPin(LCD_E_DR, LCD_E_PIN, 1);
    __delay_us(10);
    GPIO_WritePortPin(LCD_E_DR, LCD_E_PIN, 0);
    __delay_us(10);

    LCD_Port(Low_4) ;
    GPIO_WritePortPin(LCD_E_DR, LCD_E_PIN, 1);
    __delay_us(10);
    GPIO_WritePortPin(LCD_E_DR, LCD_E_PIN, 0);
    __delay_us(10);

}


static void LCD_Port(tByte Data)
{
    if(Data & 1)
        GPIO_WritePortPin(GPIO_PORTD_DATA, GPIO_PIN_4, 1);
    else
        GPIO_WritePortPin(GPIO_PORTD_DATA, GPIO_PIN_4, 0);
    if(Data & 2)
        GPIO_WritePortPin(GPIO_PORTD_DATA, GPIO_PIN_5, 1);
    else
        GPIO_WritePortPin(GPIO_PORTD_DATA, GPIO_PIN_5, 0);
    if(Data & 4)
        GPIO_WritePortPin(GPIO_PORTD_DATA, GPIO_PIN_6, 1);
    else
        GPIO_WritePortPin(GPIO_PORTD_DATA, GPIO_PIN_6, 0);
    if(Data & 8)
        GPIO_WritePortPin(GPIO_PORTD_DATA, GPIO_PIN_7, 1);
    else
        GPIO_WritePortPin(GPIO_PORTD_DATA, GPIO_PIN_7, 0);
}

void LCD_Clear(void)
{
    LCD_SendCommand(0);
    LCD_SendCommand(1);
}

static void LCD_GoTo(tByte line, tByte column)
{
    char address = 0;	
	tByte Low_4,High_4;
    
  /* Calculate address in DDRAM */
    address = (line * LCD_LINE_2_ADDRESS) + column - 1;
    High_4 = (LCD_SET_DDRAM_ADDRESS|address) >> 4;
    Low_4  = (LCD_SET_DDRAM_ADDRESS|address) & 0x0F;
  
  /* Send address to LCD */
    LCD_SendCommand(High_4);
    LCD_SendCommand(Low_4);

}

void LCD_Update (void)
{
    static tByte LCD_Update_Counter = 20;
    LCD_Update_Counter += TMR_TICK_MS;
    if(LCD_Update_Counter == 30)
    {
        LCD_SendString(LCD_LINE_1 , LCD_LINE_2_Buffer , 0);
    }
    if(LCD_Update_Counter == 60)
    {
        LCD_SendString(LCD_LINE_2 , LCD_LINE_1_Buffer , 0);
        LCD_Update_Counter = 0;
    }    
}

void Set_LCD_BUFFER1 (tByte *BUF_1 )
{
   strcpy(LCD_LINE_2_Buffer ,BUF_1);
    
}
void Set_LCD_BUFFER2 (tByte *BUF_2 )
{
   strcpy(LCD_LINE_1_Buffer ,BUF_2);
    
}