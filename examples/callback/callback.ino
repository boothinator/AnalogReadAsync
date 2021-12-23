#include <analogReadAsync.h>

void analogReadComplete(uint16_t value, void *data)
{
  Serial.println(value);
}

void setup() {
  Serial.begin(115200);
  
  analogReadAsync(A0, analogReadComplete);
}

void loop() {

}
