#include <SoftwareSerial.h>

// Define the pins for SoftwareSerial
#define RX_PIN 10
#define TX_PIN 11

// Initialize SoftwareSerial for communication with Nextion
SoftwareSerial nextion(RX_PIN, TX_PIN);

// List of specific voltages to cycle through
int voltages[] = {4, 6, 8};
const int numVoltages = sizeof(voltages) / sizeof(voltages[0]);
const float resistance = 1.0; // Resistance in ohms

void setup() {
    nextion.begin(9600);
}

void loop() {
    // Iterate over each voltage and update the display and waveform accordingly
    for (int i = 0; i < numVoltages; i++) {
        int voltage = voltages[i];
        float current = voltage / resistance;
        float power = voltage * current;

        // Update the Nextion display for each button corresponding to a port
        for (int j = 0; j < 5; j++) {
            String displayText = "V: " + String(voltage, 2) + " [V]\\r" +
                                 "I: " + String(current, 2) + " [A]\\r" +
                                 "P: " + String(power, 2) +   " [W]";
            updateNextionDisplay("b" + String(j) + ".txt", displayText);
        }

        // Display and hold values on waveform
        updateNextionWaveform(voltage, current, power, 1000); // 1 second hold time
    }
      delay(100); // Update interval
}

void updateNextionWaveform(int voltage, float current, float power, int holdTime) {
    unsigned long startTime = millis();
    while (millis() - startTime < holdTime) {
        sendToNextion(1, 0, voltage);
        sendToNextion(1, 1, int(current * 1)); // Improve resolution for current
        sendToNextion(1, 2, int(power));
        delay(1); // Refresh rate of 50 ms
    }
}

void updateNextionDisplay(String component, String value) {
    String command = component + "=\"" + value + "\"";
    sendCommand(command);
}

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
