#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define DEBUG true
#define READ_CARD_DATA false
#define MONITOR_SPEED 115200
#define SS_PIN GPIO_NUM_4
#define RST_PIN GPIO_NUM_2
#define CARD_READ_DELAY 1000
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16

MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

MFRC522 mfrc522(SS_PIN, RST_PIN);

const char* validUIDs[] = {
    "8C:7B:C2:6E",
};

void readData();
bool isAuthorizedCard(const char* uidStr);
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
    Serial.println(" ");
    #endif
}

void loop()
{
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
        readData();
        mfrc522.PICC_HaltA(); 
        mfrc522.PCD_StopCrypto1(); 
    }
}


void readData()
{
  //imprime os detalhes tecnicos do cartão/tag
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); 

  //Prepara a chave - todas as chaves estão configuradas para FFFFFFFFFFFFh (Padrão de fábrica).
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //buffer para colocar os dados ligos
  byte buffer[SIZE_BUFFER] = {0};

  //bloco que faremos a operação
  byte bloco = 1;
  byte tamanho = SIZE_BUFFER;

  //faz a autenticação do bloco que vamos operar
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloco, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  
  if (status != MFRC522::STATUS_OK) {
    #if DEBUG
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    #endif
    return;
  }

  //faz a leitura dos dados do bloco
  status = mfrc522.MIFARE_Read(bloco, buffer, &tamanho);

  if (status != MFRC522::STATUS_OK) {
    
    #if DEBUG
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    #endif
    
    // Adicionar lógica para piscar led para erro de leitura - Opicional

    return;
  }
  else{
    
    #if DEBUG
    Serial.print(F("Reading Sucess: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    #endif
    
    char uidStr[32] = "";
    dump_byte_array(uidStr, mfrc522.uid.uidByte, mfrc522.uid.size);

    if (isAuthorizedCard(uidStr)) {
        Serial.println("Cartão autorizado!");
        // Lógica para abrir fechadura e piscar led verde
    } else {
        Serial.println("Cartão negado!");
        // Lógica para  piscar led vermelho
    }
  }

  #if READ_CARD_DATA
  Serial.print(F("\nDados bloco ["));
  Serial.print(bloco);Serial.print(F("]: "));

  //imprime os dados lidos
  for (uint8_t i = 0; i < MAX_SIZE_BLOCK; i++)
  {
      Serial.write(buffer[i]);
  }
  #endif

  #if DEBUG
  Serial.println(" ");
  #endif
}

bool isAuthorizedCard(const char* uidStr) {
    
    const int numUIDs = sizeof(validUIDs) / sizeof(validUIDs[0]);
    
    for (int i = 0; i < numUIDs; i++) {
        if (strcmp(uidStr, validUIDs[i]) == 0) {
            return true;
        }
    }
    return false;
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