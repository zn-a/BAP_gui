#include <SoftwareSerial.h>

SoftwareSerial nextion(PA3, PA2); // RX, TX

const int ldrPin = PA1; // Pin where the LDR is connected
const float gain = 1.0; // Gain factor to increase sensitivity

void setup() {
  // Start the SoftwareSerial
  nextion.begin(115200);
  Serial.begin(115200); // Initialize serial for debugging
  // analogReadResolution(10); // STM32 ADC is 12-bit by default
}

void loop() {
  int ldrValue = analogRead(ldrPin); // Read the light level from the LDR
    int brightness = map(ldrValue, 685, 740, 10, 100); // Map the LDR value to a brightness level (10-100)
    brightness = constrain(brightness, 10, 100);

  // brightness = constrain(brightness * gain, 0, 100);

  // Send the command to set brightness
  nextion.print("dims=");
  nextion.print(brightness);
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);

  // Adjust brightness slider
  nextion.print("pageSettings.bslider.val=");
  nextion.print(brightness);
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);

  // Update the number component to reflect the brightness level
  nextion.print("pageSettings.n0.val=");
  nextion.print(brightness);
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);

  // Debugging output
  Serial.print("LDR Value: ");
  Serial.print(ldrValue);
  Serial.print(" -> Brightness: ");
  Serial.println(brightness);

  delay(500);
}
