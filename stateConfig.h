/*  stateConfig.h

This header defines the state machine framework. It is intended to be used with the jsonMessenger.h Library to handle receiving and parsing JSON formatted commands entered by a user, 
however this state machine also operates independently and can be used without jsonMessenger.h

This State Machine framework can be adapted to many different purposes, follow the numbered comments for an explanation

Imogen Heard
11/02/2025


*/

#pragma once

#include "globals.h"




// 1. Define all the valid states for the state machine with an enum.
//    - This should include states triggered by user input, but may also contain states that are only accessable programatically
typedef enum {
  STATE_INIT,
  STATE_WAIT,
  STATE_SERVO,
  STATE_HOME,
  STATE_TARE,
  STATE_SAMPLERATE,
  STATE_PRINTRATE,
  STATE_STARTSTREAM,
  STATE_STOPSTREAM,
  STATE_SETSECRET,
  STATE_SET_CAL,
  STATE_GET_CAL,  // get calibration from memory and load it into loadcell library
  STATE_SET_MATERIAL,
  STATE_SET_DIAMETER,
  STATE_SET_ANGLEMAX,
  STATE_SET_LOADMAX,
  STATE_GET_SETTINGS,  // get settings from memory and store in global settings struct
  STATE_INFO,          // report settings, cal data and validity to user
  STATE_HELP,
  NUM_STATES  // Sentinal value lets us get the total number of states without manually counting. Do not forget this value, it is important for correct function
} StateType;


// 2. Then we define two variables to hold the current state enum, and the previous state enum. Holding both these values allows us to compare them as we enter a state,
//    to see if it is the first time we have entered the state.

StateType smState = STATE_INIT;
StateType lastState;

// new way of defining these text strings to save precious RAM
// shortened strings to save memory
// longest string 12 chars
const char state_0[] PROGMEM = "INIT";
const char state_1[] PROGMEM = "WAIT";
const char state_2[] PROGMEM = "SERVO";
const char state_3[] PROGMEM = "HOME";
const char state_4[] PROGMEM = "TARE";
const char state_5[] PROGMEM = "SMPLRT";
const char state_6[] PROGMEM = "PRINTRT";
const char state_7[] PROGMEM = "STREAM";
const char state_8[] PROGMEM = "STPSTREAM";
const char state_9[] PROGMEM = "SETSECRET";
const char state_10[] PROGMEM = "SET_CAL";
const char state_11[] PROGMEM = "GET_CAL";
const char state_12[] PROGMEM = "SET_MATERIAL";
const char state_13[] PROGMEM = "SET_DIAMETER";
const char state_14[] PROGMEM = "SET_ANGLEMX";
const char state_15[] PROGMEM = "SET_LOADMX";
const char state_16[] PROGMEM = "GET_SETTINGS";
const char state_17[] PROGMEM = "INFO";
const char state_18[] PROGMEM = "HELP";


// using generic names means this shouldnt need to change much often

const char *const stateNames[] PROGMEM = {
  state_0, state_1, state_2, state_3, state_4, state_5, state_6, state_7,
  state_8, state_9, state_10, state_11, state_12, state_13, state_14, state_15,
  state_16, state_17, state_18
};

// 3. Define a State Names Array, this will allow us to print the enum above in human readable format
// if RAM available and PROGMEM running low use this older method
/*
const char stateNames[][20] PROGMEM = {
  "STATE_INIT",
  "STATE_WAIT",
  "STATE_SERVO",
  "STATE_HOME",
  "STATE_DO_CAL",
  "STATE_SAMPLERATE",
  "STATE_PRINTRATE",
  "STATE_STARTSTREAM",
  "STATE_STOPSTREAM",
  "STATE_PUTSECRET",
  "STATE_GETSECRET",
  "STATE_SAVECAL",
  "STATE_GETCAL",
  "STATE_INFO",
  "STATE_HELP"
};
*/

// 4. Define the state machine function prototypes.
//    Any function that will be passed data from a user input will be passed the same jsonStateData structure as an argument.
void sm_state_init(void);
void sm_state_wait(void);
void sm_state_servo(jsonStateData stateData);
void sm_state_home(void);
void sm_state_tare(void);
void sm_state_samplerate(jsonStateData stateData);
void sm_state_printrate(jsonStateData stateData);
void sm_state_start_stream(jsonStateData stateData);
void sm_state_stop_stream(void);

// new (or major updates) functions 27/11/25
void sm_state_set_secret(jsonStateData stateData);
void sm_state_set_cal(jsonStateData stateData);
void sm_state_get_cal();
void sm_state_set_material(jsonStateData stateData);
void sm_state_set_diameter(jsonStateData stateData);
void sm_state_set_angle_max(jsonStateData stateData);
void sm_state_set_load_max(jsonStateData stateData);
void sm_state_get_settings(void);
void sm_state_info(void);
void sm_state_help(void);




