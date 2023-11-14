
//LIBRARIES
#include <Adafruit_Fingerprint.h>
#include<HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#define RST_PIN 4   // Reset pin
#define SS_PIN 5    //Slave select pin


HardwareSerial mySerial(2);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
MFRC522 mfrc522(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key;
WiFiMulti wifiMulti;
MFRC522::StatusCode status;
const uint8_t fingerprint[20] = {0X09, 0X1E, 0X68, 0X9F, 0XBD, 0X40, 0X4B ,0X47, 0X8D, 0XAC, 0XBE, 0XFE, 0XEF, 0X35, 0XD6, 0X52, 0XC1, 0XA0, 0XEC, 0X9F};
uint8_t id;
const char* root_ca_website = \
      "-----BEGIN CERTIFICATE-----\n" \
      "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\n" \
      "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n" \
      "-----END CERTIFICATE-----\n";




int blockNumofID = 1;
int blockNumofName = 9;
int blockNumPhone= 13;
byte readBlockId[18];
byte readBlockName[18];
byte readBlockPhone[18];
byte bufferLen =18;
uint8_t input=0;


String Spreadsheet_data_2;
String Spreadsheet_data_1 = "";


//UTILITY FUNCTIONS
uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}


uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Place your finger on Sensor");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }


  //Convert image to template
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return 0;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
  }


  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }


  //Convert image to template
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return 0;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
  }


  //Create a model for fingerprint
  Serial.print("Creating and storing model for fingerprint with ID #");  Serial.println(id);


  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return 0;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return 0;
  } else {
    Serial.println("Unknown error");
    return 0;
  }


  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return 0;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return 0;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return 0;
  } else {
    Serial.println("Unknown error");
    return 0;
  }


  Serial.println(id);
  delay(5000);


  return true;
}




void ReadDataFromBlock(int blockNum, byte readBlockData[])
{
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
 
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  else
  {
    Serial.println("Authentication success");
  }


  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");  
  }
}


uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
  }


  //convert image to template
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return 0;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return 0;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return 0;
    default:
      Serial.println("Unknown error");
      return 0;
  }


  //search the database
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return 0;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return 0;
  } else {
    Serial.println("Unknown error");
    return 0;
  }


  // print ID of matched fingerprint
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);


  return finger.fingerID;
}




void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);
  Serial.println("SMART ATTENDANCE SYSTEM");
}


