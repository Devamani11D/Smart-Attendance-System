//include libraries
#include <Adafruit_Fingerprint.h>
#include<HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>

const char* root_ca_website = \
      "-----BEGIN CERTIFICATE-----\n" \
      "MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n" \
      "CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n" \
      "MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n" \
      "MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n" \
      "Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n" \
      "A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n" \
      "27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n" \
      "Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n" \
      "TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n" \
      "qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n" \
      "szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n" \
      "Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n" \
      "MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n" \
      "wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n" \
      "aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n" \
      "VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n" \
      "AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n" \
      "FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n" \
      "C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n" \
      "QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n" \
      "h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n" \
      "7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n" \
      "ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n" \
      "MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n" \
      "Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n" \
      "6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n" \
      "0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n" \
      "2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n" \
      "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n" \
      "-----END CERTIFICATE-----\n";


WiFiMulti wifiMulti;

HardwareSerial mySerial(2);

//instantiate
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

String Spreadsheet_data_1 = "";

uint8_t id;
void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600);
  while (!Serial);                //wait until serial port is connected
  delay(100);
  Serial.println("\nSMART ATTENDANCE SYSTEM\nFINGER PRINT ENROLLMENT");

  //set the data rate for the sensor serial port
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
    Serial.printf("SETTINGUP WAIT %d....\n", t);
    Serial.flush();
    delay(1000);
  }


  WiFi.mode(WIFI_STA);

  wifiMulti.addAP("CSK", "chandra123");

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     
{
  Spreadsheet_data_1 = "https://script.google.com/macros/s/AKfycbzwXIVsmNCQrgAHDo-4EK4DNvLSa4bjvzInuS_8lFbILyqqEaeNlpV1yFeNy44tzIVJGA/exec?";
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID (from 1 to 999) of the fingerprint that you want to enroll..");
  id = readnumber();
  if (id == 0) {
     // ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID - ");
  Serial.println(id);

  while (!  getFingerprintEnroll() );
  char buffer0[20];
  char buffer1[20];
  char buffer2[20];
  char buffer3[20];

  size_t len ;
  len=0;
  Serial.flush();
  Serial.flush();
  while (len < sizeof(buffer0) - 1) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        buffer0[len] = '\0';  
        break;
      } else {
        buffer0[len++] = c;
      }
    }
  }
  Serial.print(buffer0);

  len=0;
  //Name - buffer2
  Serial.println("Enter Your Name: ");
    Serial.flush();
  while (len < sizeof(buffer2) - 1) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        buffer2[len] = '\0';  
        break;
      } else {
        buffer2[len++] = c;
      }
    }
  }
  Serial.println(buffer2);
  
  
  len=0;
  //Phone Number-buffer3
  Serial.println("Enter Your phone number: ");
  Serial.flush();
  while (len < sizeof(buffer3) - 1) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        buffer3[len] = '\0';  
        break;
      } else {
        buffer3[len++] = c;
      }
    }
  }
  Serial.println(buffer3);
  
  len=0;
  //ID-buffer1
  Serial.println("Enter Your ID: ");
    Serial.flush();
  while (len < sizeof(buffer1) - 1) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        buffer1[len] = '\0';  
        break;
      } else {
        buffer1[len++] = c;
      }
    }
  }
  Serial.println(buffer1);
  
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
          Serial.println("Enrollment Successful");   
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
  else{
    Serial.println("Wifi Not Connected");
  }
  
}

uint8_t getFingerprintEnroll(){
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
