#include "ATX2.h"
#include "PS2X_lib.h"

#define MOTOR_L_CHANNEL       1  
#define MOTOR_R_CHANNEL       2 
#define SERVO_HAND_CHANNEL    1
#define SERVO_ARM_CHANNEL     2
#define SERVO_BRACKET_CHANNEL 3
#define PS2_DAT               24    
#define PS2_CMD               25 
#define PS2_CS                26 
#define PS2_CLK               27

#define BASE_SPEED_L          50
#define BASE_SPEED_R          50
#define MIN_SPEED_L           0
#define MIN_SPEED_R           0
#define MAX_SPEED_L           70
#define MAX_SPEED_R           70  
#define MOTOR_L_REVERSE       false
#define MOTOR_R_REVERSE       false
#define SERVO_MIN_VALUE       0
#define SERVO_MAX_VALUE       180
#define SERVO_HAND_OPEN_VALUE       70
#define SERVO_HAND_CLOSE_VALUE      100
#define SERVO_ARM_DOWN_VALUE        30
#define SERVO_ARM_UP_VALUE          110
#define SERVO_BRACKET_OPEN_VALUE    180
#define SERVO_BRACKET_CLOSE_VALUE   90
#define SERVO_HAND_OPEN_STATE       1
#define SERVO_HAND_CLOSE_STATE      2
#define SERVO_ARM_UP_STATE          3
#define SERVO_ARM_DOWN_STATE        4
#define SERVO_BRACKET_OPEN_STATE    5
#define SERVO_BRACKET_CLOSE_STATE   6
#define PSS_LX_MIN            0
#define PSS_LY_MIN            0
#define PSS_RX_MIN            0
#define PSS_RY_MIN            0
#define PSS_LX_MAX            255
#define PSS_LY_MAX            255
#define PSS_RX_MAX            255
#define PSS_RY_MAX            255
#define PSS_LX_MEAN           127
#define PSS_LY_MEAN           127
#define PSS_RX_MEAN           127
#define PSS_RY_MEAN           127
#define PSS_LX_THRESHOLD    20
#define PSS_LY_THRESHOLD    20
#define PSS_RX_THRESHOLD    20
#define PSS_RY_THRESHOLD    20
#define PSS_LX_REVERSE      false
#define PSS_LY_REVERSE      true
#define PSS_RX_REVERSE      false
#define PSS_RY_REVERSE      true

bool SERVO_HAND_STATE = true;
bool SERVO_ARM_STATE = true;
bool SERVO_BRACKET_STATE = true;
int SERVO_HAND_MANUAL = 0;
int SERVO_ARM_MANUAL = 0;

PS2X ps2x;

void driveMotor(int L_VALUE, int R_VALUE){
  if(MOTOR_L_REVERSE){
    L_VALUE *= -1;
  }
  if(MOTOR_R_REVERSE){
    R_VALUE *= -1;
  }
  motor(MOTOR_L_CHANNEL, L_VALUE);
  motor(MOTOR_R_CHANNEL, R_VALUE);
}

void testMotor(){
  driveMotor(MAX_SPEED_L, MIN_SPEED_R);
  delay(500);
  driveMotor(-MAX_SPEED_L, MIN_SPEED_R);
  delay(500);
  driveMotor(MIN_SPEED_L, MAX_SPEED_R);
  delay(500);
  driveMotor(MIN_SPEED_L, -MAX_SPEED_R);
  delay(500);
}