void loop() {
  Serial.println("CHOOSE WHAT YOU WANT TO DO:\n1.Write into RFID\n2.Read RFID Tags\n3.Fingerprint Enroll\n4.Fingerprint Detection\n5.Caluculate attendance percentage & send SMS\n");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1.WRITE RFID");
  lcd.setCursor(0,1);
  lcd.print("2.READ RFID");
  lcd.setCursor(0,2);
  lcd.print("3.ENROLL FINGER");
  lcd.setCursor(0,3);
  lcd.print("4.DETECT FINGER");
  uint8_t number=readnumber();
  lcd.clear();


  if(number==3){
    lcd.setCursor(0,0);
    lcd.print("3.ENROLL FINGER");
   
    WiFiMulti wifiMulti;
    HardwareSerial mySerial(2);
    String Spreadsheet_data_1 = "";


    Serial.begin(115200);
    mySerial.begin(9600);
    while (!Serial);  //wait until serial port is connected
    delay(100);
    Serial.println("\nSMART ATTENDANCE SYSTEM\nFINGER PRINT ENROLLMENT");


    // set the data rate for the sensor serial port
    finger.begin(57600);


    //Finding or Detecting Finger print sensor
    if (finger.verifyPassword()) {
      Serial.println("Found fingerprint sensor!");
    } else {
      Serial.println("Did not find fingerprint sensor!!");
      while (1) { delay(1); }
    }


    for (uint8_t t = 4; t > 0; t--)
      {
        Serial.printf("[SETTINGUP] WAIT %d....\n", t);
        Serial.flush();
        delay(1000);
      }




    WiFi.mode(WIFI_STA);
    wifiMulti.addAP("S1th", "S1th@123");


    Serial.println(F("Reading sensor parameters"));
    finger.getParameters();
    Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
    Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
    Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
    Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
    Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);


    Spreadsheet_data_1 = "https://script.google.com/macros/s/AKfycbzwXIVsmNCQrgAHDo-4EK4DNvLSa4bjvzInuS_8lFbILyqqEaeNlpV1yFeNy44tzIVJGA/exec?";
    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID (from 1 to 999) of the fingerprint that you want to enroll..");
    id = readnumber();
    if (id == 0) {// ID #0 not allowed, try again!
      return;
    }
    Serial.print("Enrolling ID - ");
    Serial.println(id);
    lcd.setCursor(0,1);
    lcd.print("ID-");
    lcd.print(id);


    while (!  getFingerprintEnroll() );
    char buffer0[16];
    char buffer1[16];
    char buffer2[16];
    char buffer3[16];
    size_t len ;
    len=0;
    Serial.flush();
    Serial.flush();
    while (len < sizeof(buffer0) - 1) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
          buffer0[len] = '\0';  // Null-terminate the string
          break;
        } else {
          buffer0[len++] = c;
        }
      }
    }
    Serial.print(buffer0);


    len=0;
    Serial.println("Enter Your Name: ");
      Serial.flush();
    while (len < sizeof(buffer2) - 1) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
          buffer2[len] = '\0';  // Null-terminate the string
          break;
        } else {
          buffer2[len++] = c;
        }
      }
    }
    Serial.println(buffer2);
    lcd.setCursor(0,2);
//    lcd.print("Name-");
    lcd.print((char*)buffer2);


    len=0;
    Serial.println("Enter Your phone number: ");
    Serial.flush();
    while (len < sizeof(buffer3) - 1) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
          buffer3[len] = '\0';  // Null-terminate the string
          break;
        } else {
          buffer3[len++] = c;
        }
      }
    }
    Serial.println(buffer3);
    lcd.setCursor(0,3);
//    lcd.print("phone-");
    lcd.print((char*)buffer3);


    len=0;
    Serial.println("Enter Your ID: ");
    Serial.flush();
    while (len < sizeof(buffer1) - 1) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
          buffer1[len] = '\0';  // Null-terminate the string
          break;
        } else {
          buffer1[len++] = c;
        }
      }
    }
    Serial.println(buffer1);
    lcd.clear();
    lcd.setCursor(0,0);
