#include <analogReadAsync.h>

void setup() {
  Serial.begin(115200);

  setAnalogReadFreeRunning(true);
  
  analogReadAsync(A0);

  for (int numSamples = 0; numSamples <10; numSamples++)
  {
    while (!getAnalogReadComplete());
  
    uint16_t value = getAnalogReadValue();

    Serial.print("Sample ");
    Serial.print(numSamples + 1);
    Serial.print(": ");
    Serial.println(value);
  }
  
  setAnalogReadFreeRunning(false);
}

void loop() {

}
