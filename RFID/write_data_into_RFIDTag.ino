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

  // prints UID
  Serial.print(F("Card UID:"));    
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  // prints PICC type
  Serial.print(F(" PICC type: "));   
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  /*****VARIABLES*******/
  byte bufferw1[18];//ID
  byte bufferw2[34];//Name
  byte bufferw3[18];//phone
  byte block;
  MFRC522::StatusCode status;
  byte len;

  // Writing whole data into two blocks(each block - 16 bytes)
  Serial.setTimeout(10000L) ;     // wait until 10 seconds for input from serial


  /*********ID*****/
  Serial.println(F("Enter Your ID :"));
  len = Serial.readBytesUntil('#', (char *) bufferw1, 16) ; // read ID from serial
  for (byte i = len; i < 18; i++) bufferw1[i] = ' ';     // pad with spaces

  block = 1;
  //Authenticating using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("PCD_Authenticate() success: "));
  // Write first 16 bytes into block 1
  status = mfrc522.MIFARE_Write(block, bufferw1, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));


  /******NAME******/
  // Ask data:NAME
  Serial.println(F("Enter Your Name: "));
  len = Serial.readBytesUntil('#', (char *) bufferw2, 30) ; // read name from serial
  for (byte i = len; i < 34; i++) bufferw2[i] = ' ';     // pad with spaces

  block = 9;
  //Authenticating using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  // Write first 16 bytes into block 9
  status = mfrc522.MIFARE_Write(block, bufferw2, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));

  block = 10;
  //Authenticating using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  // Write next 16 bytes into block 10
  status = mfrc522.MIFARE_Write(block, &bufferw2[16], 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));


  /******PHONE NUMBER******/
  Serial.println(F("Enter Your Phone Number: "));
  len = Serial.readBytesUntil('#', (char *) bufferw3, 16) ; // read phone number from serial
  for (byte i = len; i < 18; i++) bufferw3[i] = ' ';     // pad with spaces

  block = 13;
  //Authenticating using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  // Write first 16 bytes into block 13
  status = mfrc522.MIFARE_Write(block, bufferw3, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));


  Serial.println(F("**End Writing**"));
  delay(100); 
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
