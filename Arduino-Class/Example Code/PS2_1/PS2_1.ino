#include "PS2X_lib.h"

#define PS2_DAT        13    
#define PS2_CMD        12 
#define PS2_CS         11 
#define PS2_CLK        10

PS2X ps2x;

void readPS2X(){
  ps2x.read_gamepad(false,false);
  if(ps2x.Button(PSB_L1)  && ps2x.Button(PSB_L2)){
    Serial.println("L1 & L2");
  }
  else if(ps2x.Button(PSB_PAD_UP)){
    Serial.println("Up");
  } 
  else if(ps2x.Button(PSB_PAD_DOWN)){
    Serial.println("Down");
  } 
  else if(ps2x.Button(PSB_PAD_LEFT)){
    Serial.println("Left");
  } 
  else if (ps2x.Button(PSB_PAD_RIGHT)){
    Serial.println("Right");
  } 
  else if (ps2x.Button(PSB_L1)){
    Serial.println("L1");
  }
  else if (ps2x.Button(PSB_L2)){
     Serial.println("L2");
  }
  else if (ps2x.Button(PSB_R1)){
    Serial.println("R1");
  }
  else if (ps2x.Button(PSB_R2)){
    Serial.println("R2");  
  }
  else if (ps2x.Button(PSB_CROSS)){
    Serial.println("Cross");
  } 
  else if (ps2x.Button(PSB_CIRCLE)){
    Serial.println("Circle");
  }  
  else if (ps2x.ButtonPressed(PSB_SQUARE)){
    Serial.println("Square");
  } 
  else if (ps2x.Button(PSB_TRIANGLE)){
    Serial.println("Triangle");
  } 
  else if (ps2x.ButtonPressed(PSB_START)){
    Serial.println("Start");
  }
}



void setup() {
  Serial.begin(115200);
  while(1){
    int error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_CS, PS2_DAT,true,true);
    if(error == 0){
      Serial.println("Connected\n");
      break;
    }
    else{
      Serial.print('.');
      delay(200);
    }
  }
  
}

void loop() {
  readPS2X();
  delay(200);
}
