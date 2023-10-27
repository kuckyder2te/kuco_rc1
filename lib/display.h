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
#include "def.h"

#include "..\lib\model.h"

//#define LOCAL_DEBUG
#include "myLogger.h"

class Display : public Task::Base
{

protected:
    Adafruit_PCD8544 *_nokia;

private:
    model_t *_model;
    unsigned long _lastMillis;

public:
    keyboard_t *_keyboard;

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
    Display *setModel(model_t *model)
    { // Rückgabe wert ist das eigene Objekt (this)
        LOGGER_VERBOSE("Enter....");
        _model = model;
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
        if (_keyboard->swiState[switch_e::adjust_on])
        {
            LOGGER_NOTICE("Display adjust screen");
            adjust_screen();
        }
        else
        {
            LOGGER_NOTICE("Display fly screen");
            print_FlyScreen();
        }

    } /*--------------------- end of update -----------------------------------------------------*/

    void adjust_screen()
    {
        _nokia->clearDisplay();

        _nokia->setTextSize(1);
        _nokia->setTextColor(BLACK);
        _nokia->setCursor(10, 0);
        _nokia->println("Adjust JS");
        _nokia->display();

        _nokia->setCursor(0, 10);
        _nokia->println("Throttle:");
        _nokia->setCursor(60, 10);
        _nokia->println(_model->keyboard.throttle);
        _nokia->display();

        _nokia->setCursor(0, 20);
        _nokia->println("YAW:");
        _nokia->setCursor(60, 20);
        _nokia->println(_model->keyboard.yaw);
        _nokia->display();

        _nokia->setCursor(0, 30);
        _nokia->println("Pitch:");
        _nokia->setCursor(60, 30);
        _nokia->println(_model->keyboard.pitch);
        _nokia->display();

        _nokia->setCursor(0, 40);
        _nokia->println("Roll:");
        _nokia->setCursor(60, 40);
        _nokia->println(_model->keyboard.roll);
        _nokia->display();
    } //---------------------- end of adjust_screen ------------------------------------------------------//

    void print_FlyScreen()
    {
            _nokia->clearDisplay();

        _nokia->setTextSize(1);
        _nokia->setTextColor(BLACK);
        _nokia->setCursor(10, 0);
        _nokia->println("Flyscreen");
        _nokia->display();

        _nokia->setCursor(0, 10);
        _nokia->println("Throttle:");
        _nokia->setCursor(60, 10);
        _nokia->println(_model->RC_interface.RX_payload.altitude); 
        _nokia->display();

        _nokia->setCursor(0, 20);
        _nokia->println("YAW:");
        _nokia->setCursor(60, 20);
        _nokia->println(_model->RC_interface.TX_payload.rcAltitudeSonicAdj);
        _nokia->display();

        _nokia->setCursor(0, 30);
        _nokia->println("Pitch:");
        _nokia->setCursor(60, 30);
        _nokia->println(_model->keyboard.pitch);
        _nokia->display();

        _nokia->setCursor(0, 40);
        _nokia->println("Roll:");
        _nokia->setCursor(60, 40);
        _nokia->println(_model->keyboard.roll);
        _nokia->display();
    } //---------------------- end of printFlyScreen ------------------------------------------------------//
};
/*------------------------- end of display class ------------------------------------------------*/
