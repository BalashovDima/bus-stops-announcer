#define BUTTON_PIN 8

#include <EncButton.h>

Button btn(BUTTON_PIN);


void setup() {
    Serial.begin(9600);
}

void loop() {
    btn.tick();

    if(btn.click()) {
        Serial.println("clicked");
    }

}
