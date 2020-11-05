#include "ATX.h"
#include "PS2X_lib.h"

#define PS2_DAT        24    
#define PS2_CMD        25 
#define PS2_CS         26 
#define PS2_CLK        27
#define PSS_LX_MIN      0
#define PSS_LY_MIN      0
#define PSS_RX_MIN      0
#define PSS_RY_MIN      0
#define PSS_LX_MAX      255
#define PSS_LY_MAX      255
#define PSS_RX_MAX      255
#define PSS_RY_MAX      255
#define PSS_LX_MEAN     127
#define PSS_LY_MEAN     127
#define PSS_RX_MEAN     127
#define PSS_RY_MEAN     127
#define PSS_LX_THRESHOLD    30
#define PSS_LY_THRESHOLD    30
#define PSS_RX_THRESHOLD    30
#define PSS_RY_THRESHOLD    30

PS2X ps2x;

void connectPS2(){
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connectPS2();
}

void loop() {
  // put your main code here, to run repeatedly:

}
