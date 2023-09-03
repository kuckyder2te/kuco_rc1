
#pragma once
#ifndef MY_SENSORS_H
#define MY_SENSORS_H

#include <TaskManager.h>
#include <ezButton.h>

#define LOCAL_DEBUG
#include "myLogger.h"

typedef struct
{
    int throttle, yaw, pitch, roll;
    int battery;
    int altitude;
    int distance_down;
    int distance_front;
    int swi1State, swi2State, swi3State, swi4State;
    bool isThottleSet, isYawSet, isPitchSet, isRollSet;
} controllers_t;

typedef enum
{
    down = 0,
    save,
    joystick,
    set,
    up
} button_e;

typedef enum
{
    hold_position,
    autonom,
    adjust_on,
    NN
} switch_e;

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

protected:
private:
    button_e _button;
    switch_e _switch;

public:
    controllers_t *_controllers;
    bool switchAdjust = false;
    // bool switchYaw = false;

    int8_t _throttleOffset = 0;
    int8_t _yawOffset = 0;
    int8_t _pitchOffset = 0;
    int8_t _rollOffset = 0;

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

    } //---------------------- end of begin -----------------------------------------------//

    virtual void update() override
    {
        static uint8_t _chooseJS = 0;
        static uint8_t _isJS = 0;
        // map is considering +/- 100 %
        _controllers->throttle = map((analogRead(PIN_THROTTLE)), 0, 1023, -100, 100) + _throttleOffset;
        _controllers->yaw = map((analogRead(PIN_YAW)), 0, 1023, -100, 100) + _yawOffset;
        _controllers->pitch = map((analogRead(PIN_PITCH)), 0, 1023, -15, 15) + _pitchOffset;
        _controllers->roll = map((analogRead(PIN_ROLL)), 0, 1023, -15, 15 + _rollOffset);

        _controllers->altitude = map((analogRead(PIN_ALTITUDE)), 0, 1013, 0, 100);
        _controllers->distance_down = map(analogRead(PIN_DISTANCE_DOWN), 0, 1023, 0, 200);
        // _controllers->battery = map(analogRead(PIN_BATTERY), 0, 1023, 0, 100);
        // _controllers->battery = analogRead(PIN_BATTERY);

        for (byte i = 0; i < SWITCH_NUM; i++)
            switchArray[i].loop(); // MUST call the loop() function first

        if (switchArray[0].isPressed())
            LOGGER_NOTICE("Switch 0");
        else if (switchArray[1].isPressed())
            LOGGER_NOTICE("Switch 1");
        else if (switchArray[2].isPressed())
            LOGGER_NOTICE("Switch 2");

        else if (switchArray[3].isReleased())
        {
            switchAdjust = true;
            LOGGER_NOTICE_FMT("Switch 3 Adjust %i", switchAdjust);
        }

        for (byte i = 0; i < BUTTON_NUM; i++)
            buttonArray[i].loop(); // MUST call the loop() function first

        // Vote the option ,throttle, yaw, pitch or roll
        if (buttonArray[button_e::joystick].isPressed())
        {
            _chooseJS++;

            if (_chooseJS > 4)
                _chooseJS = 1;

            switch (_chooseJS)
            {
            case 1:
                _controllers->isThottleSet = true;
                _controllers->isYawSet = false;
                _controllers->isPitchSet = false;
                _controllers->isRollSet = false;
                LOGGER_NOTICE("Throttle is set");
                break;
            case 2:
                _controllers->isThottleSet = false;
                _controllers->isYawSet = true;
                _controllers->isPitchSet = false;
                _controllers->isRollSet = false;
                LOGGER_NOTICE("Yaw is set");
                break;
            case 3:
                _controllers->isThottleSet = false;
                _controllers->isYawSet = false;
                _controllers->isPitchSet = true;
                _controllers->isRollSet = false;
                LOGGER_NOTICE("Pitch is set");
                break;
            case 4:
                _controllers->isThottleSet = false;
                _controllers->isYawSet = false;
                _controllers->isPitchSet = false;
                _controllers->isRollSet = true;
                LOGGER_NOTICE("Roll is set");
                break;
            }
        }

        // Set the chosen option.
        if (buttonArray[button_e::set].isPressed())
        {
            _isJS = _chooseJS;
            switch (_isJS)
            {
            case 0:
            
                LOGGER_NOTICE("JS Throttle is selected");
            case 1:
                LOGGER_NOTICE("JS YAW is selected");
            case 2:
                LOGGER_NOTICE("JS Pitch is selected");
            case 3:
                LOGGER_NOTICE("JS Roll is selected");
                break;
            }
        }

        // increase the value
        if (buttonArray[button_e::up].isPressed())
        {
            switch (_isJS)
            {
            case 0:
                _throttleOffset++;
                LOGGER_NOTICE_FMT("_throttleOffset++ %i", _throttleOffset);
                break;
            case 1:
                _yawOffset++;
                LOGGER_NOTICE_FMT("_yawOffset++ %i", _yawOffset);
                break;
            case 2:
                _pitchOffset++;
                LOGGER_NOTICE_FMT("_pitchOffset++ %i", _pitchOffset);
                break;
            case 3:
                _rollOffset++;
                LOGGER_NOTICE_FMT("_rollOffset++ %i", _rollOffset);
                break;

            default:
                break;
            }
        }
        // decrease the value
        if (buttonArray[button_e::down].isPressed())
        {
            switch (_isJS)
            {
            case 1:
                _throttleOffset--;
                LOGGER_NOTICE_FMT("Throttle %i", _throttleOffset);
                break;
            case 2:
                _yawOffset--;
                LOGGER_NOTICE_FMT("Yaw %i", _yawOffset);
                break;
            case 3:
                _pitchOffset--;
                LOGGER_NOTICE_FMT("Pitch %i", _pitchOffset);
                break;
            case 4:
                _rollOffset--;
                LOGGER_NOTICE_FMT("Roll %i", _rollOffset);
                break;

            default:
                break;
            }
        }

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
