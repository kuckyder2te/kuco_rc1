
#pragma once
#ifndef MY_SENSORS_H
#define MY_SENSORS_H

#include <TaskManager.h>
#include <ezButton.h>
#include "config.h"

#define LOCAL_DEBUG
#include "myLogger.h"


typedef struct
{
    int throttle, yaw, pitch, roll;
    int battery;
    int altitude;
    int altitude_down ;
    //int swi1State, swi2State, swi3State, swi4State;
    bool switchState[SWITCH_NUM];
} controllers_t;

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
     //   _controllers->battery = map(analogRead(PIN_BATTERY), 0, 1023, 0, 100);  
        _controllers->battery = analogRead(PIN_BATTERY);  

        // LOGGER_NOTICE_FMT("Throttle = %i Yaw = %i Pitch = %i Roll = %i", _controllers->throttle, _controllers->yaw,
        //                   _controllers->pitch, _controllers->roll);

        // LOGGER_NOTICE_FMT("Altitude = %i Altitude US = %i", _controllers->altitude, _controllers->altitude_down);

    //    LOGGER_NOTICE_FMT("Battery = %i", _controllers->battery);

     //   alert();

        
        for (byte i = 0; i < SWITCH_NUM; i++)
            switchArray[i].loop(); // MUST call the loop() function first

        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            if (switchArray[i].isPressed())
            {
                LOGGER_NOTICE_FMT("The switch %i ispressed", i + 1);
                _controllers->switchState[1] = true;
                LOGGER_NOTICE_FMT(" Switch %i",_controllers->switchState[i]);

            }

            if (switchArray[i].isReleased())
            {
                LOGGER_NOTICE_FMT("The switch %i released", i + 1);
                _controllers->switchState[i] = false;
                LOGGER_NOTICE_FMT(" Switch %i",_controllers->switchState[i]);

            }
        }

        // for (byte i = 0; i < SWITCH_NUM; i++)
        //     switchArray[i].loop(); // MUST call the loop() function first

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

        // for (byte i = 0; i < SWITCH_NUM; i++)
        // {
        //     if (switchArray[i].isPressed())
        //     {
        //         LOGGER_NOTICE_FMT("The switch %i ispressed", i + 1);           
        //     }

        //     if (switchArray[i].isReleased())
        //     {
        //         LOGGER_NOTICE_FMT("The switch %i released", i + 1);
        //     }
        // }
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
