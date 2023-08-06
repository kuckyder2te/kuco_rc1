
#pragma once
#ifndef MY_SENSORS_H
#define MY_SENSORS_H

#include <TaskManager.h>
#include <ezButton.h>

//#define LOCAL_DEBUG
#include "myLogger.h"

// #include "..\lib\model.h"

#define PIN_THROTTLE A2
#define PIN_YAW      A0
#define PIN_PITCH    A4
#define PIN_ROLL     A6

#define PIN_ALTITUDE    A8
#define PIN_ALTITUDE_DOWN A10

#define PIN_BUTTON_1 36
#define PIN_BUTTON_2 32
#define PIN_BUTTON_3 34
#define PIN_BUTTON_4 A14
#define PIN_BUTTON_5 A12

#define PIN_SWITCH_1 38
#define PIN_SWITCH_2 40
#define PIN_SWITCH_3 42
#define PIN_SWITCH_4 44
#define PIN_SWITCH_5 46

#define NOKIA_CLK   23
#define NOKIA_DIN   25
#define NOKIA_DC    27
#define NOKIA_CS    29
#define NOKIA_RST   31

#define PIN_BUZZER 10

#define PIN_BATTERY A1

typedef struct
{
    int throttle, yaw, pitch, roll;
    int battery;
    int altitude;
    int altitude_down ;
    int swi1State, swi2State, swi3State;
} interfaceController_t;

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

// #define PIN_CLK 23
// #define PIN_DIN 25
// #define PIN_DC  27
// #define PIN_CE_NOKIA  29 
// #define PIN_RST 31

// Adafruit_PCD8544 display = Adafruit_PCD8544(NOKIA_CLK, NOKIA_DIN, NOKIA_DC, NOKIA_CS, NOKIA_RST);

class Controller : public Task::Base
{

private:
    interfaceController_t *_interfaceController;
    // int diff;
    // int mid = 24;

public:
    Controller(const String &name)
        : Task::Base(name)
    {
    }

    virtual ~Controller(){}

    Controller *setModel(interfaceController_t *_model)
    {
        LOGGER_VERBOSE("Enter....");
        _interfaceController = _model;
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

        // display.begin();
        // delay(100);
        // display.clearDisplay();
        // display.display();
        // display.setContrast(60);
        // display.display();
        // display.setTextSize(2);
        // display.setTextColor(BLACK);
        // display.setCursor(0, 0);
        // display.println("Kucky");
        // display.setCursor(10, 25);
        // display.println("Copter");
        // display.display();
        // delay(500);
        // display.clearDisplay();
    }//---------------------- end of begin ------------------------------------------------------//

    virtual void update() override
    {
        // map is considering +/- 100 %
        _interfaceController->throttle = map((analogRead(PIN_THROTTLE)), 0, 1023, -100, 100);
        _interfaceController->yaw = map((analogRead(PIN_YAW)), 0, 1023,-100, 100);
        _interfaceController->pitch = map((analogRead(PIN_PITCH)), 0, 1023, -100, 100); // max. 15.0° must be diveded by 10 on Receiver end
        _interfaceController->roll = map((analogRead(PIN_ROLL)), 0, 1023, -100, 100);
        _interfaceController->altitude = map((analogRead(PIN_ALTITUDE)), 0, 1013, 0, 100);
        _interfaceController->altitude_down = map(analogRead(PIN_ALTITUDE_DOWN), 0, 1023, 0, 200);
        _interfaceController->battery = map(analogRead(PIN_BATTERY), 0, 1023, 0, 100);  

        LOGGER_NOTICE_FMT("Throttle = %i Yaw = %i Pitch = %i Roll = %i", _interfaceController->throttle, _interfaceController->yaw,
                          _interfaceController->pitch, _interfaceController->roll);

        LOGGER_NOTICE_FMT("Altitude = %i Altitude US = %i", _interfaceController->altitude, _interfaceController->altitude_down);

        LOGGER_NOTICE_FMT("Battery = %i", _interfaceController->battery);

     //   alert();


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

    //     display.clearDisplay();
    //     display.setTextSize(1);
    //     display.setTextColor(BLACK);
    //     display.setCursor(0, 0);
    //     display.println("Throttle: ");
    //     display.setCursor(55, 0);
    //     display.println(_interfaceController->throttle);

    //     display.setCursor(0, 10);
    //     display.println("Roll    : ");
    //     display.setCursor(55, 10);
    //     display.println(_interfaceController->roll);
    //     int rollLine = map(_interfaceController->roll, -15, +15, 0, display.height() - 1);
    //    // Serial.print("rollLine ");Serial.println(rollLine);

    //     diff = rollLine-mid;
    //    // Serial.print("diff ");Serial.println(diff);
    //     //display.drawLine(0, 0, display.width() - 1, 47, BLACK);
    //     display.drawLine(0, rollLine + diff, display.width() - 1, rollLine - diff, BLACK);
    //     //Serial.print("rollLine+diff ");Serial.println(rollLine+diff);
    //     // Serial.print("rollLine-diff ");Serial.println(rollLine-diff);

    //     display.setCursor(0, 20);
    //     display.println("Pitch   : ");
    //     display.setCursor(55, 20);
    //     display.println(_interfaceController->pitch);
    //     int pitchLine = map(_interfaceController->pitch, -15, 15, display.height() - 1, 0);
    //     // Serial.print("pitchLine ");Serial.println(_interfaceController->pitch);
    //     display.drawLine(0, pitchLine, display.width() - 1, pitchLine, BLACK);

    //     display.setCursor(0, 30);
    //     display.println("Yaw     : ");
    //     display.setCursor(55, 30);
    //     display.println(_interfaceController->yaw);

    //     display.setCursor(0, 40);
    //     display.print("Mode ");
    //     display.setCursor(55, 40);
    //     display.println(_interfaceController->swi1State);

    //     display.display();

        // interfaceSensor.battery = analogRead(PIN_BATTERY);
        // LOGGER_NOTICE_FMT("Battery has %i Volt",interfaceSensor.battery);
    }//---------------------- end of update ------------------------------------------------------//

    void alert(){
        int lastMillis = millis();
        if(_interfaceController->battery > 50)
             if(millis()-lastMillis > 1000){
                 digitalWrite(PIN_BUZZER, HIGH);
                 lastMillis = millis();
             }
             else
                digitalWrite(PIN_BUZZER, LOW); 
        else   
            digitalWrite(PIN_BUZZER, LOW); 
    }//---------------------- end of alert ------------------------------------------------------//
};

#endif // MY_CONTRLLER_H
