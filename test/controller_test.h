#pragma once

#include "..\src\config.h"
#include "..\lib\radio.h"
#include "..\lib\monitor.h"
#include "..\lib\model.h"

extern model_t model;

Controller *controller;
Monitor *monitor;

void test_setup()
{
  monitor = new Monitor("monitor", Report_t::CONTROLLER);
  monitor->setModel(&model)->begin();
  controller = new Controller("controller");
  controller->setModel(&model.controllers)->begin();
}

void test_loop()
{
  digitalWrite(LED_MAINLOOP, LOW);
  controller->update();
  monitor->update();
  digitalWrite(LED_MAINLOOP, HIGH);
}
/*--------------------------- end of controller test function -----------------------------------*/


