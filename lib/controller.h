
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

#define PIN_BUTTON_1 36   // Button Controller 0 - 4
#define PIN_BUTTON_2 34
#define PIN_BUTTON_3 32
#define PIN_BUTTON_4 A14
#define PIN_BUTTON_5 A12

#define PIN_SWITCH_1 40   // Scwitch Controller 5 - 8
#define PIN_SWITCH_2 42
#define PIN_SWITCH_3 44
#define PIN_SWITCH_4 46

#define PIN_BUZZER 10

#define PIN_BATTERY A1

typedef struct
{
    int throttle, yaw, pitch, roll;
    int battery;
    int altitude;
    int altitude_down ;
    int swi1State, swi2State, swi3State, swi4State;
} controllers_t;

// ezButton button1(PIN_BUTTON_1);
// ezButton button2(PIN_BUTTON_2);
// ezButton button3(PIN_BUTTON_3);
// ezButton button4(PIN_BUTTON_4);
// ezButton button5(PIN_BUTTON_5);

// ezButton switch1(PIN_SWITCH_1);
// ezButton switch2(PIN_SWITCH_2);
// ezButton switch3(PIN_SWITCH_3);
// ezButton switch4(PIN_SWITCH_4);

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
    ezButton(PIN_SWITCH_4)};

#define BUTTON_NUM 5
#define SWITCH_NUM 4

class Controller : public Task::Base
{

private:
    controllers_t *_controllers;

public:
    Controller(const String &name)
        : Task::Base(name)
    {
    }

    Controller *setModel(controllers_t *_model)
    {
        LOGGER_VERBOSE("Enter....");
        _controllers = _model;
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
    }//---------------------- end of begin ------------------------------------------------------//

    virtual void update() override
    {
        // map is considering +/- 100 %
        _controllers->throttle = map((analogRead(PIN_THROTTLE)), 0, 1023, -100, 100);
        _controllers->yaw = map((analogRead(PIN_YAW)), 0, 1023,-100, 100);
        _controllers->pitch = map((analogRead(PIN_PITCH)), 0, 1023, -100, 100); // max. 15.0° must be diveded by 10 on Receiver end
        _controllers->roll = map((analogRead(PIN_ROLL)), 0, 1023, -100, 100);
        _controllers->altitude = map((analogRead(PIN_ALTITUDE)), 0, 1013, 0, 100);
        _controllers->altitude_down = map(analogRead(PIN_ALTITUDE_DOWN), 0, 1023, 0, 200);
        _controllers->battery = map(analogRead(PIN_BATTERY), 0, 1023, 0, 100);  

        LOGGER_NOTICE_FMT("Throttle = %i Yaw = %i Pitch = %i Roll = %i", _controllers->throttle, _controllers->yaw,
                          _controllers->pitch, _controllers->roll);

        // LOGGER_NOTICE_FMT("Altitude = %i Altitude US = %i", _controllers->altitude, _controllers->altitude_down);

        // LOGGER_NOTICE_FMT("Battery = %i", _controllers->battery);

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

        // _controllers->swi1State = switchArray[0].getState();
        //     LOGGER_NOTICE_FMT("The switch1 state is %i ", _controllers->swi1State);
        // _controllers->swi2State = switchArray[1].getState();
        //     LOGGER_NOTICE_FMT("The switch2 state is %i ", _controllers->swi2State);
        // _controllers->swi3State = switchArray[2].getState();
        //     LOGGER_NOTICE_FMT("The switch3 state is %i ", _controllers->swi3State);
        // _controllers->swi4State = switchArray[3].getState();
        //     LOGGER_NOTICE_FMT("The switch4 state is %i ", _controllers->swi4State);
        // _controllers->swi5State = switchArray[4].getState();
        //     LOGGER_NOTICE_FMT("The switch5 state is %i ", _controllers->swi5State);

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
        // _controllers.battery = analogRead(PIN_BATTERY);
        // LOGGER_NOTICE_FMT("Battery has %i Volt",_controllers.battery);
    }//---------------------- end of update ------------------------------------------------------//

    void alert(){
        int lastMillis = millis();
        if(_controllers->battery > 50)
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

#endif // MY_CONTROLLER_H
