#ifndef __LCD_H__
#define __LCD_H__


#include "Main.h"

#define LCD_CONFIG_WORD             (0x03)
#define LCD_LINE_2_ADDRESS          (0x40)
#define LCD_SET_CGRAM_ADDRESS       (0x40)
#define LCD_SET_DDRAM_ADDRESS       (0x80)

#define LCD_LINE_1          (0)
#define LCD_LINE_2          (1)
#define LCD_LINE_WIDTH      (16)


void LCD_Init(void);
void LCD_SendChar(tByte character, tByte line, tByte column);
void LCD_SendString(tByte line, tByte * str , tByte col);
void LCD_Clear(void);
void LCD_Update (void );
void Set_LCD_BUFFER1 (tByte *BUF_1 );
void Set_LCD_BUFFER2 (tByte *BUF_2 );


#endif