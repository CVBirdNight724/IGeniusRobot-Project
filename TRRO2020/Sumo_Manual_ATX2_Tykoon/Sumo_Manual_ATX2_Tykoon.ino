#include "ATX2.h"
#include "PS2X_lib.h"

#define FL_CHANNEL      1
#define FR_CHANNEL      2
#define BL_CHANNEL      4
#define BR_CHANNEL      3 
#define PS2_DAT         24    
#define PS2_CMD         25 
#define PS2_CS          26 
#define PS2_CLK         27

#define BASE_SPEED_L    70
#define BASE_SPEED_R    70
#define MIN_SPEED_L     0
#define MIN_SPEED_R     0
#define MAX_SPEED_L     100
#define MAX_SPEED_R     100
#define FL_MIN          0
#define FR_MIN          0
#define BL_MIN          0
#define BR_MIN          0
#define FL_MAX          100
#define FR_MAX          100
#define BL_MAX          100
#define BR_MAX          100
#define FL_REVERSE      true
#define FR_REVERSE      false
#define BL_REVERSE      false
#define BR_REVERSE      true
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
#define PSS_LX_THRESHOLD    20
#define PSS_LY_THRESHOLD    20
#define PSS_RX_THRESHOLD    20
#define PSS_RY_THRESHOLD    20
#define PSS_LX_REVERSE  false
#define PSS_LY_REVERSE  true
#define PSS_RX_REVERSE  false
#define PSS_RY_REVERSE  true

PS2X ps2x;

void driveMotor(int FL, int FR, int BL, int BR){
  if(FL_REVERSE){
    FL *= -1;
  }
  if(FR_REVERSE){
    FR *= -1;
  }
  if(BL_REVERSE){
    BL *= -1;
  }
  if(BR_REVERSE){
    BR *= -1;
  }
  motor(FL_CHANNEL, FL);
  motor(FR_CHANNEL, FR);
  motor(BL_CHANNEL, BL);
  motor(BR_CHANNEL, BR);
}

void testMotor(){
  driveMotor(FL_MAX, FR_MIN, BL_MIN, BR_MIN);
  delay(500);
  driveMotor(-FL_MAX, FR_MIN, BL_MIN, BR_MIN);
  delay(500);
  driveMotor(FL_MIN, FR_MAX, BL_MIN, BR_MIN);
  delay(500);
  driveMotor(FL_MIN, -FR_MAX, BL_MIN, BR_MIN);
  delay(500);
  driveMotor(FL_MIN, FR_MIN, BL_MAX, BR_MIN);
  delay(500);
  driveMotor(FL_MIN, FR_MIN, -BL_MAX, BR_MIN);
  delay(500);
  driveMotor(FL_MIN, FR_MIN, BL_MIN, BR_MAX);
  delay(500);
  driveMotor(FL_MIN, FR_MIN, BL_MIN, -BR_MAX);
  delay(500);
}

void driveSumo(int L_SPEED, int R_SPEED){
  driveMotor(L_SPEED, R_SPEED, L_SPEED, R_SPEED);
}

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

