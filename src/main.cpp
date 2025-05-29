#include <Arduino.h>

#define LED_GREEN GPIO_NUM_32
#define LED_RED GPIO_NUM_33
#define TIME 500

void setup() {
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
}

void loop() {
    digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));
    digitalWrite(LED_RED, !digitalRead(LED_RED));
    delay(TIME);
}
