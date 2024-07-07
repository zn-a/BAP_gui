

// in this file the type-c port controller will be writen

// if it works

// 
// send 
// num of bytes <- 0x12
// message header <- 0x4361
// data opject 1
// data opject 2
// data opject 3
// data opject 4

#define allert_pin PB8

#define numb_bytes 0x12
#define massage_header 0x4361

#define opject_1 0x0401912C //(5.00V at 2.4A) = 4 bytes
#define opject_2 0x0402D12C //(9.00V at 3A) = 4 bytes
#define opject_3 0x0404B12C //(15.00V at 3A) = 4 bytes
#define opject_4 0x040641F4 //(20.00V at 5A) = 4 bytes


int write_num_bytes(long bytes_4, int num){

  for(int i = 0; i < num; i++){
    byte tx_byte = bytes_4&0xFF;
    Wire.write(tx_byte);
    bytes_4 = bytes_4>>8;
  }

  return 0;
}

int does_it_ask_for_specificionts(){
  // first test if allert is low
  if(digitalRead(allert_pin) == LOW){
    Serial.println("receafde a allerd pin, reading allert bit");
    long allert_bit = read_allert_massage();
    //check if it hase a register to read
    if(hase_rx_sog(allert_bit)){
      write_allert_bit(allert_bit);
      int anser =read_and_decode_buffer();
      if(anser == 1){
        // holi shit it wants to know the capabilites
        clear_rx_sop_reg();
        send_TCPC();
        return 1;
      }else{
        // i have no fucking clue wat is going on
        Serial.println("The TCPM ask for things i doint understand");
        Serial.print("It failes with error:");
        Serial.println(anser);
        return anser;
      }
    }else{
      Serial.println("No massage availbeble");
      Serial.print("allert_bit_reads:");
      Serial.println(allert_bit,HEX);
      return -4;
    }
  }
  return 0;
}

int clear_rx_sop_reg(){
 Wire.beginTransmission(max_adres);
  Wire.write(0x10);
  Wire.write(0x04);
  Wire.write(0x00);
  Wire.endTransmission();
  return 0;
}

int read_and_decode_buffer(){
  int recieaved = Wire.requestFrom(max_adres,32,0x30,1,true);
  Serial.println(recieaved,HEX);
  // if(recieaved != 1) return -1;
  int num_of_bytes = Wire.read();
  Serial.print("num of bits receaved:");
  Serial.println(num_of_bytes);
  if(num_of_bytes == 3){ // the only option that i know
    Wire.read(); //<- should be 0 otherwise i doint know
    int interesting_byte = Wire.read();
    if(interesting_byte&0b11111 == 0b111){
      // now it will ask for soure cap, the only option explaind in datasheet
      return 1;
    }
    return -2;
  }
  return -3;
}

bool hase_rx_sog(long allert_bit){
  return ((allert_bit&4) != 0);
}


long read_allert_massage(){
  int recieaved = Wire.requestFrom(max_adres,2,0x10,1,true);
  if(recieaved == 1)return (Wire.read() + Wire.read()*0xFF);
  return -1;
}

int write_allert_bit(int allert_bit){
  Wire.beginTransmission(max_adres);
  Wire.write(0x10);
  Wire.write(allert_bit&0xFF);
  Wire.write((allert_bit/0xFF)&0xFF);
  return Wire.endTransmission();
}

int send_TCPC(){
  // send num of to transmitt registers
  Wire.beginTransmission(max_adres);
  Wire.write(0x51); //register number
  Wire.write(0x12); // Write the number of bytes of the TCPC structure
  // write_num_bytes(massage_header,2);
  Wire.write(0x61); // Write the header of the protocol
  Wire.write(0x43);
  // write_num_bytes(opject_1,4); 0x0401912C
  Wire.write(0x2C);
  Wire.write(0x91);
  Wire.write(0x01);
  Wire.write(0x04);
  // write_num_bytes(opject_2,4); 0x0402D12C
  Wire.write(0x2C);
  Wire.write(0xD1);
  Wire.write(0x02);
  Wire.write(0x04);
  // write_num_bytes(opject_3,4);0x0404B12C
  Wire.write(0x2C);
  Wire.write(0xB1);
  Wire.write(0x04);
  Wire.write(0x04);
  // write_num_bytes(opject_4,4);0x040641F4
  Wire.write(0xF4);
  Wire.write(0x41);
  Wire.write(0x06);
  Wire.write(0x04);
  Wire.endTransmission();

  Wire.beginTransmission(max_adres);
  Wire.write(0x50);
  Wire.write(0x30);
  Wire.endTransmission();

  Serial.println(read_allert_massage(),HEX);


  Wire.beginTransmission(max_adres);
  Wire.write(0x10);
  Wire.write(0x40);
  Wire.write(0x00);
  Wire.endTransmission();


  
  return 0;
}