// 5. Define all the state machine functions
//    - A typical state function template is shown below

/*
void sm_state_template(jsonStateData stateData) {
  if (lastState != smState) {  // Check to see if first time state has been called in sequence
    // 5a. Do anything that needs to happen the First time this state is called
    lastState = smState;  // Set the last state to the current state
  }
  // 5b. Do everything that repeats as long as this state is active

  // 5c. Extract data from jsonStateData (if required)
          - remember to validate data if coming from user input!

  if (stateData.signedInt < 1 || stateData.signedInt > 2000){
    // Error message to user if data is out of accepted bounds
  } else {
    // accept the data and process
  }

  // 5d. Add any logic to navigate to other states
  smState = STATE_WAIT;
}
*/



// Init state, only called at startup
void sm_state_init() {
  if (lastState != smState) {
    Serial.println("state: init");
  }
  // stepper.stop(HARD);
  // stepper.setRPM(0);
  //  step_rpm = 0;
  //  step_hz = 0;
  //  smState = STATE_HOME;
  smState = STATE_TARE;
}




// Not a state, but function called by "help" state to print commands list to users
/*
void print_cmds() {
  Serial.println(F("   {\"servo\": 0 to 100 } -> Set Servo Position as a percentage"));
  Serial.println(F("   {\"home\":0}           -> Home the Servo (writes angle to 40)"));
  Serial.println(F("   {\"tare\":\"\"}        -> Zero encoder position & Zero Load Cell"));
  Serial.println(F("   {\"sample\": 1 to 200} -> Set Samplerate in Hz (dflt: 200)"));  // Note, when changing print & sample rates, the size of the JSON doc may not be able to handle additional data. Max number of samples is governed by JSON doc size
  Serial.println(F("   {\"print\": 1 to 50}   -> Set Print Rate in Hz (dflt: 50)"));
  Serial.println(F("   {\"stream\":\"\"}      -> Start Data Streaming"));
  Serial.println(F("   {\"endst\":\"\"}       -> End Data Streaming"));
  Serial.println(F("   {\"secret\":\"xxxx\"}  -> set the secret word"));                         // Take a Snapshot of data
  Serial.println(F("   {\"gtscrt\":0 }        -> Return true if secret has been set"));          // Change the time over which the data snapshot is taken
  Serial.println(F("   {\"savcal\":x.xxx}     -> Save Calibration offset to eeprom or flash"));  // Ping the wobble-shaft with the servo
  Serial.println(F("   {\"getcal\":0}         -> Get current calibration table"));               // Print commands list
  Serial.println(F("   {\"info\":0}           -> Get Experiment Information"));                  // Print commands list
  Serial.println(F("   {\"help\":\"\"}        -> Print Commands to Serial Monitor"));            // Print commands list
}
*/

// State Wait is the default state for this program
void sm_state_wait() {
  if (lastState != smState) {
    // If first iteration print state machine status
#if DEBUG_STATES == true
    Serial.println(F("state: WAIT"));
#endif

#if COMMAND_HINTS == true  // print suggested commands
    Serial.println(F("\nEnter cmd in format:"));
    jsonRX.print_cmds();
#endif
    lastState = smState;
  }
}




// Trigger the servo to tension the torsion bar

void sm_state_servo(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SERVO"));
#endif
    lastState = smState;
    //servo.attach(SERVO_PPM_PIN);
    // servo_attach_time_mS = millis();
    if (stateData.signedInt >= 0 && stateData.signedInt <= 100) {  // check data is in bounds

      if (stateData.signedInt > servo_pos) {                                                 // If commanded servo position exceeds current position, check for flags
        if (ANGLE_SOFT_LIMIT || LOAD_SOFT_LIMIT) {                                           // if either flag is true. then exit without updating servo position & warn user
          Serial.println(F("{\"WARNING\":\"Limit Reached -> servo extension disabled\"}"));  // warn user and block extension
          smState = STATE_WAIT;
          return;  // exit state early and go to wait
        }
      }
      set_servo_pos(stateData.signedInt);  // otherwise, both checks have passed, servo is moving backwards, and/or neither flag has been set
      servo_pos = stateData.signedInt;     // Update servo position
    }
  }
  smState = STATE_WAIT;  // this had to move Imogen 21/11/2025
}



// Stop the Motor
void sm_state_home(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: HOME"));
#endif
    lastState = smState;
  }
  set_servo_pos(0);
  servo_pos = 0;
  smState = STATE_WAIT;
}


