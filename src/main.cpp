#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define DEBUG true
#define MONITOR_SPEED 115200
#define SS_PIN GPIO_NUM_4
#define RST_PIN GPIO_NUM_2
#define CARD_READ_DELAY 1000

MFRC522 mfrc522(SS_PIN, RST_PIN);


void dump_byte_array(char *uidStr, byte *buffer, byte bufferSize);

void setup()
{
    Serial.begin(MONITOR_SPEED);
    SPI.begin();
    mfrc522.PCD_Init();

    #if DEBUG
    Serial.println("DEBUG ON");
    Serial.print(F("Reader "));
    Serial.print(F(": "));
    mfrc522.PCD_DumpVersionToSerial();
    Serial.println("Approach an RFID card to the reader...");
    #endif
}

void loop()
{

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
        #if DEBUG
        char uidStr[40] = "";
        dump_byte_array(uidStr, mfrc522.uid.uidByte, mfrc522.uid.size);
        Serial.println(uidStr);
        delay(CARD_READ_DELAY);

        if (strcmp(uidStr, "53:B2:61:0B") == 0) {
            Serial.println("Cartão autorizado!");
        } else {
            Serial.println("Cartão negado!");
        }
        #endif
    }
}

void dump_byte_array(char *uidStr, byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        
        if (i > 0) {
            strcat(uidStr, ":");
        }

        char hexByte[4];
        sprintf(hexByte, "%02X", buffer[i]);
        strcat(uidStr, hexByte);
    }
}




