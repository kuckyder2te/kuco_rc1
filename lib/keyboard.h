#pragma once
/*  File name : controller.h
    Project name : KuCo_Phantom 1
    Author: Wilhelm Kuckelsberg
    Date : 2023-09-03

    Description : RC

*/
#ifndef MY_SENSORS_H
#define MY_SENSORS_H

#include <TaskManager.h>
#include <ezButton.h>

//#define LOCAL_DEBUG
#include "myLogger.h"

#include "config.h"
#include "def.h"

typedef enum
{
    down = 0,
    save,
    joystick,
    set,
    up
} button_e;

// typedef enum
// {
//     hold_position,
//     autonom_mode,
//     adjust_on,
//     fly_mode
// } switch_e;

ezButton buttonArray[] =
    {
        ezButton(PIN_BUTTON_1),
        ezButton(PIN_BUTTON_2),
        ezButton(PIN_BUTTON_3),
        ezButton(PIN_BUTTON_4),
        ezButton(PIN_BUTTON_5)};

ezButton switchArray[] =
    {
        ezButton(PIN_SWITCH_0),
        ezButton(PIN_SWITCH_1),
        ezButton(PIN_SWITCH_2),
        ezButton(PIN_SWITCH_3)};

// typedef struct
// {
//     ezButton switchArray[4];

// } test_t;

typedef struct
{
    int throttle, yaw, pitch, roll;
    int battery;
    int altitude;
    int distance_down;
    int distance_front;
    bool swiState[4];
    bool isThottleSet, isYawSet, isPitchSet, isRollSet;
    int8_t _throttleOffset, _yawOffset, _pitchOffset, _rollOffset;
} keyboard_t;

class Keyboard : public Task::Base
{

protected:
    keyboard_t *_keyboard;
    keyboard_t __keyboard;

private:


public:
    Keyboard(const String &name)
        : Task::Base(name)
    {
    }

    Keyboard *setModel(keyboard_t *_model)
    {
        LOGGER_VERBOSE("Enter....");
        _keyboard = _model;
        LOGGER_VERBOSE("....leave");
        return this;
    } //---------------------- end of setModel --------------------------------------------------//

