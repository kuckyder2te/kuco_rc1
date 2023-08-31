#pragma once
/*  File name : display.h
    Project name : KuCo_Phantom 1
    Author: Wilhelm Kuckelsberg
    Date : 2022-

    Description : Drohne

*/

#include <Arduino.h>
#include <TaskManager.h>
#include "config.h"

#include "..\lib\model.h"

#define LOCAL_DEBUG
#include "myLogger.h"


class Display : public Task::Base
{

protected:
    Adafruit_PCD8544 *_nokia;
//    Display *_display;
    
private:
    model_t *_model;
    unsigned long _lastMillis;

public:
    /// @brief Constructor
    /// @param name
    /// @param report
    /// @param delay
    Display(const String &name)
        : Task::Base(name)
    {
 
    }
    /// @brief
    /// @param _mod
    /// @return Reference of himself for multiple function call
    Display *setModel(model_t *_mod)
    { // Rückgabe wert ist das eigene Objekt (this)
        LOGGER_VERBOSE("Enter....");
        _model = _mod;
        LOGGER_VERBOSE("....leave");
        return this;
    } /*--------------------- end of setModel --------------------------------------------------*/

    virtual void begin() override
    {
        _nokia = new Adafruit_PCD8544(PIN_NOKIA_CLK, PIN_NOKIA_DIN, PIN_NOKIA_DC, PIN_NOKIA_CS, PIN_NOKIA_RST);

        // _nokia->begin();
        // _nokia->setContrast(60);
        // _nokia->display();
        // delay(100);
        // _nokia->clearDisplay();
        
        // _nokia->display();

        // _nokia->display();
        // _nokia->setTextSize(2);
        // _nokia->setTextColor(BLACK);
        // _nokia->setCursor(0, 0);
        // _nokia->println("Kucky");
        // _nokia->setCursor(10, 25);
        // _nokia->println("Copter");
        // _nokia->display();
        // delay(2000);
        // _nokia->clearDisplay();

    } /*--------------------- end of begin -----------------------------------------------------*/

    virtual void update() override
    {
            // _nokia->clearDisplay();

            // _nokia->setTextSize(2);
            // _nokia->setTextColor(BLACK);
            // _nokia->setCursor(0, 0);
            // _nokia->println("Adjust");
            // _nokia->display();

            // int test =_controller->_throttleOffset;

            // LOGGER_NOTICE_FMT_CHK(test, __test, "getThrottle %i", test);

            // _nokia->setTextSize(1);
            // _nokia->setCursor(5, 20);
            // _nokia->println(test);
            // _nokia->display();

    } /*--------------------- end of update -----------------------------------------------------*/
};
/*------------------------- end of monitor class ------------------------------------------------*/
