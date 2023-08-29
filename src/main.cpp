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
#include "config.h"
#include "..\resources\intro.h"

#include "..\lib\radio.h"
#include "..\lib\Controller.h"
#include "..\lib\model.h"
#include "..\lib\monitor.h"

// #define LOCAL_DEBUG
#include "..\lib\myLogger.h"

model_t model;

void base_setup()
{
  Serial.begin(COM_SPEED);
  Serial2.begin(BT_SPEED);
  Serial.println("Serial COM OK");
  Serial2.println("BT COM OK ");
  Serial2.print(__DATE__);
  Serial2.print(" ");
  Serial2.println(__TIME__);

  pinMode(LED_RADIO, OUTPUT);
  digitalWrite(LED_RADIO, LOW);

  pinMode(LED_MAINLOOP, OUTPUT); // green
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

  delay(100);
  LOGGER_NOTICE("Program will initialized");

    Wire.begin();

}

#ifdef _MAIN

void main_setup()
{
  Tasks.add<Radio>("radio")->setModel(&model.RC_interface)->startFps(10);
  Tasks.add<Controller>("actuators")->setModel(&model.controllers)->startFps(10);
  Tasks.add<Monitor>("Monitor")->setModel(&model)->startFps(10);

  Serial.println("setup done");
}

void main_loop()
{
  //Serial.println("loop");
  static unsigned long _lastMillis = millis();
  Tasks.update();
  digitalWrite(LED_MAINLOOP, LOW);

  model.RC_interface.TX_payload.rcThrottle = model.controllers.throttle;
  model.RC_interface.TX_payload.rcYaw = model.controllers.yaw;
  model.RC_interface.TX_payload.rcPitch = model.controllers.pitch;
  model.RC_interface.TX_payload.rcRoll = model.controllers.roll;
  model.RC_interface.TX_payload.rcSwi1 = model.controllers.swi1State;
  model.RC_interface.TX_payload.rcSwi2 = model.controllers.swi2State;
  model.RC_interface.TX_payload.rcSwi3 = model.controllers.swi3State;
  model.RC_interface.TX_payload.rcSwi4 = model.controllers.swi4State;
  model.RC_interface.TX_payload.rcAltitudeBaroAdj = model.controllers.altitude;
  model.RC_interface.TX_payload.rcAltitudeSonicAdj = model.controllers.altitude_down;
  if (millis() - _lastMillis > 1000)
  {
    _lastMillis = millis();

    LOGGER_NOTICE_FMT("Throttle = %i Yaw = %i Pitch = %i Roll %i,", (uint16_t)model.controllers.throttle,
                      (uint16_t)model.controllers.yaw,
                      (uint16_t)model.controllers.pitch,
                      (uint16_t)model.controllers.roll);

    LOGGER_NOTICE_FMT("Swi 1 = %i Swi2 = %i Swi3 = %i,", (uint16_t)model.controllers.swi1State,
                      (uint16_t)model.controllers.swi2State,
                      (uint16_t)model.controllers.swi3State);

    LOGGER_NOTICE_FMT("Altitude = %i Ground = %i Front = %i,", (uint16_t)model.RC_interface.RX_payload.altitude,
                      (uint16_t)model.RC_interface.RX_payload.distance_down,
                      (uint16_t)model.RC_interface.RX_payload.distance_front);

   LOGGER_NOTICE_FMT("Temp: %i",(uint16_t)model.RC_interface.RX_payload.temperature);
   LOGGER_NOTICE_FMT("Batt.: %i",(uint16_t)model.RC_interface.RX_payload.battery);
    // Serial.println(model.RC_interface.RX_payload.temperature);
  }

  digitalWrite(LED_MAINLOOP, HIGH);
}
// MAIN end
#elif _RADIO
#include "..\test\radio_test.h"
// Radio *radio;
// Monitor *monitor;
// Controller *controller;
// void radio_test_setup()
// {
//   radio = new Radio("radio");
//   radio->setModel(&model.RC_interface)->begin();
//   //radio->begin();
//   monitor = new Monitor("monitor", Report_t::RADIO);
//   monitor->setModel(&model);
//   monitor->begin();
//   controller = new Controller("controller");
//   controller->setModel(&model.controllers);
//   controller->begin();
// }

// void radio_test_loop()
// {
//   digitalWrite(LED_RADIO, LOW);
//   radio->update();
//   monitor->update();
//   controller->update();
//   // Assign measurement to TX_Payload for sending to Coppter
//   model.RC_interface.TX_payload.rcThrottle = model.controllers.throttle;
//   model.RC_interface.TX_payload.rcYaw = model.controllers.yaw;
//   model.RC_interface.TX_payload.rcPitch = model.controllers.pitch;
//   model.RC_interface.TX_payload.rcRoll = model.controllers.roll;
//   model.RC_interface.TX_payload.rcSwi1 = model.controllers.swi1State;
//   model.RC_interface.TX_payload.rcSwi2 = model.controllers.swi2State;
//   model.RC_interface.TX_payload.rcSwi3 = model.controllers.swi3State;
//   model.RC_interface.TX_payload.rcAltitudeBaroAdj = model.controllers.altitude;
//   model.RC_interface.TX_payload.rcAltitudeSonicAdj = model.controllers.altitude_down;
//   digitalWrite(LED_RADIO, HIGH);
//   delay(100);
}
/*--------------------------- end of radio test function ----------------------------------------*/

#elif _CONTROLLER
#include "..\test\controller_test.h";
// Controller *controller;
// Monitor *monitor;
// void controller_test_setup()
// {
//   monitor = new Monitor("monitor", Report_t::CONTROLLER);
//   monitor->setModel(&model)->begin();
//   controller = new Controller("controller");
//   controller->setModel(&model.controllers)->begin();
// }

// void controller_test_loop()
// {
//   controller->update();
//   monitor->update();
// }
#endif
/*--------------------------- end of controller test function -----------------------------------*/

void setup()
{
  base_setup();
#ifdef _MAIN
  main_setup();
#else
  test_setup();
#endif
}

void loop()
{
#ifdef _MAIN
  main_loop();
#else
  test_loop();
#endif
}
/*--------------------------- end of standart setup and loop function ---------------------------*/