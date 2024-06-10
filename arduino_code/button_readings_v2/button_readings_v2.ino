#include "math.h"
#include <SoftwareSerial.h>

SoftwareSerial nextion(10, 11); // RX, TX.



void setup() {
  // nextion.begin(9600);
  nextion.begin(115200);
  Serial.begin(9600); // Initialize communication with Nextion display at 9600 bps

  // Initially display the welcome page
  // sendCommand("page splashScreen");
  // delay(1000); // Wait for 2 seconds to simulate the "fading" with a delay

  // // Switch to the main page
  // sendCommand("page pageMain");
}

void loop() {

  int amplitude = 1;
  int frequency = 50;


  // for (int i = 0; i < 360; i++) {
  // float radians = i * DEG_TO_RAD; // Convert degrees to radians
  // int value = amplitude * (sin(radians * frequency) + 1) / 2; // Calculate sine wave value and normalize

  // sendWaveformData(0, value); // Assuming '0' is the channel ID of the waveform component
  // delay(50); // Delay to control the speed of wave propagation
  // }

  // // Read from Nextion if there's any input (for handling button presses or other events)
  // if (nextion.available()) {
  //   String data = readSerial();
  //   // handleNextionEvents(data);
  // }

  // Generate simulated data
  float voltage = random(330, 500) / 100.0; // Simulate voltage from 3.3V to 5.0V
  float current = random(120, 180) / 100.0; // Simulate current from 1.2A to 1.8A
  float power = voltage * current; // Calculate power

  // Format the display text to include all three measurements
  String displayText = "V:\t" + String(voltage, 2) + " [V]\\r" +
                       "I:\t" + String(current, 2) + "  [A]\\r" +
                       "P:\t" + String(power, 2) + " [W]";

  // Update the Nextion display for each button
  updateNextionDisplay("b0.txt", displayText);
  updateNextionDisplay("b1.txt", displayText);
  updateNextionDisplay("b2.txt", displayText);

  updateNextionDisplay("b3.txt", displayText);
  updateNextionDisplay("b4.txt", displayText);
  updateNextionDisplay("b5.txt", displayText);

  delay(500); // Update interval
}

// Function to send updates to the Nextion
void updateNextionDisplay(String component, String value) {
  String command = component + "=\"" + value + "\"";
  sendCommand(command);
}

void sendWaveformData(uint8_t channel, int value) {
  nextion.print("add "); 
  nextion.print(channel);
  nextion.print(",");
  nextion.print(value);
  nextion.write(0xff);
  nextion.write(0xff);
  nextion.write(0xff);
}

// Function to send commands to the Nextion
void sendCommand(String command) {
  nextion.print(command);
  nextion.write(0xff);
  nextion.write(0xff);
  nextion.write(0xff);
}

// // Function to read data from Serial
// String readSerial() {
//   String data = "";
//   while (Serial.available()) {
//     data += char(Serial.read());
//   }
//   return data;
// }

// // Function to handle events (like button presses)
// void handleNextionEvents(String data) {
//   // Here, add code to react to button presses if needed
//   // This is where you would handle any specific interactions beyond displaying data
// }