//    lcd.print("Adm. Num-");
    lcd.print((char*)buffer1);






    String idString = String(id);
    Spreadsheet_data_1+= "id=";
    Spreadsheet_data_1+=idString;


    Spreadsheet_data_1 += "&admNum=";
    for(int j=0;buffer1[j]!='\0';j++) Spreadsheet_data_1 += ((char)buffer1[j]);
     
    Spreadsheet_data_1 += "&name=";
    for(int j=0;buffer2[j]!='\0';j++) Spreadsheet_data_1 += ((char)buffer2[j]);


    Spreadsheet_data_1 += "&phone=";
    for(int j=0;buffer3[j]!='\0';j++) Spreadsheet_data_1 += ((char)buffer3[j]);




    if ((wifiMulti.run() == WL_CONNECTED))
      {
        WiFiClientSecure client;
        client.setCACert(root_ca_website);
        Spreadsheet_data_1.trim();
        Serial.println(Spreadsheet_data_1);
     
        HTTPClient https;
        Serial.print(F("[HTTPS] begin...\n"));
        if (https.begin(client, (String)Spreadsheet_data_1))
        {  
          Serial.print(F("[HTTPS] GET...\n"));
       
          int httpCode = https.GET();


          if (httpCode >0) {        
            Serial.println("http get request successful");
            lcd.setCursor(0,1);
            lcd.print("You can enter in");
            digitalWrite(2, HIGH);
            delay(1000);
            digitalWrite(2, LOW);
            delay(1000);
            digitalWrite(2, HIGH);
            delay(1000);
            digitalWrite(2, LOW);
           
          }
          else
          {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
          }
          https.end();
          delay(1000);
        }
        else
        {
          Serial.printf("[HTTPS} Unable to connect\n");
        }
      }
  }
















 
  if(number==4)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("4.DETECT FINGER");


    WiFiMulti wifiMulti;
    //instantiate class
    Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


    String Spreadsheet_data_2;
    String Spreadsheet_data_1 = "";


    Serial.begin(115200);
    mySerial.begin(9600);
    while (!Serial);  
    delay(100);
    Serial.println("\nSMART ATTENDANCE SYSTEM USING FINGERPRINT SENSOR");
    finger.begin(57600);
    delay(5);


    //detect fingerprint sensor
    if (finger.verifyPassword()) {
      Serial.println("Found fingerprint sensor!");
    } else {
      Serial.println("Did not find fingerprint sensor :(");
      while (1) { delay(1); }
    }


    for (uint8_t t = 4; t > 0; t--)
    {
      Serial.printf("[SETTINGUP] WAIT %d....\n", t);
      Serial.flush();
      delay(1000);
    }


    WiFi.mode(WIFI_STA);
    wifiMulti.addAP("S1th","S1th@123");


    Serial.println(F("Reading sensor parameters"));
    finger.getParameters();
    Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
    Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
    Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
    Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
    Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);


    finger.getTemplateCount();
    //If there are no templates,then no one has enrolled
    if (finger.templateCount == 0) {
      Serial.print("There are no fingerprints enrolled.(database is empty)");
    }
    else {
      Serial.println("Place your finger on sensor");
      lcd.setCursor(0,1);
      lcd.print("Place your finger");
      delay(5000);
    }


    Spreadsheet_data_1 = "https://script.google.com/macros/s/AKfycbzwXIVsmNCQrgAHDo-4EK4DNvLSa4bjvzInuS_8lFbILyqqEaeNlpV1yFeNy44tzIVJGA/exec?id=";
    int id=getFingerprintID();
    if(id==0){
      return;
    }
    String idString = String(id);
    lcd.setCursor(0,2);
    lcd.print("ID-");
    lcd.print(idString);
    Spreadsheet_data_1+=idString;


    if ((wifiMulti.run() == WL_CONNECTED))
    {
      WiFiClientSecure client;


      client.setCACert(root_ca_website);
      Spreadsheet_data_1.trim();
      Serial.println(Spreadsheet_data_1);
   
      HTTPClient http;
      Serial.print(F("[HTTPS] begin...\n"));
      http.setFollowRedirects(HTTPC_DISABLE_FOLLOW_REDIRECTS);
      if (http.begin(client, (String)Spreadsheet_data_1))
      {  
        Serial.print(F("[HTTPS] GET...\n"));
        int httpCode = http.GET();
     
        if (httpCode >0) {
          Serial.println("http get request successful");
          digitalWrite(2, HIGH);
          delay(1000);
          digitalWrite(2, LOW);
          delay(1000);
          digitalWrite(2, HIGH);
          delay(1000);
          digitalWrite(2, LOW);
        }
      }
    }
  }
   
 
  if(number==1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("1.WRITE RFID");
    Serial.begin(115200);        // Initialize serial communications with the PC
    SPI.begin();               // Init SPI bus
    mfrc522.PCD_Init();        // Init MFRC522 card
    Serial.println(F("Write personal data on a MIFARE PICC "));


    /*INITIALIZE***/
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;


    delay(5000);
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }


    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }


    Serial.print(F("Card UID:"));    //Dump UID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.print(F(" PICC type: "));   // Dump PICC type
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));


    /**VARIABLES**/
    byte bufferw1[16];
    byte bufferw2[16];
    byte bufferw3[16];
    byte block;
    MFRC522::StatusCode status;
    byte len;


    Serial.setTimeout(10000L) ;    




    /***NAME***/
    Serial.println(F("Enter Your Name: "));
    len = Serial.readBytesUntil('#', (char *) bufferw1, 16) ; // read  name from serial
    for (byte i = len; i < 16; i++) bufferw1[i] = ' ';     // pad with spaces
    lcd.setCursor(0,1);
    lcd.print("Enter Name-");
    lcd.print((char*)bufferw1);
    block = 9;
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }


    // Write block
    status = mfrc522.MIFARE_Write(block, bufferw1, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("MIFARE_Write() success: "));




  /***PHONE NUMBER***/
    Serial.println(F("Enter Your Phone Number: "));
    len = Serial.readBytesUntil('#', (char *) bufferw2, 16) ;
    for (byte i = len; i < 16; i++) bufferw2[i] = ' ';     // pad with spaces
    lcd.setCursor(0,2);
    lcd.print("Enter phone number-");
    lcd.print((char*)bufferw2);
    block = 13;


    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }


    // Write block
    status = mfrc522.MIFARE_Write(block, bufferw2, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("MIFARE_Write() success: "));




  /****ID**/
    Serial.println(F("Enter Your ID :"));
    len = Serial.readBytesUntil('#', (char *) bufferw3, 16) ;
    for (byte i = len; i < 16; i++) bufferw3[i] = ' ';     // pad with spaces
    lcd.setCursor(0,3);
    lcd.print("Enter Adm. number-");
    lcd.print((char*)bufferw3);


    block = 1;
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("PCD_Authenticate() success: "));


    // Write block
    status = mfrc522.MIFARE_Write(block, bufferw3, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("MIFARE_Write() success: "));


    //initialize variables
    byte buffer1[16];
    byte buffer2[16];
    byte buffer3[16];
    len = 16;


    //ID - buffer3
    block = 1;
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }


    status = mfrc522.MIFARE_Read(block, buffer3, &len);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }


    //PRINT ID
    Serial.print("ID: ");
    for (uint8_t i = 0; i < 16; i++)
    {
      if (buffer3[i] != 32)
      {
        Serial.write(buffer3[i]);
      }
    }
    Serial.print(" ");


    //Name-buffer1
    block = 9;
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 9, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }


    status = mfrc522.MIFARE_Read(block, buffer1, &len);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }


    //PRINT NAME
    Serial.print("Name: ");
    for (uint8_t i = 0; i < 16; i++)
    {
      if (buffer1[i] != 32)
      {
        Serial.write(buffer1[i]);
      }
    }
    Serial.print(" ");




  //phone number - buffer2
  block = 13;
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 13, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }


    status = mfrc522.MIFARE_Read(block, buffer2, &len);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }


    //PRINT PHONE NUMBER
    Serial.print("Phone Number: ");
    for (uint8_t i = 0; i < 16; i++)
    {
      if (buffer2[i] != 32)
      {
        Serial.write(buffer2[i]);
      }
    }
    Serial.print(" ");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Written data successfully");


  }
 
  else if(number==2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("2.READ RFID");
   
    Serial.begin(115200);
    Serial.println("Reading RFID CARD");
    for (uint8_t t = 4; t > 0; t--)
    {
      Serial.printf("[SETTINGUP] WAIT %d....\n", t);
      Serial.flush();
      delay(1000);
    }
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP("S1th", "S1th@123");
    SPI.begin();
    mfrc522.PCD_Init();


    while ( ! mfrc522.PICC_IsNewCardPresent())
    {
      ;
    }
    Serial.println("card detected");
   
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
      return;
    }


    Serial.println();
 
    Serial.println(F("Reading last data from RFID..."));


    ReadDataFromBlock(blockNumofName, readBlockName);
    ReadDataFromBlock(blockNumofID, readBlockId);
    ReadDataFromBlock(blockNumPhone, readBlockPhone);
    lcd.setCursor(0,1);
