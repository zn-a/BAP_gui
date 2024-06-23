#include <SoftwareSerial.h>

SoftwareSerial nextion(PA3, PA2); // RX, TX

const int ldrPin = PA1; // Pin where the LDR is connected
const float gain = 1.1; // Gain factor to increase sensitivity

bool adaptiveBrightness = false;

void setup() {
  // Start the SoftwareSerial
  nextion.begin(115200);
  Serial.begin(115200); // Initialize serial for debugging
}

void loop() {
  // Check if there is data available from Nextion
  if (nextion.available()) {
    Serial.print("Received: ");
    while (nextion.available()) {
      char c = nextion.read();
      Serial.print(c, HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

  delay(500); // Adjust as needed
}
