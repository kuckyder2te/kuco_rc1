#pragma once
/*  File name : display.h
    Project name : KuckyCopter
    Author: Wilhelm Kuckelsberg / Stephan Scholz
    Date : 2024-01-07

    Description : Drohne
*/

#include <Arduino.h>
#include <TaskManager.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "def.h"

#include "..\lib\model.h"

#define LOCAL_DEBUG
#include "myLogger.h"

class Display : public Task::Base
{

protected:
    Adafruit_ILI9341 *_tft;

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
        Serial.println("begin");
        _tft = new Adafruit_ILI9341(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_MOSI, PIN_TFT_CLK, PIN_TFT_RST, PIN_TFT_MISO);

        _tft->begin();

        delay(100);
        _tft->setRotation(2);
        _tft->setCursor(30, 20);
        _tft->fillScreen(ILI9341_LIGHTGREY);
        _tft->setTextColor(ILI9341_RED);
        _tft->setTextSize(4);
        _tft->println("KuckyCopter begin");

        delay(2000);

    //    read diagnostics (optional but can help debug problems)
        uint8_t x = _tft->readcommand8(ILI9341_RDMODE);
        Serial.print("Display Power Mode: 0x");
        Serial.println(x, HEX);
        x = _tft->readcommand8(ILI9341_RDMADCTL);
        Serial.print("MADCTL Mode: 0x");
        Serial.println(x, HEX);
        x = _tft->readcommand8(ILI9341_RDPIXFMT);
        Serial.print("Pixel Format: 0x");
        Serial.println(x, HEX);
        x = _tft->readcommand8(ILI9341_RDIMGFMT);
        Serial.print("Image Format: 0x");
        Serial.println(x, HEX);
        x = _tft->readcommand8(ILI9341_RDSELFDIAG);
        Serial.print("Self Diagnostic: 0x");
        Serial.println(x, HEX);

Serial.println("begin end");
    } /*--------------------- end of begin -----------------------------------------------------*/

    virtual void update() override
    {
        _tft->setCursor(0, 0);
        _tft->setTextColor(ILI9341_BLUE);
        _tft->setTextSize(1);
        _tft->println("KuckyCopter");
        
        Serial.println("KuckyCopter");
        Serial3.println("KuckyCopter on BT");

        // {
        //     LOGGER_NOTICE("Display fly screen");
        // }

    } /*--------------------- end of update -----------------------------------------------------*/


};
/*------------------------- end of display class ------------------------------------------------*/
