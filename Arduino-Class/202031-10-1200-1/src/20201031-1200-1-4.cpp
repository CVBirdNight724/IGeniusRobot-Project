#include <Arduino.h>
int n = 0;

void setup(){
    Serial.begin(9600);
}

void loop(){
    n += 1;
    if((n % 2 == 0) and (n % 3 == 0)){
        Serial.print(n);
        Serial.print(": ");
        Serial.println("OK");
    }
    else if(n % 2 == 0){
        Serial.print(n);
        Serial.print(": ");
        Serial.println("Two");
    }
    else if(n % 3 == 0){
        Serial.print(n);
        Serial.print(": ");
        Serial.println("Three");
    }
    else{
        Serial.print(n);
        Serial.print(": ");
        Serial.println(n);
    }
}