// Start the Motor / Update entered speed in Hz or RPM to output
void sm_state_tare(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: TARE"));
#endif
    lastState = smState;
  }
  set_servo_pos(0);
  delay(4000);
  load.tare();
  encoder.calibrate_position();
  smState = STATE_WAIT;
}






// Change the samplerate of streamed or snapshotted data (Init at 10 Hz)
void sm_state_samplerate(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("st: SAMPLERATE"));
#endif
    lastState = smState;
  }
  if (stateData.uInt < 1 || stateData.uInt > 200) {
    // errors.set_error(false, -10, "Out of Bounds Sample Rate Commanded", errors.WARNING, "state-samplerate");
    //  errors.print_json_status();
  } else if (stateData.uInt < print_rate_Hz) {
    //  errors.set_error(false, -10, "Cannot set sample rate less than print rate", errors.WARNING, "state-samplerate");
    //  errors.print_json_status();
  } else {
    //   sampleRate_Hz = stateData.uInt;
    // sampleDelay_mS = 1000 / sampleRate_Hz;
    //   sampleDelay_mS = 1E3 / sampleRate_Hz - SAMPLE_DELAY_OFFSET;
    //   num_samples_req = uint8_t(sampleRate_Hz / print_rate_Hz);  // Number of samples required to collect between each print cycle
  }
  smState = STATE_WAIT;
}



// Change the samplerate of streamed or snapshotted data (Init at 10 Hz)
void sm_state_printrate(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("st: PRINT RATE"));
#endif
    lastState = smState;
  }
  if (stateData.uInt < 1 || stateData.uInt > 50) {
    //   errors.set_error(false, -10, "Out of Bounds Print Rate Commanded", errors.WARNING, "state-printrate");
    //   errors.print_json_status();
  } else if (stateData.uInt > sampleRate_Hz) {
    //   errors.set_error(false, -10, "Cannot set print rate greater than sample rate", errors.WARNING, "state-printrate");
    //   errors.print_json_status();
  } else {
    //  print_rate_Hz = stateData.uInt;
    //  print_delay_mS = 1000 / print_rate_Hz;
    //  num_samples_req = uint8_t(sampleRate_Hz / print_rate_Hz);  // Number of samples required to collect between each print cycle
  }
  smState = STATE_WAIT;
}





// Start streaming data
void sm_state_start_stream() {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("st: STARTSTREAM"));
#endif
    lastState = smState;
  }
  streaming_active = true;
  samples_written = 0;  // start sampling again, wondering if this is the cause of the issues
  //  snapshot_timer_mS = jsonStateData.signedInt;
  smState = STATE_WAIT;
}



// Stop streaming data
void sm_state_stop_stream(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: STOPSTREAM"));
#endif
    lastState = smState;
  }
  streaming_active = false;
  // snapshot_timer_mS = 0;
  smState = STATE_WAIT;
}


// NEWER FUNCTIONS from here -------------------------------->>>>

// Trigger a snapshot of data to be taken over X amount of time
void sm_state_set_secret(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET SECRET"));
#endif
    lastState = smState;
    if (stateData.msg[0] == '\0') {  //empty string
      Serial.println(F("{\"WARNING\":\"Secret not Set\"}"));
      smState = STATE_WAIT;
      return;
    } else {
      memory.set_secret(stateData.msg);
    }
  }
  smState = STATE_WAIT;
}



void sm_state_set_cal(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET CAL"));
#endif
    lastState = smState;
  //  Serial.println(stateData.signedInt);
  //  Serial.println(stateData.auth);
    memory.set_cal_value(stateData.signedInt, stateData.auth);
  }
  smState = STATE_WAIT;
}





void sm_state_get_cal(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: GET CAL"));
#endif
    lastState = smState;
  }
  cal = memory.get_cal();
  memory.report_cal();
  load.set_scale(cal.calData);
  smState = STATE_WAIT;
}


void sm_state_set_material(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET MATERIAL"));
#endif
    lastState = smState;
    memory.set_material(stateData.msg, stateData.auth);
  }
  smState = STATE_WAIT;
}


void sm_state_set_diameter(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET DIAMETER"));
#endif
    lastState = smState;
    memory.set_diameter(stateData.msg, stateData.auth);
  }
  smState = STATE_WAIT;
}

void sm_state_set_angle_max(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET ANGLE MAX"));
#endif
    lastState = smState;
    memory.set_angle_max(stateData.signedInt, stateData.auth);
  }
  smState = STATE_WAIT;
}


void sm_state_set_load_max(jsonStateData stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET LOAD MAX"));
#endif
    lastState = smState;
    memory.set_load_max(stateData.signedInt, stateData.auth);
  }
  smState = STATE_WAIT;
}

