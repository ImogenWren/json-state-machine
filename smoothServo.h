/*
 Smooth Servo

 A smoother servo implementation for Arduino pin D10 that avoids signal jitter & servo jittering

*/

#include "globals.h"

#pragma once

void servoSetup() {
  pinMode(SERVO_PPM_PIN, OUTPUT);
  // --- Configure Timer1 for Fast PWM with ICR1 as TOP ---
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  // Fast PWM, TOP = ICR1
  TCCR1A |= (1 << COM1B1);  // Clear OC1B (pin 10) on compare match
  TCCR1A |= (1 << WGM11);   // Mode 14 (Fast PWM with ICR1 TOP)
  TCCR1B |= (1 << WGM12) | (1 << WGM13);
  // Prescaler = 8 → 0.5 µs per tick
  TCCR1B |= (1 << CS11);
  ICR1 = 40000;  // TOP = 20 ms (40000 * 0.5µs = 20,000 µs)
  OCR1B = 3000;  // default 1.5 ms pulse → (1500 µs * 2 = 3000 ticks)
  interrupts();
}

void set_servo_pos(int16_t servo_percent = 0) {
  servo_pos = servo_percent;
  OCR1B = ((servo_percent * 10) + 1000) * 2;   // OCR1B is pulsewidth*2
}