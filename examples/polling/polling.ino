#include <analogReadAsync.h>

void setup() {
  Serial.begin(115200);
  
  analogReadAsync(A0);
  
  while (!analogReadComplete());

  uint16_t value = getAnalogReadValue();

  Serial.println(value);
}

void loop() {
}
