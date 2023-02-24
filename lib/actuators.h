#pragma once
#ifndef MY_ACTUATORS_H
#define MY_ACTUATORS_H

#include <TaskManager.h>
#include <ezButton.h>

#define LOCAL_DEBUG
#include "myLogger.h"

//#include "..\lib\model.h"


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

ezButton buttonArray[] = {
    ezButton(PIN_BUTTON_1),
    ezButton(PIN_BUTTON_2),
    ezButton(PIN_BUTTON_3),
    ezButton(PIN_BUTTON_4),
    ezButton(PIN_BUTTON_5),
    ezButton(PIN_SWITCH_1),
    ezButton(PIN_SWITCH_2a),
    ezButton(PIN_SWITCH_2b)
};

#define BUTTON_NUM 8

#define PIN_CLK 23
#define PIN_DIN 25
#define PIN_DC  27
#define PIN_CE  29
#define PIN_RST 31

Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_CE, PIN_RST);

class Actuators : public Task::Base {

public: 
actuatorValues_t *_actuatorValues; 
int diff;
    

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

          for (byte i = 0; i < BUTTON_NUM; i++) {
            buttonArray[i].setDebounceTime(50); // set debounce time to 50 milliseconds
  }
    //    Serial.println("Display begin");
        display.begin();
        delay(100);
        display.clearDisplay(); 
        display.display();
        display.setContrast(60);
        display.display(); // show splashscreen
        display.setTextSize(2);
        display.setTextColor(BLACK);
        display.setCursor(0,0);
        display.println("Kucky");
        display.setCursor(10,25);
        display.println("Copter");
        display.display();
        delay(2000);
        display.clearDisplay();
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

        // Serial.print("Throttle = ");Serial.println(actuatorValues.throttle);
        // Serial.print("yaw = ");Serial.println(actuatorValues.yaw);
        // Serial.print("pitch = ");Serial.println(actuatorValues.pitch);
        // Serial.print("roll = ");Serial.println(actuatorValues.roll);

        // Serial.print("Altitude = ");Serial.println(actuatorValues.altitude);
        // Serial.print("Altitude_us = ");Serial.println(actuatorValues.altitude_us);

    for (byte i = 0; i < BUTTON_NUM; i++)
        buttonArray[i].loop(); // MUST call the loop() function first

    for (byte i = 0; i < BUTTON_NUM; i++) {
        if (buttonArray[i].isPressed()) {
        Serial.print("The button ");
        Serial.print(i + 1);
        Serial.println(" is pressed");
        }

        if (buttonArray[i].isReleased()) {
        Serial.print("The button ");
        Serial.print(i + 1);
        Serial.println(" is released");
        }
    }

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor(0,0);
        display.println("Throttle: ");
        display.setCursor(55,0);
        display.println(actuatorValues.throttle);

        display.setCursor(0,10);
        display.println("Roll    : "); 
        display.setCursor(55,10);
        display.println(actuatorValues.roll);

        int rollLine = map(actuatorValues.roll, 0, 1023, display.height()-1, 0);
        
        Serial.println(rollLine);
        display.drawLine(0, rollLine, display.width()-1, rollLine, BLACK);

        display.setCursor(0,20);
        display.println("Pitch   : ");
        display.setCursor(55,20);
        display.println(actuatorValues.pitch);

       int pitchLine = map(actuatorValues.pitch, 0, 1023, display.height()-1, 0);
       Serial.println(pitchLine);
       display.drawLine(0, pitchLine, display.width()-1, pitchLine, BLACK);

        display.setCursor(0,30);
        display.println("Yaw     : "); 
        display.setCursor(55,30);
        display.println(actuatorValues.yaw);

        display.display();

        // actuatorValues.battery = analogRead(PIN_BATTERY);
     //    Serial.print("Volt ");Serial.println(actuatorValues.battery);
    }

};

#endif  // MY_ACTUATORS_H
