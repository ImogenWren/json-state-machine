
#include "globals.h"

#pragma once

void print_info(){
  Serial.print(F("\n{\"model\":\""));
  Serial.print(EXPERIMENT_NAME);
  Serial.print(F("\",\"version\":\""));
  Serial.print(FIRMWARE_VERSION);
  Serial.print(F("\",\"fw-developed-by\":\""));
  Serial.print(DEVELOPER);
  Serial.println(F("\"}"));
}