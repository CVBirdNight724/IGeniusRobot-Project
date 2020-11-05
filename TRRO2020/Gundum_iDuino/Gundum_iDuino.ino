#include "PS2X_lib.h"

#define PS2_DAT         13    
#define PS2_CMD         12 
#define PS2_CS          11 
#define PS2_CLK         10
#define BL_A            7
#define BL_B            6
#define BR_A            5
#define BR_B            4
#define FL_A            A0
#define FL_B            A1
#define FR_A            A2
#define FR_B            A3

#define FL_THRESHOLD    20
#define FR_THRESHOLD    20
#define BL_THRESHOLD    20
#define BR_THRESHOLD    20
#define FL_MIN          0
#define FR_MIN          0
#define BL_MIN          0
#define BR_MIN          0
#define FL_MAX          100
#define FR_MAX          100
#define BL_MAX          100
#define BR_MAX          100
#define DRIVE_X_THRESHOLD   20
#define DRIVE_Y_THRESHOLD   20
#define DRIVE_X_MIN     0
#define DRIVE_Y_MIN     100
#define DRIVE_X_MAX     100
#define DRIVE_Y_MAX     100
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
#define MECANUM_CW      1
#define MECANUM_CCW     -1

PS2X ps2x;

void setMotor(){
  pinMode(FL_A, OUTPUT);
  pinMode(FL_B, OUTPUT);
  pinMode(FR_A, OUTPUT);
  pinMode(FR_B, OUTPUT);
  pinMode(BL_A, OUTPUT);
  pinMode(BL_B, OUTPUT);
  pinMode(BR_A, OUTPUT);
  pinMode(BR_B, OUTPUT);
}

void driveMotor(int FL, int FR, int BL, int BR){
  // FL 
  if(FL > FL_THRESHOLD){
    digitalWrite(FL_A, HIGH);
    digitalWrite(FL_B, LOW);
  }
  else if(FL < -FL_THRESHOLD){
    digitalWrite(FL_A, LOW);
    digitalWrite(FL_B, HIGH);
  }
  else if(FL < FL_THRESHOLD && FL > -FL_THRESHOLD){
    digitalWrite(FL_A, HIGH);
    digitalWrite(FL_B, HIGH);
  }

  // FR
  if(FR > FR_THRESHOLD){
    digitalWrite(FR_A, HIGH);
    digitalWrite(FR_B, LOW);
  }
  else if(FR < -FR_THRESHOLD){
    digitalWrite(FR_A, LOW);
    digitalWrite(FR_B, HIGH);
  }
  else if(abs(FR) < FR_THRESHOLD){
    digitalWrite(FR_A, HIGH);
    digitalWrite(FR_B, HIGH);
  }

  // BL
  if(BL > BL_THRESHOLD){
    digitalWrite(BL_A, HIGH);
    digitalWrite(BL_B, LOW);
  }
  else if(BL < -BL_THRESHOLD){
    digitalWrite(BL_A, LOW);
    digitalWrite(BL_B, HIGH);
  }
  else if(BL < BL_THRESHOLD && BL > -BL_THRESHOLD){
    digitalWrite(BL_A, HIGH);
    digitalWrite(BL_B, HIGH);
  }
  
  // BR
  if(BR > BR_THRESHOLD){
    digitalWrite(BR_A, HIGH);
    digitalWrite(BR_B, LOW);
  }
  else if(BR < -BR_THRESHOLD){
    digitalWrite(BR_A, LOW);
    digitalWrite(BR_B, HIGH);
  }
  else if(abs(BR) < BR_THRESHOLD){
    digitalWrite(BR_A, HIGH);
    digitalWrite(BR_B, HIGH);
  }
}

