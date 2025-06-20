#include <Arduino.h>

// Start condition
void I2C_claimBus()
{

  // set SDA to LOW first
  digitalWrite(2, LOW);

  delay(3);

  // set SCL to LOW second
  digitalWrite(0, LOW);
}

// Stop condition
void I2C_releaseBus()
{

  // set SCL to HIGH first
  digitalWrite(0, HIGH);

  delay(3);

  // set SDA to HIGH second
  digitalWrite(2, HIGH);
}

void I2C_sendData(uint8_t device, uint8_t reg_addr, uint8_t data)
{

  //                                                                                                                                                                                    ↓ your read W/R bit is now here.
  // Reverse the order of bits for each variable; these need to be sent with MSB first, so MSB needs to be Bit 0 instead of Bit 7.                                Ex: 0110 0001 becomes 1000 0110. -> data now gets sent from right to left now.
  // Shift device variable over 1 to pad a 0 in the LSB, since 'device' includes both the address and a 0(write) bit.                                                         ↑ 
  //                                                                                                                                                                          is your W/R bit in the original variable.                     

  device = device << 1;
  uint8_t firstByte = 0;
  for (int i = 0; i < 8; i++)
  {
    if ((device >> i) & 1)
    {
      firstByte |= 1 << (7 - i);
    }
  }

  uint8_t secondByte = 0;
  for (int i = 0; i < 8; i++)
  {
    if ((reg_addr >> i) & 1)
    {
      secondByte |= 1 << (7 - i);
    }
  }

  // uint8_t thirdByte = 0;
  // for (int i = 0; i < 8; i++)
  // {
  //   if ((data >> i) & 1)
  //   {
  //     thirdByte |= 1 << (7 - i);
  //   }
  // }

  // Claim the bus
  I2C_claimBus();

  // Iterate through first byte of data with the 7-bit address and send over SDA
  for (uint8_t i = 0; i < 8; i++)
  {
    if ((firstByte >> i & 0x01) == 1) // if 'firstbyte' shifted over right i times AND 0x01 equals 1, write a 1
    {
      // Wait 1ms 
      delay(1);
      // Write a '1' to the SDA line
      digitalWrite(2, HIGH);
    }
    else // else the bit is a 0
    {
      delay(1);
      digitalWrite(2, LOW);
    }
    // wait two ms to cover the 3ms clock time (1ms from the bit write stage at i'th iteration + 2ms gap to equal 3ms)
    delay(2);

    // clock period pulse from high to low for 3ms
    digitalWrite(0, HIGH);
    delay(3);
    digitalWrite(0, LOW);
  }

  // Read the ACK bit
  delay(3);
  digitalWrite(0, HIGH);

  // Same gap timing rule here
  delay(1);
  uint8_t ACK = digitalRead(2);
  delay(2);

  digitalWrite(0, LOW);

  // Repeat for second byte
  for (uint8_t i = 0; i < 8; i++)
  {
    if ((secondByte >> i & 0x01) == 1)
    {
      delay(1);
      digitalWrite(2, HIGH);
    }
    else
    {
      delay(1);
      digitalWrite(2, LOW);
    }
    delay(2);
    digitalWrite(0, HIGH);
    delay(3);
    digitalWrite(0, LOW);
  }

  delay(3);
  digitalWrite(0, HIGH);

  delay(1);
  uint8_t ACK2 = digitalRead(2);
  delay(2);

  digitalWrite(0, LOW);

  // for (uint8_t i = 0; i < 8; i++)
  // {
  //   if((thirdByte >> i & 0x01) == 1){
  //     delay(1);
  //     digitalWrite(2, HIGH);
  //   }else{
  //     delay(1);
  //     digitalWrite(2,LOW);
  //   }
  //   delay(2);
  //   digitalWrite(0,HIGH);
  //   delay(3);
  //   digitalWrite(0,LOW);
  // }

  // wait 1ms before releasing the bus
  delay(3);

  // set SCL to HIGH first
  digitalWrite(0, HIGH);

  delay(3);

  // set SDA to HIGH second
  digitalWrite(2, HIGH);
}

// Variable to hold address and data
uint8_t address = 0;
uint8_t regis = 0;
uint8_t addr_data = 0;

void setup()
{

  // Setup serial terminal at 115200 baud rate
  Serial.begin(115200);

  // I2C SCL on pin 0
  pinMode(0, OUTPUT);
  // I2C SDA on pin 2
  pinMode(2, OUTPUT);

  // Set I2C bus to idle state (both SDA and SCL to HIGH)
  digitalWrite(2, HIGH);
  digitalWrite(0, HIGH);

  Serial.print("I2C bus initialized\n");

  delay(100);

  I2C_sendData(address, regis, addr_data);

  Serial.print("I2C data sent\n");

  delay(200);

  I2C_sendData(0x0E, 0x00, addr_data);

  Serial.print("I2C data sent\n");

  // // Both SDA and SCL are LOW now
  // I2C_claimBus();

  // // Note: The bit being sent to the device is set when SCL is LOW, with the receiving device sampling/reading the data when SCL is HIGH

  // // send 0x0A (0000 1010) with MSB sent first
  // delay(3);
  // digitalWrite(0,HIGH);
  // delay(3);
  // digitalWrite(0,LOW);

  // delay(3);
  // digitalWrite(0,HIGH);
  // delay(3);
  // digitalWrite(0,LOW);

  // delay(3);
  // digitalWrite(0,HIGH);
  // delay(3);
  // digitalWrite(0,LOW);

  // delay(1);
  // digitalWrite(2,HIGH);
  // delay(2);
  // digitalWrite(0,HIGH);
  // delay(3);
  // digitalWrite(0,LOW);

  // delay(1);
  // digitalWrite(2,LOW);
  // delay(2);
  // digitalWrite(0,HIGH);
  // delay(3);
  // digitalWrite(0,LOW);

  // delay(1);
  // digitalWrite(2,HIGH);
  // delay(2);
  // digitalWrite(0,HIGH);
  // delay(3);
  // digitalWrite(0,LOW);

  // delay(1);
  // digitalWrite(2,LOW);
  // delay(2);
  // digitalWrite(0,HIGH);
  // delay(3);
  // digitalWrite(0,LOW);

  // delay(1);
  // digitalWrite(2,LOW);
  // delay(2);
  // digitalWrite(0,HIGH);
  // delay(3);
  // digitalWrite(0,LOW);

  // delay(3);
  // digitalWrite(0,HIGH);
  // delay(3);
  // digitalWrite(2, HIGH);
}

void loop()
{
  // Nothing here ¯\_(ツ)_/¯
}
