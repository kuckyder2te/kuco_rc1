#pragma once
/*  File name : monitor.h
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

typedef enum
{
    RADIO,
    CONTROLLER,
    DEFAULTS
} Report_t;

class Monitor : public Task::Base
{

private:
    model_t *_model;
    unsigned long _lastMillis;
    uint16_t _display_delay;
    Report_t _report;
    int8_t __test;

public:
    /// @brief Constructor
    /// @param name
    /// @param report
    /// @param delay
    Monitor(const String &name, Report_t report = Report_t::DEFAULTS, uint16_t delay = DISPLAY_DELAY)
        : Task::Base(name)
    {
        _lastMillis = millis();
        _report = report;
        _display_delay = delay;
    }
    /// @brief
    /// @param _mod
    /// @return Reference of himself for multiple function call
    Monitor *setModel(model_t *_mod)
    { // Rückgabe wert ist das eigene Objekt (this)
        LOGGER_VERBOSE("Enter....");
        _model = _mod;
        LOGGER_VERBOSE("....leave");
        return this;
    } /*--------------------- end of setModel ---------------------------------------------------*/

    virtual void begin() override
    {
        switch (_report)
        {
        case RADIO:
            Serial.println("yaw,pitch,roll,altitude,distance_down,distance_front,pressure,temperature,battery,isconnected");
            break;
        default:
            Serial.println("Please add Channel descriptin in monitor.begin()");
            break;
        }
        Serial.println("********************************");

    } /*--------------------- end of begin ------------------------------------------------------*/

    virtual void update() override
    {
        if (millis() - _lastMillis > _display_delay)
        {
            _lastMillis = millis();

            switch (_report)
            {
            case RADIO:
                Serial.println("RC RADIO");
                sprintf(strBuf, "/*%i, %i, %i, %i, %i, %i, %i, %i, %i, %i*/\r\n",
                        _model->RC_interface.RX_payload.yaw,
                        _model->RC_interface.RX_payload.secondaryAxis,
                        _model->RC_interface.RX_payload.primaryAxis,
                        _model->RC_interface.RX_payload.altitude,
                        _model->RC_interface.RX_payload.distance_down,
                        _model->RC_interface.RX_payload.distance_front,
                        (int16_t)_model->RC_interface.RX_payload.pressure,
                        (int16_t)_model->RC_interface.RX_payload.temperature,
                        _model->RC_interface.RX_payload.battery,
                        (uint8_t)_model->RC_interface.isconnect);
                Serial.print(strBuf);

            case CONTROLLER:
                Serial.println("CONTROLLER");
                break;
            default:
                Serial.println("Default");
                break;
            }
        }
    } /*--------------------- end of update -----------------------------------------------------*/
};
/*------------------------- end of monitor class ------------------------------------------------*/
