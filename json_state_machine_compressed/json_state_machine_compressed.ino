/* JSON State Machine
// Experimental Sketch to test out the limits of abstraction for this use case
// Demonstration of using a single class to abstract the function of jsonMessenger and stateConfig
// Demonstration of jsonReporter as a class

// Version 1.0
Sketch uses 27062 bytes (88%) of program storage space. Maximum is 30720 bytes.
Global variables use 1043 bytes (50%) of dynamic memory, leaving 1005 bytes for local variables. Maximum is 2048 bytes.
- excessive use of switch case and if/else lists


Version 2.0 changed jsonMessenger to use map structure isntead of array, and simplify main loop code by passing new state enum directly
Sketch uses 26224 bytes (85%) of program storage space. Maximum is 30720 bytes.
Global variables use 1062 bytes (51%) of dynamic memory, leaving 986 bytes for local variables. Maximum is 2048 bytes.

Version 2.1
- going back to using this function pointer structure to try and cut down on manual typing
Sketch uses 26580 bytes (86%) of program storage space. Maximum is 30720 bytes.
Global variables use 1172 bytes (57%) of dynamic memory, leaving 876 bytes for local variables. Maximum is 2048 bytes.
A little more memory intensive but much less typing dependent.
- We have eliminated both the state machine case structure, and the main loop if/else structure
- Still more things that can be done to save overhead, state map in jsonConfig no longer needs to define its own enum list, can use the table of 
    commands directly in the map, 
    - also may be able to PROGMEM the commands list, though this will require a small re-write of jsonMessenger.cpp to get those values from PROGMEM during runtime.

// bug fixes for above
Sketch uses 25228 bytes (82%) of program storage space. Maximum is 30720 bytes.
Global variables use 1144 bytes (55%) of dynamic memory, leaving 904 bytes for local variables. Maximum is 2048 bytes.
  
Version 2.2
- Removed all code specific to last implementation
Sketch uses 21988 bytes (71%) of program storage space. Maximum is 30720 bytes.
Global variables use 970 bytes (47%) of dynamic memory, leaving 1078 bytes for local variables. Maximum is 2048 bytes.

now implementing removing the jsonState enums as they are redundent


*/


#include "globals.h"





void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);  // give time for Serial object to start
  }
  print_info();  // print the program info -> enables identification of current loaded firmware


  // Setup IO PIns
  pinMode(LED_BUILTIN, OUTPUT);


  // get settings and cal data from memory
  cal = memory.get_cal();
  settings = memory.get_settings();


  // Start/Calibrate Sensors -> load scales from memory


  delay(1000);  // delay to allow system to settle

  // Tare/zero sensors
}





void loop() {


  // Json Messenger & State Machine
  jsonStateData_t nextState_data = jsonRX.jsonReadSerialLoop();


  if (nextState_data.cmd_received) {  // If command is receive
    // This is the bit that parses the command recieved by user, and sets the state machine to go to the correct state
    if (nextState_data.stateEnum != STATE_NULL) {
      smState = nextState_data.stateEnum;
    }
  }
  sm_Run(nextState_data);  // This Runs the state machine in the correct state, and is passed all of the data sent by the last command










  // Do sampling Data at the specified rate
  if (sampleDelay.millisDelay(sampleDelay_mS)) {
    if (samples_written < num_samples_req && samples_written < DATA_ARRAY_SIZE) {  // check to make sure collecting the correct number of samples for the samplerate, and smaller than the
      timestamp_array[samples_written] = millis();
      data_array_one[samples_written] = arbitaryData;
      data_array_two[samples_written] = arbitaryData + 1;  // encoder.getEncoderPos();
      data_array_three[samples_written] = arbitaryData + 2;
      arbitaryData++;  // just a placeholder to generate moving data
      samples_written++;
    }
  }



// Periodic Measurements & limits go here -> they might use data sampled above to make decisions
#if LIMITS_ENABLED == true

#endif











  // do streaming data at the specified rate

  if (samples_written >= num_samples_req) {  // REMOVED PRINT TIMER because the number of samples taken is already calculated to meet the printing time   // if (printDelay.millisDelay(print_delay_mS)) {
    sampleDelay.resetDelayTime_mS();         // makes sure that the sample loop is synced to the printing loop //moved to try and improve timings (doing this first so next sample is sooner)
    samples_written = 0;
    if (streaming_active || snapshop_active) {
      //print the sampled data
      update_json(samples_written);
    }
  }


  // Time Out Tools and utility loop functions
  led.performBlink();
}



// Interrupt Functions if Required
// Interrupt on A changing state
void isr_A() {
  // encoder._isr_A();  // example of calling ISR from library
}

// Interrupt on B changing state
void isr_B() {
  //encoder._isr_B();
}