void controlServo(int SERVO_HAND_VALUE, int SERVO_ARM_VALUE, int SERVO_BRACKET_VALUE, int STATE=0){
  if(STATE == 0){
    if(SERVO_HAND_VALUE >= SERVO_MAX_VALUE){
      SERVO_HAND_VALUE = SERVO_MAX_VALUE;
    }
    else if(SERVO_HAND_VALUE <= SERVO_MIN_VALUE){
      SERVO_HAND_VALUE = SERVO_MIN_VALUE;
    }
    if(SERVO_ARM_VALUE >= SERVO_MAX_VALUE){
      SERVO_ARM_VALUE = SERVO_MAX_VALUE;
    }
    else if(SERVO_ARM_VALUE <= SERVO_MIN_VALUE){
      SERVO_ARM_VALUE = SERVO_MIN_VALUE;
    }
    servo(SERVO_HAND_CHANNEL, SERVO_HAND_VALUE);
    servo(SERVO_ARM_CHANNEL, SERVO_ARM_VALUE);
    servo(SERVO_BRACKET_CHANNEL, SERVO_BRACKET_VALUE);
  }
  else if(STATE == SERVO_HAND_OPEN_STATE){
    SERVO_HAND_MANUAL = SERVO_HAND_OPEN_VALUE;
    servo(SERVO_HAND_CHANNEL, SERVO_HAND_OPEN_VALUE);
    driveMotor(MIN_SPEED_L, MIN_SPEED_R);
  }
  else if(STATE == SERVO_HAND_CLOSE_STATE){
    SERVO_HAND_MANUAL = SERVO_HAND_CLOSE_VALUE;
    servo(SERVO_HAND_CHANNEL, SERVO_HAND_CLOSE_VALUE);
    driveMotor(MIN_SPEED_L, MIN_SPEED_R);
  }
  else if(STATE == SERVO_ARM_UP_STATE){
    SERVO_ARM_MANUAL = SERVO_ARM_UP_VALUE;
    servo(SERVO_ARM_CHANNEL, SERVO_ARM_UP_VALUE);
    driveMotor(MIN_SPEED_L, MIN_SPEED_R);
  }
  else if(STATE == SERVO_ARM_DOWN_STATE){
    SERVO_ARM_MANUAL = SERVO_ARM_DOWN_VALUE;
    servo(SERVO_ARM_CHANNEL, SERVO_ARM_DOWN_VALUE);
    driveMotor(MIN_SPEED_L, MIN_SPEED_R);
  }
  else if(STATE == SERVO_BRACKET_OPEN_STATE){
    servo(SERVO_BRACKET_CHANNEL, SERVO_BRACKET_OPEN_VALUE);
    driveMotor(MIN_SPEED_L, MIN_SPEED_R);
  }
  else if(STATE == SERVO_BRACKET_CLOSE_STATE){
    servo(SERVO_BRACKET_CHANNEL, SERVO_BRACKET_CLOSE_VALUE);
    driveMotor(MIN_SPEED_L, MIN_SPEED_R);
  }
}

