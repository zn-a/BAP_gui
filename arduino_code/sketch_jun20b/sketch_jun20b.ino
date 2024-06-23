//                      RX    TX
HardwareSerial Serial3(PA10, PA9);

void setup() {
  Serial3.begin(115200); 
}

void loop() {
  Serial3.println("Hello World!");
  delay(1000);
}