#include <Arduino.h>
#include <SPI.h>

#define PIN_MOSI 23
#define PIN_MISO 19
#define PIN_SCK 18
#define PIN_SS 5 // Qualquer pino só para SPI.begin()

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_SS);
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

}

void loop()
{

    byte dataToSend = 0x12;
    byte received;

    digitalWrite(PIN_SS, LOW);
    received = SPI.transfer(dataToSend);
    digitalWrite(PIN_SS, HIGH);
    SPI.endTransaction();

    if (received == dataToSend)
    {
        Serial.println("✅ SPI funcionando (loopback ok)");
    }
    else
    {
        Serial.println("❌ SPI falhou - problema no ESP32 ou fiação");
    }

    delay(500);
}
