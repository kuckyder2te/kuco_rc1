/*  File name: model.h
    Project name: KuCo_Phantom 1
    Date: 2022-05-31
    Author: Wilhelm Kuckelsberg
    Description: Global constance
*/
#pragma once

#include "radio.h"
#include "controller.h"


typedef struct
{
    keyboard_t keyboard;
    RC_interface_t RC_interface;
} model_t;