void readPS2X(){
  ps2x.read_gamepad(false, false);
//  Serial.print("LX : ");
//  Serial.print(ps2x.Analog(PSS_LX));
//  Serial.print(", LY: ");
//  Serial.print(ps2x.Analog(PSS_LY));
//  Serial.print(", RX : ");
//  Serial.print(ps2x.Analog(PSS_RX));
//  Serial.print(", RY: ");
//  Serial.println(ps2x.Analog(PSS_RY));
  int PSS_LX_VALUE = ps2x.Analog(PSS_LX);
  int PSS_LY_VALUE = ps2x.Analog(PSS_LY);
  int PSS_RX_VALUE = ps2x.Analog(PSS_RX);
  int PSS_RY_VALUE = ps2x.Analog(PSS_RY);
  
  if(PSS_LX_REVERSE){
    PSS_LX_VALUE = PSS_LX_MAX - PSS_LX_VALUE;
  }
  if(PSS_LY_REVERSE){
    PSS_LY_VALUE = PSS_LY_MAX - PSS_LY_VALUE;
  }
  if(PSS_RX_REVERSE){
    PSS_RX_VALUE = PSS_RX_MAX - PSS_RX_VALUE;
  }
  if(PSS_RY_REVERSE){
    PSS_RY_VALUE = PSS_RY_MAX - PSS_RY_VALUE;
  }
  if(ps2x.Button(PSB_PAD_UP)){
//    Serial.println("Up");
    driveSumo(MAX_SPEED_L, MAX_SPEED_R);
  } 
  else if(ps2x.Button(PSB_PAD_DOWN)){
//    Serial.println("Down");
    driveSumo(-BASE_SPEED_L, -BASE_SPEED_R);
  } 
  else if(ps2x.Button(PSB_PAD_LEFT)){
//    Serial.println("Left");
    driveSumo(-BASE_SPEED_L, BASE_SPEED_R);
  } 
  else if (ps2x.Button(PSB_PAD_RIGHT)){
//    Serial.println("Right");
    driveSumo(BASE_SPEED_L, -BASE_SPEED_R);
  } 
  else if (ps2x.Button(PSB_L1)){
//    Serial.println("L1");
//    driveSumo(BASE_SPEED_L, MAX_SPEED_R);
  }
  else if (ps2x.Button(PSB_L2)){
//     Serial.println("L2");
    driveSumo(MAX_SPEED_L, BASE_SPEED_R);
  }
  else if (ps2x.Button(PSB_R1)){
//    Serial.println("R1");
  }
  else if (ps2x.Button(PSB_R2)){
//    Serial.println("R2");  
  }
  else if (ps2x.Button(PSB_CROSS)){
//    Serial.println("Cross");
    driveSumo(MIN_SPEED_L, MIN_SPEED_R);
  } 
  else if (ps2x.Button(PSB_CIRCLE)){
//    Serial.println("Circle");
    driveSumo(BASE_SPEED_L, MIN_SPEED_R);
  }  
  else if (ps2x.ButtonPressed(PSB_SQUARE)){
//    Serial.println("Square");
    driveSumo(MIN_SPEED_L, BASE_SPEED_R);
  } 
  else if (ps2x.Button(PSB_TRIANGLE)){
    Serial.println("Triangle");
  } 
  else if (ps2x.ButtonPressed(PSB_START)){
    Serial.println("Start");
  }
  else if(ps2x.ButtonPressed(PSB_SELECT)){
    Serial.println("Select");
  }
  else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
//    Serial.println("Stop");
    driveSumo(MIN_SPEED_L, MIN_SPEED_R);
  }
  else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
//    Serial.println("North");
    driveSumo(MAX_SPEED_L, MAX_SPEED_R);
  }
  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
//    Serial.println("North East");
    driveSumo(BASE_SPEED_L, MIN_SPEED_R);
  }
  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
//    Serial.println("East");
    driveSumo(BASE_SPEED_L, -BASE_SPEED_R);
  }
  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
//    Serial.println("South East");
    driveSumo(-BASE_SPEED_L, MIN_SPEED_R);
  }
    else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
//    Serial.println("South");
    driveSumo(-BASE_SPEED_L, -BASE_SPEED_R);
  }
  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
//    Serial.println("South West");
    driveSumo(MIN_SPEED_L, -BASE_SPEED_R);
  }
  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
//    Serial.println("West");
    driveSumo(-BASE_SPEED_L, BASE_SPEED_R);
  }
  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
//    Serial.println("North West");
    driveSumo(MIN_SPEED_L, BASE_SPEED_R);
  }
}



void setup() {
  Serial.begin(115200);
  connectPS2();
}

void loop() {
  testMotor();
//  readPS2X();
//  delay(50);
}
