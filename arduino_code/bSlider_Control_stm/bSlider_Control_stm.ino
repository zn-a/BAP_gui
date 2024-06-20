#include <SoftwareSerial.h>

SoftwareSerial nextion(PA3, PA2); // RX, TX

const int ldrPin = PA1; // Pin where the LDR is connected

const int LDR_MIN = 530;  // Minimum LDR value when covered
const int LDR_MAX = 570;  // Maximum LDR value when exposed to bright light

void setup() {
  // Start the SoftwareSerial
  nextion.begin(115200);
  Serial.begin(115200); // Initialize serial for debugging
}

void loop() {

  Serial.println("test");
  // int ldrValue = analogRead(ldrPin); // Read the light level from the LDR
  
  // // Calibrate the LDR value
  // ldrValue = constrain(ldrValue, LDR_MIN, LDR_MAX);
  
  // // Map the calibrated LDR value to a brightness level (10-100)
  // int brightness = map(ldrValue, LDR_MIN, LDR_MAX, 10, 100);

  // // Send the command to set brightness
  // nextion.print("dims=");
  // nextion.print(brightness);
  // nextion.write(0xFF);
  // nextion.write(0xFF);
  // nextion.write(0xFF);

  // // Adjust brightness slider
  // nextion.print("pageSettings.bslider.val=");
  // nextion.print(brightness);
  // nextion.write(0xFF);
  // nextion.write(0xFF);
  // nextion.write(0xFF);

  // // Update the number component to reflect the brightness level
  // nextion.print("pageSettings.n0.val=");
  // nextion.print(brightness);
  // nextion.write(0xFF);
  // nextion.write(0xFF);
  // nextion.write(0xFF);

  // // Print the LDR value for calibration
  // Serial.print("LDR Value: ");
  // Serial.println(ldrValue);

  // // Debugging output
  // Serial.print("LDR Value: ");
  // Serial.print(ldrValue);
  // Serial.print(" -> Brightness: ");
  // Serial.println(brightness);

  delay(500);
}
