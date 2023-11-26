
#include"Helper.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte blockAddr = 60;
byte buffer[18];
byte size = sizeof(buffer);
byte status;
byte dataBlock[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF };
byte sector = 15;
byte trailerBlock = 7;
byte piccType;


fclass::fclass(){}



void fclass::dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}



void fclass::cardInit(){
  Serial.begin(9600);
  while (!Serial);
}



void fclass::cardProtcolInit(){
  SPI.begin();
  mfrc522.PCD_Init();
}




bool fclass::authCardByKey(){
  Serial.println(F("Authenticating using key ..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  return true;
}



void fclass::printCardInfo(){
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  Serial.print(F("PICC type: "));
  piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
}





char fclass::readSectorData(){
char succ = "A";
char fai = "B";
  Serial.println(F("Current data in sector:"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);

  Serial.print(F("Reading data from block "));

  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
     return succ;
  }
  Serial.print(F("Data in right block"));
  Serial.print("");
  Serial.print(blockAddr);
   return fai;
  // Serial.println(F(":"));
  // dump_byte_array(buffer, 16);
  // Serial.println();
  // Serial.println();
}




bool fclass::overwightblockData(){
  Serial.print(F("Writing data into block "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));
  dump_byte_array(dataBlock, 16);
  Serial.println();
  status = mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
     return false;
  }
   return true;
}



fclass lib=fclass();
