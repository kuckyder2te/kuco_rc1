#pragma once
#ifndef MY_Display_TASK_H
#define MY_Display_TASK_H

/*




*/

#include <TaskManager.h>

//#include <Adafruit_GFX.h>
//#include <Adafruit_PCD8544.h>
#include "..\lib\model.h"

//#define LOCAL_DEBUG
#include "myLogger.h"

#define PIN_CLK 23
#define PIN_DIN 25
#define PIN_DC  27
#define PIN_CE  29
#define PIN_RST 31

Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_CE, PIN_RST);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


class Display : public Task::Base {
public:
    Display(const String& name)
    : Task::Base(name) {
    }

    virtual ~Display() {
    }
   
    virtual void begin() override {
        Serial.println("Display begin");
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
        Serial.println("Display update");
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

        display.setCursor(0,20);
        display.println("Pitch   : ");
        display.setCursor(55,20);
        display.println(actuatorValues.pitch);

        display.setCursor(0,30);
        display.println("Yaw     : "); 
        display.setCursor(55,30);
        display.println(actuatorValues.yaw);

        display.display();
    //    display.clearDisplay();
    
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

#endif  // MY_Display_TASK_H
