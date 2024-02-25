/*  File name : main.cpp
    Project name : KuCo_Phantom 1

    Date : 2023-02-19

    Description : Drohne remote
    Hardware : Arduino MEGA2560 Mini
               Display : ILI9341 TFT 240 * 320

*/

#include <Arduino.h>
#include <RF24.h>
#include "printf.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <TaskManager.h>
#include "..\lib\radio.h"
#include "..\lib\keyboard.h"
#include "..\lib\model.h"
#include "..\lib\monitor.h"
#include "..\lib\display.h"

//#define LOCAL_DEBUG
#include "..\lib\myLogger.h"

model_t model;

void base_setup()
{
  Serial.begin(COM_SPEED);
  Serial3.begin(BT_SPEED);
  Serial.println("Serial COM OK");
  Serial.println("BT COM OK ");
  
  Serial.println("********************************");
  Serial.println("*       KuCoCopter RC_1        *");
  Serial.println("*                              *");
  Serial.print("*     ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.print(__TIME__);
  Serial.println("     *");
  Serial.println("********************************");
  Serial.flush();
 
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
}//-------------------------- end of basesetup --------------------------------------------------//

#ifdef _MAIN

void main_setup()
{
  //Tasks.add<Radio>("radio")->setModel(&model.RC_interface)->startFps(10);
  Tasks.add<Keyboard>("keyboard")->setModel(&model.keyboard)->startFps(10);
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
  model.RC_interface.TX_payload.rcSwi[0] = model.keyboard.swiState[0];
  model.RC_interface.TX_payload.rcSwi[1] = model.keyboard.swiState[1];
  model.RC_interface.TX_payload.rcSwi[2] = model.keyboard.swiState[2];
  model.RC_interface.TX_payload.rcSwi[3] = model.keyboard.swiState[3];
  model.RC_interface.TX_payload.rcSwi[2] = model.keyboard.swiState[4];
  model.RC_interface.TX_payload.rcSwi[3] = model.keyboard.swiState[5];
  model.RC_interface.TX_payload.rcAltitudeSonicAdj = model.keyboard.distance_down;
  model.RC_interface.TX_payload.rcAltitudeSonicAdj = model.keyboard.distance_front;
  if (millis() - _lastMillis > 1000)
  {
    LOGGER_NOTICE_FMT("Throttle = %i Yaw = %i Pitch = %i Roll %i,", 
                      (uint16_t)model.keyboard.throttle,
                      (uint16_t)model.keyboard.yaw,
                      (uint16_t)model.keyboard.pitch,
                      (uint16_t)model.keyboard.roll);

    LOGGER_NOTICE_FMT("Swi1 = %i Swi2 = %i Swi3 = %i Swi4 = %i Swi5 = %i Swi6 = %i,", 
                      (uint16_t)model.keyboard.swiState[0],
                      (uint16_t)model.keyboard.swiState[1],
                      (uint16_t)model.keyboard.swiState[2],
                      (uint16_t)model.keyboard.swiState[3],
                      (uint16_t)model.keyboard.swiState[4],
                      (uint16_t)model.keyboard.swiState[5]);                     

    LOGGER_NOTICE_FMT("Ground = %i Front = %i,", (uint16_t)model.RC_interface.RX_payload.distance_down,
                                                 (uint16_t)model.RC_interface.RX_payload.distance_front);

   LOGGER_NOTICE_FMT("Temp: %i",(uint16_t)model.RC_interface.RX_payload.temperature);
   LOGGER_NOTICE_FMT("Batt.: %i",(uint16_t)model.RC_interface.RX_payload.battery);

   _lastMillis = millis();
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