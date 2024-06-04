#include <SoftwareSerial.h>

SoftwareSerial nextion(10, 11); // RX, TX


const int ldrPin = A0; // Pin where the LDR is connected


const int adaptiveEnabled = 1;


void setup() {
  // Start the SoftwareSerial
  nextion.begin(9600);
  Serial.begin(9600);
  
}

void loop() {
  int ldrValue = analogRead(ldrPin); // Read the light level from the LDR
  int brightness = map(ldrValue, 0, 1023, 0, 100); // Map the LDR value to a brightness level (0-100)

  // Send the command to set brightness
  nextion.print("dims=" + String(brightness));
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);

  // Print the brightness setting to the Serial Monitor for debugging
  // Serial.println("dims=" + String(brightness));
  // Serial.println(adaptiveEnabled);

  // Adjust brightness slider
  nextion.print("settings.bslider.val=" + String(brightness));
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);

  delay(200);
}
