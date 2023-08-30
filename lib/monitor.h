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
#include "config.h"

#include "model.h"

#define LOCAL_DEBUG
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

protected:
    Adafruit_PCD8544 *_nokia;
    Controller *_controller;

private:
    model_t *_model;
    unsigned long _lastMillis;
    uint16_t _display_delay;
    Report_t _report;

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
    } /*--------------------- end of setModel --------------------------------------------------*/

    virtual void begin() override
    {

        _controller = new Controller("controller");
        _nokia = new Adafruit_PCD8544(PIN_NOKIA_CLK, PIN_NOKIA_DIN, PIN_NOKIA_DC, PIN_NOKIA_CS, PIN_NOKIA_RST);

        _nokia->begin();
        _nokia->setContrast(60);
        _nokia->display();
        delay(100);
        _nokia->clearDisplay();
        //     _nokia->drawBitmap(0, 0, KuCo_logo, 16, 16, 1);
        _nokia->display();

        _nokia->display();
        _nokia->setTextSize(2);
        _nokia->setTextColor(BLACK);
        _nokia->setCursor(0, 0);
        _nokia->println("Kucky");
        _nokia->setCursor(10, 25);
        _nokia->println("Copter");
        _nokia->display();
        delay(2000);
        _nokia->clearDisplay();
        Serial.println("******** Channel description *******");
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

    } /*--------------------- end of begin -----------------------------------------------------*/

    virtual void update() override
    {
        static uint32_t lastIntervall = millis();
        static bool toogle;
    
//        if(_controller->switchAdjust){

            _nokia->clearDisplay();

            _nokia->setTextSize(2);
            _nokia->setTextColor(BLACK);
            _nokia->setCursor(0, 0);
            _nokia->println("Adjust");
            _nokia->display();

            int test =_controller->getThrottle();

        //    LOGGER_NOTICE_FMT("getThrottle %i", test);

            _nokia->setTextSize(1);
            _nokia->setCursor(5, 20);
            _nokia->println(test);
            _nokia->display();

  //      }

        // if (millis() - _lastMillis > _display_delay)
        // {
        //     _lastMillis = millis();

        //     switch (_report)
        //     {

        //     case RADIO:
        //         Serial.println("RC RADIO");
        //         sprintf(strBuf, "/*%i, %i, %i, %i, %i, %i, %i, %i, %i, %i*/\r\n",
        //                 (int16_t)_model->RC_interface.RX_payload.yaw,
        //                 (int16_t)_model->RC_interface.RX_payload.pitch,
        //                 (int16_t)_model->RC_interface.RX_payload.roll,
        //                 _model->RC_interface.RX_payload.altitude,
        //                 _model->RC_interface.RX_payload.distance_down,
        //                 _model->RC_interface.RX_payload.distance_front,
        //                 (int16_t)_model->RC_interface.RX_payload.pressure,
        //                 (int16_t)_model->RC_interface.RX_payload.temperature,
        //                 _model->RC_interface.RX_payload.battery,
        //                 (uint8_t)_model->RC_interface.isconnect);
        //         Serial.print(strBuf);         // auskommentiert wegen leerer Daten

        //         // if (millis() - lastIntervall > 5000)
        //         // {
        //         //     toogle = !toogle;
        //         //     lastIntervall = millis();
        //         // }

        //         // if (toogle)
        //         // {
        //              display_screen_1();
        //         // }
        //         // else
        //         // {
        //         //     display_screen_2();
        //         // }
        //         break;

        //     case CONTROLLER:
        //     //    Serial.println("CONTROLLER");
        //         LOGGER_NOTICE_FMT("Switch 1", _model->controllers.swi1State());
        //         LOGGER_NOTICE_FMT("Switch 2", _model->controllers.swi2State());
        //         LOGGER_NOTICE_FMT("Switch 3", _model->controllers.swi3State());
        //         LOGGER_NOTICE_FMT("Switch 4", _model->controllers.swi4State());
        //         break;
        //     default:
        //         Serial.println("Default");
        //         break;
        //     }
        // }

    } /*--------------------- end of update -----------------------------------------------------*/

    void display_screen_1()
    {
        _nokia->clearDisplay();
        _nokia->setTextSize(1);
        _nokia->setTextColor(BLACK);

        _nokia->setCursor(0, 0);
        _nokia->println("Yaw");
        _nokia->setCursor(50, 0);
        _nokia->println(":");
        _nokia->setCursor(56, 0);
        _nokia->println(_model->RC_interface.RX_payload.yaw, 0);

        _nokia->setCursor(0, 9);
        _nokia->println("Pitch");
        _nokia->setCursor(50, 9);
        _nokia->println(":");
        _nokia->setCursor(56, 9);
        _nokia->println(_model->RC_interface.RX_payload.pitch, 0);

        _nokia->setCursor(0, 18);
        _nokia->println("Roll");
        _nokia->setCursor(50, 18);
        _nokia->println(":");   
        _nokia->setCursor(56, 18);
        _nokia->println(_model->RC_interface.RX_payload.roll, 0);

        _nokia->setCursor(0, 27);
        _nokia->print("Temp.");
        _nokia->setCursor(50, 27);
        _nokia->println(":");   
        _nokia->setCursor(56, 27);
        _nokia->println(_model->RC_interface.RX_payload.temperature);

        _nokia->setCursor(0, 36);
        _nokia->print("Pressure");
        _nokia->setCursor(50, 36);
        _nokia->println(":");   
        _nokia->setCursor(56, 36);
        _nokia->println(_model->RC_interface.RX_payload.pressure, 0);
        _nokia->display();

    } /*--------------------- end of display_screen_1 -------------------------------------------*/

    void display_screen_2()
    {
        _nokia->clearDisplay();
        _nokia->setTextSize(1);
        _nokia->setTextColor(BLACK);

        _nokia->setCursor(0, 0);
        _nokia->println("Altitude");
        _nokia->setCursor(50, 0);
        _nokia->println(":");        
        _nokia->setCursor(56, 0);
        _nokia->println(_model->RC_interface.RX_payload.altitude, 0);
    
        _nokia->setCursor(0, 13);
        _nokia->println("Ground");
        _nokia->setCursor(50, 13);
        _nokia->println(":");   
        _nokia->setCursor(56, 13);
        _nokia->println(_model->RC_interface.RX_payload.distance_down, 0);

        _nokia->setCursor(0, 26);
        _nokia->println("Front");
        _nokia->setCursor(50, 26);
        _nokia->println(":");   
        _nokia->setCursor(56, 26);
        _nokia->println(_model->RC_interface.RX_payload.distance_front, 0);

        _nokia->setCursor(0, 39);
        _nokia->println("Battery");
        _nokia->setCursor(50, 39);
        _nokia->println(":");   
        _nokia->setCursor(56, 39);
        _nokia->println(_model->RC_interface.RX_payload.battery);
        _nokia->display();

    } /*--------------------- end of display_screen_2 -------------------------------------------*/

    void set(int x){
        LOGGER_NOTICE_FMT("Throttle %i", x);
    }
};
/*------------------------- end of monitor class ------------------------------------------------*/
