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

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#include "def.h"

#include "..\lib\model.h"

#define LOCAL_DEBUG
#include "myLogger.h"

#define MENU_COL 20
#define MENU_ROW 40
#define ROW_GAB 20
#define COL_GAB 120

#define TFT_GREY 0x5AEB

class Display : public Task::Base
{

protected:
    // Adafruit_ILI9341 *_tft;
    TFT_eSPI *_tft;

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
    } /*--------------------- end of setModel ---------------------------------------------------*/

    virtual void begin() override
    {
        LOGGER_VERBOSE("enter....");
        static int menu_row = MENU_ROW;
        // _tft = new Adafruit_ILI9341(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_MOSI, PIN_TFT_CLK, PIN_TFT_RST, PIN_TFT_MISO);
        _tft = new TFT_eSPI();
        _tft->init();

        // read diagnostics (optional but can help debug problems)
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

        delay(2000);

        _tft->fillScreen(TFT_GREY);

        _tft->setTextColor(TFT_WHITE, TFT_GREY); // Adding a background colour erases previous text automatically

        _tft->drawCentreString("Kucky Copter 2", COL_GAB, 10, 4);
        _tft->setRotation(3);
        _tft->begin();
        _tft->setTextSize(2);
        _tft->setTextColor(TFT_YELLOW);
        _tft->drawString("YAW", MENU_COL, menu_row);
        _tft->drawString("Pitch :", MENU_COL, menu_row += ROW_GAB);
        _tft->drawString("Roll :", MENU_COL, menu_row += ROW_GAB);
        _tft->drawString("Altitude :", MENU_COL, menu_row += ROW_GAB);
        _tft->drawString("to ground :", MENU_COL, menu_row += ROW_GAB);
        _tft->drawString("Battery :", MENU_COL, menu_row += ROW_GAB);

        LOGGER_VERBOSE("....leave");
    } /*--------------------- end of begin ------------------------------------------------------*/

    virtual void update() override
    {
        LOGGER_VERBOSE("enter....");
        static int menu_row = MENU_ROW;

        _tft->setTextSize(2);
        _tft->setTextColor(TFT_YELLOW); // show values
        _tft->drawNumber(_model->RC_interface.TX_payload.rcThrottle, MENU_COL + COL_GAB, menu_row);
        _tft->drawNumber(_model->RC_interface.TX_payload.rcYaw, MENU_COL + COL_GAB, menu_row += ROW_GAB);
        _tft->drawNumber(_model->RC_interface.TX_payload.rcPitch, MENU_COL + COL_GAB, menu_row += ROW_GAB);
        _tft->drawNumber(_model->RC_interface.TX_payload.rcRoll, MENU_COL + COL_GAB, menu_row += ROW_GAB);
        _tft->drawNumber(_model->RC_interface.TX_payload.rcAltitudeBaroAdj, MENU_COL + COL_GAB, menu_row += ROW_GAB);
        _tft->drawNumber(_model->RC_interface.TX_payload.rcAltitudeSonicAdj, MENU_COL + COL_GAB, menu_row += ROW_GAB);

        _tft->setTextColor(TFT_GREY); // hide values
        _tft->drawNumber(_model->RC_interface.TX_payload.rcThrottle, MENU_COL + COL_GAB, menu_row);
        _tft->drawNumber(_model->RC_interface.TX_payload.rcYaw, MENU_COL + COL_GAB, menu_row += ROW_GAB);
        _tft->drawNumber(_model->RC_interface.TX_payload.rcPitch, MENU_COL + COL_GAB, menu_row += ROW_GAB);
        _tft->drawNumber(_model->RC_interface.TX_payload.rcRoll, MENU_COL + COL_GAB, menu_row += ROW_GAB);
        _tft->drawNumber(_model->RC_interface.TX_payload.rcAltitudeBaroAdj, MENU_COL + COL_GAB, menu_row += ROW_GAB);
        _tft->drawNumber(_model->RC_interface.TX_payload.rcAltitudeSonicAdj, MENU_COL + COL_GAB, menu_row += ROW_GAB);

        LOGGER_VERBOSE("....leave");
    } /*--------------------- end of update -----------------------------------------------------*/
};
/*------------------------- end of display class ------------------------------------------------*/
