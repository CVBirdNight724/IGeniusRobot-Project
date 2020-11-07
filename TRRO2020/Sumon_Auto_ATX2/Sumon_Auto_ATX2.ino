#include "ATX2.h"

#define FL_CHANNEL      1
#define FR_CHANNEL      2
#define BL_CHANNEL      4
#define BR_CHANNEL      3 
#define IR_CHANNEL_W    0
#define IR_CHANNEL_NW   1
#define IR_CHANNEL_N    2
#define IR_CHANNEL_NE   3
#define IR_CHANNEL_E    4
#define IR_CHANNEL_L    29
#define IR_CHANNEL_R    30

#define BASE_SPEED_L    70
#define BASE_SPEED_R    70
#define FL_MIN          0
#define FR_MIN          0
#define BL_MIN          0
#define BR_MIN          0
#define FL_MAX          100
#define FR_MAX          100
#define BL_MAX          100
#define BR_MAX          100
#define FL_REVERSE      false
#define FR_REVERSE      false
#define BL_REVERSE      false
#define BR_REVERSE      false
#define IR_L_REVERSE    false
#define IR_R_REVERSE    false
#define IR_BLACK_VALUE  true

unsigned int IR_DISTANCE_W;
unsigned int IR_DISTANCE_NW;
unsigned int IR_DISTANCE_N;
unsigned int IR_DISTANCE_NE;
unsigned int IR_DISTANCE_E;
bool IR_VALUE_L;
bool IR_VALUE_R;

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

void readIR(){
  IR_DISTANCE_W = analog(IR_CHANNEL_W);
  IR_DISTANCE_NW = analog(IR_CHANNEL_NW);
  IR_DISTANCE_N = analog(IR_CHANNEL_N);
  IR_DISTANCE_NE = analog(IR_CHANNEL_NE);
  IR_DISTANCE_E = analog(IR_CHANNEL_E);
//  Serial.print("IR_W: ");
//  Serial.print(IR_DISTANCE_W);  
//  Serial.print(", IR_NW: ");
//  Serial.print(IR_DISTANCE_NW);
//  Serial.print(", IR_N: ");
//  Serial.print(IR_DISTANCE_N);
//  Serial.print(", IR_NE: ");
//  Serial.print(IR_DISTANCE_NE);
//  Serial.print(", IR_E: ");
//  Serial.println(IR_DISTANCE_E);

  IR_VALUE_L = digitalRead(IR_CHANNEL_L);
  IR_VALUE_R = digitalRead(IR_CHANNEL_R);

  if(IR_L_REVERSE){
    IR_VALUE_L = !IR_VALUE_L;
  }
  if(IR_R_REVERSE){
    IR_VALUE_R = !IR_VALUE_R;
  }
//  
//  Serial.print("IR_L: ");
//  Serial.print(IR_VALUE_L);
//  Serial.print(", IR_R: ");
//  Serial.println(IR_VALUE_R);
}

void setup() {
  pinMode(IR_CHANNEL_L, INPUT);
  pinMode(IR_CHANNEL_R, INPUT);
  Serial.begin(115200);
}

void loop() {
  readIR();
  delay(100);
}
