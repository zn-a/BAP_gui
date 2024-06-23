#include <SoftwareSerial.h>

SoftwareSerial nextion(10, 11); // RX, TX (adjust pins as necessary)

const int ldrPin = A0; // Pin where the LDR is connected
bool adaptiveBrightness = false;

void setup() {
  // Initialize SoftwareSerial for Nextion
  nextion.begin(115200); // Use the baud rate configured for Nextion
  Serial.begin(115200);  // Initialize serial for debugging

  // Wait for serial port to connect
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB
  }

  Serial.println("Debugging checkbox state:");
}

void loop() {
  // Check if there is data available from Nextion
  if (nextion.available()) {
    while (nextion.available()) {
      char c = nextion.read();
      if (c == 0x23) { // Custom command prefix
        char val = nextion.read();
        if (val == 0x01) {
          adaptiveBrightness = true; // Adaptive brightness ON
        } else if (val == 0x00) {
          adaptiveBrightness = false; // Adaptive brightness OFF
        }
        // Debugging output
        Serial.print("Adaptive Brightness: ");
        Serial.println(adaptiveBrightness ? "ON" : "OFF");
      }
    }
  }

  if (adaptiveBrightness) {
    int ldrValue = analogRead(ldrPin); // Read the light level from the LDR
    int brightness = map(ldrValue, 685, 740, 10, 100); // Map the LDR value to a brightness level (10-100)
    brightness = constrain(brightness, 10, 100);

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
  }

  delay(500);
}
