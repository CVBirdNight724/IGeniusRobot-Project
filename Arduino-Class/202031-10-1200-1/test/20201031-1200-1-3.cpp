#include <Arduino.h>

int n = 0;

void setup(){
    Serial.begin(9600);
}

void loop(){
    n += 1;
    if(n % 3 == 0){
        Serial.println("true");
    }
    else{
        Serial.println("false");
    }
}