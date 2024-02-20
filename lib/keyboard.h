#pragma once
/*  File name : controller.h
    Project name : KuCo_Phantom 1
    Authors: Wilhelm Kuckelsberg / Stephan Scholz
    Date : 2023-09-03

    Description : RC

*/
#ifndef MY_SENSORS_H
#define MY_SENSORS_H

#include <TaskManager.h>
#include <ezButton.h>

#define LOCAL_DEBUG
#include "myLogger.h"
#include "def.h"

// typedef enum
// {
//     option_1a,
//     option_1b,
//     option_2a,
//     option_2b,
//     option_3,
//     option_4,
//     option_5
// } switch_e;

ezButton switchArray[] =
    {
        ezButton(PIN_SWITCH_1),
        ezButton(PIN_SWITCH_2),
        ezButton(PIN_SWITCH_3),
        ezButton(PIN_SWITCH_4a),
        ezButton(PIN_SWITCH_4b),
        ezButton(PIN_SWITCH_5a),
        ezButton(PIN_SWITCH_5b),
        ezButton(PIN_SWITCH_6),
        ezButton(PIN_SWITCH_7),
        ezButton(PIN_SWITCH_8)};

typedef struct
{
    uint16_t throttle, yaw, pitch, roll;
    uint16_t battery;
    uint16_t distance_down;
    uint16_t distance_front;
    bool swiState[SWITCH_NUM];
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
        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            switchArray[i].setDebounceTime(50);
        }
    } //---------------------- end of begin -----------------------------------------------------//

    virtual void update() override
    {
        //   Serial.println("updated keyboard");
        readAnalogInputs();
        // readSwitchState();
        getSwitchState();
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
    } //---------------------- end of alert -----------------------------------------------------//

    void getSwitchState()
    {
        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            switchArray[i].loop(); // MUST call the loop() function first
         //   Serial.println(i);
        }
        for (byte i = 0; i < SWITCH_NUM; i++)
        {
           // Serial.print("i ");Serial.println(i);
            _keyboard->swiState[i] = switchArray[i].getState();
            LOGGER_NOTICE_FMT("Switch state %i %i ", i, _keyboard->swiState[i]);
        }
        delay(2000); // temp_debug
    } //---------------------- end of getSwitchState --------------------------------------------//

    void readAnalogInputs() /* read 2 Jojsticks 2 Potis and 1 Temperature sensor */
    {
        // _keyboard->throttle = calcAnalogValue(analogRead(PIN_THROTTLE));
        // LOGGER_NOTICE_FMT_CHK(_keyboard->throttle, __keyboard.throttle, "Throttle: %i", _keyboard->throttle);

        // _keyboard->yaw = calcAnalogValue(analogRead(PIN_YAW));
        // LOGGER_NOTICE_FMT_CHK(_keyboard->yaw, __keyboard.yaw, "Yaw: %i", _keyboard->yaw);

        // _keyboard->pitch = calcAnalogValue(analogRead(PIN_PITCH));
        // LOGGER_NOTICE_FMT_CHK(_keyboard->pitch, __keyboard.pitch, "Pitch: %i", _keyboard->pitch);

        // _keyboard->roll = calcAnalogValue(analogRead(PIN_ROLL));
        // LOGGER_NOTICE_FMT_CHK(_keyboard->roll, __keyboard.roll, "Roll: %i", _keyboard->roll);

        _keyboard->distance_down = map(analogRead(PIN_DISTANCE_DOWN), 0, 200, 0, 1023);
        LOGGER_NOTICE_FMT_CHK(_keyboard->distance_down, __keyboard.distance_down, "Down: %i", _keyboard->distance_down);

        _keyboard->distance_front = map(analogRead(PIN_DISTANCE_FRONT), 0, 200, 0, 1023);
        LOGGER_NOTICE_FMT_CHK(_keyboard->distance_front, __keyboard.distance_front, "Front: %i", _keyboard->distance_front);

        _keyboard->battery = map(analogRead(PIN_BATTERY), 0, 1023, 0, 100);
        LOGGER_NOTICE_FMT_CHK(_keyboard->battery, __keyboard.battery, "Battrey: %i", _keyboard->battery);

    } //---------------------- end of readJoyStick ----------------------------------------------//

    int16_t calcAnalogValue(int16_t value)
    {
        int16_t res;
        res = map(value, 0, 1023, -ANALOG_MAX, ANALOG_MAX);
        if ((res < ZERO_WINDOW) && (res > -ZERO_WINDOW))
        {
            return 0;
        }
        else
        {
            (res < 0 ? res += ZERO_WINDOW : res -= ZERO_WINDOW);
            return res;
        }
    } //---------------------- end of calcAnalogValue -------------------------------------------//

    void readSwitchState()
    {
        for (byte i = 0; i < SWITCH_NUM; i++)
            switchArray[i].loop(); // MUST call the loop() function first

        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            if (_keyboard->swiState[i])
                LOGGER_NOTICE_FMT("Switch %i is pressed", i);

             if (!_keyboard->swiState[i])
                 LOGGER_NOTICE_FMT("Switch %i is released", i);
         }
        /*
        if (switchArray[switch_e::option_1a].isPressed())
        {
            _keyboard->swiState[switch_e::option_1a] = true;
            LOGGER_NOTICE("option_1a is pressed");
        }
        else if (switchArray[switch_e::option_1a].isReleased())
        {
            _keyboard->swiState[switch_e::option_1a] = false;
            LOGGER_NOTICE("option_1a released");
        }
        else if (switchArray[switch_e::option_1b].isPressed())
        {
            _keyboard->swiState[switch_e::option_1b] = true;
            LOGGER_NOTICE("option_1b is pressed");
        }
        else if (switchArray[switch_e::option_1b].isReleased())
        {
            _keyboard->swiState[switch_e::option_1b] = false;
            LOGGER_NOTICE("option_1b released");
        }
        else if (switchArray[switch_e::option_2a].isPressed())
        {
            _keyboard->swiState[switch_e::option_2a] = true;
            LOGGER_NOTICE("option_2a is pressed");
        }
        else if (switchArray[switch_e::option_2a].isReleased())
        {
            _keyboard->swiState[switch_e::option_2a] = false;
            LOGGER_NOTICE("option_2a is released");
        }
        else if (switchArray[switch_e::option_2b].isPressed())
        {
            _keyboard->swiState[switch_e::option_2b] = true;
            LOGGER_NOTICE("option_2b is is pressed");
        }
        else if (switchArray[switch_e::option_2b].isReleased())
        {
            _keyboard->swiState[switch_e::option_2b] = false;
            LOGGER_NOTICE("option_2b is released");
        }
        else if (switchArray[switch_e::option_3].isPressed())
        {
            _keyboard->swiState[switch_e::option_3] = true;
            LOGGER_NOTICE("option_3 is is pressed");
        }
        else if (switchArray[switch_e::option_3].isReleased())
        {
            _keyboard->swiState[switch_e::option_3] = false;
            LOGGER_NOTICE("option_3 is released");
        }
        else if (switchArray[switch_e::option_4].isPressed())
        {
            _keyboard->swiState[switch_e::option_4] = true;
            LOGGER_NOTICE("option_4 is is pressed");
        }
        else if (switchArray[switch_e::option_4].isReleased())
        {
            _keyboard->swiState[switch_e::option_4] = false;
            LOGGER_NOTICE("option_4 is released");
        }
        else if (switchArray[switch_e::option_5].isPressed())
        {
            _keyboard->swiState[switch_e::option_5] = true;
            LOGGER_NOTICE("option_5 is is pressed");
        }
        else if (switchArray[switch_e::option_5].isReleased())
        {
            _keyboard->swiState[switch_e::option_5] = false;
            LOGGER_NOTICE("option_5 is released");
        }
        */
    } //---------------------- end of readSwitchState --------------------------------------------//
};
/*--------------------------- end of keyboard class ---------------------------------------------*/
#endif // MY_KEYBOARD_H
