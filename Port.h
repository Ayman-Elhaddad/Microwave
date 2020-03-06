#ifndef __PORT_H__
#define __PORT_H__

#include "Main.h"

/*timer tick period*/
#define TMR_TICK_MS				(10)


/* Heater pin*/
#define HEATER_PORT_DR   (GPIO_PORTC_DATA)
#define HEATER_PORT_CR   (GPIO_PORTC_CONTROL)
#define HEATER_PIN       (GPIO_PIN_5)

/*Lamps pins*/
#define LAMP_1_PORT_DR   (GPIO_PORTB_DATA)
#define LAMP_1_PORT_CR   (GPIO_PORTB_CONTROL)
#define LAMP_1_PIN       (GPIO_PIN_7)

#define LAMP_2_PORT_DR   (GPIO_PORTB_DATA)
#define LAMP_2_PORT_CR   (GPIO_PORTB_CONTROL)
#define LAMP_2_PIN       (GPIO_PIN_6)

/*Motor pin*/
#define MOTOR_PORT_DR   (GPIO_PORTC_DATA)
#define MOTOR_PORT_CR   (GPIO_PORTC_CONTROL)
#define MOTOR_PIN       (GPIO_PIN_2)

/* Fill missing code below */
/* Switches for simulating "door & weight" sensors */
#define DOOR_SW_PORT_DR   (GPIO_PORTB_DATA)
#define DOOR_SW_PORT_CR   (GPIO_PORTB_CONTROL)
#define DOOR_SW_PIN       (GPIO_PIN_3)

#define WEIGHT_SW_PORT_DR   (GPIO_PORTB_DATA)
#define WEIGHT_SW_PORT_CR   (GPIO_PORTB_CONTROL)
#define WEIGHT_SW_PIN       (GPIO_PIN_4)



/* LCD */
#define LCD_DATA_PORT_DR    (GPIO_PORTD_DATA)
#define LCD_DATA_PORT_CR    (GPIO_PORTD_CONTROL)

#define LCD_RS_DR           (GPIO_PORTE_DATA)
#define LCD_RS_CR           (GPIO_PORTE_CONTROL)
#define LCD_RS_PIN          (GPIO_PIN_2)

#define LCD_E_DR           (GPIO_PORTE_DATA)
#define LCD_E_CR           (GPIO_PORTE_CONTROL)
#define LCD_E_PIN          (GPIO_PIN_1)


#define KEYPAD_OUTPUT_PORT_DR   (GPIO_PORTB_DATA)
#define KEYPAD_OUTPUT_PORT_CR   (GPIO_PORTB_CONTROL)

#define KEYPAD_OUTPUT_1_PIN           (GPIO_PIN_0)
#define KEYPAD_OUTPUT_2_PIN           (GPIO_PIN_1)
#define KEYPAD_OUTPUT_3_PIN           (GPIO_PIN_2)

#define KEYPAD_INPUT_PORT_DR   (GPIO_PORTD_DATA)
#define KEYPAD_INPUT_PORT_CR   (GPIO_PORTD_CONTROL)

#define KEYPAD_INPUT_1_PIN           (GPIO_PIN_3)
#define KEYPAD_INPUT_2_PIN           (GPIO_PIN_2)
#define KEYPAD_INPUT_3_PIN           (GPIO_PIN_1)
#define KEYPAD_INPUT_4_PIN           (GPIO_PIN_0)





#endif // __PORT_H__
