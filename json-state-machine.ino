/* JSON State Machine
// Experimental Sketch to test out the limits of abstraction for this use case
// Demonstration of using a single class to abstract the function of jsonMessenger and stateConfig
// Demonstration of jsonReporter as a class

// Version 1.0
Sketch uses 27062 bytes (88%) of program storage space. Maximum is 30720 bytes.
Global variables use 1043 bytes (50%) of dynamic memory, leaving 1005 bytes for local variables. Maximum is 2048 bytes.
- excessive use of switch case and if/else lists

Version 2.0
- going back to using this structure to try and cut down on manual typing

    (*StateMachine[SmState].func)();


*/


#include "globals.h"

#define LIMITS_ENABLED true
#define LIMIT_SWITCH_ENABLED true


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);  // give time for Serial object to start
  }
  print_info();


  //pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
  servoSetup();
  pinMode(LED_BUILTIN, OUTPUT);

  set_servo_pos(0);

  delay(2000);  // give time for servo to reach home position

  led.begin();
  limit.begin();

  encoder.begin();
  attachInterrupt(digitalPinToInterrupt(ENCODER_WHITE_PIN), isr_A, CHANGE);  // interrupt 0 is pin 2
  attachInterrupt(digitalPinToInterrupt(ENCODER_GREEN_PIN), isr_B, CHANGE);  // interrupt 0 is pin 2

  // get settings and cal data from memory

  cal = memory.get_cal();
  settings = memory.get_settings();


  load.begin(LOAD_CELL_DAT, LOAD_CELL_CLK);
  load.set_gain(HX711_CHANNEL_A_GAIN_128);
  load.set_scale(cal.calData);
  // load.set_scale(17740);        // init: 17783


  delay(1000);  // delay to allow system to settle
  load.tare();
  encoder.calibrate_position();
}


