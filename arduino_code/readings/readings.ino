void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  int fakeData[] = {10, 20, 30, 40, 50};

  // Calculate the number of elements in the array
  int arrayLength = sizeof(fakeData) / sizeof(fakeData[0]);

  // Loop through the array
  for (int i = 0; i < arrayLength; i++) {
  // Print the current element to the serial monitor
  // Serial.println(myArray[i]);
  Serial.print("voltage1.val=");
  Serial.print(fakeData[i]);

  // Wait for 1000 milliseconds (1 second)
  delay(1000);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  }

  // Serial.print("voltage1.val=");
  // Serial.print(Value);


  // Serial.write(0xff);
  // Serial.write(0xff);
  // Serial.write(0xff);

}
