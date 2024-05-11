#include "./display_7_segment.h"
#include <Arduino.h>

void resetNumber() {
    digitalWrite(RESET_PIN, HIGH);
    digitalWrite(RESET_PIN, LOW);
}


void showNumber(int n) {
    resetNumber();

    while (n--) {
        digitalWrite(CLOCK_PIN, HIGH);
        digitalWrite(CLOCK_PIN, LOW);
    }
}