//    lcd.print("Name-");
    lcd.print((char*)readBlockName);
    lcd.setCursor(0,2);
//    lcd.print("ID-");
    lcd.print((char*)readBlockId);
    lcd.setCursor(0,3);
//    lcd.print("Phone Number-");
    lcd.print((char*)readBlockPhone);


    Spreadsheet_data_1 = "https://script.google.com/macros/s/AKfycbzwXIVsmNCQrgAHDo-4EK4DNvLSa4bjvzInuS_8lFbILyqqEaeNlpV1yFeNy44tzIVJGA/exec?";
    Spreadsheet_data_1 += "admNum=";
    for(int j=0;j<16 && readBlockId[j] != 32 && readBlockId[j] != 10;j++) Spreadsheet_data_1 += ((char)readBlockId[j]);
   
    Spreadsheet_data_1 += "&name=";
    for(int j=0;j<16 && readBlockName[j] != 32 && readBlockName[j] != 10;j++) Spreadsheet_data_1 += ((char)readBlockName[j]);


    Spreadsheet_data_1 += "&phone=";
    for(int j=0;j<16 && readBlockPhone[j] != 32 && readBlockPhone[j] != 10;j++) Spreadsheet_data_1 += ((char)readBlockPhone[j]);


    if ((wifiMulti.run() == WL_CONNECTED))
    {
      WiFiClientSecure client;
      client.setCACert(root_ca_website);
      Spreadsheet_data_1.trim();
      Serial.println(Spreadsheet_data_1);
   
      HTTPClient https;
      Serial.print(F("[HTTPS] begin...\n"));
      if (https.begin(client, (String)Spreadsheet_data_1))
      {  
        Serial.print(F("[HTTPS] GET...\n"));
     
        int httpCode = https.GET();
     
        if (httpCode > 0)
        {
          Serial.printf("http request successful");
            digitalWrite(2, HIGH);
            delay(1000);
            digitalWrite(2, LOW);
            delay(1000);
            digitalWrite(2, HIGH);
            delay(1000);
            digitalWrite(2, LOW);
         
        }
        else
        {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
        delay(1000);
      }
      else
      {
        Serial.printf("[HTTPS} Unable to connect\n");
      }
    }
  }
  else if(number == 5)
  {
      Serial.println("Calculating attendance percentage of each student & sending SMS to their phone numbers");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("5.ATTENDANCE %");
      Spreadsheet_data_1 = "https://script.google.com/macros/s/AKfycbzwXIVsmNCQrgAHDo-4EK4DNvLSa4bjvzInuS_8lFbILyqqEaeNlpV1yFeNy44tzIVJGA/exec?percent=true&sms=true";
      for (uint8_t t = 4; t > 0; t--)
      {
        Serial.printf("[SETTINGUP] WAIT %d....\n", t);
        Serial.flush();
        delay(1000);
      }


      WiFi.mode(WIFI_STA);
      wifiMulti.addAP("S1th", "S1th@123");


    if ((wifiMulti.run() == WL_CONNECTED))
    {


      WiFiClientSecure client;
      client.setCACert(root_ca_website);


      Spreadsheet_data_1.trim();
      Serial.println(Spreadsheet_data_1);
   
      HTTPClient https;
      Serial.print(F("[HTTPS] begin...\n"));
      if (https.begin(client, (String)Spreadsheet_data_1))
      {  
        Serial.print(F("[HTTPS] GET...\n"));
     
        int httpCode = https.GET();
        if (httpCode > 0)
        {
          Serial.printf("http request successful");
          digitalWrite(2, HIGH);
          delay(1000);
          digitalWrite(2, LOW);
          delay(1000);
          digitalWrite(2, HIGH);
          delay(1000);
          digitalWrite(2, LOW);
        }
        else
        {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
        delay(1000);
        }
      else
      {
        Serial.printf("[HTTPS} Unable to connect\n");
      }
    }
  }
}
