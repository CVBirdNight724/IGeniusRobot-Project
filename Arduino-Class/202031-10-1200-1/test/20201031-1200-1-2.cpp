#include <Arduino.h>

int n_loop = 0;
bool even_number = false;

void setup() {
    Serial.begin(9600);
}

void loop() {
    n_loop += 1;
    if(even_number){
        Serial.println(-n_loop);
    }
    else{
        Serial.println(n_loop);
    }
    even_number = !even_number;
}