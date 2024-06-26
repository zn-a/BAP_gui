
void nextion_serial_polling(){
    // Check for Nextion data
  if (nextion.available()) {
    while (nextion.available()) {
      char c = nextion.read();
      if (c == 0x23) {  // Custom command prefix for adaptive brightness
        char val = nextion.read();
        if (val == 0x01) {
          adaptiveBrightness = true;  // Adaptive brightness ON
        } else if (val == 0x00) {
          adaptiveBrightness = false;  // Adaptive brightness OFF
        }
        // Debugging output
        Serial.print("Adaptive Brightness: ");
        Serial.println(adaptiveBrightness ? "ON" : "OFF");
      } else if (c == 0x24 || c == 0x26) {  // Custom command prefix for set_on/set_off (static power div)
        char val = nextion.read();
        voltage_buttons_handeler(val);
      }
    }
  }
}

void voltage_buttons_handeler(byte val){
  switch (val) {
    case 0x00:
      Serial.println("Set OFF function activated");
      turn_of_vbus();
    case 0x01:
      Serial.println("Set ON function activated");
      turn_on_v_bus();
      delay(2);
      set_voltage(5.2);
    case 0x05:
      Serial.println("Adjusting to 5V");
      set_voltage(5);
      break;
    case 0x09:
      Serial.println("Adjusting to 9V");
      set_voltage(9);
      break;
    case 0x12:
      Serial.println("Adjusting to 15V");
      set_voltage(15);
      break;
    case 0x20:
      Serial.println("Adjusting to 20V");
      set_voltage(20);
      break;
    default:
      Serial.println("Unknown command");
      break;
  }
}

void handleAdaptiveBrightness() {
  int ldrValue = analogRead(ldrPin); // Read the light level from the LDR
  int brightness = map(ldrValue, 685, 770, 10, 100); // Map the LDR value to a brightness level (10-100)
  brightness = constrain(brightness, 10, 100);

  // Send the command to set brightness
  set_value("dims=",brightness);

  // Adjust brightness slider
  set_value("pageSettings.bslider.val=",brightness);

  // Update the number component to reflect the brightness level
  set_value("pageSettings.n0.val=",brightness);

  // Debugging output
  Serial.print("LDR Value: ");
  Serial.print(ldrValue);
  Serial.print(" -> Brightness: ");
  Serial.println(brightness);
}

void generateDCWave(){
    updateNextionWaveform(read_voltage_float(), read_current_float()); // 0.5 second hold time
}

void update_buttons_info() {
  for (int j = 0; j < 6; j++) {
    // Select a random device for each port
    Device device = devices[j];
    float voltage = device.voltage;
    float current = device.current;
    if(j == 0){
        voltage = read_voltage_float();
        current = read_current_float();
    }
    write_to_screan(voltage,current,j);
    // Display and hold values on waveform
  }
}

void write_to_screan(float voltage, float current, int j){
  float power = voltage*current;
  String displayText = "V:\t" + String(voltage, 2) + " [V]\\r" +
                                 "I:\t" + String(current, 2) + " [A]\\r" +
                                 "P:\t" + String(power, 2) + " [W]";
            updateNextionDisplay("b" + String(j) + ".txt", displayText);

}

void updateNextionWaveform(float voltage, float current) {
        float power = voltage*current;
        sendToNextion(1, 0, voltage*4);
        sendToNextion(1, 1, int(current * 20)); // Multiply by 10 for better resolution
        sendToNextion(1, 2, int(power));
}

void updateNextionDisplay(String component, String value) {
  String command = component + "=\"" + value + "\"";
  sendCommand(command);
}

void set_value(String name, int value){
  nextion.print(name);
  nextion.print(value);
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);
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