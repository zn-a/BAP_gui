#include <Wire.h>

#define max_adres 0x50



int start_up_MAX(){
  Serial.println("---Setup MAX25432B---");

  Wire.begin();
  Wire.setClock(1000000);

   // Test the adress 
  Wire.beginTransmission(max_adres);
  int error = Wire.endTransmission(true);
  if(error == 0 ){
    Serial.println("adress found");
  }else{
    Serial.println("adress not found");
    delay(20000);
  }
  // turn on the v bus
  // turn_on_v_bus();
  // if(turn_on_v_bus() >= 0){
  //   Serial.println("Vbus set succesfoly");
  // }else{
  //   Serial.println("Vbus NOT set succesfoly");
  // }
  return 0;
}



int read_one_reg(int reg_adress){
  // request one byte from MAX25432
  int receiaved = Wire.requestFrom(max_adres,1,reg_adress,1,true);
  if(receiaved == 1)return Wire.read();
  return -1;
}

int write_register(int reg_adress, int value){
  Wire.beginTransmission(max_adres);
  Wire.write(reg_adress);
  Wire.write(value);
  return Wire.endTransmission(true);
}

int enable_voltage_monitoring(){
  //read the current value
  int current_value = read_one_reg(0x1C);
  if(current_value == -1) return -1;
  // current_value  = current_value && ~(0x01<<6);

  // Serial.print("next_value =");
  // Serial.println(current_value);
  return write_register(0x1C,0b00110000);
}

int read_voltage_mv(){
  int byte_v_l = read_one_reg(0x70);
  int byte_v_h = read_one_reg(0x71);
  // Serial.println(byte_v_l);
  // Serial.println(byte_v_h);
  if(byte_v_l  == -1 || byte_v_h  == -1) return -1;

  int word_v = (byte_v_h & 0x03)*0xFF + byte_v_l;
  int voltage = word_v*25; // voltage in mv
  return voltage;
}

float mili_to_dec(int input){
  if(input == -1) return -1;
  return double(input) /1000.0; 
} 

double read_voltage_float(){
  return mili_to_dec(read_voltage_mv());
}

int read_current_ma(){
  int byte_current = read_one_reg(0x80);
  if(byte_current == -1)return -1;
  int current = byte_current*50;
  return current;
}

double read_current_float(){
  return mili_to_dec(read_current_ma());
}

int set_current_limit(float a){
  return set_current_limit_ma(a*1000);
}

int set_current_limit_ma(int current_lim_value){
  //check if given current limmid is in range
  if(current_lim_value < 1000 || current_lim_value > 6350) return -1;
  //convert to byte to transmitt to MAX25432 
  int current_limit_byte = (current_lim_value/25)&0xFF;
  return write_register(0x82, current_limit_byte);
}


int send_command(int value){
  return write_register(0x23, value);
}

int turn_on_v_bus(){
  // turn on discharge and voltage measuring
  if(enable_voltage_monitoring() == -1)return -1;
  // Set the V_bus resolution (high resolution)
  if(write_register(0x86, 0b1000100) == -1) return -2;
  // write command to set voltage to high voltage
  if(send_command(0x77) == -1)return -3;
  return 0;
}

int turn_of_vbus(){
  return send_command(0x66);
}

int set_voltage(float v){
  return set_voltage_mv(int(v*1000));
}


int set_voltage_mv(int voltage){
  enable_voltage_monitoring();
  turn_on_v_bus();
  delay(2);
  //check if voltage is in range
  if(voltage < 3300 && voltage >= 0){
    turn_of_vbus();
    return 0;
  }else if(voltage < 0 || voltage > 21000){
    return -1;
  }
  //conversion found in datasheet
  long word_voltage = voltage/10 - (floor(304*voltage/100000) -10);
  //Write tot the 2 registers
  int a = write_register(0x7A, word_voltage&0xFF);
  int b = write_register(0x7B, (word_voltage>>8)&0xFF);
  // give command to update the voltage
  int c = send_command(0x88);
  if(a==-1 || b == -1 || c == -1) return -1;
  return 0;
}

int read_current_limit_ma(){
  int byte_current = read_one_reg(0x82);
  if(byte_current == -1)return -1;
  int current = byte_current*25;
  return current;
}

float read_current_limit_float(){
  return mili_to_dec(read_current_limit_ma());
}

int log_info(){
  //read current 
  float i = read_current_float();
  //read voltage
  float v = read_voltage_float();
  //read current limit
  float i_lim = read_current_limit_float();
  Serial.print("Register measurments ->");
  Serial.print("V: ");
  Serial.print(v);
  Serial.print(" I: ");
  Serial.print(i);
  Serial.print(" I_lim: ");
  Serial.println(i_lim);

  return 0;
}

int change_voltage_with_serial(){
  if(Serial.available()){
    float input = Serial.parseFloat();
    // set_voltage(input);
    char selected = Serial.read();
    if(selected == 'V' || selected == 'v'){
      int reaction = set_voltage(input);
      if(reaction < 0){
        Serial.println("invalid input, voltage not set");
      }else{
        Serial.print("Voltage is set to: ");
        Serial.println(input);
      }
    }

    if(selected == 'I' || selected == 'i'){
      int reaction = set_current_limit(input);
      if(reaction < 0){
        Serial.println("invalid input, current lim  not set");
      }else{
        Serial.print("current lim is set to: ");
        Serial.println(input);
      }
    }
  }

  return 0;
}