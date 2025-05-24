#include <Arduino.h>

void setup() {
    Serial.begin(19200);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

    int estado_led = 0;

    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);

    estado_led = digitalRead(LED_BUILTIN);
    Serial.println(estado_led);

    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);

    estado_led = digitalRead(LED_BUILTIN);
    Serial.println(estado_led);
    
}