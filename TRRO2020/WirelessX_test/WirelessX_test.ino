#include "ATX2.h"

#define WIRELESS_X_L_UP       0x80
#define WIRELESS_X_L_LEFT     0x40
#define WIRELESS_X_L_RIGHT    0x20
#define WIRELESS_X_L_DOWN     0x10
#define WIRELESS_X_R_UP       0x08
#define WIRELESS_X_R_LEFT     0x04
#define WIRELESS_X_R_RIGHT    0x02
#define WIRELESS_X_R_DOWN     0x01

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(9600);
}

void readWirelessX(){
  byte DATA = Serial1.read();
  if(DATA == WIRELESS_X_L_UP){
    Serial.println("L Up");
  }
  else if(DATA == WIRELESS_X_L_LEFT){
    Serial.println("L Left");
  }
  else if(DATA == WIRELESS_X_L_RIGHT){
    Serial.println("L Right");
  }
  else if(DATA == WIRELESS_X_L_DOWN){
    Serial.println("L Down");
  }
  else if(DATA == WIRELESS_X_R_UP){
    Serial.println("R Up");
  }
  else if(DATA == WIRELESS_X_R_LEFT){
    Serial.println("R Left");
  }
  else if(DATA == WIRELESS_X_R_RIGHT){
    Serial.println("R Right");
  }
  else if(DATA == WIRELESS_X_R_DOWN){
    Serial.println("R Down");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  readWirelessX();
  delay(100);
}
