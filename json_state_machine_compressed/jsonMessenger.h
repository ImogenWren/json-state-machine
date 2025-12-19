/* jsonMessenger.h

_Library to simplify two way communication over Serial interfaces using JSON formatted messages_

Imogen Heard
17/10/24

Please see: https://github.com/ImogenWren/jsonMessenger_library for latest version and usage instructions
NOTE: This library is memory intensive and may not be suitable for lightweight microcontrollers (uno, nano, AtTiny etc)

Version V1.0.0
- Focus on receiving & parsing incoming Serial CMD messages in JSON format

Version v3
- Tying everything together finally


*/

//#pragma once

#ifndef jsonMessenger_h
#define jsonMessenger_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include <stdio.h>
#include <ArduinoJson.h>


#include "jsonConfig.h"

#define JSON_MESSENGER_VERSION "3.0.0"




class jsonMessenger {
private:



public:
  // constructor
  jsonMessenger();

  void jsonBegin();

  jsonStateData_t jsonReadSerialLoop();  // function needs to be called periodically to accept incoming serial messages, parse & return jsonStateData_t with new state /and data required for new state

  void printJSON(StaticJsonDocument<JSON_RX_SIZE> *jsonDoc);  // Prints the static jsonRXdoc


 // const char *getCMDkey(jsonStates state);  // to print the state in human readable form without coping the entire list of keys to global scope

  const char *getDataType(dataTypes type);

  void printJSONdata(jsonStateData_t *data);

  //  void print_cmds();

#if JSON_USE_QUEUE == true
  int8_t enque_cmd(jsonStateData_t *newItem);  // Returns the number of items in the queue, or -1 on fail

  jsonStateData_t dequeue_cmd();  // returns first queue item or 0 if queue is empty when called
#endif

private:
  void safe_copy_string(char *target, const char *newString, uint8_t targetLength);

#if JSON_USE_QUEUE == true
  jsonStateData_t jsonQueue[CMD_QUEUE_LENGTH];
  int8_t queue_size = 0;  // must be managed "manually"
#endif
};



#endif
