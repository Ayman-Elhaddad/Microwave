#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <xc.h> 


#include "Main.h"
#include "Port.h"

typedef enum 
{
    key_0,
    key_1,
    key_2,
    key_3,
    key_4,
    key_5,
    key_6,      
    key_7,
    key_8,
    key_9,
    KEY_CANCEL,
    KEY_START,
   Key_INIT       
            
}tKey;



void Keypad_Init (void);
void Keypad_Update (void);
tByte Keypad_Get_Data_From_buffer(tByte *pKey);


#endif