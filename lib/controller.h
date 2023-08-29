
#pragma once
#ifndef MY_SENSORS_H
#define MY_SENSORS_H

#include <TaskManager.h>
#include <ezButton.h>
#include "config.h"

#include "..\lib\menu.h"

#define LOCAL_DEBUG
#include "myLogger.h"

typedef struct
{
    int throttle, yaw, pitch, roll;
    int battery;
    int altitude;
    int altitude_down;
    // int swi1State, swi2State, swi3State, swi4State;
    bool switchState[SWITCH_NUM];
} controllers_t;

typedef enum
{
    down = 0,   
    save,
    mode,
    set,
    up
} button_e;

typedef enum
{
    adjust_on = 0,   
    NN1,
    autonom,
    NN2
}switch_e;

ezButton buttonArray[] =
{
    ezButton(PIN_BUTTON_1),
    ezButton(PIN_BUTTON_2),
    ezButton(PIN_BUTTON_3),
    ezButton(PIN_BUTTON_4),
    ezButton(PIN_BUTTON_5)};

ezButton switchArray[] = 
{
    ezButton(PIN_SWITCH_1),
    ezButton(PIN_SWITCH_2),
    ezButton(PIN_SWITCH_3),
    ezButton(PIN_SWITCH_4)};

class Controller : public Task::Base
{

private:
    controllers_t *_controllers;
    button_e _button;
    switch_e _switch;

    Menu *_menuOption;
    uint8_t _mode;
    uint8_t _throttleOffset = 0;
    uint8_t _yawOffset = 0;
    uint8_t _pitchOffset = 0;
    uint8_t _rollOffset = 0;

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
        _menuOption = new Menu("menu");
        byte _mode = 10;

        for (byte i = 0; i < BUTTON_NUM; i++)
        {
            buttonArray[i].setDebounceTime(50); // set debounce time to 50 milliseconds
        }

        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            switchArray[i].setDebounceTime(50); // set debounce time to 50 milliseconds
        }
    } //---------------------- end of begin ------------------------------------------------------//

    virtual void update() override
    {
        static uint8_t count = 0;
        // map is considering +/- 100 %
        _controllers->throttle = map((analogRead(PIN_THROTTLE)), 0, 1023, -100, 100);
        _controllers->yaw = map((analogRead(PIN_YAW)), 0, 1023, -100, 100);
        _controllers->pitch = map((analogRead(PIN_PITCH)), 0, 1023, -100, 100); // max. 15.0° must be diveded by 10 on Receiver end
        _controllers->roll = map((analogRead(PIN_ROLL)), 0, 1023, -100, 100);
        _controllers->altitude = map((analogRead(PIN_ALTITUDE)), 0, 1013, 0, 100);
        _controllers->altitude_down = map(analogRead(PIN_ALTITUDE_DOWN), 0, 1023, 0, 200);
        //   _controllers->battery = map(analogRead(PIN_BATTERY), 0, 1023, 0, 100);
       // _controllers->battery = analogRead(PIN_BATTERY);

        // LOGGER_NOTICE_FMT("Throttle = %i Yaw = %i Pitch = %i Roll = %i", _controllers->throttle, _controllers->yaw,
        //                   _controllers->pitch, _controllers->roll);

        // LOGGER_NOTICE_FMT("Altitude = %i Altitude US = %i", _controllers->altitude, _controllers->altitude_down);

        //LOGGER_NOTICE_FMT("Battery = %i", _controllers->battery);

        //   alert();

        for (byte i = 0; i < SWITCH_NUM; i++)
            switchArray[i].loop(); // MUST call the loop() function first


        if(switchArray[0].isPressed())
            LOGGER_NOTICE("Switch 0");
           

        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            if (switchArray[i].isPressed())
            {
                LOGGER_NOTICE_FMT("The switch %i ispressed", i + 1);
                _controllers->switchState[1] = true;
                LOGGER_NOTICE_FMT(" Switch %i", _controllers->switchState[i]);
            }

            if (switchArray[i].isReleased())
            {
                LOGGER_NOTICE_FMT("The switch %i released", i + 1);
                _controllers->switchState[i] = false;
                LOGGER_NOTICE_FMT(" Switch %i", _controllers->switchState[i]);
            }
        }

        for (byte i = 0; i < BUTTON_NUM; i++)
            buttonArray[i].loop(); // MUST call the loop() function first

        // Vote the option ,throttle, yaw, pitch or roll
        if(buttonArray[button_e::mode].isPressed())
        {
            count++;
        
            if(count > 5)
                count = 1;
            LOGGER_NOTICE_FMT("Count %i ", count);                  
        }

        // Set the chosen option.
        if(buttonArray[button_e::set].isPressed())
        {
            _mode = count;
            LOGGER_NOTICE_FMT("Mode %i ", _mode);
        }

        // increase the value
        if(buttonArray[button_e::up].isPressed())
        {
            switch (_mode)
            {
            case 1:
                _throttleOffset++;
                LOGGER_NOTICE_FMT("Throttle %i",_throttleOffset);
                break;
             case 2:
                _yawOffset++;
                LOGGER_NOTICE_FMT("Yaw %i",_yawOffset);
                break;
             case 3:
                _pitchOffset++;
                LOGGER_NOTICE_FMT("Pitch %i",_pitchOffset);
                break;
             case 4:
                _rollOffset++;
                LOGGER_NOTICE_FMT("Roll %i", _rollOffset);
                break; 
                                 
            default:
                break;
            }
        }

        // decrease the value
        if(buttonArray[button_e::up].isPressed())
        {
            switch (_mode)
            {
            case 1:
                _throttleOffset--;
                if(_throttleOffset < 0)
                    _throttleOffset = 0;
                LOGGER_NOTICE_FMT("Throttle %i",_throttleOffset);
                break;
             case 2:
                _yawOffset--;
                if(_yawOffset < 0)
                    _yawOffset = 0;
                LOGGER_NOTICE_FMT("Yaw %i",_yawOffset);
                break;
             case 3:
                _pitchOffset--;
                if(_pitchOffset < 0)
                    _pitchOffset = 0;
                LOGGER_NOTICE_FMT("Pitch %i",_pitchOffset);
                break;
             case 4:
                _rollOffset--;
                if(_rollOffset < 0)
                    _rollOffset = 0;
                LOGGER_NOTICE_FMT("Roll %i", _rollOffset);
                break; 
                                 
            default:
                break;
            }
        }       

        // for (byte i = 0; i < BUTTON_NUM; i++)
        // {
        //     if (buttonArray[i].isPressed())
        //     {
        //         LOGGER_NOTICE_FMT("The button %i ispressed", i + 1);
        //     }

        //     if (buttonArray[i].isReleased())
        //     {
        //         LOGGER_NOTICE_FMT("The button %i released", i + 1);
        //     }
        // }
        // _controllers.battery = analogRead(PIN_BATTERY);
        // LOGGER_NOTICE_FMT("Battery has %i Volt",_controllers.battery);
    } //---------------------- end of update ------------------------------------------------------//

    void alert()
    {
        int lastMillis = millis();
        if (_controllers->battery > 50)
            if (millis() - lastMillis > 1000)
            {
                digitalWrite(PIN_BUZZER, HIGH);
                lastMillis = millis();
            }
            else
                digitalWrite(PIN_BUZZER, LOW);
        else
            digitalWrite(PIN_BUZZER, LOW);
    } //---------------------- end of alert ------------------------------------------------------//
};

#endif // MY_CONTROLLER_H
