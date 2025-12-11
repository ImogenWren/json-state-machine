/*  jsonReporter.h

 Functions to load data into a json template and print to Serial Output

*/




#include "globals.h"



// rounds a number to 2 decimal places
// example: round(3.14159) -> 3.14
double round2(double value) {
  return (int)(value * 100 + 0.5) / 100.0;
}


// rounds a number to X decimal places and ensures fixed width of signed and unsigned
void formatFloat(float value, char *buffer, size_t length, int decimalPlaces = 2) {
  if (length < 4) {  // Ensure buffer is large enough
    if (length > 0) buffer[0] = '\0';
    return;
  }

  char temp[16];  // Temporary buffer
  dtostrf(value, length - 1, decimalPlaces, temp);

  // Replace '+' with space for positive numbers
  if (temp[0] == '+') temp[0] = ' ';

  // Right-align by shifting if necessary
  size_t tempLen = strlen(temp);
  if (tempLen < length - 1) {
    size_t padding = length - 1 - tempLen;
    memmove(buffer + padding, temp, tempLen + 1);
    memset(buffer, ' ', padding);
  } else {
    strncpy(buffer, temp, length - 1);
    buffer[length - 1] = '\0';  // Ensure null termination
  }
}




void update_json(int16_t num_samples) {

  StaticJsonDocument<JSON_TX_BUFFER_SIZE> jsonTX;


  char float_buffer[10];  // buffer to holf fixed length floats
  //freeRAM = ram.getPrintStats("update_json");
  // Header
  jsonTX[F("timestamp")].set(millis());  //Message Timestamp set this last before printing
                                         // Payload
                                         // jsonTX[F("payload")][F("state")][F("val")].set(smState);
                                         // jsonTX[F("payload")][F("state")][F("name")].set(stateNames[smState]);
                                         // jsonTX[F("payload")][F("state")].set(stateNames[smState]);

  // new method to get state name text from progmem
   char buffer[14];
   strcpy_P(buffer, (char*)pgm_read_ptr(&(stateNames[smState])));  // Necessary casts and dereferencing, just copy.
    jsonTX[F("payload")][F("state")].set(buffer);


  //char buffer[13];
  //const char *ptr = (const char *)pgm_read_word(&stateNames[smState]);
 // strncpy_P(buffer, ptr, sizeof(buffer));
 // buffer[sizeof(buffer) - 1] = "\0";
 // jsonTX[F("payload")][F("state")].set(buffer);

  // jsonTX[F("payload")][F("state")][F("val")].set(smState);  only needed for debugging state machine


  // Output Data
  jsonTX[F("payload")][F("servo")].set(servo_pos);




  // For Arrays
  // create all the JSONarrays
  JsonArray encodePosArray = jsonTX[F("payload")][F("encode")].createNestedArray("pos");
  JsonArray encodeAngleArray = jsonTX[F("payload")][F("encode")].createNestedArray("angle");
  JsonArray loadCellArray = jsonTX[F("payload")][F("loadcell")].createNestedArray("force");
  jsonTX[F("payload")][("meta")][F("samples")].set(num_samples);
  JsonArray timeArray = jsonTX[F("payload")][F("meta")].createNestedArray("time");

  // for loop to place data into arrays (is it possible to just pass whole string without looping up to 200 times!?)

  for (int i = 0; i < num_samples; i++) {

    // Serial.print("i: ");
    // Serial.print(i);
    // Serial.print(", ");

    encodePosArray.add(encoder_pos_array[i]);

    formatFloat(encoder_angle_array[i], float_buffer, sizeof(float_buffer), 3);
    encodeAngleArray.add(float_buffer);

    formatFloat(loadcell_array[i], float_buffer, sizeof(float_buffer), 3);
    loadCellArray.add(float_buffer);

    timeArray.add(timestamp_array[i]);
    //Serial.print(encode_array[i]);
    //  Serial.print(", ");
  }
  // Serial.println();



  // Meta Data & Debugging
  //jsonTX[F("meta")][F("sizeof")].set(sizeof(jsonTX));


  // int JSON_bytes = sizeof(jsonTX);  // used for debugging
  //Serial.print("Size of JSON: ");
  //Serial.println(JSON_bytes);

  if (PRINT_JSON) {
#if PRETTY_PRINT_JSON == true
    //#pragma JSON set to Pretty Print !-May experience problems with Machine Parsing
    serializeJsonPretty(jsonTX, Serial);
#else
    //#pragma JSON set to standard JSON output - For human readability try enabling Pretty Print !
    serializeJson(jsonTX, Serial);
#endif
    Serial.print(F("\n"));
  }
}