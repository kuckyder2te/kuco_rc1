
#pragma once
#ifndef MY_SENSORS_H
#define MY_SENSORS_H

#include <TaskManager.h>
#include <ezButton.h>

//#define LOCAL_DEBUG
#include "myLogger.h"

// #include "..\lib\model.h"

#define PIN_THROTTLE A0
#define PIN_YAW      A2
#define PIN_PITCH    A6
#define PIN_ROLL     A4

#define PIN_ALTITUDE    A8
#define PIN_ALTITUDE_US A10

#define PIN_BUTTON_1 A12
#define PIN_BUTTON_2 A14
#define PIN_BUTTON_3 32
#define PIN_BUTTON_4 34
#define PIN_BUTTON_5 36

#define PIN_SWITCH_1 38
#define PIN_SWITCH_2 40
#define PIN_SWITCH_3 42
#define PIN_SWITCH_4 44
#define PIN_SWITCH_5 46

#define PIN_BUZZER 10

#define PIN_BATTERY A1

typedef struct
{
    int throttle, yaw, pitch, roll;
    int battery;
    int altitude;
    int altitude_us;
    // int btn1State, btn2State, btn3State, btn4State, btn5State;
    int swi1State, swi2State, swi3State;
} interfaceSensor_t;

ezButton button1(PIN_BUTTON_1);
ezButton button2(PIN_BUTTON_2);
ezButton button3(PIN_BUTTON_3);
ezButton button4(PIN_BUTTON_4);
ezButton button5(PIN_BUTTON_5);

ezButton switch1(PIN_SWITCH_1);
ezButton switch2(PIN_SWITCH_2);
ezButton switch3(PIN_SWITCH_3);
ezButton switch4(PIN_SWITCH_4);
ezButton switch5(PIN_SWITCH_5);

ezButton buttonArray[] = {
    ezButton(PIN_BUTTON_1),
    ezButton(PIN_BUTTON_2),
    ezButton(PIN_BUTTON_3),
    ezButton(PIN_BUTTON_4),
    ezButton(PIN_BUTTON_5)};

ezButton switchArray[] = {
    ezButton(PIN_SWITCH_1),
    ezButton(PIN_SWITCH_2),
    ezButton(PIN_SWITCH_3),
    ezButton(PIN_SWITCH_4),
    ezButton(PIN_SWITCH_5)};

#define BUTTON_NUM 5
#define SWITCH_NUM 5

#define PIN_CLK 23
#define PIN_DIN 25
#define PIN_DC  27
#define PIN_CE_NOKIA  29 
#define PIN_RST 31

Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_CE_NOKIA, PIN_RST);

class Sensors : public Task::Base
{

private:
    interfaceSensor_t *_interfaceSensor;
    int diff;
    int mid = 24;

public:
    Sensors(const String &name)
        : Task::Base(name)
    {
    }

    virtual ~Sensors(){}

    Sensors *setModel(interfaceSensor_t *_model)
    {
        LOGGER_VERBOSE("Enter....");
        _interfaceSensor = _model;
        LOGGER_VERBOSE("....leave");
        return this;
    }

    virtual void begin() override
    {

        for (byte i = 0; i < BUTTON_NUM; i++)
        {
            buttonArray[i].setDebounceTime(50); // set debounce time to 50 milliseconds
        }

        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            switchArray[i].setDebounceTime(50); // set debounce time to 50 milliseconds
        }

