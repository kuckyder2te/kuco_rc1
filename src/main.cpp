/*  File name : main.cpp
    Project name : KuCo_Phantom 1

    Date : 2023-02-19

    Description : Drohne
    Hardware : MEGA 2560
               Display : Nokia 5110
              
*/

#include <Arduino.h>
#include <RF24.h>
#include "printf.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <TaskManager.h>

#include "..\lib\myLogger.h"
#include "..\lib\radio.h"
#include "..\lib\actuators.h"
#include "..\lib\display.h"
//#include "..\lib\model.h"

#define COM_SPEED 115200
#define BT_SPEED 115200

model_t model;

void setup() {
  Serial.begin(COM_SPEED);
  Serial2.begin(BT_SPEED);
  Serial.println("Serial COM OK");
  Serial2.println("BT COM OK ");
  Serial2.print(__DATE__);
  Serial2.print(" ");
  Serial2.println(__TIME__);

  #ifdef _DEBUG_
    Logger::setOutputFunction(&localLogger);
    delay(50);
    Logger::setLogLevel(Logger::_DEBUG_); // Muss immer einen Wert in platformio.ini haben (SILENT)
  #endif

  delay(100);
  LOGGER_NOTICE("Program will initialized");

  Serial.println("********************************");
  Serial.println("*       KuCo RC                *");
  Serial.println("*                              *");
  Serial.print("*     ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.print(__TIME__);
  Serial.println("     *");
  Serial.println("********************************");
  Serial.flush();
  Wire.begin();

  //EEPROM.begin(512);
  
  delay(100);

  Tasks.add<Radio>("radio")->setModel(&model.RC_interface)->startFps(10);
  Tasks.add<Actuators>("actuators")->setModel(&model.actuatorValues)->startFps(10);
  Tasks.add<Display>("display")->startFps(10);

}

void loop() {
  Tasks.update();
}