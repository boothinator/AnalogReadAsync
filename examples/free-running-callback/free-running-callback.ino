#include <analogReadAsync.h>

int numSamples = 0;

void analogReadComplete(uint16_t value, void *data)
{
  numSamples++;

  Serial.print("Sample ");
  Serial.print(numSamples);
  Serial.print(": ");
  Serial.println(value);

  if (numSamples >= 9)
  {
    // Note that we'll get one more analog read after disabling free-running
    setAnalogReadFreeRunning(false);
  }
}

void setup() {
  Serial.begin(115200);

  setAnalogReadFreeRunning(true);
  
  analogReadAsync(A0, analogReadComplete);
}

void loop() {

}