        display.begin();
        delay(100);
        display.clearDisplay();
        display.display();
        display.setContrast(60);
        display.display();
        display.setTextSize(2);
        display.setTextColor(BLACK);
        display.setCursor(0, 0);
        display.println("Kucky");
        display.setCursor(10, 25);
        display.println("Copter");
        display.display();
        delay(500);
        display.clearDisplay();
    }//---------------------- end of begin ------------------------------------------------------//

    virtual void update() override
    {
        // map is considering +/- 100 %
        _interfaceSensor->throttle = map((analogRead(PIN_THROTTLE)), 0, 1023, -100, 100);
        _interfaceSensor->yaw = map((analogRead(PIN_YAW)), 0, 1023,-100, 100);
        _interfaceSensor->pitch = map((analogRead(PIN_PITCH)), 0, 1023, -100, 100); // max. 15.0° must be diveded by 10 on Receiver end
        _interfaceSensor->roll = map((analogRead(PIN_ROLL)), 0, 1023, -100, 100);
        _interfaceSensor->altitude = map((analogRead(PIN_ALTITUDE)), 0, 1013, 0, 100);
        _interfaceSensor->altitude_us = map(analogRead(PIN_ALTITUDE_US), 0, 1023, 0, 100);

        LOGGER_NOTICE_FMT("Throttle = %i Yaw = %i Pitch = %i Roll = %i", _interfaceSensor->throttle, _interfaceSensor->yaw,
                          _interfaceSensor->pitch, _interfaceSensor->roll);

        LOGGER_FATAL_FMT("Altitude = %i Altitude US = %i", _interfaceSensor->altitude, _interfaceSensor->altitude_us);

        for (byte i = 0; i < BUTTON_NUM; i++)
            buttonArray[i].loop(); // MUST call the loop() function first

        for (byte i = 0; i < BUTTON_NUM; i++)
        {
            if (buttonArray[i].isPressed())
            {
                LOGGER_NOTICE_FMT("The button %i ispressed", i + 1);
            }

            if (buttonArray[i].isReleased())
            {
                LOGGER_NOTICE_FMT("The button %i released", i + 1);
            }
        }

        for (byte i = 0; i < SWITCH_NUM; i++)
            switchArray[i].loop(); // MUST call the loop() function first

        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            if (switchArray[i].isPressed())
            {
                LOGGER_NOTICE_FMT("The switch %i ispressed", i + 1);
            }

            if (switchArray[i].isReleased())
            {
                LOGGER_NOTICE_FMT("The switch %i released", i + 1);
            }
        }

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor(0, 0);
        display.println("Throttle: ");
        display.setCursor(55, 0);
        display.println(_interfaceSensor->throttle);

        display.setCursor(0, 10);
        display.println("Roll    : ");
        display.setCursor(55, 10);
        display.println(_interfaceSensor->roll);
        int rollLine = map(_interfaceSensor->roll, -15, +15, 0, display.height() - 1);
       // Serial.print("rollLine ");Serial.println(rollLine);

        diff = rollLine-mid;
       // Serial.print("diff ");Serial.println(diff);
        //display.drawLine(0, 0, display.width() - 1, 47, BLACK);
        display.drawLine(0, rollLine + diff, display.width() - 1, rollLine - diff, BLACK);
        //Serial.print("rollLine+diff ");Serial.println(rollLine+diff);
        // Serial.print("rollLine-diff ");Serial.println(rollLine-diff);

        display.setCursor(0, 20);
        display.println("Pitch   : ");
        display.setCursor(55, 20);
        display.println(_interfaceSensor->pitch);
        int pitchLine = map(_interfaceSensor->pitch, -15, 15, display.height() - 1, 0);
        // Serial.print("pitchLine ");Serial.println(_interfaceSensor->pitch);
        display.drawLine(0, pitchLine, display.width() - 1, pitchLine, BLACK);

        display.setCursor(0, 30);
        display.println("Yaw     : ");
        display.setCursor(55, 30);
        display.println(_interfaceSensor->yaw);

        display.setCursor(0, 40);
        display.print("Mode ");
        display.setCursor(55, 40);
        display.println(_interfaceSensor->swi1State);

        display.display();

        // interfaceSensor.battery = analogRead(PIN_BATTERY);
        // LOGGER_NOTICE_FMT("Battery has %i Volt",interfaceSensor.battery);
    }//---------------------- end of update ------------------------------------------------------//
};

#endif // MY_SENSORS_H
