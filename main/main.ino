#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

byte piccType;
byte status;
byte trailerBlock = 7;
byte sector = 15;
byte blockAddr = 60;
byte dataBlock[] = {  // NEW DATA to Write
  0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xF3, 0xFF
};
byte buffer[18];
byte size = sizeof(buffer);

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  pinMode(2, OUTPUT);

  cardInit();

  cardProtcolInit();

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

  if (!mfrc522.PICC_IsNewCardPresent())
    return;


  // if (!mfrc522.PICC_ReadCardSerial())
  //   return;

  printCardInfo();

  // if ( ! authCardByKey())
  //   return;

  if (!readSectorData()) {
    return;
  }



  // Serial.println(F("Authenticating again using key (updated)..."));
  // status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
  // if (status != MFRC522::STATUS_OK) {
  //   Serial.print(F("PCD_Authenticate() failed: "));
  //   Serial.println(mfrc522.GetStatusCodeName(status));
  // }


  // if (!overwightblockData())
    // return;
    // Serial.print(F("4444444444444444444444444444"));


    Serial.print(F("Reading data from block "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  // if (status != MFRC522::STATUS_OK) {
  //   Serial.print(F("MIFARE_Read() failed: "));
  //   Serial.println(mfrc522.GetStatusCodeName(status));
  // }
  Serial.print(F("Data in block "));
  Serial.print(blockAddr);
  Serial.println(F(":"));
  dump_byte_array(buffer, 16);
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


  if (uidString != "4 F4 11 5C 39 61 80") {
    Serial.println("UID==matches ");

    digitalWrite(2, HIGH);
    // Do something when the card matches
  } else {
    Serial.println("UID==NOOOOO ");

    digitalWrite(2, LOW);
  }
}










//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}


void cardInit() {
  Serial.begin(9600);
  while (!Serial)
    ;
}














void cardProtcolInit() {

  SPI.begin();
  mfrc522.PCD_Init();
}


void printCardInfo() {

  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  Serial.print(F("PICC type: "));
  piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
}

bool authCardByKey() {
  Serial.println(F("Authenticating using key ..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  return true;
}


bool readSectorData() {

  Serial.println(F("Current data in sector:"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);

  Serial.print(F("Reading data from block................................ "));

  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  // if (status != MFRC522::STATUS_OK) {
  //   Serial.print(F("MIFARE_Read() failed: "));
  //   Serial.println(mfrc522.GetStatusCodeName(status));
  //   // return false;
  // }
  Serial.print(F("Data in right block"));
  Serial.print("");
  Serial.print(blockAddr);
  return true;
}



bool overwightblockData() {
  Serial.print(F("Writing data into block "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));
  dump_byte_array(dataBlock, 16);
  Serial.println();
  status = mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    // return false;
  }
  // return true;
}
