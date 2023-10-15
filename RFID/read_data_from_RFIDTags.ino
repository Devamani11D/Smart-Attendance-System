// libraries for SPI communication and RFID
#include <SPI.h>
#include <MFRC522.h>

// reset pin and slave select pin
#define RST_PIN         4          
#define SS_PIN          5          

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance(object)

void setup() {
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();               // Initialize SPI bus
  mfrc522.PCD_Init();        // Initialize MFRC522 card
  Serial.println(F("Write Data into RFID tags.."));
}

void loop() {

  /****INITIALIZE********/
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) 
  key.keyByte[i] = 0xFF;      //all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.

  //check for new card
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.println(F("**Card Detected:**"));
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //prints UID of card

  //initialize variables
  byte block;
  byte len=18;
  MFRC522::StatusCode status;
  byte buffer1[18];//ID
  byte buffer2[18];//name-1
  byte buffer3[18];//name-2
  byte buffer4[18];//phone 

  //ID - buffer1
  block = 1;
  //Authenticate using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); 
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  // Read data in that block
  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  //PRINT ID
  Serial.print("ID: ");
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]);
    }
  }
  Serial.print(" ");


  //Name - buffer2 and buffer3
  block = 9;
  //Authenticate using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  // Read data in that block
  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  //PRINT NAME(first 16 bytes)
  Serial.print("Name: ");
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer2[i] != 32)
    {
      Serial.write(buffer2[i]);
    }
  }
  Serial.print(" ");

  block = 10;
  //Authenticate using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  // Read data in that block
  status = mfrc522.MIFARE_Read(block, buffer3, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  //PRINT last NAME(last 16 bytes)
  Serial.print("Name: ");
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer3[i] != 32)
    {
      Serial.write(buffer3[i]);
    }
  }
  Serial.print(" ");


  //phone number - buffer4
  block = 13;
  //Authenticate using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  // Read data in that block
  status = mfrc522.MIFARE_Read(block, buffer4, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  //PRINT PHONE NUMBER
  Serial.print("Phone Number: ");
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer4[i] != 32)
    {
      Serial.write(buffer4[i]);
    }
  }
  Serial.print(" ");


  Serial.println(F("**End Reading**"));
  delay(100); //change value if you want to read cards faster
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