void loop() {


  // Json Messenger & State Machine
  jsonStateData nextState_data = jsonRX.jsonReadSerialLoop();


  if (nextState_data.cmd_received) {  // If command is receive   //delay(10);


    const char* cmd = jsonRX.getCMDkey(nextState_data.cmdState);  // I feel like the entire point of using ENUMs is being totally lost by doing this, but it is working
    //std::cout << std::endl;
    // std::cout << "{\"rx-cmd\":\"" << cmd << "\",\"datatype\":\"" << jsonRX.getDataType(nextState_data.data_type) << "\",\"data\":\"";
    Serial.print(F("{\"rx-cmd\":\""));
    Serial.print(cmd);
    Serial.print(F("\",\"datatype\":\""));
    Serial.print(jsonRX.getDataType(nextState_data.data_type));
    Serial.print(F("\",\"data\":\""));
    if (nextState_data.data_type == INTEGER) Serial.print(nextState_data.signedInt);  //std::cout << nextState_data.signedInt;
    if (nextState_data.data_type == FLOAT) Serial.print(nextState_data.floatData);    //std::cout << nextState_data.floatData;
    if (nextState_data.data_type == CSTRING) Serial.print(nextState_data.msg);        //std::cout << nextState_data.msg;
    if (nextState_data.data_type == EMPTY) Serial.print("n/a");                       //std::cout << "n/a";
    // Is this now missing float clause?
    //std::cout << "\"}" << std::endl;
    Serial.println("\"}");

    // This is the bit that parses the command recieved by user, and sets the state machine to go to the correct state
    if (nextState_data.cmdState == SERVO) {  // if fan speed change command received
      smState = STATE_SERVO;
    } else if (nextState_data.cmdState == HOME) {
      smState = STATE_HOME;
    } else if (nextState_data.cmdState == TARE) {
      smState = STATE_TARE;
    } else if (nextState_data.cmdState == SAMPLERATE) {
      smState = STATE_SAMPLERATE;
    } else if (nextState_data.cmdState == PRINTRATE) {
      smState = STATE_PRINTRATE;
    } else if (nextState_data.cmdState == STARTSTREAM) {
      smState = STATE_STARTSTREAM;
    } else if (nextState_data.cmdState == ENDSTREAM) {
      smState = STATE_STOPSTREAM;
    } else if (nextState_data.cmdState == SET_SECRET) {
      smState = STATE_SETSECRET;
    } else if (nextState_data.cmdState == SET_CAL) {
      smState = STATE_SET_CAL;
    } else if (nextState_data.cmdState == GET_CAL) {
      smState = STATE_GET_CAL;
    } else if (nextState_data.cmdState == SET_MATERIAL) {
      smState = STATE_SET_MATERIAL;
    } else if (nextState_data.cmdState == SET_DIAMETER) {
      smState = STATE_SET_DIAMETER;
    } else if (nextState_data.cmdState == SET_ANGLEMAX) {
      smState = STATE_SET_ANGLEMAX;
    } else if (nextState_data.cmdState == SET_LOADMAX) {
      smState = STATE_SET_LOADMAX;
    } else if (nextState_data.cmdState == GET_SETTINGS) {
      smState = STATE_GET_SETTINGS;
    } else if (nextState_data.cmdState == INFO) {
      smState = STATE_INFO;
    } else if (nextState_data.cmdState == HELP) {
      smState = STATE_HELP;
    } else {
      // std::cout << "{\"WARNING\":\"Unrecognised cmdState\"}" << std::endl;
      Serial.println(F("{\"WARNING\":\"Unrecognised cmdState\"}"));
    }
  }
  sm_Run(nextState_data);  // This Runs the state machine in the correct state, and is passed all of the data sent by the last command







  //Load Cell


  // Do sampling Data at the specified rate
  if (sampleDelay.millisDelay(sampleDelay_mS)) {
    //   Serial.print("delay function");                                                //disabled this as only 1 sample //|| samples_written == 0) {           // added OR if samples written has been reset to 0 after printing
    if (samples_written < num_samples_req && samples_written < DATA_ARRAY_SIZE) {  // check to make sure collecting the correct number of samples for the samplerate, and smaller than the
                                                                                   //  Serial.print("moar samples please");
      if (load.is_ready()) {

        timestamp_array[samples_written] = millis();
        loadcell_array[samples_written] = load.get_units(10);
        int16_t encode_test = encoder.getEncoderPos();
        //Serial.println(encode_test); this was just here for testing
        encoder_pos_array[samples_written] = encode_test;  // encoder.getEncoderPos();
        encoder_angle_array[samples_written] = encoder.convertPosDeg();
        samples_written++;
      }
    }
    //  samples_written++;    // this line being here was what was causing the bug, it should have been nested within the loop that actually writes a sample,
  }



// Periodic Measurements & limits
// angle limit
#if LIMITS_ENABLED == true
  if (servo_pos > 5) {
    if (encoder_angle_array[0] >= settings.angleMax) {
      smState = STATE_HOME;  // if it exceeds limit, go to home state
      Serial.println(F("{\"ERROR\":\"angle limit exceeded, homing\"}"));
    } else if (encoder_angle_array[0] >= (settings.angleMax - ANGLE_SOFT_LIMIT_WIDTH)) {
      Serial.println(F("{\"WARNING\":\"angle limit -> extension disabled\"}"));
      // set flag to tell servo state not to extend servo
      ANGLE_SOFT_LIMIT = true;
    } else {
      ANGLE_SOFT_LIMIT = false;
    }

    // load limit
    if (loadcell_array[0] >= settings.loadMax) {
      smState = STATE_HOME;  // if it exceeds limit, go to home state
      Serial.println(F("{\"ERROR\":\"load limit exceeded, homing\"}"));
    } else if (loadcell_array[0] >= (settings.loadMax - LOAD_SOFT_LIMIT_WIDTH)) {
      Serial.println(F("{\"WARNING\":\"load limit -> extension disabled\"}"));
      // set flag to tell servo state not to extend servo
      LOAD_SOFT_LIMIT = true;
    } else {
      LOAD_SOFT_LIMIT = false;
    }
  }
#endif
  // load limit



  // Limit Switch Management
  limit.buttonLoop(750);  // long int passed controls the time required for a long press. (defaults to 1 second)
#if LIMIT_SWITCH_ENABLED == true
  if (limit.buttonPress) {
    led.callBlink(3, 200, 200);
    limit.buttonReset();
    smState = STATE_HOME;
  }
#endif

  led.performBlink();



  // do streaming data at the specified rate
  if (streaming_active || snapshop_active) {
    if (printDelay.millisDelay(print_delay_mS)) {
      if (samples_written >= num_samples_req) {  // REMOVED PRINT TIMER
        sampleDelay.resetDelayTime_mS();         // makes sure that the sample loop is synced to the printing loop //moved to try and improve timings (doing this first so next sample is sooner)
        //print the sampled data
        update_json(samples_written);
        samples_written = 0;
      }
    }
  } else {
    samples_written = 0;  // this still needs to be reset to allow data to be collected for other purposes (limits etc)
  }
  // Time Out Tools and utility loop functions
  // Fix for servo chattering, servo is attached during PING state, and unattached on timer
  //  if (servo.attached()) {
  //   if (millis() - servo_attach_time_mS >= SERVO_TIMEOUT_mS) {
  //     servo.detach();
  //     Serial.println("Servo Unattached");
  //   }
  //  }
}


// Interrupt on A changing state
void isr_A() {
  encoder._isr_A();
}

// Interrupt on B changing state
void isr_B() {
  encoder._isr_B();
}
