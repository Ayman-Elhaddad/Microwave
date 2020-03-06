#include "Main.h"
#include "Port.h"
#include "TMR0.h"
#include "SW.h"

#include <xc.h>

/* SW_Update period */
#define SW_PERIOD_MS   (20)

/* Number of samples per SW */
#define N_SAMPLES      (2)

#define N_SWITCHES      (2)

/* SW voltage level */
#define SW_PRESSED_LEVEL        (0)
#define SW_RELEASED_LEVEL       (1)



/* SW info: two samples, state */
typedef struct
{
    tByte sw_samples[N_SAMPLES];
    tSW_State sw_state;
}tSW_Info;

/* Switches info */

static tSW_Info SWs_Info[N_SWITCHES];


void SW_Init(tSW sw)
{
    /* Initialize switches as inputs */
    switch(sw)
    {
        case DOOR_SW:
            /* set the pin direction*/
            GPIO_InitPortPin(DOOR_SW_PORT_CR, DOOR_SW_PIN, GPIO_IN);
            /* Initialize switch samples */
            SWs_Info[DOOR_SW].sw_samples[0] = SW_RELEASED_LEVEL;
            SWs_Info[DOOR_SW].sw_samples[1] = SW_RELEASED_LEVEL;

            /* Initialize sw state */
            SWs_Info[DOOR_SW].sw_state = SW_RELEASED;
            
            break;
            
        case WEIGHT_SW:
            /* set the pin direction*/
            GPIO_InitPortPin(WEIGHT_SW_PORT_CR, WEIGHT_SW_PIN, GPIO_IN);
            /* Initialize switch samples */
            SWs_Info[WEIGHT_SW].sw_samples[0] = SW_RELEASED_LEVEL;
            SWs_Info[WEIGHT_SW].sw_samples[1] = SW_RELEASED_LEVEL;

            /* Initialize sw state */
            SWs_Info[WEIGHT_SW].sw_state = SW_RELEASED;
            
            break;

        default :
            break;
    }
    
}


tSW_State SW_GetState(tSW sw)
{
    /* Add your code here! */
    /* return sw state */
    return SWs_Info[sw].sw_state;
    /* End of your code!*/
}


/* Update >> 1- check timing
2 - update samples
3 - update state */

void SW_Update(void)
{
    static tWord SW_counter = 0;
    tByte index = 0;

    /* Check if it is time for the SW_Update to run */
    SW_counter += TMR_TICK_MS;

    if (SW_counter != SW_PERIOD_MS){
        return;
    }

    SW_counter = 0;

    /* Update switches states */
    for (index = DOOR_SW; index < N_SWITCHES; index++)
    {
        /* Update switch samples */
        SWs_Info[index].sw_samples[0] = SWs_Info[index].sw_samples[1];

        if (index == DOOR_SW)
        {
            SWs_Info[index].sw_samples[1] = GPIO_ReadPortPin(DOOR_SW_PORT_DR, DOOR_SW_PIN);
        }
        else if (index == WEIGHT_SW)
        {
            SWs_Info[index].sw_samples[1] = GPIO_ReadPortPin(WEIGHT_SW_PORT_DR, WEIGHT_SW_PIN);
        }
        else
        {
            /* Do nothing should not be here !! */
        }
        
        /* Update switch state */
        /* Add your code here! */
        switch(SWs_Info[index].sw_state)
        {

            case SW_RELEASED:
                /* Go to pre-pressed state if needed */
                if ((SWs_Info[index].sw_samples[0] == SW_PRESSED_LEVEL) &&
                    (SWs_Info[index].sw_samples[1] == SW_PRESSED_LEVEL))
                {
                    SWs_Info[index].sw_state = SW_PRE_PRESSED;
                } else
                {
                    /* Do nothing */
                }
                break;
            case SW_PRE_PRESSED:
                /* Go to pressed state if needed  */
                if (SWs_Info[index].sw_samples[1] == SW_PRESSED_LEVEL)
                {
                    SWs_Info[index].sw_state = SW_PRESSED;
                } else
                {
                    /* Do nothing */
                }
                break;
            case SW_PRESSED:
                /* Go to pre-released state if needed */
                if ((SWs_Info[index].sw_samples[0] == SW_RELEASED_LEVEL) &&
                    (SWs_Info[index].sw_samples[1] == SW_RELEASED_LEVEL))
                {
                    SWs_Info[index].sw_state = SW_PRE_RELEASED;
                }
                else
                {
                    /* Do nothing */
                }
                break;
            case SW_PRE_RELEASED:
                /* Go to released state if needed */
                if (SWs_Info[index].sw_samples[1] == SW_RELEASED_LEVEL)
                {
                    SWs_Info[index].sw_state = SW_RELEASED;
                } else
                {
                    /* Do nothing */
                }
                break;
            /* Should not be here */
            default:
                break;
        }
        
    }

}