void testServo(){
  servo(SERVO_HAND_CHANNEL, SERVO_HAND_OPEN_VALUE);
  delay(300);
  servo(SERVO_HAND_CHANNEL, SERVO_HAND_CLOSE_VALUE);
  delay(2000);
  servo(SERVO_ARM_CHANNEL, SERVO_ARM_DOWN_VALUE);
  delay(700);
  servo(SERVO_ARM_CHANNEL, SERVO_ARM_UP_VALUE);
  delay(2000);
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
    driveMotor(MAX_SPEED_L, MAX_SPEED_R);
  } 
  else if(ps2x.Button(PSB_PAD_DOWN)){
//    Serial.println("Down");
    driveMotor(-BASE_SPEED_L, -BASE_SPEED_R);
  } 
  else if(ps2x.Button(PSB_PAD_LEFT)){
//    Serial.println("Left");
    driveMotor(-BASE_SPEED_L, BASE_SPEED_R);
  } 
  else if (ps2x.Button(PSB_PAD_RIGHT)){
//    Serial.println("Right");
    driveMotor(BASE_SPEED_L, -BASE_SPEED_R);
  } 
  else if (ps2x.Button(PSB_L1)){
//    Serial.println("L1");
    SERVO_HAND_MANUAL += 2;
    controlServo(SERVO_HAND_MANUAL, SERVO_ARM_DOWN_VALUE, SERVO_BRACKET_CLOSE_VALUE);
  }
  else if (ps2x.Button(PSB_L2)){
//    Serial.println("L2");
    SERVO_ARM_MANUAL += 2;
    controlServo(SERVO_HAND_MANUAL, SERVO_ARM_MANUAL, SERVO_BRACKET_CLOSE_VALUE);
  }
  else if (ps2x.Button(PSB_R1)){
//    Serial.println("R1");
    SERVO_HAND_MANUAL -= 2;
    controlServo(SERVO_HAND_MANUAL, SERVO_ARM_DOWN_VALUE, SERVO_BRACKET_CLOSE_VALUE);
  }
  else if (ps2x.Button(PSB_R2)){
//    Serial.println("R2");  
    SERVO_ARM_MANUAL -= 2;
    controlServo(SERVO_HAND_MANUAL, SERVO_ARM_MANUAL, SERVO_BRACKET_CLOSE_VALUE);
  }
  else if (ps2x.Button(PSB_CROSS)){
//    Serial.println("Square");
    if(SERVO_BRACKET_STATE){
      controlServo(0, 0, 0, SERVO_BRACKET_OPEN_STATE);
      SERVO_BRACKET_STATE = !SERVO_BRACKET_STATE;
    }
    else{
      controlServo(0, 0, 0, SERVO_BRACKET_CLOSE_STATE);
      SERVO_BRACKET_STATE = !SERVO_BRACKET_STATE;
    }
  } 
  else if (ps2x.Button(PSB_CIRCLE)){
//    Serial.println("Circle");
    if(SERVO_HAND_STATE){
      controlServo(0, 0, 0, SERVO_HAND_OPEN_STATE);
      SERVO_HAND_STATE = !SERVO_HAND_STATE;
    }
    else{
      controlServo(0, 0, 0, SERVO_HAND_CLOSE_STATE);
      SERVO_HAND_STATE = !SERVO_HAND_STATE;
    }
  }  
  else if (ps2x.ButtonPressed(PSB_SQUARE)){
//    Serial.println("Square");
    if(SERVO_ARM_STATE){
      controlServo(0, 0, 0, SERVO_ARM_UP_STATE);
      SERVO_ARM_STATE = !SERVO_ARM_STATE;
    }
    else{
      controlServo(0, 0, 0, SERVO_ARM_DOWN_STATE);
      SERVO_ARM_STATE = !SERVO_ARM_STATE;
    }
  } 
  else if (ps2x.Button(PSB_TRIANGLE)){
//    Serial.println("Triangle");
    driveMotor(MAX_SPEED_L, MAX_SPEED_R);
  } 
  else if (ps2x.ButtonPressed(PSB_START)){
    Serial.println("Start");
  }
  else if(ps2x.ButtonPressed(PSB_SELECT)){
    Serial.println("Select");
  }
  else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
//    Serial.println("Stop");
    driveMotor(MIN_SPEED_L, MIN_SPEED_R);
  }
  else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
//    Serial.println("North");
    driveMotor(BASE_SPEED_L, BASE_SPEED_R);
  }
  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
//    Serial.println("North East");
    driveMotor(BASE_SPEED_L, MIN_SPEED_R);
  }
  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
//    Serial.println("East");
    driveMotor(BASE_SPEED_L, -BASE_SPEED_R);
  }
  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
//    Serial.println("South East");
    driveMotor(-BASE_SPEED_L, MIN_SPEED_R);
  }
    else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
//    Serial.println("South");
    driveMotor(-BASE_SPEED_L, -BASE_SPEED_R);
  }
  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
//    Serial.println("South West");
    driveMotor(MIN_SPEED_L, -BASE_SPEED_R);
  }
  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
//    Serial.println("West");
    driveMotor(-BASE_SPEED_L, BASE_SPEED_R);
  }
  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
//    Serial.println("North West");
    driveMotor(MIN_SPEED_L, BASE_SPEED_R);
  }
}

void setup() {
  Serial.begin(115200);
  connectPS2();
}

void loop() {
//  testMotor();
//  testServo();
  readPS2X();
  delay(100);
  
}
