#include <Arduino.h>

#define PIN GPIO_NUM_32
#define TIME 500

void setup() {
    pinMode(PIN, OUTPUT);
}

void loop() {
    digitalWrite(PIN, !digitalRead(PIN));
    delay(TIME);
}
