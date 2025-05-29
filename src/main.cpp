#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define DEBUG true
#define READ_CARD_DATA false
#define MONITOR_SPEED 115200
#define SS_PIN GPIO_NUM_5
#define RST_PIN GPIO_NUM_2
#define CARD_READ_DELAY 1000
#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16
#define LED_GREEN GPIO_NUM_32
#define LED_RED GPIO_NUM_33
#define ELETRONIC_LOCK GPIO_NUM_15
#define TIME_ELETRONIC_LOCK 10000
#define ALERT_DELAY 200
#define COUNT_ALERT_PULSES 10

MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

MFRC522 mfrc522(SS_PIN, RST_PIN);

const char *validUIDs[] = {
    "8C:7B:C2:6E",
};

void readData(void);
bool isAuthorizedCard(const char *uidStr);
void dump_byte_array(char *uidStr, byte *buffer, byte bufferSize);
void openElectroniclock(bool open);

void setup(void)
{
#if DEBUG
    Serial.begin(MONITOR_SPEED);
#endif
    SPI.begin();
    mfrc522.PCD_Init();
    pinMode(ELETRONIC_LOCK, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    digitalWrite(ELETRONIC_LOCK, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
    
#if DEBUG
    Serial.println("DEBUGER ON");
    Serial.print(F("Reader "));
    Serial.print(F(": "));
    mfrc522.PCD_DumpVersionToSerial();
    Serial.println("Aproxime o cartão para leitura...");
    Serial.println(" ");
#endif
}

void loop(void)
{    
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
        readData();
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
    }
}

void readData(void)
{
#if DEBUG
    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));
#endif

    for (byte i = 0; i < 6; i++)
        key.keyByte[i] = 0xFF;

    byte buffer[SIZE_BUFFER] = {0};
    byte bloco = 1;
    byte tamanho = SIZE_BUFFER;
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloco, &key, &(mfrc522.uid)); // line 834 of MFRC522.cpp file

    if (status != MFRC522::STATUS_OK)
    {
#if DEBUG
        Serial.print(F("Authentication failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
#endif
        return;
    }

    // faz a leitura dos dados do bloco
    status = mfrc522.MIFARE_Read(bloco, buffer, &tamanho);

    if (status != MFRC522::STATUS_OK)
    {

#if DEBUG
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
#endif

        // Adicionar lógica para piscar led para erro de leitura - Opicional

        return;
    }
    else
    {

#if DEBUG
        Serial.print(F("Reading Sucess: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
#endif

        char uidStr[32] = "";
        dump_byte_array(uidStr, mfrc522.uid.uidByte, mfrc522.uid.size);

        bool isOpen = digitalRead(ELETRONIC_LOCK);

        if (isAuthorizedCard(uidStr))
        {
            if (!isOpen) {
#if DEBUG
                Serial.println("Cartão Cadastrado!");
                Serial.println("Fechadura liberada!");
#endif
                openElectroniclock(true);
            } 
            else
            {
#if DEBUG
                Serial.println("Cartão Cadastrado!");
                Serial.println("Fechadura Bloqueada!");
#endif
                openElectroniclock(false);
            }
        }
        else
        {
#if DEBUG
            Serial.println("Cartão negado!");
#endif
        }
    }

#if DEBUG
    Serial.print(F("Dados bloco ["));
    Serial.print(bloco);
    Serial.print(F("]: "));

    // imprime os dados lidos
    for (uint8_t i = 0; i < MAX_SIZE_BLOCK; i++)
    {
        Serial.write(buffer[i]);
    }

    Serial.println("\n");
#endif
}

bool isAuthorizedCard(const char *uidStr)
{

    const int numUIDs = sizeof(validUIDs) / sizeof(validUIDs[0]);

    for (int i = 0; i < numUIDs; i++)
    {
        if (strcmp(uidStr, validUIDs[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

void dump_byte_array(char *uidStr, byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {

        if (i > 0)
        {
            strcat(uidStr, ":");
        }

        char hexByte[4];
        sprintf(hexByte, "%02X", buffer[i]);
        strcat(uidStr, hexByte);
    }
}

void openElectroniclock(bool open)
{

    if (open)
    {
        digitalWrite(ELETRONIC_LOCK, HIGH);
        digitalWrite(LED_RED, LOW);
        for (size_t i = 0; i < COUNT_ALERT_PULSES; i++)
        {
            digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));
            delay(ALERT_DELAY);
        }
        digitalWrite(LED_GREEN, HIGH);
    }
    else
    {
        digitalWrite(ELETRONIC_LOCK, LOW);
        digitalWrite(LED_GREEN, LOW);
        for (size_t i = 0; i < COUNT_ALERT_PULSES; i++)
        {
            digitalWrite(LED_RED, !digitalRead(LED_RED));
            delay(ALERT_DELAY);
        }
        digitalWrite(LED_RED, HIGH);
    }
}