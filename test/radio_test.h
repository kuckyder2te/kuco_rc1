#pragma once

#include "..\lib\radio.h"
#include "..\lib\Controller.h"
#include "..\lib\model.h"
#include "..\lib\monitor.h"

extern model_t model;

Radio *radio;
Monitor *monitor;
Controller *controller;

void test_setup()
{
  radio = new Radio("radio");
  radio->setModel(&model.RC_interface)->begin();
  monitor = new Monitor("monitor", Report_t::RADIO);
  monitor->setModel(&model)->begin();
  controller = new Controller("controller");
  controller->setModel(&model.controllers);
  controller->begin();
}

void test_loop()
{
  digitalWrite(LED_MAINLOOP, LOW);
  digitalWrite(LED_RADIO, LOW);
  radio->update();
  monitor->update();
  controller->update();
  // Assign measurement to TX_Payload for sending to Coppter
  model.RC_interface.TX_payload.rcThrottle = model.controllers.throttle;
  model.RC_interface.TX_payload.rcYaw = model.controllers.yaw;
  model.RC_interface.TX_payload.rcPitch = model.controllers.pitch;
  model.RC_interface.TX_payload.rcRoll = model.controllers.roll;
  model.RC_interface.TX_payload.rcSwi1 = model.controllers.swi1State;
  model.RC_interface.TX_payload.rcSwi2 = model.controllers.swi2State;
  model.RC_interface.TX_payload.rcSwi3 = model.controllers.swi3State;
  model.RC_interface.TX_payload.rcAltitudeBaroAdj = model.controllers.altitude;
  model.RC_interface.TX_payload.rcAltitudeSonicAdj = model.controllers.distance_down;
  digitalWrite(LED_RADIO, HIGH);
  digitalWrite(LED_MAINLOOP, HIGH);
  delay(100);
}
/*--------------------------- end of radio test function ----------------------------------------*/
