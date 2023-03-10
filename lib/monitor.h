#pragma once
/*  File name : 
    Project name : KuCo_Phantom 1
    Author: Wilhelm Kuckelsberg
    Date : 2022-

    Description : Drohne
 
*/

#include <Arduino.h>
#include <TaskManager.h>
#include <print.h>
#include <stdio.h>

#include "model.h"

//#define LOCAL_DEBUG
#include "myLogger.h"

char strBuf[100];

class Monitor : public Task::Base {
private:
    model_t *_model;
public:
    Monitor(const String& name)
    : Task::Base(name) {
    }

    virtual ~Monitor() {}


    Monitor *setModel(model_t *_mod)
    { // Rückgabe wert ist das eigene Objekt (this)
        LOGGER_VERBOSE("Enter....");
        _model = _mod;
        LOGGER_VERBOSE("....leave");
        return this;
    } /*--------------------- end of setModel -----------------------------------------*/


    // optional (you can remove this method)
    // virtual void begin() override {
    // }

    virtual void update() override {
    /*
    IMU-> Yaw,Pitch,Roll
    Sonic-> Distance
    Radio-> 
    Baro->
    Motor
    */

        sprintf(strBuf, "/*%i, %i, %i, %i, %i, %i*/\r\n",
            _model->interfaceSensor.throttle,
            _model->interfaceSensor.yaw,
            _model->interfaceSensor.pitch,
            _model->interfaceSensor.roll,            
            _model->interfaceSensor.altitude,
            _model->interfaceSensor.altitude_us);
        Serial.print(strBuf);
    }
};
