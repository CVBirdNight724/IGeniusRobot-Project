#include <Arduino.h>

int n_loop = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  n_loop += 1;
  Serial.println(n_loop);
  delay(3000);
}