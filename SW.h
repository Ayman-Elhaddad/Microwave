#ifndef __SW_H__
#define __SW_H__

typedef enum
{
    DOOR_SW = 0,
    WEIGHT_SW   
}tSW;

typedef enum
{
    SW_RELEASED,
    SW_PRE_PRESSED,
    SW_PRESSED,
    SW_PRE_RELEASED
}tSW_State;

void SW_Init(tSW);
tSW_State SW_GetState(tSW sw);
void SW_Update(void);

#endif // __SW_H__
