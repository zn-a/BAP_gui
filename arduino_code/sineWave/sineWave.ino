#include <SoftwareSerial.h>
#include <math.h>

// Define the pins for SoftwareSerial
#define RX_PIN 10
#define TX_PIN 11

// Initialize SoftwareSerial for communication with Nextion
SoftwareSerial nextion(RX_PIN, TX_PIN);

// Constants for the sine wave
const int amplitude = 40; // Half of 255, as the waveform component in Nextion takes values from 0 to 255
const int offset = 2*40;    // Center the wave on the y-axis
const int samples = 100;   // Number of samples per cycle
const float frequency = 1; // Frequency in Hz

// Constants for the phone charging simulation
const int voltage = 85;
const int current = 30;
const int power = 150;

void setup() {
  // Begin Serial communication at 9600 baud
  nextion.begin(9600);
}

void loop() {
  // Generate the sine wave and send to Nextion
  for (int i = 0; i < samples; i++) {
    // Calculate the sine wave value
    float radian = 2 * PI * i / samples;
    int y = amplitude * sin(radian * frequency) + offset;

    // Send the value to the Nextion display
    nextion.print("add 1,0,");
    nextion.print(y);
    nextion.write(0xFF);
    nextion.write(0xFF);
    nextion.write(0xFF);

    // Delay to control the frequency of the sine wave
    delay(0.01); // Adjust this delay to change the wave speed
  }

  // // Send the voltage value to Channel 1
  // nextion.print("add 1,0,");
  // nextion.print(voltage);
  // nextion.write(0xFF);
  // nextion.write(0xFF);
  // nextion.write(0xFF);

  // Send the current value to Channel 2
  // nextion.print("add 1,1,");
  // nextion.print(current);
  // nextion.write(0xFF);
  // nextion.write(0xFF);
  // nextion.write(0xFF);

  // // Send the power value to Channel 3
  // nextion.print("add 1,2,");
  // nextion.print(power);
  // nextion.write(0xFF);
  // nextion.write(0xFF);
  // nextion.write(0xFF);


}
