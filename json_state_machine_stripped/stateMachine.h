
/*
Step 3:
  - This file defines all of the actual state functions,
  - and the state machine function that selects the active state based on the active smState variable


  - Biggest modification from previous version
  - back to using the function structure topology, however this requires that every state is passed stateData, even if that state does not use any values from it
  - suggest having a null line in each function that reads some variable from stateData just to shut up compiler warnings about unused variables.



*/


#ifndef stateMachine_h
#define stateMachine_h

#include "states.h"



stateDef smState = STATE_INIT;
stateDef lastState;




// 4. Define the state machine function prototypes.
//    Any function that will be passed data from a user input will be passed the same jsonStateData_t structure as an argument.
void sm_state_null(jsonStateData_t stateData);
void sm_state_init(jsonStateData_t stateData);
void sm_state_wait(jsonStateData_t stateData);
void sm_state_servo(jsonStateData_t stateData);
void sm_state_home(jsonStateData_t stateData);
void sm_state_tare(jsonStateData_t stateData);
void sm_state_samplerate(jsonStateData_t stateData);
void sm_state_printrate(jsonStateData_t stateData);
void sm_state_start_stream(jsonStateData_t stateData);
void sm_state_stop_stream(jsonStateData_t stateData);

// new (or major updates) functions 27/11/25
void sm_state_set_secret(jsonStateData_t stateData);
void sm_state_set_cal(jsonStateData_t stateData);
void sm_state_get_cal(jsonStateData_t stateData);
void sm_state_set_material(jsonStateData_t stateData);
void sm_state_set_diameter(jsonStateData_t stateData);
void sm_state_set_angle_max(jsonStateData_t stateData);
void sm_state_set_load_max(jsonStateData_t stateData);
void sm_state_get_settings(jsonStateData_t stateData);
void sm_state_info(jsonStateData_t stateData);
void sm_state_help(jsonStateData_t stateData);


// NEW -> (actually old? using function pointers to do the actual state machine)


typedef struct {
  stateDef State;      //< Defines thestate enum
  void (*func)(jsonStateData_t stateData);  //< Defines the function to run
} StateMachine_t;


//A table that defines the valid states of the state machine and the state which is called by that state keyword

// array of struct
StateMachine_t StateMachine[] = {  // NUM_STATES]
  { STATE_NULL, sm_state_null },
  { STATE_INIT, sm_state_init },
  { STATE_WAIT, sm_state_wait },
  { STATE_SERVO, sm_state_servo },
  { STATE_HOME, sm_state_home },
  { STATE_TARE, sm_state_tare },
  { STATE_SAMPLERATE, sm_state_samplerate },
  { STATE_PRINTRATE, sm_state_printrate },
  { STATE_STARTSTREAM, sm_state_start_stream },
  { STATE_STOPSTREAM, sm_state_stop_stream },
  { STATE_SETSECRET, sm_state_set_secret },
  { STATE_SET_CAL, sm_state_set_cal },
  { STATE_GET_CAL, sm_state_get_cal },
  { STATE_SET_MATERIAL, sm_state_set_material },
  { STATE_SET_DIAMETER, sm_state_set_diameter },
  { STATE_SET_ANGLEMAX, sm_state_set_angle_max },
  { STATE_SET_LOADMAX, sm_state_set_load_max },
  { STATE_GET_SETTINGS, sm_state_get_settings },
  { STATE_INFO, sm_state_info },
  { STATE_HELP, sm_state_help }
};









//int NUM_STATES = 6;  // dont need this as guard value included in origional list in stateConfig.h


// 5. Define all the state machine functions
//    - A typical state function template is shown below

