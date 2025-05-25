#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN GPIO_NUM_4
#define RST_PIN GPIO_NUM_2
#define MONITOR_SPEED 115200
#define CARD_READ_DELAY 1000

MFRC522 mfrc522(SS_PIN, RST_PIN);

String dump_byte_array(byte *buffer, byte bufferSize);

void setup()
{
    Serial.begin(MONITOR_SPEED);
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.print(F("Reader "));
    Serial.print(F(": "));
    mfrc522.PCD_DumpVersionToSerial();
    Serial.println("Aproxime um cartão RFID do leitor...");
}

void loop()
{

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
        String uid = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
        Serial.println(uid);
        delay(CARD_READ_DELAY);
    }
}

String dump_byte_array(byte *buffer, byte bufferSize)
{
    String uidStr = "";

    for (byte i = 0; i < bufferSize; i++)
    {
        if (buffer[i] < 0x10)
            uidStr += "0";
        uidStr += String(buffer[i], HEX);
        if (i < bufferSize - 1)
            uidStr += ":";
    }

    uidStr.toUpperCase();
    return uidStr;
}