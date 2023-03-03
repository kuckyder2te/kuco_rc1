/*  File name: model.h
    Project name: KuCo_Phantom 1
    Date: 2022-05-31
    Author: Wilhelm Kuckelsberg
    Description: Global constance
*/
#pragma once

// #include "sensors.h"
// #include "sonic.h"
#include "radio.h"
#include "sensors.h"
// #include "performance.h"
// #include "axisYaw.h"
// #include "axisMotor.h"
// #include "battery.h"
// #include "newPID.h" 

typedef struct
{
    interfaceSensor_t interfaceSensor;
    RC_interface_t RC_interface;
} model_t;