/*
void sm_state_template(jsonStateData_t stateData) {
  if (lastState != smState) {  // Check to see if first time state has been called in sequence
    // 5a. Do anything that needs to happen the First time this state is called
    lastState = smState;  // Set the last state to the current state
  }
  // 5b. Do everything that repeats as long as this state is active

  // 5c. Extract data from jsonStateData_t (if required)
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

void sm_state_null(jsonStateData_t stateData) {
  Serial.println("ERROR: ->NULL STATE ENTERED!");
  stateData.uInt++;
}

// Init state, only called at startup
void sm_state_init(jsonStateData_t stateData) {
  if (lastState != smState) {
    Serial.println("state: init");
    stateData.uInt++;
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
void sm_state_wait(jsonStateData_t stateData) {
  if (lastState != smState) {
    stateData.uInt++;
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

void sm_state_servo(jsonStateData_t stateData) {
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
void sm_state_home(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: HOME"));
#endif
    stateData.uInt++;
    lastState = smState;
  }
  set_servo_pos(0);
  servo_pos = 0;
  smState = STATE_WAIT;
}


// Start the Motor / Update entered speed in Hz or RPM to output
void sm_state_tare(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: TARE"));
#endif
    stateData.uInt++;
    lastState = smState;
  }
  set_servo_pos(0);
  delay(4000);
  load.tare();
  encoder.calibrate_position();
  smState = STATE_WAIT;
}






// Change the samplerate of streamed or snapshotted data (Init at 10 Hz)
void sm_state_samplerate(jsonStateData_t stateData) {
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
void sm_state_printrate(jsonStateData_t stateData) {
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
void sm_state_start_stream(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("st: STARTSTREAM"));
#endif
    stateData.uInt++;
    lastState = smState;
  }
  streaming_active = true;
  samples_written = 0;  // start sampling again, wondering if this is the cause of the issues
  //  snapshot_timer_mS = jsonStateData_t.signedInt;
  smState = STATE_WAIT;
}



// Stop streaming data
void sm_state_stop_stream(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: STOPSTREAM"));
#endif
    stateData.uInt++;
    lastState = smState;
  }
  streaming_active = false;
  // snapshot_timer_mS = 0;
  smState = STATE_WAIT;
}


// NEWER FUNCTIONS from here -------------------------------->>>>

// Trigger a snapshot of data to be taken over X amount of time
void sm_state_set_secret(jsonStateData_t stateData) {
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




void sm_state_set_cal(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET CAL"));
#endif
    lastState = smState;
    //  Serial.println(stateData.signedInt);
    //  Serial.println(stateData.auth);
    memory.set_cal_value(stateData.floatData, stateData.auth);
  }
  smState = STATE_WAIT;
}






void sm_state_get_cal(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: GET CAL"));
#endif
    stateData.uInt++;
    lastState = smState;
  }
  cal = memory.get_cal();
  memory.report_cal();
  load.set_scale(cal.calData);
  smState = STATE_WAIT;
}


void sm_state_set_material(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET MATERIAL"));
#endif
    lastState = smState;
    memory.set_material(stateData.msg, stateData.auth);
  }
  smState = STATE_WAIT;
}


void sm_state_set_diameter(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET DIAMETER"));
#endif
    lastState = smState;
    memory.set_diameter(stateData.msg, stateData.auth);
  }
  smState = STATE_WAIT;
}

void sm_state_set_angle_max(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET ANGLE MAX"));
#endif
    lastState = smState;
    memory.set_angle_max(stateData.signedInt, stateData.auth);
  }
  smState = STATE_WAIT;
}


void sm_state_set_load_max(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET LOAD MAX"));
#endif
    lastState = smState;
    memory.set_load_max(stateData.signedInt, stateData.auth);
  }
  smState = STATE_WAIT;
}

void sm_state_get_settings(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: SET ANGLE MAX"));
#endif
    lastState = smState;
    stateData.uInt++;
    settings = memory.get_settings();
  }
  smState = STATE_WAIT;
}



// Print the commands list to the Serial Output
void sm_state_info(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: help"));
#endif
    stateData.uInt++;
    lastState = smState;
  }
  memory.report_cal();
  memory.report_settings();
  smState = STATE_WAIT;
}



// Print the commands list to the Serial Output
void sm_state_help(jsonStateData_t stateData) {
  if (lastState != smState) {
#if DEBUG_STATES == true
    Serial.println(F("state: help"));
#endif
    stateData.uInt++;
    lastState = smState;
  }
  print_cmds();
  smState = STATE_WAIT;
}



#include "stateMachine.h"




/*




 * Stores the current state of the state machine

 
stateDef SmState = STATE_START_CALIBRATION;


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

// 6. Finally define the state machine function
//    - Automatically generate the switch case from the list of ENUM states and list of functions! -> https://github.com/ImogenWren/switch-case-generator
void sm_Run(jsonStateData_t stateData) {
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
    (*StateMachine[smState].func)(stateData);

  } else {
    sm_state_wait(stateData);
    Serial.println(F("sm: Exception -> NUM_STATES exceeded"));
  }
}


#endif
