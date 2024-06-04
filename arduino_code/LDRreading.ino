// Define the pin where the LDR is connected
const int ldrPin = A0;

void setup() {
  // Begin serial communication at a baud rate of 9600
  Serial.begin(9600);
}

void loop() {
  // Read the value from the LDR
  int ldrValue = analogRead(ldrPin);

  // Map the LDR value to a brightness range (0 to 100)
  int brightness = map(ldrValue, 0, 1023, 0, 100);

  // Print the mapped brightness value to the Serial Monitor
  Serial.print("Brightness: ");
  Serial.println(brightness);

  // Wait for a second before reading again
  delay(500);
}
