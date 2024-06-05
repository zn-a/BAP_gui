#include <SoftwareSerial.h>

// Define the pins for SoftwareSerial
#define RX_PIN 10
#define TX_PIN 11

// Initialize SoftwareSerial for communication with Nextion
SoftwareSerial nextion(RX_PIN, TX_PIN);

// List of specific voltages to cycle through
int voltages[] = {9, 12, 40};
const int numVoltages = sizeof(voltages) / sizeof(voltages[0]);
const float resistance = 10.0; // Resistance in ohms

void setup() {
  // Begin Serial communication at 9600 baud
  nextion.begin(9600);
  // randomSeed(analogRead(0)); // Initialize random number generator
}

void loop() {

  // Generate DC wave
  for (int i = 0; i < numVoltages; i++) {
    int voltage = voltages[i];
    // Calculate current and power based on the current voltage
    float current = voltage / resistance;
    float power = voltage * current;

    // Random hold time between 1 and 2 seconds
    int holdTime = random(3000, 4001);

    // Send values to Nextion and hold
    updateNextionWaveform(voltage, current, power, holdTime);

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
  }





}

void updateNextionWaveform(int voltage, float current, float power, int holdTime) {
  unsigned long startTime = millis();
  while (millis() - startTime < holdTime) {
    // Send the voltage value to Channel 1
    sendToNextion(1, 0, voltage);
    // Send the current value to Channel 2
    sendToNextion(1, 1, int(current * 1));  // Multiply by 10 for better resolution
    // Send the power value to Channel 3
    sendToNextion(1, 2, int(power));

    delay(1); // Refresh rate of 50 ms
  }
}

// Function to send updates to the Nextion
void updateNextionDisplay(String component, String value) {
  String command = component + "=\"" + value + "\"";
  sendCommand(command);
}

// Function to send commands to the Nextion
void sendCommand(String command) {
  nextion.print(command);
  nextion.write(0xff);
  nextion.write(0xff);
  nextion.write(0xff);
}

void sendToNextion(int id, int ch, int value) {
  nextion.print("add ");
  nextion.print(id);
  nextion.print(",");
  nextion.print(ch);
  nextion.print(",");
  nextion.print(value);
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);
}
