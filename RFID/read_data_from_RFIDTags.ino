#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>

const uint8_t fingerprint[20] = {0X09, 0X1E, 0X68, 0X9F, 0XBD, 0X40, 0X4B ,0X47, 0X8D, 0XAC, 0XBE, 0XFE, 0XEF, 0X35, 0XD6, 0X52, 0XC1, 0XA0, 0XEC, 0X9F};

const char* root_ca_website = \
      "-----BEGIN CERTIFICATE-----\n" \
      "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\n" \
      "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n" \
      "-----END CERTIFICATE-----\n";


#define RST_PIN 4  // Reset pin
#define SS_PIN 5 //Slave select pin
#define BUZZER 3 // Buzzer pin


MFRC522 mfrc522(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key;
WiFiMulti wifiMulti;
MFRC522::StatusCode status;


int blockNumofID = 1;


int blockNumofName = 9;


int blockNumPhone= 13;


byte bufferLen =18;
byte readBlockId[18];
byte readBlockName[18];
byte readBlockPhone[18];


String Spreadsheet_data_2; 
String Spreadsheet_data_1 = "";


void setup()
{
  Serial.begin(9600);


  Serial.println();


  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.printf("[SETTINGUP] WAIT %d....\n", t);
    Serial.flush();
    delay(1000);
  }


  WiFi.mode(WIFI_STA);

  wifiMulti.addAP("S1th", "S1th@123");

  pinMode(BUZZER, OUTPUT);
  
  SPI.begin();
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
void loop()
{
   mfrc522.PCD_Init();


  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }


  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }


  Serial.println();
 
  Serial.println(F("Reading last data from RFID..."));


  ReadDataFromBlock(blockNumofID, readBlockId);
  ReadDataFromBlock(blockNumofName, readBlockName);
  ReadDataFromBlock(blockNumPhone, readBlockPhone);

  Spreadsheet_data_1 = "https://script.google.com/macros/s/AKfycbzwXIVsmNCQrgAHDo-4EK4DNvLSa4bjvzInuS_8lFbILyqqEaeNlpV1yFeNy44tzIVJGA/exec?";

  Spreadsheet_data_1 += "admNum=";
  for(int j=0;j<16 && readBlockId[j] != 32 && readBlockId[j] != 10;j++) Spreadsheet_data_1 += ((char)readBlockId[j]);
  
  Spreadsheet_data_1 += "&name=";
  for(int j=0;j<16 && readBlockName[j] != 32 && readBlockName[j] != 10;j++) Spreadsheet_data_1 += ((char)readBlockName[j]); 

  Spreadsheet_data_1 += "&phone=";
  for(int j=0;j<16 && readBlockPhone[j] != 32 && readBlockPhone[j] != 10;j++) Spreadsheet_data_1 += ((char)readBlockPhone[j]);

  digitalWrite(BUZZER, HIGH);
  delay(300);
  digitalWrite(BUZZER, LOW);
  delay(300);
  digitalWrite(BUZZER, HIGH);
  delay(300);
  digitalWrite(BUZZER, LOW);

  if ((wifiMulti.run() == WL_CONNECTED))
  {
    // std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    WiFiClientSecure client;
    // client->setFingerprint(fingerprint);

    client.setCACert(root_ca_website);
   
    // Spreadsheet_data_2 = Spreadsheet_data_1+"name="+ String((char*)readBlockName)+"&admNum="+String((char*)readBlockId)+"&phone="+String((char*)readBlockPhone);

    // Spreadsheet_data_2 = Spreadsheet_data_1+"name="+ String((char*)readBlockName)+"&admNum="+String((char*)readBlockId)+"&phone="+String((char*)readBlockPhone);

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
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
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
