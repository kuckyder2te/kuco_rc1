#pragma once

#include <Arduino.h>


void intro(){
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
}
/*------------------------- end of intro code ---------------------------------------------------*/
