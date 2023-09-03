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
#include "..\lib\controller.h"
#include "..\lib\model.h"
#include "..\lib\monitor.h"
#include "..\lib\display.h"


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
  Tasks.add<Controller>("actuators")->setModel(&model.keyboard)->startFps(10);
  //Tasks.add<Monitor>("Monitor")->setModel(&model)->startFps(10);
  Tasks.add<Display>("Display")->setModel(&model)->startFps(10);
  Serial.println("setup done");
}

void main_loop()
{
  //Serial.println("loop");
  static unsigned long _lastMillis = millis();
  Tasks.update();
  digitalWrite(LED_MAINLOOP, LOW);

  model.RC_interface.TX_payload.rcThrottle = model.keyboard.throttle;
  model.RC_interface.TX_payload.rcYaw = model.keyboard.yaw;
  model.RC_interface.TX_payload.rcPitch = model.keyboard.pitch;
  model.RC_interface.TX_payload.rcRoll = model.keyboard.roll;
  model.RC_interface.TX_payload.rcSwi1 = model.keyboard.swi1State;
  model.RC_interface.TX_payload.rcSwi2 = model.keyboard.swi2State;
  model.RC_interface.TX_payload.rcSwi3 = model.keyboard.swi3State;
  //model.RC_interface.TX_payload.rcSwi4 = model.keyboard.swi4State;
  model.RC_interface.TX_payload.rcAltitudeBaroAdj = model.keyboard.altitude;
  model.RC_interface.TX_payload.rcAltitudeSonicAdj = model.keyboard.distance_down;
  model.RC_interface.TX_payload.rcAltitudeSonicAdj = model.keyboard.distance_front;
  if (millis() - _lastMillis > 1000)
  {
    _lastMillis = millis();

    LOGGER_NOTICE_FMT("Throttle = %i Yaw = %i Pitch = %i Roll %i,", (uint16_t)model.keyboard.throttle,
                      (uint16_t)model.keyboard.yaw,
                      (uint16_t)model.keyboard.pitch,
                      (uint16_t)model.keyboard.roll);

    LOGGER_NOTICE_FMT("Swi 1 = %i Swi2 = %i Swi3 = %i,", (uint16_t)model.keyboard.swi1State,
                      (uint16_t)model.keyboard.swi2State,
                      (uint16_t)model.keyboard.swi3State);

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
/*--------------------------- end of main loop --------------------------------------------------*/

#elif _RADIO
  #include "..\test\radio_test.h"
#elif _CONTROLLER
  #include "..\test\controller_test.h";
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