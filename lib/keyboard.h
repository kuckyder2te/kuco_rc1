#pragma once
/*  File name : controller.h
    Project name : KuCo_Phantom 1
    Authors: Wilhelm Kuckelsberg / Stephan Scholz
    Date : 2023-09-03

    Description : RC#emote Control

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
        ezButton(PIN_SWITCH_4),
        ezButton(PIN_SWITCH_5),
        ezButton(PIN_SWITCH_6),
        ezButton(PIN_SWITCH_7),
        ezButton(PIN_SWITCH_8),
        ezButton(PIN_SWITCH_9),
        ezButton(PIN_SWITCH_10)};

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
            switchArray[i].setDebounceTime(50);// set debounce time to 50 milliseconds
        }
    } //---------------------- end of begin -----------------------------------------------------//

    virtual void update() override
    {


        
        // Serial.println("updated keyboard");
    //   readAnalogInputs();
        // readSwitchState();
        // getSwitchState();
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
        }
        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            // Serial.print("i ");Serial.println(i);
            _keyboard->swiState[i] = switchArray[i].getState();
            LOGGER_NOTICE_FMT("Switch state %i %i ", i, _keyboard->swiState[i]);
        }
        delay(2000); // temp_debug
    }                //---------------------- end of getSwitchState --------------------------------------------//

        //   Serial.println("updated keyboard");
        readAnalogInputs();
        readSwitchState();
        // getSwitchState();
    } //---------------------- end of update ----------------------------------------------------//

    void readAnalogInputs() /* read 2 Jojsticks 2 Potis and 1 Temperature sensor */
    {
        //  _keyboard->throttle = calcAnalogValue(analogRead(PIN_THROTTLE));
        //  LOGGER_NOTICE_FMT_CHK(_keyboard->throttle, __keyboard.throttle, "Throttle: %i", _keyboard->throttle);

        // _keyboard->yaw = calcAnalogValue(analogRead(PIN_YAW));
        // LOGGER_NOTICE_FMT_CHK(_keyboard->yaw, __keyboard.yaw, "Yaw: %i", _keyboard->yaw);

        // _keyboard->pitch = calcAnalogValue(analogRead(PIN_PITCH));
        // LOGGER_NOTICE_FMT_CHK(_keyboard->pitch, __keyboard.pitch, "Pitch: %i", _keyboard->pitch);

        // _keyboard->roll = calcAnalogValue(analogRead(PIN_ROLL));
        // LOGGER_NOTICE_FMT_CHK(_keyboard->roll, __keyboard.roll, "Roll: %i", _keyboard->roll);

        uint16_t temp = analogRead(PIN_DISTANCE_DOWN);
        _keyboard->distance_down = map(analogRead(PIN_DISTANCE_DOWN), 0, 1023, 0, 200);
        LOGGER_NOTICE_FMT_CHK(_keyboard->distance_down, __keyboard.distance_down, "Down: %i", _keyboard->distance_down);

        temp = analogRead(PIN_DISTANCE_FRONT);
        _keyboard->distance_front = map(analogRead(PIN_DISTANCE_FRONT), 0, 1023, 0, 200);
        LOGGER_NOTICE_FMT_CHK(_keyboard->distance_front, __keyboard.distance_front, "Front: %i", _keyboard->distance_front);

        temp = analogRead(PIN_BATTERY);
        _keyboard->battery = map(analogRead(PIN_BATTERY), 0, 1023, 0, 100);
        LOGGER_NOTICE_FMT_CHK(_keyboard->battery, __keyboard.battery, "Battery: %i temp: %i", _keyboard->battery, temp);

    } //---------------------- end of readJoyStick ----------------------------------------------//

    void getSwitchState()
    {
        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            switchArray[i].loop(); // MUST call the loop() function first
        }
        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            _keyboard->swiState[i] = switchArray[i].getState();
            LOGGER_NOTICE_FMT("Switch state %i %i ", i, _keyboard->swiState[i]);
        }
    } //---------------------- end of getSwitchState --------------------------------------------//

    void readSwitchState()
    {
        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            switchArray[i].loop(); // MUST call the loop() function first
            //    Serial.print(i);
        }

        for (byte i = 0; i < SWITCH_NUM; i++)
        {
            if (switchArray[i].isPressed())
            {
                _keyboard->swiState[i] = true;
                // Serial.print("The switch ");
                // Serial.print(i + 1);
                // Serial.println(" is pressed");
                LOGGER_NOTICE_FMT("Switch is pressed %i %i ", i, _keyboard->swiState[i]);
            }

            if (switchArray[i].isReleased())
            {
                _keyboard->swiState[i] = false;
                // Serial.print("The switch ");
                // Serial.print(i + 1);
                // Serial.println(" is released");
                LOGGER_NOTICE_FMT("Switch is released %i %i ", i, _keyboard->swiState[i]);
            }
        }
    } //---------------------- end of readSwitchState -------------------------------------------//

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
};
/*--------------------------- end of keyboard class ---------------------------------------------*/
#endif // MY_KEYBOARD_H
