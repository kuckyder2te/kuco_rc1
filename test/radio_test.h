#pragma once

#include "..\lib\radio.h"
#include "..\lib\keyboard.h"
#include "..\lib\model.h"
#include "..\lib\monitor.h"

extern model_t model;

Radio *radio;
Monitor *monitor;
Keyboard *keyboard;

void test_setup()
{
  radio = new Radio("radio");
  radio->setModel(&model.RC_interface)->begin();
  monitor = new Monitor("monitor", Report_t::RADIO);
  monitor->setModel(&model)->begin();
  keyboard = new Keyboard("keyboard");
  keyboard->setModel(&model.keyboard);
  keyboard->begin();
}

void test_loop()
{
  digitalWrite(LED_MAINLOOP, LOW);
  digitalWrite(LED_RADIO, LOW);
  radio->update();
  monitor->update();
  keyboard->update();
  // Assign measurement to TX_Payload for sending to Coppter
  model.RC_interface.TX_payload.rcThrottle = model.keyboard.throttle;
  model.RC_interface.TX_payload.rcYaw = model.keyboard.yaw;
  model.RC_interface.TX_payload.rcPitch = model.keyboard.pitch;
  model.RC_interface.TX_payload.rcRoll = model.keyboard.roll;
  model.RC_interface.TX_payload.rcSwi1 = model.keyboard.swi1State;
  model.RC_interface.TX_payload.rcSwi2 = model.keyboard.swi2State;
  model.RC_interface.TX_payload.rcSwi3 = model.keyboard.swi3State;
  model.RC_interface.TX_payload.rcAltitudeBaroAdj = model.keyboard.altitude;
  model.RC_interface.TX_payload.rcAltitudeSonicAdj = model.keyboard.distance_down;
  digitalWrite(LED_RADIO, HIGH);
  digitalWrite(LED_MAINLOOP, HIGH);
  delay(100);
}
/*--------------------------- end of radio test function ----------------------------------------*/
