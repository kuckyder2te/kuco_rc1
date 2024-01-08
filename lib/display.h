#pragma once
/*  File name : display.h
    Project name : KuckyCopter
    Author: Wilhelm Kuckelsberg / Stephan Scholz
    Date : 2024-01-07

    Description : Drohne
*/

#include <Arduino.h>
#include <TaskManager.h>
#include <Adafruit_ILI9341.h>
#include "def.h"

#include "..\lib\model.h"

//#define LOCAL_DEBUG
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
        _tft = new Adafruit_ILI9341(PIN_TFT_CLK, PIN_TFT_DC, PIN_TFT_CS, PIN_TFT_RST, PIN_TFT_MOSI, PIN_TFT_MISO);

        _tft->begin();
        
          // read diagnostics (optional but can help debug problems)
        uint8_t x = _tft->readcommand8(ILI9341_RDMODE);
        Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
        x = _tft->readcommand8(ILI9341_RDMADCTL);
        Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
        x = _tft->readcommand8(ILI9341_RDPIXFMT);
        Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
        x = _tft->readcommand8(ILI9341_RDIMGFMT);
        Serial.print("Image Format: 0x"); Serial.println(x, HEX);
        x = _tft->readcommand8(ILI9341_RDSELFDIAG);
        Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
        
        Serial.println(F("Benchmark                Time (microseconds)"));
        delay(10);
        
        Serial.print(F("Text                     "));
        Serial.println(testText());
        delay(3000);

        Serial.print(F("Lines                    "));
        Serial.println(testLines(ILI9341_CYAN));
        delay(500);

    } /*--------------------- end of begin -----------------------------------------------------*/

    virtual void update() override
    {
        // if (_keyboard->swiState[switch_e::adjust_on])
        // {
        //     LOGGER_NOTICE("Display adjust screen");
        //     adjust_screen();
        // }
        // else
        {
            LOGGER_NOTICE("Display fly screen");

        }

    } /*--------------------- end of update -----------------------------------------------------*/

    unsigned long testLines(uint16_t color) {
    unsigned long start, t;
    int           x1, y1, x2, y2,
                    w = _tft->width(),
                    h = _tft->height();

    _tft->fillScreen(ILI9341_BLACK);
    yield();
    
    x1 = y1 = 0;
    y2    = h - 1;
    start = micros();
    for(x2=0; x2<w; x2+=6) _tft->drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for(y2=0; y2<h; y2+=6) _tft->drawLine(x1, y1, x2, y2, color);
    t     = micros() - start; // fillScreen doesn't count against timing

    yield();
    _tft->fillScreen(ILI9341_BLACK);
    yield();

    x1    = w - 1;
    y1    = 0;
    y2    = h - 1;
    start = micros();
    for(x2=0; x2<w; x2+=6) _tft->drawLine(x1, y1, x2, y2, color);
    x2    = 0;
    for(y2=0; y2<h; y2+=6) _tft->drawLine(x1, y1, x2, y2, color);
    t    += micros() - start;

    yield();
    _tft->fillScreen(ILI9341_BLACK);
    yield();

    x1    = 0;
    y1    = h - 1;
    y2    = 0;
    start = micros();
    for(x2=0; x2<w; x2+=6) _tft->drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for(y2=0; y2<h; y2+=6) _tft->drawLine(x1, y1, x2, y2, color);
    t    += micros() - start;

    yield();
    _tft->fillScreen(ILI9341_BLACK);
    yield();

    x1    = w - 1;
    y1    = h - 1;
    y2    = 0;
    start = micros();
    for(x2=0; x2<w; x2+=6) _tft->drawLine(x1, y1, x2, y2, color);
    x2    = 0;
    for(y2=0; y2<h; y2+=6) _tft->drawLine(x1, y1, x2, y2, color);

    yield();
    return micros() - start;
    }//-------------------------- end of testLines --------------------------------------------------//

    unsigned long testText() {
    _tft->fillScreen(ILI9341_BLACK);
    unsigned long start = micros();
    _tft->setCursor(0, 0);
    _tft->setTextColor(ILI9341_WHITE);  _tft->setTextSize(1);
    _tft->println("KuckyCopter");
    _tft->setTextColor(ILI9341_YELLOW); _tft->setTextSize(2);
    _tft->println(1234.56);
    _tft->setTextColor(ILI9341_RED);    _tft->setTextSize(3);
    _tft->println(0xDEADBEEF, HEX);
    _tft->println();
    _tft->setTextColor(ILI9341_GREEN);
    _tft->setTextSize(5);
    _tft->println("Groop");
    _tft->setTextSize(2);
    _tft->println("I implore thee,");
    _tft->setTextSize(1);
    _tft->println("my foonting turlingdromes.");
    _tft->println("And hooptiously drangle me");
    _tft->println("with crinkly bindlewurdles,");
    _tft->println("Or I will rend thee");
    _tft->println("in the gobberwarts");
    _tft->println("with my blurglecruncheon,");
    _tft->println("see if I don't!");
    return micros() - start;
    }//-------------------------- end of testText --------------------------------------------------//

};
/*------------------------- end of display class ------------------------------------------------*/
