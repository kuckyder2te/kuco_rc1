#pragma once
/*  File name : def.h
    Project name : KuCo_Phantom 1
    Author: Wilhelm Kuckelsberg
    Date : 2023-09-06

    Description : RC

*/

/*Radio*/
#define PIN_CE  49
#define PIN_CSN 53

#define ZERO_WINDOW 40
#define ANALOG_MAX (100 + ZERO_WINDOW)


typedef enum
{
    hold_position,
    autonom_mode,
    adjust_on,
    fly_mode
} switch_e;