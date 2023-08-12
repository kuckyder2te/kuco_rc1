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

#define TEST

// Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_NOKIA_CLK, PIN_NOKIA_DIN, PIN_NOKIA_DC, PIN_NOKIA_CS, PIN_NOKIA_RST);

// static const unsigned char PROGMEM KuCo_logo[] =
// {
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x80, 0xC0, 0xC0, 0xE0, 0x70, 0x70, 0xF8, 0xFC, 0xFA, 0x81, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0,
// 0xF8, 0xF0, 0xE0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x80, 0x80, 0x80, 0xFC, 0xF8, 0xF8, 0xE0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x20, 0x40, 0xC0, 0x80,
// 0x08, 0x04, 0x02, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x07, 0x07,
// 0x07, 0x0F, 0x0F, 0x0E, 0x1E, 0x1E, 0x3C, 0x3C, 0x3C, 0x78, 0xF8, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF,
// 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xF8, 0xF8, 0x38, 0x3C, 0x3C,
// 0x1C, 0x1E, 0x0E, 0x0E, 0x0F, 0x07, 0x07, 0x07, 0x03, 0x03, 0x07, 0x03, 0x00, 0x01, 0x01, 0x03,
// 0x07, 0x06, 0x08, 0x10, 0x00, 0x80, 0xC0, 0xE0, 0x30, 0x18, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0xF9, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0,
// 0xF8, 0xF8, 0xF8, 0x7C, 0x7C, 0x7C, 0x7E, 0x3E, 0x3E, 0x3F, 0x1E, 0x1E, 0x1E, 0x0F, 0x0F, 0xFF,
// 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
// 0x0F, 0x0E, 0x1E, 0x1E, 0x1F, 0x3E, 0x3E, 0x7E, 0x7E, 0x7C, 0x7C, 0xFC, 0xF8, 0xF8, 0xF8, 0xF0,
// 0xF0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF8, 0xFC, 0xFF, 0xFD, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0F, 0x7F, 0xFF, 0x87, 0x07, 0x03, 0x03, 0x03,
// 0x03, 0x03, 0x03, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
// 0x3F, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x01, 0x09, 0x0F, 0x0F, 0x07, 0x03, 0x03, 0x03, 0x07, 0xCF, 0xFF, 0x1F, 0x07, 0x02,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F,
// 0x7C, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x07, 0x07, 0x07,
// 0x07, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x7E, 0x07,
// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// };

// static const unsigned char PROGMEM KuCo_logo[] =
// { 0B00000000, 0B11000000,
//   0B00000001, 0B11000000,
//   0B00000001, 0B11000000,
//   0B00000011, 0B11100000,
//   0B11110011, 0B11100000,
//   0B11111110, 0B11111000,
//   0B01111110, 0B11111111,
//   0B00110011, 0B10011111,
//   0B00011111, 0B11111100,
//   0B00001101, 0B01110000,
//   0B00011011, 0B10100000,
//   0B00111111, 0B11100000,
//   0B00111111, 0B11110000,
//   0B01111100, 0B11110000,
//   0B01110000, 0B01110000,
//   0B00000000, 0B00110000 };

char strBuf[100];

#define PIN_NOKIA_CLK 23
#define PIN_NOKIA_DIN 25
#define PIN_NOKIA_DC 27
#define PIN_NOKIA_CS 29
#define PIN_NOKIA_RST 31

#define DISPLAY_DELAY 1000

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
        : Task::Base(name) {
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
    } /*--------------------- end of setModel -----------------------------------------*/

    virtual void begin() override
    {

        Serial.println("Monitor begin");

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
        switch(_report){
            case RADIO:
                Serial.println("yaw,pitch,roll,altitude,distance_down,distance_front,pressure,temperature,battery,isconnected");
            break;
            default:
                Serial.println("Please add Channel descriptin in monitor.begin()");
            break;
        }
        Serial.println("********************************");

    } /*--------------------- end of begin -----------------------------------------*/

    virtual void update() override
    {
        if (millis() - _lastMillis > _display_delay)
        {
            _lastMillis = millis();

            switch (_report)
            {

            case RADIO:

                sprintf(strBuf, "/*%i, %i, %i, %i, %i, %i, %i, %i, %i, %i*/\r\n",
                        (int16_t)_model->RC_interface.RX_payload.yaw,
                        (int16_t)_model->RC_interface.RX_payload.pitch,
                        (int16_t)_model->RC_interface.RX_payload.roll,
                        _model->RC_interface.RX_payload.altitude,
                        _model->RC_interface.RX_payload.distance_down,
                        _model->RC_interface.RX_payload.distance_front,
                        (int16_t)_model->RC_interface.RX_payload.pressure,
                        (int16_t)_model->RC_interface.RX_payload.temperature,
                        _model->RC_interface.RX_payload.battery,
                        (uint8_t)_model->RC_interface.isconnect
                        );
                Serial.print(strBuf);

                _nokia->clearDisplay();
                _nokia->setTextSize(1);
                _nokia->setTextColor(BLACK);

                _nokia->setCursor(0, 0);
                _nokia->println("Yaw :");
                _nokia->setCursor(40, 0);
                _nokia->println(_model->RC_interface.RX_payload.yaw);
                _nokia->display();

                _nokia->setCursor(0, 8);
                _nokia->println("Pitch :");
                _nokia->setCursor(40, 8);
                _nokia->println(_model->RC_interface.RX_payload.pitch);
                _nokia->display();

                _nokia->setCursor(0, 16);
                _nokia->println("Roll  :");
                _nokia->setCursor(55, 16);
                _nokia->println(_model->RC_interface.RX_payload.roll);
                _nokia->display();

                _nokia->setCursor(0, 24);
                _nokia->println("Alti. :");
                _nokia->setCursor(55, 24);
                _nokia->println(_model->RC_interface.RX_payload.altitude);
                _nokia->display();

                _nokia->setCursor(0, 32);
                _nokia->print("ground  :");
                _nokia->setCursor(55, 32);
                _nokia->println(_model->RC_interface.RX_payload.distance_down);
                _nokia->display();
                break;

            case CONTROLLER:

                break;
            default:
                Serial.println("Default");
                break;
            }
        }

    } /*--------------------- end of upDate -----------------------------------------*/
};
/*------------------------- end of monitor class -----------------------------------*/