    virtual void begin() override
    {
        for (byte i = 0; i < BUTTON_NUM; i++)
        {
            buttonArray[i].setDebounceTime(50); // set debounce time to 50 milliseconds
        }
        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            switchArray[i].setDebounceTime(50);
        }

    } //---------------------- end of begin -----------------------------------------------------//

    virtual void update() override
    {
        readJoyStick();
        //readModeSwitch();
        //readAdjustButtons();
    } //---------------------- end of update ----------------------------------------------------//

    void alert()
    {
        int lastMillis = millis();
        if (_keyboard->battery > 50)
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

/*    void getSwitchState()
    {
        for (byte i = 0; i < SWITCH_NUM; i++)
            switchArray[i].loop(); // MUST call the loop() function first

        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            _keyboard->swiState[i] = switchArray[i].getState();
            LOGGER_NOTICE_FMT("Switch state %i ", _keyboard->swiState[i]);
        }
        delay(1000);
    } //---------------------- end of getSwitchState --------------------------------------------//
*/
    void readJoyStick()
    {
        // map is considering +/- 100 %
        //_keyboard->throttle = map((analogRead(PIN_THROTTLE)), 0, 1023, -80, 80) + _keyboard->_throttleOffset;
         _keyboard->throttle = calcAnalogValue(analogRead(PIN_THROTTLE));
        LOGGER_NOTICE_FMT_CHK(_keyboard->throttle, __keyboard.throttle, "Throttle: %i", _keyboard->throttle);
        _keyboard->yaw = calcAnalogValue(analogRead(PIN_YAW));
        _keyboard->pitch = calcAnalogValue(analogRead(PIN_PITCH));
        _keyboard->roll = calcAnalogValue(analogRead(PIN_ROLL));
        _keyboard->altitude = calcAnalogValue(analogRead(PIN_ALTITUDE));
        _keyboard->distance_down = calcAnalogValue(analogRead(PIN_DISTANCE_DOWN));
        // _keyboard->battery = map(analogRead(PIN_BATTERY), 0, 1023, 0, 100);
        //_keyboard->battery = analogRead(PIN_BATTERY);
    } //---------------------- end of readJoyStick ----------------------------------------------//

    int16_t calcAnalogValue(int16_t value){
        int16_t res;
        res = map(value, 0, 1023, -ANALOG_MAX, ANALOG_MAX);
        if((res<ZERO_WINDOW) && (res > -ZERO_WINDOW)){
            return 0;
        }else{
            (res<0?res+=ZERO_WINDOW:res-=ZERO_WINDOW);
            return res;
        }
    }
    void readAdjustButtons()
    {
        static uint8_t _chooseJS = 0;
        static uint8_t _isJS = 0;

        if (_keyboard->swiState[3])
        {
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
                    _keyboard->isThottleSet = true;
                    _keyboard->isYawSet = false;
                    _keyboard->isPitchSet = false;
                    _keyboard->isRollSet = false;
                    LOGGER_NOTICE("Throttle is set");
                    break;
                case 2:
                    _keyboard->isThottleSet = false;
                    _keyboard->isYawSet = true;
                    _keyboard->isPitchSet = false;
                    _keyboard->isRollSet = false;
                    LOGGER_NOTICE("Yaw is set");
                    break;
                case 3:
                    _keyboard->isThottleSet = false;
                    _keyboard->isYawSet = false;
                    _keyboard->isPitchSet = true;
                    _keyboard->isRollSet = false;
                    LOGGER_NOTICE("Pitch is set");
                    break;
                case 4:
                    _keyboard->isThottleSet = false;
                    _keyboard->isYawSet = false;
                    _keyboard->isPitchSet = false;
                    _keyboard->isRollSet = true;
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
                    _keyboard->_throttleOffset++;
                    LOGGER_NOTICE_FMT("_throttleOffset++ %i", _keyboard->_throttleOffset);
                    break;
                case 1:
                    _keyboard->_yawOffset++;
                    LOGGER_NOTICE_FMT("_yawOffset++ %i", _keyboard->_yawOffset);
                    break;
                case 2:
                    _keyboard->_pitchOffset++;
                    LOGGER_NOTICE_FMT("_pitchOffset++ %i", _keyboard->_pitchOffset);
                    break;
                case 3:
                    _keyboard->_rollOffset++;
                    LOGGER_NOTICE_FMT("_rollOffset++ %i", _keyboard->_rollOffset);
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
                    _keyboard->_throttleOffset--;
                    LOGGER_NOTICE_FMT("Throttle %i", _keyboard->_throttleOffset);
                    break;
                case 2:
                    _keyboard->_yawOffset--;
                    LOGGER_NOTICE_FMT("Yaw %i", _keyboard->_yawOffset);
                    break;
                case 3:
                    _keyboard->_pitchOffset--;
                    LOGGER_NOTICE_FMT("Pitch %i", _keyboard->_pitchOffset);
                    break;
                case 4:
                    _keyboard->_rollOffset--;
                    LOGGER_NOTICE_FMT("Roll %i", _keyboard->_rollOffset);
                    break;

                default:
                    break;
                }
            }
        }
    } //---------------------- end of readAdjustButtons -----------------------------------------//

    void readModeSwitch()
    {
        for (byte i = 0; i < SWITCH_NUM; i++)
            switchArray[i].loop(); // MUST call the loop() function first

        if (switchArray[switch_e::autonom_mode].isPressed())
        {
            _keyboard->swiState[switch_e::autonom_mode] = true;
            LOGGER_NOTICE("Switch autonom");
        }
        else if (switchArray[switch_e::autonom_mode].isReleased())
        {
            _keyboard->swiState[switch_e::autonom_mode] = false;
            LOGGER_NOTICE("Switch autonom released");
        }

        else if (switchArray[switch_e::hold_position].isPressed())
        {
            _keyboard->swiState[switch_e::hold_position] = true;
            LOGGER_NOTICE("Switch Hold Altitude");
        }
        else if (switchArray[switch_e::hold_position].isReleased())
        {
            _keyboard->swiState[switch_e::hold_position] = false;
            LOGGER_NOTICE("Switch Hold Altitude released");
        }

        else if (switchArray[switch_e::fly_mode].isPressed())
        {
            _keyboard->swiState[switch_e::fly_mode] = true;
            LOGGER_NOTICE("Switch Fly mode");
        }
        else if (switchArray[switch_e::fly_mode].isReleased())
        {
            _keyboard->swiState[switch_e::fly_mode] = false;
            LOGGER_NOTICE("Switch Fly mode released");
        }

        else if (switchArray[switch_e::adjust_on].isPressed())
        {
            _keyboard->swiState[switch_e::adjust_on] = true;
            LOGGER_NOTICE("Switch 3 Adjust mode");
        }
        else if (switchArray[switch_e::adjust_on].isReleased())
        {
            _keyboard->swiState[switch_e::adjust_on] = false;
            LOGGER_NOTICE("Switch 3 released");
        }

    } //---------------------- end of readModeSwitch --------------------------------------------//
};
/*--------------------------- end of keyboard class ---------------------------------------------*/
#endif // MY_KEYBOARD_H
