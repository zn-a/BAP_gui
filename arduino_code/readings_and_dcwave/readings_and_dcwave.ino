#include <SoftwareSerial.h>

// Define the pins for SoftwareSerial
#define RX_PIN PA3
#define TX_PIN PA2
#define BAUDRATE 115200

// Initialize SoftwareSerial for communication with Nextion
SoftwareSerial nextion(RX_PIN, TX_PIN);

// List of specific voltages to cycle through
// float voltages[] = {5.00, 9.00, 12.00, 40.00};
float voltages[] = {random(330, 500) / 100.0, random(330, 500) / 100.0, random(330, 500) / 100.0};
const int numVoltages = sizeof(voltages) / sizeof(voltages[0]);
const float resistance = 1.00; // Resistance in ohms

void setup() {
    nextion.begin(BAUDRATE);
}

void loop() {
    for (int i = 0; i < numVoltages; i++) {
        float voltage = voltages[i];
        float current = voltage / resistance;
        float power = voltage * current;

        // Update the Nextion display for each button corresponding to a port
        for (int j = 0; j < 6; j++) {
            String displayText = "V:\t" + String(voltage, 2) + " [V]\\r" +
                                 "I:\t" + String(current, 2) + " [A]\\r" +
                                 "P:\t" + String(power, 2) + " [W]";
            updateNextionDisplay("b" + String(j) + ".txt", displayText);
        }

        // Display and hold values on waveform
        updateNextionWaveform(voltage, current, power, 500); // 1 second hold time
    }
}

void updateNextionWaveform(int voltage, float current, float power, int holdTime) {
    unsigned long startTime = millis();
    while (millis() - startTime < holdTime) {
        sendToNextion(1, 0, voltage);
        sendToNextion(1, 1, int(current * 10)); // Multiply by 10 for better resolution
        sendToNextion(1, 2, int(power));
        delay(50); // Refresh rate of 50 ms
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
