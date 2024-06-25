#include <SoftwareSerial.h>

#define RX_PIN PA3
#define TX_PIN PA2
#define BAUDRATE 115200

// Initialize SoftwareSerial for communication with Nextion
SoftwareSerial nextion(RX_PIN, TX_PIN);

const int ldrPin = PA1; // Pin where the LDR is connected
const float gain = 1.0; // Gain factor to increase sensitivity
bool adaptiveBrightness = false;

// Realistic voltage, current, and power values for different devices
struct Device {
  float voltage;
  float current;
  float power;
};

Device devices[] = {
  {5.00, 1.00, 5.00},  // Smartphone
  {9.00, 2.00, 18.00}, // Tablet
  {3.70, 0.50, 1.85},  // Earbuds
  {4.20, 0.35, 1.47},  // Smartwatch
  {12.00, 8.33, 100.0} // Soldering Iron
};
const int numDevices = sizeof(devices) / sizeof(devices[0]);

void setup() {
  // Initialize SoftwareSerial for Nextion
  nextion.begin(BAUDRATE); // Use the baud rate configured for Nextion
  Serial.begin(BAUDRATE);  // Initialize serial for debugging

  // Wait for serial port to connect
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB
  }

  Serial.println("Debugging checkbox state:");
}

void loop() {
  // Always run DC wave generation
  generateDCWave();
  
  // Check for Nextion data
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

  delay(50);

  // Conditionally run adaptive brightness control
  if (adaptiveBrightness) {
    handleAdaptiveBrightness();
  }

  delay(50); // Adjust as needed
}

void handleAdaptiveBrightness() {
  int ldrValue = analogRead(ldrPin); // Read the light level from the LDR
  int brightness = map(ldrValue, 685, 770, 10, 100); // Map the LDR value to a brightness level (10-100)
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

void generateDCWave() {
  for (int j = 0; j < 6; j++) {
    // Select a random device for each port
    Device device = devices[random(numDevices)];
    float voltage = device.voltage;
    float current = device.current;
    float power = device.power;

    // Update the Nextion display for each button corresponding to a port
    String displayText = "V: " + String(voltage, 2) + " [V]\\r" +
                         "I: " + String(current, 2) + " [A]\\r" +
                         "P: " + String(power, 2) + " [W]";
    updateNextionDisplay("b" + String(j) + ".txt", displayText);

    // Display and hold values on waveform
    updateNextionWaveform(voltage, current, power, 100); // 0.5 second hold time
  }
}

void updateNextionWaveform(float voltage, float current, float power, int holdTime) {
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