void driveMecanum(int X, int Y, int CASE=0){
  if(abs(X) <= DRIVE_X_THRESHOLD && abs(Y) >= DRIVE_Y_THRESHOLD){ // Stop
    driveMotor(FL_MIN, FR_MIN, BL_MIN, BR_MIN);
  }
  else if(CASE == MECANUM_CW){     // Clockwise (CW)
    driveMotor(FL_MAX, -FR_MAX, BL_MAX, -BR_MAX);
  }
  else if(CASE == MECANUM_CCW){    // Counterclockwise (CCW)
    driveMotor(-FL_MAX, FR_MAX, -BL_MAX, BR_MAX);
  }
  else if(abs(X) <= DRIVE_X_THRESHOLD && Y > DRIVE_Y_THRESHOLD){  // North
    driveMotor(FL_MAX, FR_MAX, BL_MAX, BR_MAX);
  }
  else if(X > DRIVE_X_THRESHOLD && Y > DRIVE_Y_THRESHOLD){        // North East
    driveMotor(FL_MAX, FR_MIN, BL_MIN, BR_MAX);
  }
  else if(X > DRIVE_X_THRESHOLD && abs(Y) <= DRIVE_Y_THRESHOLD){  // East
    driveMotor(FL_MAX, -FR_MAX, -BL_MAX, BR_MAX);
  }
  else if(X > DRIVE_X_THRESHOLD && Y < -DRIVE_Y_THRESHOLD){       // South East
    driveMotor(FL_MIN, -FR_MAX, -BL_MAX, BR_MIN);
  }
  else if(abs(X) <= DRIVE_X_THRESHOLD && Y < -DRIVE_Y_THRESHOLD){ // South
    driveMotor(-FL_MAX, -FR_MAX, -BL_MAX, -BR_MAX);
  }
  else if(X < -DRIVE_X_THRESHOLD && Y < -DRIVE_Y_THRESHOLD){      // South West 
    driveMotor(-FL_MAX, FR_MIN, BL_MIN, -BR_MAX);
  }
  else if(X < -DRIVE_X_THRESHOLD && abs(Y) <= DRIVE_Y_THRESHOLD){ // West
    driveMotor(-FL_MAX, FR_MAX, BL_MAX, -BR_MAX);
  }
  else if(X < -DRIVE_X_THRESHOLD && Y > DRIVE_Y_THRESHOLD){       // North West
    driveMotor(FL_MIN, FR_MAX, BL_MAX, BR_MIN);
  }
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
  ps2x.read_gamepad(false,false);
  if(ps2x.Button(PSB_PAD_UP)){
//    Serial.println("Up");
    driveMecanum(DRIVE_X_MIN, DRIVE_Y_MAX);
  } 
  else if(ps2x.Button(PSB_PAD_DOWN)){
//    Serial.println("Down");
    driveMecanum(DRIVE_X_MIN, -DRIVE_Y_MAX);
  } 
  else if(ps2x.Button(PSB_PAD_LEFT)){
//    Serial.println("Left");
    driveMecanum(-DRIVE_X_MAX, DRIVE_Y_MIN);
  } 
  else if (ps2x.Button(PSB_PAD_RIGHT)){
//    Serial.println("Right");
    driveMecanum(DRIVE_X_MAX, DRIVE_Y_MIN);
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
//    Serial.println("Cross");
    driveMecanum(DRIVE_X_MIN, DRIVE_Y_MIN);
  } 
  else if (ps2x.Button(PSB_CIRCLE)){
//    Serial.println("Circle");
    driveMecanum(DRIVE_X_MIN, DRIVE_Y_MIN, MECANUM_CCW);
  }  
  else if (ps2x.ButtonPressed(PSB_SQUARE)){
//    Serial.println("Square");
    driveMecanum(DRIVE_X_MIN, DRIVE_Y_MIN, MECANUM_CW);
  } 
  else if (ps2x.Button(PSB_TRIANGLE)){
//    Serial.println("Triangle");
  } 
  else if (ps2x.ButtonPressed(PSB_START)){
    Serial.println("Start");
  }
  else if(ps2x.ButtonPressed(PSB_SELECT)){
    Serial.println("Select");
  }
  else if(abs(ps2x.Analog(PSS_LX)-PSS_LX_MEAN) < PSS_LX_THRESHOLD && ps2x.Analog(PSS_LY) > PSS_LY_MEAN+PSS_LY_THRESHOLD){
//    Serial.println("North");
    driveMecanum(DRIVE_X_MIN, DRIVE_Y_MAX);
  }
  else if(ps2x.Analog(PSS_LX) > PSS_LX_MEAN+PSS_LX_THRESHOLD && ps2x.Analog(PSS_LY) > PSS_LY_MEAN+PSS_LY_THRESHOLD){
//    Serial.println("North East");
    driveMecanum(DRIVE_X_MAX, DRIVE_Y_MAX); 
  }
  else if(ps2x.Analog(PSS_LX) > PSS_LX_MEAN+PSS_LX_THRESHOLD && abs(ps2x.Analog(PSS_LY)-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
//    Serial.println("East");
    driveMecanum(DRIVE_X_MAX, DRIVE_Y_MIN);
  }
  else if(ps2x.Analog(PSS_LX) > PSS_LX_MEAN+PSS_LX_THRESHOLD && ps2x.Analog(PSS_LY) < PSS_LY_MEAN-PSS_LY_THRESHOLD){
//    Serial.println("South East");
    driveMecanum(DRIVE_X_MAX, -DRIVE_Y_MAX);
  }
  else if(abs(ps2x.Analog(PSS_LX)-PSS_LX_MEAN) < PSS_LX_THRESHOLD && ps2x.Analog(PSS_LY) < PSS_LY_MEAN-PSS_LY_THRESHOLD){
//    Serial.println("South");
    driveMecanum(DRIVE_X_MIN, -DRIVE_Y_MAX);
  }
  else if(ps2x.Analog(PSS_LX) < PSS_LX_MEAN-PSS_LX_THRESHOLD && ps2x.Analog(PSS_LY) < PSS_LY_MEAN-PSS_LY_THRESHOLD){
//    Serial.println("South West");
    driveMecanum(-DRIVE_X_MAX, -DRIVE_Y_MAX);
  }
  else if(ps2x.Analog(PSS_LX) < PSS_LX_MEAN-PSS_LX_THRESHOLD && abs(ps2x.Analog(PSS_LY)-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
//    Serial.println("West");
    driveMecanum(-DRIVE_X_MAX, DRIVE_Y_MIN);
  }
  else if(ps2x.Analog(PSS_LX) < PSS_LX_MEAN-PSS_LX_THRESHOLD && ps2x.Analog(PSS_LY) > PSS_LY_MEAN+PSS_LY_THRESHOLD){
//    Serial.println("North West");
    driveMecanum(-DRIVE_X_MAX, DRIVE_Y_MAX);
  }
}

void setup() {
  Serial.begin(115200);
  connectPS2();
}

void loop() {
  readPS2X();
}
