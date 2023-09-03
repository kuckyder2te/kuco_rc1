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
    //    Display *_nokia;

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
    /// @param _model
    /// @return Reference of himself for multiple function call
    Display *setModel(model_t *_model)
    { // Rückgabe wert ist das eigene Objekt (this)
        LOGGER_VERBOSE("Enter....");
        _model = _model;
        LOGGER_VERBOSE("....leave");
        return this;
    } /*--------------------- end of setModel --------------------------------------------------*/

    virtual void begin() override
    {
        _nokia = new Adafruit_PCD8544(PIN_NOKIA_CLK, PIN_NOKIA_DIN, PIN_NOKIA_DC, PIN_NOKIA_CS, PIN_NOKIA_RST);

        _nokia->begin();
        _nokia->setContrast(60);
        _nokia->display();
        delay(100);
        _nokia->clearDisplay();

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

    } /*--------------------- end of begin -----------------------------------------------------*/

    virtual void update() override
    {
        _nokia->clearDisplay();

        _nokia->setTextSize(2);
        _nokia->setTextColor(BLACK);
        _nokia->setCursor(0, 0);
        _nokia->println("Adjust JS");
        _nokia->display();

        _nokia->setCursor(5, 20);
        _nokia->println("Throttle:");
        _nokia->setCursor(5, 40);
        _nokia->println(_model->controllers.throttle);

        _nokia->setCursor(15, 20);
        _nokia->println("YAW:");
        _nokia->setCursor(15, 40);
        _nokia->println(_model->controllers.yaw);

        _nokia->setCursor(20, 20);
        _nokia->println("Pitch:");
        _nokia->setCursor(20, 40);
        _nokia->println(_model->controllers.pitch);

        _nokia->setCursor(25, 20);
        _nokia->println("ROLL:");
        _nokia->setCursor(55, 40);
        _nokia->println(_model->controllers.roll);

        _nokia->display();

    } /*--------------------- end of update -----------------------------------------------------*/
};
/*------------------------- end of monitor class ------------------------------------------------*/
