#pragma once
#ifndef MY_ACTUATORS_H
#define MY_ACTUATORS_H

#include <TaskManager.h>
#include <ezButton.h>

//#define LOCAL_DEBUG
#include "myLogger.h"


#define PIN_THROTTLE A0
#define PIN_YAW      A2
#define PIN_PITCH    A6
#define PIN_ROLL     A4

#define PIN_ALTITUDE     A8
#define PIN_ALTITUDE_US  A10

#define PIN_BUTTON_1 32
#define PIN_BUTTON_2 34
#define PIN_BUTTON_3 36
#define PIN_BUTTON_4 38
#define PIN_BUTTON_5 A14

#define PIN_SWITCH_1  42
#define PIN_SWITCH_2a 44
#define PIN_SWITCH_2b 46

#define PIN_BUZZER   10

#define PIN_BATTERY  A1


typedef struct {

    int throttle, yaw, pitch, roll;
    int battery;
    int altitude, altitude_us;
    int btn1State, btn2State, btn3State, btn4State, btn5State;
    int swi1State, swi2aState, swi2bState;

} actuatorValues_t;
  actuatorValues_t actuatorValues;  

ezButton button1(PIN_BUTTON_1);  
ezButton button2(PIN_BUTTON_2); 
ezButton button3(PIN_BUTTON_3);  
ezButton button4(PIN_BUTTON_4);  
ezButton button5(PIN_BUTTON_5); 

ezButton switch1(PIN_SWITCH_1);
ezButton switch2a(PIN_SWITCH_2a);
ezButton switch2b(PIN_SWITCH_2b);

class Actuators : public Task::Base {

public: 
actuatorValues_t *_actuatorValues; 

public:
    Actuators(const String& name)
    : Task::Base(name) {
    }

    virtual ~Actuators() {
    }

    Actuators *setModel(actuatorValues_t *_model)
    { 
        LOGGER_VERBOSE("Enter....");
        _actuatorValues = _model;
        LOGGER_VERBOSE("....leave");
        return this;
    }

     virtual void begin() override {

        button1.setDebounceTime(50);
        button2.setDebounceTime(50);
        button3.setDebounceTime(50);
        button4.setDebounceTime(50); 
        button5.setDebounceTime(50);

        switch1.setDebounceTime(50);
        switch2a.setDebounceTime(50); 
        switch2b.setDebounceTime(50);
     }

    // optional (you can remove this method)
    // virtual void enter() override {
    // }

    virtual void update() override {

        actuatorValues.throttle = analogRead(PIN_THROTTLE);
        actuatorValues.yaw = analogRead(PIN_YAW);
        actuatorValues.pitch = analogRead(PIN_PITCH);
        actuatorValues.roll = analogRead(PIN_ROLL);

        actuatorValues.altitude = analogRead(PIN_ALTITUDE); 
        actuatorValues.altitude_us = analogRead(PIN_ALTITUDE_US);

        Serial.print("Throttle = ");Serial.println(actuatorValues.throttle);
        Serial.print("yaw = ");Serial.println(actuatorValues.yaw);
        Serial.print("pitch = ");Serial.println(actuatorValues.pitch);
        Serial.print("roll = ");Serial.println(actuatorValues.roll);

        Serial.print("Altitude = ");Serial.println(actuatorValues.altitude);
        Serial.print("Altitude_us = ");Serial.println(actuatorValues.altitude_us);

        button1.loop(); 
        button2.loop(); 
        button3.loop();
        button4.loop(); 
        button5.loop();

        switch1.loop(); 
        switch2a.loop(); 
        switch2b.loop(); 
        
        actuatorValues.btn1State = button1.getState();
        actuatorValues.btn2State = button2.getState();
        actuatorValues.btn3State = button3.getState();
        actuatorValues.btn4State = button4.getState();
        actuatorValues.btn5State = button5.getState();

        actuatorValues.swi1State = switch1.getState();
        actuatorValues.swi2aState = switch2a.getState();
        actuatorValues.swi2bState = switch2b.getState();

        Serial.print("button 1 state: ");
        Serial.println(actuatorValues.btn1State);
        Serial.print("button 2 state: ");
        Serial.println(actuatorValues.btn2State);
        Serial.print("button 3 state: ");
        Serial.println(actuatorValues.btn3State);
        Serial.print("button 4 state: ");
        Serial.println(actuatorValues.btn4State);
        Serial.print("button 5 state: ");
        Serial.println(actuatorValues.btn5State);

        Serial.print("switch 1 state: ");
        Serial.println(actuatorValues.swi1State);
        Serial.print("switch 2a state: ");
        Serial.println(actuatorValues.swi2aState);
        Serial.print("switch 2b state: ");
        Serial.println(actuatorValues.swi2bState);
        

        if(button1.isPressed())
            Serial.println("The button 1 is pressed");

        if(button1.isReleased())
            Serial.println("The button 1 is released");

        if(button2.isPressed())
            Serial.println("The button 2 is pressed");

        if(button2.isReleased())
            Serial.println("The button 2 is released");

        if(button3.isPressed())
            Serial.println("The button 3 is pressed");

        if(button3.isReleased())
            Serial.println("The button 3 is released");

        if(button4.isPressed())
            Serial.println("The button 4 is pressed");

        if(button4.isReleased())
            Serial.println("The button 4 is released");

        if(button5.isPressed())
            Serial.println("The button 5 is pressed");

        if(button5.isReleased())
            Serial.println("The button 5 is released");



        if(switch1.isPressed())
            Serial.println("The switch 1 is pressed");

        if(switch1.isReleased())
            Serial.println("The switch 1 is released");   

        if(switch2a.isPressed())
            Serial.println("The switch 2a is pressed");

        if(switch2a.isReleased())
            Serial.println("The switch 2a is released");

        if(switch2b.isPressed())
            Serial.println("The switch 2b is pressed");

        if(switch2b.isReleased())
            Serial.println("The switch 2b is released");    


        actuatorValues.battery = analogRead(PIN_BATTERY);
        Serial.print("Volt ");Serial.println(actuatorValues.battery);
    }

    // optional (you can remove this method)
    // virtual void exit() override {
    // }

    // optional (you can remove this method)
    // virtual void idle() override {
    // }

    // optional (you can remove this method)
    // virtual void reset() override {
    // }
};

#endif  // MY_ACTUATORS_H
