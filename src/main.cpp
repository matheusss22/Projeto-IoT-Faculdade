#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// Debug and hardware definitions
#define DEBUG true
#define MONITOR_SPEED 115200
#define SS_PIN GPIO_NUM_5
#define RST_PIN GPIO_NUM_2
#define LED_GREEN GPIO_NUM_32
#define LED_RED GPIO_NUM_33
#define ELECTRONIC_LOCK GPIO_NUM_15
#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16
#define LOCK_OPEN_TIME 10000
#define ALERT_DELAY 200
#define ALERT_PULSE_COUNT 10

MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
MFRC522 mfrc522(SS_PIN, RST_PIN);

// List of authorized UIDs
const char *authorizedUIDs[] = {
    "8C:7B:C2:6E",
    "53:B2:61:0B"
};

// Function declarations
void handleCardRead();
bool isAuthorized(const char *uidStr);
void formatUID(char *uidStr, byte *buffer, byte bufferSize);
void controlLock(bool open);

void setup() {
    SPI.begin();
    mfrc522.PCD_Init();

    pinMode(ELECTRONIC_LOCK, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    digitalWrite(ELECTRONIC_LOCK, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);

    #if DEBUG
    Serial.begin(MONITOR_SPEED);
    Serial.println("DEBUG MODE ENABLED");
    Serial.print("Reader: ");
    mfrc522.PCD_DumpVersionToSerial();
    Serial.println("Approach a card to the reader...\n");
    #endif
}

void loop() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        handleCardRead();
    }
}

void handleCardRead() {
    #if DEBUG
    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));
    #endif

    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    byte buffer[SIZE_BUFFER] = {0};
    byte block = 1;
    byte size = SIZE_BUFFER;

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
    #if DEBUG
        Serial.print("Authentication failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
    #endif
        return;
    }

    status = mfrc522.MIFARE_Read(block, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
    #if DEBUG
        Serial.print("Reading failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
    #endif
        return;
    }

    #if DEBUG
    Serial.print("Reading success: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    #endif

    char uidStr[32] = "";
    formatUID(uidStr, mfrc522.uid.uidByte, mfrc522.uid.size);
    bool lockState = digitalRead(ELECTRONIC_LOCK);

    if (isAuthorized(uidStr)) {
    #if DEBUG
        Serial.println("Authorized card detected.");
    #endif
        controlLock(!lockState);
        Serial.println(lockState ? "Lock closed!" : "Lock opened!");
    } else {
    #if DEBUG
        Serial.println("Unauthorized card.");
    #endif
    }

    #if DEBUG
    Serial.print("Block [");
    Serial.print(block);
    Serial.print("] data: ");
    for (uint8_t i = 0; i < MAX_SIZE_BLOCK; i++) {
        Serial.write(buffer[i]);
    }
    Serial.println("\n");
    #endif

    mfrc522.PICC_HaltA();    
    mfrc522.PCD_StopCrypto1();
}

bool isAuthorized(const char *uidStr) {
    const int totalUIDs = sizeof(authorizedUIDs) / sizeof(authorizedUIDs[0]);

    for (int i = 0; i < totalUIDs; i++) {
        if (strcmp(uidStr, authorizedUIDs[i]) == 0) {
            return true;
        }
    }
    return false;
}

void formatUID(char *uidStr, byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        if (i > 0) strcat(uidStr, ":");
        char hexByte[4];
        sprintf(hexByte, "%02X", buffer[i]);
        strcat(uidStr, hexByte);
    }
}

void controlLock(bool open) {
    if (open) {
        digitalWrite(ELECTRONIC_LOCK, HIGH);
        digitalWrite(LED_RED, LOW);
        for (size_t i = 0; i < ALERT_PULSE_COUNT; i++) {
            digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));
            delay(ALERT_DELAY);
        }
        digitalWrite(LED_GREEN, HIGH);
    } else {
        digitalWrite(ELECTRONIC_LOCK, LOW);
        digitalWrite(LED_GREEN, LOW);
        for (size_t i = 0; i < ALERT_PULSE_COUNT; i++) {
            digitalWrite(LED_RED, !digitalRead(LED_RED));
            delay(ALERT_DELAY);
        }
        digitalWrite(LED_RED, HIGH);
    }
}