void sm_state_get_settings(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET ANGLE MAX"));
#endif
    lastState = smState;
    settings = memory.get_settings();
  }
  smState = STATE_WAIT;
}



// Print the commands list to the Serial Output
void sm_state_info(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: help"));
#endif
    lastState = smState;
  }
  memory.report_cal();
  memory.report_settings();
  smState = STATE_WAIT;
}



// Print the commands list to the Serial Output
void sm_state_help(void) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: help"));
#endif
    lastState = smState;
  }
  print_cmds();
  smState = STATE_WAIT;
}




// 6. Finally define the state machine function
//    - Automatically generate the switch case from the list of ENUM states and list of functions! -> https://github.com/ImogenWren/switch-case-generator
void sm_Run(jsonStateData stateData) {
  if (smState < NUM_STATES) {
#if DEBUG_STATE_MACHINE == true
    if (lastState != smState) {
      char buffer[14];
      Serial.print(F("{\"sm\":\""));
      strcpy_P(buffer, (char *)pgm_read_ptr(&(stateNames[smState])));  // Necessary casts and dereferencing, just copy.
      Serial.print(buffer);
      //  Serial.print(stateNames[smState]);   // old RAM heavy version
      Serial.println(F("\"}"));
    }
#endif
    switch (smState) {
      case STATE_INIT:
        sm_state_init();
        break;
      case STATE_WAIT:
        sm_state_wait();
        break;
      case STATE_SERVO:
        sm_state_servo(stateData);
        break;
      case STATE_HOME:
        sm_state_home();
        break;
      case STATE_TARE:
        sm_state_tare();
        break;
      case STATE_SAMPLERATE:
        sm_state_samplerate(stateData);
        break;
      case STATE_PRINTRATE:
        sm_state_printrate(stateData);
        break;
      case STATE_STARTSTREAM:
        sm_state_start_stream();
        break;
      case STATE_STOPSTREAM:
        sm_state_stop_stream();
        break;
      case STATE_SETSECRET:
        sm_state_set_secret(stateData);
        break;
      case STATE_SET_CAL:
        sm_state_set_cal(stateData);
        break;
      case STATE_GET_CAL:
        sm_state_get_cal();
        break;
      case STATE_SET_MATERIAL:
        sm_state_set_material(stateData);
        break;
      case STATE_SET_DIAMETER:
        sm_state_set_diameter(stateData);
        break;
      case STATE_SET_ANGLEMAX:
        sm_state_set_angle_max(stateData);
        break;
      case STATE_SET_LOADMAX:
        sm_state_set_load_max(stateData);
        break;
      case STATE_GET_SETTINGS:
        sm_state_get_settings();
        break;
      case STATE_INFO:
        sm_state_info();
        break;
      case STATE_HELP:
        sm_state_help();
        break;
      default:
        sm_state_wait();
        Serial.println(F("sm: Exception -> default"));
        break;
    }
  } else {
    sm_state_wait();
    Serial.println(F("sm: Exception -> NUM_STATES exceeded"));
  }
}




/*



 // Defines the valid states for the state machine
 
typedef enum
{
  STATE_START_CALIBRATION,
  STATE_ZERO_ENCODER,
  STATE_AWAITING_STOP,
  STATE_DRIVING,
  STATE_START,
  STATE_STOPPED,
} StateType;


// Type definition used to define the state

 
typedef struct
{
  StateType State;    //< Defines the command 
  void (*func)(void); //< Defines the function to run 
} StateMachineType;
 

 A table that defines the valid states of the state machine and

// array of struct
StateMachineType StateMachine[] = {
  {STATE_START_CALIBRATION, Sm_State_Start_Calibration},
  {STATE_ZERO_ENCODER, Sm_State_Zero_Encoder},
  {STATE_AWAITING_STOP, Sm_State_Awaiting_Stop},
  {STATE_DRIVING, Sm_State_Driving},
  {STATE_START, Sm_State_Start},
  {STATE_STOPPED, Sm_State_Stopped}
};
 
int NUM_STATES = 6;


 * Stores the current state of the state machine

 
StateType SmState = STATE_START_CALIBRATION;


    (*StateMachine[SmState].func)();

*/


/*

used for debugging the state name recall from PROGMEM
      Serial.print(F("stateNames[0] ptr = 0x"));
      Serial.println((uint16_t)stateNames, HEX);
      extern int __heap_start, *__brkval;
      int v;
      Serial.print("Free RAM: ");
      Serial.println((int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval));
      uint16_t p = pgm_read_word(&stateNames[smState]);
      Serial.print("ptr = 0x");
      Serial.println(p, HEX);
      Serial.print(F("{\"statemachine\":\""));



*/
