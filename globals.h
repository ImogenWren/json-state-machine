/*  globals.h
      Define all user set values, global constants, global vars, included libraries and global objects

      Imogen Heard
      21/03/2025
*/

#pragma once

// Imported Libraries
#include "HX711.h"
//#include "LinearServo.h"
#include <ArduinoJson.h>  // installed version 6.21.5 [Arduino Library Manager]
//#include <FlashStorage.h>
#include "rotaryEncoder.h"
#include <buttonObject.h>
#include <ledObject.h>
//#include <Servo.h>  // [Arduino Library Manager]
#include "secretObject.h"
#include "cmds.h"

#include <autoDelay.h>

// User Created and Owned Libraries

#include "stateConfig.h"    // must always be first as jsonConfig uses variables within
#include "jsonConfig.h"     
#include "jsonMessenger.h"  // jsonMessenger also relies on variables desifined in jsonConfig
// stateMachine is defined at the end of this file, as it needs access to all globals

// Program Attributes
#define EXPERIMENT_NAME "torsion-lab"
#define FIRMWARE_VERSION "V1.0.3"
#define DEVELOPER "Imogen-Heard"


// Hardware Definitions
// Pins & IOs
// Encoder
#define ENCODER_WHITE_PIN 2
#define ENCODER_GREEN_PIN 3
// Servo
#define SERVO_PPM_PIN 10

// Limit Switch
#define LIMIT_SWITCH_PIN 5
// Load Cell HX711
#define LOAD_CELL_CLK 9
#define LOAD_CELL_DAT 8
// LED
//#define LED_BUILTIN 13

// Hardware Consts
#define ROTARY_PPR 2400  // Pulse per revolution
#define ENCODER_TYPE TWO_WIRE
#define SENSOR_NAME "en"


// User Options & Program Config
// Servo
#define SERVO_UPPER_LIMIT 100
#define SERVO_LOWER_LIMIT 0
#define SERVO_ZERO_uS 800
#define SERVO_OPEN_uS 2200

// JSON Reporting
#define PRINT_JSON true
#define PRETTY_PRINT_JSON false
#define JSON_TX_BUFFER_SIZE 240

// Sample & Printing Rates
#define INIT_SAMPLE_RATE_Hz 1
#define INIT_PRINT_RATE_Hz 1


// Utility
#define EEPROM_ADDRESS 0xFF  // EEPROM location for calibration data if required

// Debugging
#define DEBUG_STATES false
#define DEBUG_STATE_MACHINE false



// Objects
// Encoder
rotaryEncodeSensor encoder(ROTARY_PPR, ENCODER_WHITE_PIN, ENCODER_GREEN_PIN, ENCODER_TYPE, SENSOR_NAME);
// Load Cell
HX711 load;
// Servo
//Servo servo;
// Limit Switch
buttonObject limit(LIMIT_SWITCH_PIN, BUTTON_PULL_LOW);
// Indicator LED
ledObject led(LED_BUILTIN);

// jsonMessenger Object to handle incoming Serial JSON commands
jsonMessenger jsonRX;  // create a jsonMessenger object to handle commands received over Serial connection


// Delay Objects
autoDelay sampleDelay;

secretObject memory;     // object for persistant storage and calibration data
secretObject::calStruc cal;             // struct to contain calibration data
secretObject::settingsStruc settings;   // struct to contain settings data

#define ANGLE_SOFT_LIMIT_WIDTH 10
#define LOAD_SOFT_LIMIT_WIDTH 10

bool ANGLE_SOFT_LIMIT = false;
bool LOAD_SOFT_LIMIT = false;

// Servo Vars
int16_t servo_pos = 0;
uint32_t servo_attach_time_mS = 0;
#define SERVO_TIMEOUT_mS 3000  // give enough time to move into position

// Sampling Vars
// Sampling Vars
bool streaming_active = true;
bool snapshop_active = false;
uint16_t snapshot_timer_mS = 25000;
uint32_t snapshot_starttime_mS;

#define SAMPLE_DELAY_OFFSET 0  // offset in mS

uint16_t sampleRate_Hz = INIT_SAMPLE_RATE_Hz;
//uint32_t sampleDelay_mS = uint32_t(1000 / sampleRate_Hz) - 4;  // added -5 to make the delay just a little shorter, and ensure we get all samples in before they are sent.
uint32_t sampleDelay_mS = uint32_t(1E3 / sampleRate_Hz) - SAMPLE_DELAY_OFFSET;  //
// The function will stop sampling once buffer it full, so this should make more consistant outputs
// #TODO MAKE SURE STATE FUNCTION IS UPDATED TO MATCH

autoDelay printDelay;
uint16_t print_rate_Hz = INIT_PRINT_RATE_Hz;
uint32_t print_delay_mS = uint32_t(1000 / print_rate_Hz);

uint8_t num_samples_req = uint8_t(sampleRate_Hz / print_rate_Hz);  // Number of samples required to collect between each print cycle

// Maximum sample rate of 200Hz, minimum print rate of 1Hz, so max number of data to be collected is 200 samples x 8 datapoints, 4 bytes per float = 5600 bytes. Very doable data levels on STM32, HOWEVER;
// remember all this data will need to be copied to JSON so lets double that 11200! -> still very doable on STM32!

// but in the future, this must be restricted
// on smaller microcontrollers. Suggest limit of 4, so print rate cant even be less than 4 times sample rate. This can be defined by DATA_ARRAY_SIZE, and will (later) be used to resitrict sample rates and print rates to
// stay within this value, each time one is updated it must be checked for conformity to this against the other.

#define DATA_ARRAY_SIZE 1

uint32_t timestamp_array[DATA_ARRAY_SIZE];
float loadcell_array[DATA_ARRAY_SIZE];
float encoder_angle_array[DATA_ARRAY_SIZE];
int16_t encoder_pos_array[DATA_ARRAY_SIZE];

void init_arrays() {
  for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
    timestamp_array[i] = 0;
    loadcell_array[i] = 0;
    encoder_angle_array[i] = 0;
    encoder_pos_array[i] = 0;
  }
  /*
  for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
    Serial.println("Testing Array Vals");
    Serial.print(timestamp_array[i]);
    Serial.print(", ");
    Serial.print(loadcell_array[i]);
    Serial.print(", ");
    Serial.print(encoder_angle_array[i]);
    Serial.print(", ");
    Serial.print(encoder_pos_array[i]);
    Serial.println(", ");
  }
  */
}

int16_t samples_written = 0;

// Include headers that need to pull from globals right at the end
#include "smoothServo.h"
//#include "stateConfig.h"  // now included in jsonConfig?
#include "stateMachine.h"
#include "jsonReporter.h"
#include "utilityFunctions.h"
