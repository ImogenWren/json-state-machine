/*  jsonConfig.h

STEP 2:
 - This header defines:
      - all the possible datatypes that can be passed with a JSON message
      - the commands that will be required for users to interact with the state machine
      - A map of each command along with the datatype that will be passed with the command, and the state which this command will navigate to
      - A structure that will be passed out of the jsonMessenger library into every state, which will contain all the required data for any state


*/

//#pragma once




#ifndef jsonConfig_h
#define jsonConfig_h

#include "stateConfig.h"  /// we should have already defined all the valid states and we will need them here

// As alternative to <map> data structure has been developed, this is all depreciated
/*
#ifdef __AVR__
#include <ArduinoSTL.h>  // [Arduino Library Manager][Modified Version -> https://github.com/ImogenWren/ArduinoSTL]
#pragma "ArduinoSTL Libary Included"
#elif defined(STM32) || defined(ARDUINO_ARCH_STM32)
#pragma "STM32 Board -> Using Arrays Instead of std::stl"
#endif

//#include <map>  // [std::map]
*/


#define DEBUG_JSON_MESSENGER true

#define JSON_USE_QUEUE false        //At least one of these should be true
#define JSON_USE_SINGLE_FRAME true  // Single frame is always valid, but this can be used to disable features not wanted when using queue

#define JSON_RX_SIZE 64     // 32 Working on Arduino Nano
#define CMD_QUEUE_LENGTH 3  // 3 Working on Arduino Nano
#define JSON_MSG_LENGTH 17  // Length of msg array in json data structure
#define AUTH_MSG_LENGTH 9


// 1. Define an enum to define variable type used, These will be linked to a state enum so when a keyword is received, we can look up what data type should be sent with it

typedef enum {  // enum to pass variable types between functions
  EMPTY,
  INTEGER,
  UINT,
  FLOAT,
  CSTRING,
  BOOL,
  AUTH_INT,    // 14/07/25 adding new datatype, this will tell jsonMessenger that 2 datas will be present, but will use already existing datatypes in RX data structure
  AUTH_FLOAT,  // 12/12/25 auth with float data
  AUTH_MSG     // 27/11/25 adding new datatype, auth with char message
} dataTypes;

// 1b. Place these enums into an array to enable lookup by index (This shouldnt be nessissary but it fixed a bug at one point)
const dataTypes dataTypes_array[8] = { EMPTY, INTEGER, UINT, FLOAT, CSTRING, BOOL, AUTH_INT, AUTH_MSG };

// these are just to silence compiler warnings, maybe there is a better way of doing this but I havnt figured it out yet
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

// 1c. List the same enums as cStrings, this will enable human readability of enum above
static char typeNames[][8] = {
  "EMPTY",
  "INTEGER",
  "UINT",
  "FLOAT",
  "CSTRING",
  "BOOL",
  "AUTHINT",
  "AUTHFLT",
  "AUTHMSG"
};

#pragma GCC diagnostic pop



// 2. Declare a list of all possible key values as ENUM. These values will be passed out of the jsonMessenger Object and can be used to trigger different states
// NOTE: this list may not include all possible states for the state machine, JUST the states that are triggered by receiving a command.
// - Include a null value at 0 -> This is because this enum will be initialised at 0 to represent jsonMessenger not receiving data, or being unable to parse a command

// NEW INSTRUCTION -> This list is likely depreciated and Is only here currently for legacy reasons, it may be removed in the next version
typedef enum {
  NONE,  // Include null or none state
  SERVO,
  HOME,
  TARE,
  SAMPLERATE,
  PRINTRATE,
  STARTSTREAM,
  ENDSTREAM,
  SET_SECRET,
  SET_CAL,
  GET_CAL,
  SET_MATERIAL,
  SET_DIAMETER,
  SET_ANGLEMAX,
  SET_LOADMAX,
  GET_SETTINGS,
  INFO,
  HELP,
  NUM_VALUES  // Add sentinal NUM_VALUES to count number of elements, this is very important and will be used to size for loops inside the jsonMessenger object
} jsonStates;



// these are just to silence compiler warnings, maybe there is a better way of doing this but I havnt figured it out yet
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

// 4. Declare a list of key commands that will be required to be parsed. This must match the order of the enums above
static char jsonCommandKeys[][8] = {
  "na",
  "servo",
  "home",
  "tare",
  "sample",
  "print",
  "stream",
  "endst",
  "secret",
  "cal",
  "getcal",
  "setmat",
  "setdia",
  "setang",
  "setload",
  "recall",
  "info",
  "help"
};

#pragma GCC diagnostic pop


// 3. Link each jsonState ENUM with the datatype ENUM in a ~~map~~<depreciated> structure -> Now uses 2D array!.
//const std::map<jsonStates, dataTypes> jsonStateMap = {  // Old version
// also link directly to the state it triggers?



typedef struct {
  jsonStates cmd;       //< Defines the ENUM for the command
  dataTypes data_type;  //< Defines the ENUM for the data type
  stateDef state;
} jsonStateMapData_t;

