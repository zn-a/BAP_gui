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
unsigned long half_sec = 0;
unsigned long old_half_sec = 0;
void setup() {
  // Initialize SoftwareSerial for Nextion
  nextion.begin(BAUDRATE); // Use the baud rate configured for Nextion
  Serial.begin(BAUDRATE);  // Initialize serial for debugging
  
  start_up_MAX();
  turn_on_v_bus();
  // Wait for serial port to connect
  // while (!Serial) {
  //   ; // Wait for serial port to connect. Needed for native USB
  // }
  delay(2000);
  Serial.println("Debugging checkbox state:");
}

void loop() {
  // Check for Nextion data
  nextion_serial_polling();


  // Conditionally run adaptive brightness control
  if (adaptiveBrightness) {
    handleAdaptiveBrightness();
  }

  // Always run DC wave generation
  generateDCWave();

  half_sec = millis()/500;
  if(old_half_sec != half_sec){
    old_half_sec = half_sec;
    update_buttons_info();
  }

  change_voltage_with_serial();
  log_info();

}
