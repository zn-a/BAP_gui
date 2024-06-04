int LED = 10;

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);

  // Set pin modes for both LEDs
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT); // Setup built-in LED
}

void loop() {
  // Check if there is incoming serial data
  if (Serial.available() > 0) {
    String Received = Serial.readString();

    if (int(Received[0]) == 1) {
      digitalWrite(LED, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
    }

    if (int(Received[0]) == 0) {
      digitalWrite(LED, LOW);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
