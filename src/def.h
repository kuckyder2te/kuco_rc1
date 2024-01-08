#pragma once
/*  File name : def.h
    Project name : KuCo_Phantom 1
    Author: Wilhelm Kuckelsberg / Stephan Scholz
    Date : 2023-09-06

    Description : RC

*/

/* generally */
#define ZERO_WINDOW 40
#define ANALOG_MAX (100 + ZERO_WINDOW)

#define COM_SPEED 115200
#define BT_SPEED  115200

/* Alerts and controlls */
#define PIN_BUZZER   10
#define PIN_BATTERY  A1
#define LED_MAINLOOP 4  // yellow
#define LED_ALERT   10  // red
#define LED_RADIO    6  // blue

/* Potis */
#define PIN_DISTANCE_DOWN  A8
#define PIN_DISTANCE_FRONT A10

/* Joystick */
#define PIN_THROTTLE A0
#define PIN_YAW      A2
#define PIN_PITCH    A4
#define PIN_ROLL     A6

/* Option switches */
#define SWITCH_NUM 6
#define PIN_SWITCH_1a A12   // Switch Controller
#define PIN_SWITCH_1b A14
#define PIN_SWITCH_2a 32
#define PIN_SWITCH_2b 34
#define PIN_SWITCH_3a 36
#define PIN_SWITCH_3b 38

/*Radio*/
#define PIN_CE      49
#define PIN_CSN     53

/*Display*/
#define PIN_TFT_CLK 29
#define PIN_TFT_DC  25
#define PIN_TFT_CS  21
#define PIN_TFT_RST 23
#define PIN_TFT_MOSI 27
#define PIN_TFT_MISO 31

#define DISPLAY_DELAY 1000







// typedef enum
// {
//     hold_position,
//     autonom_mode,
//     adjust_on,
//     fly_mode
// } switch_e;