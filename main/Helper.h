#ifndef Helper_H
#define Helper_H

#include <SPI.h>
#include <MFRC522.h>


#define RST_PIN 9
#define SS_PIN 10

/*byte piccType;
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
MFRC522::MIFARE_Key key;*/

extern MFRC522 mfrc522;
extern MFRC522::MIFARE_Key key;
extern byte blockAddr;
extern byte buffer[18];
extern byte size;
extern byte status;
extern byte dataBlock[];
extern byte sector;
extern byte trailerBlock;
extern byte piccType;

class fclass{
  public:
  fclass();

void dump_byte_array(byte *buffer, byte bufferSize);
void cardInit();
void cardProtcolInit();
void printCardInfo();
bool authCardByKey();
char readSectorData();
bool overwightblockData();

};

extern fclass lib;



#endif