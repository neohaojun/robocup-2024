#include "main.h"

void setup() {
  #ifdef SERIAL_DEBUG
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Debug serial connection established.");
  #endif
}

void loop() {
  
}