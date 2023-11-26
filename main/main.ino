
#include"Helper.h"



void setup() {
  pinMode(2, OUTPUT);

  lib.cardInit();

 lib.cardProtcolInit();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  //Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
  // Serial.print(F("Using key (for A and B):"));
  // dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
  //Serial.println();

  //Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));
}

void loop() {

lib.cardInit();
lib.cardProtcolInit();


  if (!mfrc522.PICC_IsNewCardPresent())
    return;
  Serial.print(F("00000000000000000000000000000000000000"));


  if (!mfrc522.PICC_ReadCardSerial())
    // return;
  Serial.print(F("1111111111111111111111111111111"));

  lib.printCardInfo();
  Serial.print(F("22222222222222222222222"));

  // if ( ! authCardByKey())
  //   return;

  if (lib.readSectorData()=="A"){

  Serial.print(F("33333333333333333"));
  }
    // return;



  // Serial.println(F("Authenticating again using key (updated)..."));
  // status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
  // if (status != MFRC522::STATUS_OK) {
  //   Serial.print(F("PCD_Authenticate() failed: "));
  //   Serial.println(mfrc522.GetStatusCodeName(status));
  // }


  if (!lib.overwightblockData())
    // return;
  // Serial.print(F("4444444444444444444444444444"));


  Serial.print(F("Reading data from block "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  Serial.print(F("Data in block "));
  Serial.print(blockAddr);
  Serial.println(F(":"));
  lib.dump_byte_array(buffer, 16);
  Serial.println();

  Serial.println(F("Checking result..."));
  byte count = 0;
  for (byte i = 0; i < 16; i++) {
    if (buffer[i] == dataBlock[i])
      count++;
  }
  Serial.print(F("Number of bytes that match = "));
  Serial.println(count);
  if (count == 16) {
    Serial.println(F("Success :-)"));
  } else {
    Serial.println(F("Failure, no match :-("));
    Serial.println(F("  perhaps the write didn't work properly..."));
  }


  Serial.println(F("Current data in sector:"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);






  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; ++i) {
    uidString += String(mfrc522.uid.uidByte[i], HEX);
    uidString.toUpperCase();
    if (i + 1 < mfrc522.uid.size) {
      uidString += " ";
    }
  }

  Serial.println("UID " + uidString);


  if (uidString == "4 F4 11 5C 39 61 80") {
    Serial.println("UID==matches ");

    digitalWrite(2, HIGH);
    // Do something when the card matches
  } else {
    Serial.println("UID==NOOOOO ");

    digitalWrite(2, LOW);
  }
}




























