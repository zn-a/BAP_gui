#include <SoftwareSerial.h>

#define RX_PIN PA2
#define TX_PIN PA3
#define BAUDRATE 115200

// Initialize SoftwareSerial for communication with Nextion
SoftwareSerial nextion(RX_PIN, TX_PIN);

const int ldrPin = PA1; // Pin where the LDR is connected
const float gain = 1.0; // Gain factor to increase sensitivity
bool adaptiveBrightness = false;

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
      if (c == 0x23) { // Custom command prefix for adaptive brightness
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
      else if (c == 0x24) { // Custom command prefix for set_on/set_off
        char val = nextion.read();
        if (val == 0x01) {
          set_on(); // Activate set_on function
        } else if (val == 0x00) {
          set_off(); // Activate set_off function
        }
        // Debugging output
        Serial.print("Button State: ");
        Serial.println(val == 0x01 ? "ON" : "OFF");
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

void set_on() {
  // Code to handle the ON state
  Serial.println("Set ON function activated");
  // Example: Turn on an LED connected to PC13
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, LOW); // Turn on LED (assuming active low)
}

void set_off() {
  // Code to handle the OFF state
  Serial.println("Set OFF function activated");
  // Example: Turn off an LED connected to PC13
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, HIGH); // Turn off LED (assuming active low)
}

void handleAdaptiveBrightness() {
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

void generateDCWave() {
  float voltages[] = {random(330, 500) / 100.0, random(330, 500) / 100.0, random(330, 500) / 100.0};
  const int numVoltages = sizeof(voltages) / sizeof(voltages[0]);
  const float resistance = 1.00; // Resistance in ohms

  for (int i = 0; i < numVoltages; i++) {
    float voltage = voltages[i];
    float current = voltage / resistance;
    float power = voltage * current;

    // Update the Nextion display for each button corresponding to a port
    for (int j = 0; j < 6; j++) {
      String displayText = "V: " + String(voltage, 2) + " [V]\\r" +
                           "I: " + String(current, 2) + " [A]\\r" +
                           "P: " + String(power, 2) + " [W]";
      updateNextionDisplay("b" + String(j) + ".txt", displayText);
    }

    // Display and hold values on waveform
    updateNextionWaveform(voltage, current, power, 500); // 0.5 second hold time
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