const jsonStateMapData_t jsonStateMap[NUM_VALUES] = {
  { jsonStates::NONE, dataTypes::EMPTY, STATE_NULL },
  { jsonStates::SERVO, dataTypes::INTEGER, STATE_SERVO },
  { jsonStates::HOME, dataTypes::EMPTY, STATE_HOME },
  { jsonStates::TARE, dataTypes::INTEGER, STATE_TARE },
  { jsonStates::SAMPLERATE, dataTypes::UINT, STATE_SAMPLERATE },
  { jsonStates::PRINTRATE, dataTypes::UINT, STATE_PRINTRATE },
  { jsonStates::STARTSTREAM, dataTypes::EMPTY, STATE_STARTSTREAM },
  { jsonStates::ENDSTREAM, dataTypes::EMPTY, STATE_STOPSTREAM },

  { jsonStates::SET_SECRET, dataTypes::CSTRING, STATE_SETSECRET },
  { jsonStates::SET_CAL, dataTypes::AUTH_FLOAT, STATE_SET_CAL },
  { jsonStates::GET_CAL, dataTypes::EMPTY, STATE_GET_CAL },
  { jsonStates::SET_MATERIAL, dataTypes::AUTH_MSG, STATE_SET_MATERIAL },
  { jsonStates::SET_DIAMETER, dataTypes::AUTH_MSG, STATE_SET_DIAMETER },
  { jsonStates::SET_ANGLEMAX, dataTypes::AUTH_INT, STATE_SET_ANGLEMAX },
  { jsonStates::SET_LOADMAX, dataTypes::AUTH_INT, STATE_SET_LOADMAX },
  { jsonStates::GET_SETTINGS, dataTypes::EMPTY, STATE_GET_SETTINGS },
  { jsonStates::INFO, dataTypes::EMPTY, STATE_INFO },
  { jsonStates::HELP, dataTypes::EMPTY, STATE_HELP }
};





// NOTE, this can also be used to turn the enums above back into strings for human readability


//4a. declare a list of descriptions for each key command, this will be used by the "help" state to print
// command list and functions to user (maintain order as above)

// THis might be causing issues so going back to an older way of doing it, more RAM intensive but less promem use which seems to be the problem now
// list defined in cmds.h

/*
const char desc_0[] PROGMEM = "n/a";
const char desc_1[] PROGMEM = "Set Servo Position as %";
const char desc_2[] PROGMEM = "Home the Servo (pos 0)";
const char desc_3[] PROGMEM = "Zero encoder position & Load Cell";
const char desc_4[] PROGMEM = "Set Samplerate in Hz (dflt: 1)";
const char desc_5[] PROGMEM = "Set Print Rate in Hz (dflt: 1)";
const char desc_6[] PROGMEM = "Start Data Streaming";
const char desc_7[] PROGMEM = "End Data Streaming";
const char desc_8[] PROGMEM = "set the secret word";
const char desc_9[] PROGMEM = "save load cell cal to memy";
const char desc_10[] PROGMEM = "get calibration from memory";
const char desc_11[] PROGMEM = "set material";
const char desc_12[] PROGMEM = "set diameter";
const char desc_13[] PROGMEM = "set angle max";
const char desc_14[] PROGMEM = "set load max";
const char desc_15[] PROGMEM = "print settings, cal data & validity";
const char desc_16[] PROGMEM = "print commands list";
const char desc_17[] PROGMEM = "n/a";




const char *const jsonCmdDescription[] PROGMEM = {
  desc_0, desc_1, desc_2, desc_3, desc_4, desc_5, desc_6, desc_7,
  desc_8, desc_9, desc_10, desc_11, desc_12, desc_13, desc_14,
  desc_15, desc_16, desc_17
};



//4b. declare a list of the range of input data for each state

const char range_0[] PROGMEM = "n/a";
const char range_1[] PROGMEM = "0 to 100";
const char range_2[] PROGMEM = "null";
const char range_3[] PROGMEM = "null";
const char range_4[] PROGMEM = "1 to 200";
const char range_5[] PROGMEM = "1 to 200";
const char range_6[] PROGMEM = "null";
const char range_7[] PROGMEM = "null";
const char range_8[] PROGMEM = "char[8]";
const char range_9[] PROGMEM = "integer ,\"auth\":\"char[8]\"";
const char range_10[] PROGMEM = "null";
const char range_11[] PROGMEM = "char[16],\"auth\":\"char[8]\"";
const char range_12[] PROGMEM = "char[8],\"auth\":\"char[8]\"";
const char range_13[] PROGMEM = "uint,\"auth\":\"char[8]\"";
const char range_14[] PROGMEM = "int,\"auth\":\"char[8]\"";
const char range_15[] PROGMEM = "null";
const char range_16[] PROGMEM = "null";
const char range_17[] PROGMEM = "n/a";


const char *const jsonCmdRange[] PROGMEM = {
  range_0, range_1, range_2, range_3, range_4, range_5, range_6, range_7,
  range_8, range_9, range_10, range_11, range_12, range_13, range_14,
  range_15, range_16, range_17
};


*/








// 5. Finally Declare a structure that will hold both the jsonStates enum, and any data that will need to be passed from jsonMessenger, into the states.
// We can make this fairly generic by including additional datatypes, or we can reduce the size of the memory used by removing the unneeded ones
struct jsonStateData_t {
  stateDef stateEnum;   // This likely supercedes the commandState enum, as we can just direcly plug this variable into smState, saving another list of if/elses
  jsonStates cmdState;  // The command state enum to tell state machine what state to go to next
  dataTypes data_type;  // The type of data being passed along with structure (though state should know what data to expect anyway), this could be removed to save space
  int16_t signedInt;    // empty generic data slots for each data type
  uint16_t uInt;
  float floatData;
  char msg[JSON_MSG_LENGTH];
  char auth[AUTH_MSG_LENGTH];
  bool cmd_received;  // Flag set true by jsonLoop when cmd is received
};

// Note on this structure, passing a huge (and growing!) structure back and forwards certainly isnt the most memory efficient way of doing this. Need to do more research into template functions with different return types
// that should enable similar functionality with less overhead?



#endif