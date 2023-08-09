/*  File name : main.cpp
    Project name : KuCo_Phantom 1

    Date : 2023-02-19

    Description : Drohne remote
    Hardware : Arduino MEGA2560 Mini
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
//#include <HardwareSerial.h>

//#define SERIAL_STUDIO


#include "..\lib\radio.h"
#include "..\lib\Controller.h"
#include "..\lib\model.h"
#include "..\lib\monitor.h"

//#define LOCAL_DEBUG
#include "..\lib\myLogger.h"

#define COM_SPEED 115200
#define BT_SPEED  115200

#define LED_MAINLOOP 4  //yellow

model_t model;

void main_setup() {
  Serial.begin(COM_SPEED);
  Serial2.begin(BT_SPEED);
  Serial.println("Serial COM OK");
  Serial2.println("BT COM OK ");
  Serial2.print(__DATE__);
  Serial2.print(" ");
  Serial2.println(__TIME__);

  pinMode(PIN_RADIO_LED, OUTPUT);
  digitalWrite(PIN_RADIO_LED, LOW);

  pinMode(LED_MAINLOOP, OUTPUT);      // Yellow
  digitalWrite(LED_MAINLOOP, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  pinMode(LED_ALERT, OUTPUT);
  digitalWrite(LED_ALERT, LOW);

  #ifdef _DEBUG_
    Logger::setOutputFunction(&localLogger);
    delay(50);
    Logger::setLogLevel(Logger::_DEBUG_); // Muss immer einen Wert in platformio.ini haben (SILENT)
  #endif

  delay(1000);
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
  Tasks.add<Controller>("actuators")->setModel(&model.interfaceController)->startFps(10);
  //#ifdef SERIAL_STUDIO
  Tasks.add<Monitor>("Monitor")->setModel(&model)->startFps(10);
  //#endif
  Serial.println("setup done");
}

void main_loop() {
//  Serial.println("loop");
  static unsigned long _lastMillis = millis();
  Tasks.update();
  digitalWrite(LED_MAINLOOP, LOW);

    model.RC_interface.TX_payload.rcThrottle = model.interfaceController.throttle;
    model.RC_interface.TX_payload.rcYaw = model.interfaceController.yaw;
    model.RC_interface.TX_payload.rcPitch = model.interfaceController.pitch;
    model.RC_interface.TX_payload.rcRoll = model.interfaceController.roll;
    model.RC_interface.TX_payload.rcSwi1 = model.interfaceController.swi1State;
    model.RC_interface.TX_payload.rcSwi2 = model.interfaceController.swi2State;
    model.RC_interface.TX_payload.rcSwi3 = model.interfaceController.swi3State;
    model.RC_interface.TX_payload.rcAltitudeBaroAdj = model.interfaceController.altitude;
    model.RC_interface.TX_payload.rcAltitudeSonicAdj = model.interfaceController.altitude_down;
    if(millis()-_lastMillis > 1000){
      _lastMillis = millis();

      LOGGER_NOTICE_FMT("Throttle = %i Yaw = %i Pitch = %i Roll %i,",(uint16_t)model.interfaceController.throttle,
                                                                     (uint16_t)model.interfaceController.yaw,
                                                                     (uint16_t)model.interfaceController.pitch,
                                                                     (uint16_t)model.interfaceController.roll);

      LOGGER_NOTICE_FMT("Swi 1 = %i Swi2 = %i Swi3 = %i,",(uint16_t)model.interfaceController.swi1State,
                                                          (uint16_t)model.interfaceController.swi2State,
                                                          (uint16_t)model.interfaceController.swi3State);

      LOGGER_NOTICE_FMT("Altitude = %i Ground = %i Front = %i,",(uint16_t)model.RC_interface.RX_payload.altitude,
                                                                (uint16_t)model.RC_interface.RX_payload.distance_down,
                                                                (uint16_t)model.RC_interface.RX_payload.distance_front);                                                          
                                                                    

      // LOGGER_NOTICE_FMT("Temp: %i",(uint16_t)model.RC_interface.RX_payload.temperature);
      // Serial.println(model.RC_interface.RX_payload.temperature);
    }
  
  digitalWrite(LED_MAINLOOP, HIGH);
}

void setup(){
  main_setup();
}

void loop(){
  main_loop();
}