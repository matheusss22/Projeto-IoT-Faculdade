#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// Pinos conforme a imagem
#define SS_PIN GPIO_NUM_4  // SDA
#define RST_PIN GPIO_NUM_2 // RST

MFRC522 mfrc522(SS_PIN, RST_PIN); // Criar instância do objeto MFRC522

void setup()
{
    Serial.begin(115200);
    SPI.begin();        // Iniciar barramento SPI
    mfrc522.PCD_Init(); // Iniciar MFRC522
    Serial.println("Aproxime um cartão RFID do leitor...");
}

void loop()
{
    // Verifica se há nova tag presente
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    // Verifica se consegue ler o conteúdo
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    // Mostrar UID no Serial Monitor
    Serial.print("UID da tag: ");
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    delay(1000); // Aguarda 1 segundo antes de continuar
}