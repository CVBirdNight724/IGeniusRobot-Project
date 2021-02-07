#include "ATX2.h"
#include "PS2X_lib.h"

#define MOTOR_FL_CHANNEL      1  
#define MOTOR_BL_CHANNEL      2
#define MOTOR_FR_CHANNEL      3
#define MOTOR_BR_CHANNEL      4
#define SERVO_HAND_CHANNEL     1
#define SERVO_BRACKET_CHANNEL 2
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
#define MOTOR_FL_REVERSE      false
#define MOTOR_FR_REVERSE      true
#define MOTOR_BL_REVERSE      false
#define MOTOR_BR_REVERSE      true
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
#define DRIVE_Y_MIN     0
#define DRIVE_X_MAX     100
#define DRIVE_Y_MAX     100
#define DRIVE_ROTATE_SPEED    30
#define SERVO_MIN_VALUE       0
#define SERVO_MAX_VALUE       180
#define SERVO_HAND_OPEN_VALUE       120
#define SERVO_HAND_CLOSE_VALUE      0
#define SERVO_BRACKET_UP_VALUE      70
#define SERVO_BRACKET_DOWN_VALUE    95
#define SERVO_HAND_OPEN_STATE       1
#define SERVO_HAND_CLOSE_STATE      2
#define SERVO_BRACKET_OPEN_STATE    3
#define SERVO_BRACKET_CLOSE_STATE   4
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
#define MECANUM_CW      1
#define MECANUM_CCW     -1

bool SERVO_HAND_STATE = true;
bool SERVO_ARM_STATE = true;
bool SERVO_BRACKET_STATE = true;
int SERVO_HAND_MANUAL = 0;
int SERVO_ARM_MANUAL = 0;

PS2X ps2x;

void driveMotor(int FL_VALUE, int BL_VALUE, int FR_VALUE, int BR_VALUE){
  if(MOTOR_FL_REVERSE){
    FL_VALUE *= -1;
  }
  if(MOTOR_BL_REVERSE){
    BL_VALUE *= -1;
  }
  if(MOTOR_FR_REVERSE){
    FR_VALUE *= -1;
  }
  if(MOTOR_BR_REVERSE){
    BR_VALUE *= -1;
  }
  motor(MOTOR_FL_CHANNEL, FL_VALUE);
  motor(MOTOR_BL_CHANNEL, BL_VALUE);
  motor(MOTOR_FR_CHANNEL, FR_VALUE);
  motor(MOTOR_BR_CHANNEL, BR_VALUE);
}   

void testMotor(){
  driveMotor(100, 0, 0, 0);
  delay(500);
  driveMotor(-100, 0, 0, 0);
  delay(500);
  driveMotor(0, 100, 0, 0);
  delay(500);
  driveMotor(0, -100, 0, 0);
  delay(500);
  driveMotor(0, 0, 100, 0);
  delay(500);
  driveMotor(0, 0, -100, 0);
  delay(500);
  driveMotor(0, 0, 0, 100);
  delay(500);
  driveMotor(0, 0, 0, -100);
  delay(500);
  
}

void driveMecanum(int X, int Y, int CASE=0){
  if(CASE == MECANUM_CW){         // Clockwise (CW)
    Serial.println("Mecanum CW");
    driveMotor(DRIVE_ROTATE_SPEED, DRIVE_ROTATE_SPEED, -DRIVE_ROTATE_SPEED, -DRIVE_ROTATE_SPEED);
  }
  else if(CASE == MECANUM_CCW){    // Counterclockwise (CCW)
    Serial.println("Mecanum CCW");
    driveMotor(-DRIVE_ROTATE_SPEED, -DRIVE_ROTATE_SPEED, DRIVE_ROTATE_SPEED, DRIVE_ROTATE_SPEED);
  }
  else if(abs(X) <= DRIVE_X_THRESHOLD && abs(Y) <= DRIVE_Y_THRESHOLD){ // Stop
    Serial.println("Mecanum Stop");
    driveMotor(FL_MIN, FR_MIN, BL_MIN, BR_MIN);
  }
  else if(abs(X) <= DRIVE_X_THRESHOLD && Y > DRIVE_Y_THRESHOLD){  // North
    Serial.println("Mecanum North");
    driveMotor(FL_MAX, FR_MAX, BL_MAX, BR_MAX);
  }
  else if(X > DRIVE_X_THRESHOLD && Y > DRIVE_Y_THRESHOLD){        // North East
    Serial.println("Mecanum North East");
    driveMotor(FL_MAX, FR_MIN, BL_MIN, BR_MAX);
  }
  else if(X > DRIVE_X_THRESHOLD && abs(Y) <= DRIVE_Y_THRESHOLD){  // East
    Serial.println("Mecanum East");
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

void controlServo(int SERVO_HAND_VALUE, int SERVO_BRACKET_VALUE, int STATE=0){
  if(STATE == 0){
    if(SERVO_HAND_VALUE >= SERVO_MAX_VALUE){
      SERVO_HAND_VALUE = SERVO_MAX_VALUE;
    }
    else if(SERVO_HAND_VALUE <= SERVO_MIN_VALUE){
      SERVO_HAND_VALUE = SERVO_MIN_VALUE;
    }
    servo(SERVO_HAND_CHANNEL, SERVO_HAND_VALUE);
    servo(SERVO_BRACKET_CHANNEL, SERVO_BRACKET_VALUE);
  }
  else if(STATE == SERVO_HAND_OPEN_STATE){
    SERVO_HAND_MANUAL = SERVO_HAND_OPEN_VALUE;
    servo(SERVO_HAND_CHANNEL, SERVO_HAND_OPEN_VALUE);
//    driveMotor(MIN_SPEED_L, MIN_SPEED_R);
  }
  else if(STATE == SERVO_HAND_CLOSE_STATE){
    SERVO_HAND_MANUAL = SERVO_HAND_CLOSE_VALUE;
    servo(SERVO_HAND_CHANNEL, SERVO_HAND_CLOSE_VALUE);
//    driveMotor(MIN_SPEED_L, MIN_SPEED_R);
  }
  else if(STATE == SERVO_BRACKET_OPEN_STATE){
    servo(SERVO_BRACKET_CHANNEL, SERVO_BRACKET_UP_VALUE);
  }
  else if(STATE == SERVO_BRACKET_CLOSE_STATE){
    servo(SERVO_BRACKET_CHANNEL, SERVO_BRACKET_DOWN_VALUE);
  }
}
//
void testServo(){
//  servo(SERVO_HAND_CHANNEL, SERVO_HAND_OPEN_VALUE);
//  delay(1000);
//  servo(SERVO_HAND_CHANNEL, SERVO_HAND_CLOSE_VALUE);
//  delay(2000);
  servo(SERVO_BRACKET_CHANNEL, SERVO_BRACKET_UP_VALUE);
  delay(1000);
  servo(SERVO_BRACKET_CHANNEL, SERVO_BRACKET_DOWN_VALUE);
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
    driveMecanum(DRIVE_X_MIN, DRIVE_Y_MIN, MECANUM_CCW);
  }
  else if (ps2x.Button(PSB_L2)){
     Serial.println("L2");
  }
  else if (ps2x.Button(PSB_R1)){
    Serial.println("R1");
    driveMecanum(DRIVE_X_MIN, DRIVE_Y_MIN, MECANUM_CW);
  }
  else if (ps2x.Button(PSB_R2)){
    Serial.println("R2");  
  }
  else if (ps2x.Button(PSB_CROSS)){
//    Serial.println("Cross");
    if(SERVO_BRACKET_STATE){
      controlServo(0, 0, SERVO_BRACKET_OPEN_STATE);
      SERVO_BRACKET_STATE = !SERVO_BRACKET_STATE;
    }
    else{
      controlServo(0, 0, SERVO_BRACKET_CLOSE_STATE);
      SERVO_BRACKET_STATE = !SERVO_BRACKET_STATE;
    }
    delay(100);
  } 
  else if (ps2x.Button(PSB_CIRCLE)){
//    Serial.println("Circle");
    if(SERVO_HAND_STATE){
      controlServo(0, 0, SERVO_HAND_OPEN_STATE);
      SERVO_HAND_STATE = !SERVO_HAND_STATE;
    }
    else{
      controlServo(0, 0, SERVO_HAND_CLOSE_STATE);
      SERVO_HAND_STATE = !SERVO_HAND_STATE;
    }
    delay(100);
  }  
  else if (ps2x.ButtonPressed(PSB_SQUARE)){
//    Serial.println("Square");
//    driveMecanum(DRIVE_X_MIN, DRIVE_Y_MIN, MECANUM_CW);
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
  else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
    Serial.println("Stop");
    driveMecanum(DRIVE_X_MIN, DRIVE_Y_MIN );  
  }
  else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
    Serial.println("North");
    driveMecanum(DRIVE_X_MIN, DRIVE_Y_MAX);
  }
  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
    Serial.println("North East");
    driveMecanum(DRIVE_X_MAX, DRIVE_Y_MAX); 
  }
  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
    Serial.println("East");
    driveMecanum(DRIVE_X_MAX, DRIVE_Y_MIN);
  }
  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
    Serial.println("South East");
    driveMecanum(DRIVE_X_MAX, -DRIVE_Y_MAX);
  }
  else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
    Serial.println("South");
    driveMecanum(DRIVE_X_MIN, -DRIVE_Y_MAX);
  }
  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
    Serial.println("South West");
    driveMecanum(-DRIVE_X_MAX, -DRIVE_Y_MAX);
  }
  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
    Serial.println("West");
    driveMecanum(-DRIVE_X_MAX, DRIVE_Y_MIN);
  }
  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
    Serial.println("North West");
    driveMecanum(-DRIVE_X_MAX, DRIVE_Y_MAX);
  }
}
//
//void readPS2X(){
//  ps2x.read_gamepad(false, false);
////  Serial.print("LX : ");
////  Serial.print(ps2x.Analog(PSS_LX));
////  Serial.print(", LY: ");
////  Serial.print(ps2x.Analog(PSS_LY));
////  Serial.print(", RX : ");
////  Serial.print(ps2x.Analog(PSS_RX));
////  Serial.print(", RY: ");
////  Serial.println(ps2x.Analog(PSS_RY));
//  int PSS_LX_VALUE = ps2x.Analog(PSS_LX);
//  int PSS_LY_VALUE = ps2x.Analog(PSS_LY);
//  int PSS_RX_VALUE = ps2x.Analog(PSS_RX);
//  int PSS_RY_VALUE = ps2x.Analog(PSS_RY);
//  
//  if(PSS_LX_REVERSE){
//    PSS_LX_VALUE = PSS_LX_MAX - PSS_LX_VALUE;
//  }
//  if(PSS_LY_REVERSE){
//    PSS_LY_VALUE = PSS_LY_MAX - PSS_LY_VALUE;
//  }
//  if(PSS_RX_REVERSE){
//    PSS_RX_VALUE = PSS_RX_MAX - PSS_RX_VALUE;
//  }
//  if(PSS_RY_REVERSE){
//    PSS_RY_VALUE = PSS_RY_MAX - PSS_RY_VALUE;
//  }
//  if(ps2x.Button(PSB_PAD_UP)){
////    Serial.println("Up");
//    driveMotor(MAX_SPEED_L, MAX_SPEED_R);
//  } 
//  else if(ps2x.Button(PSB_PAD_DOWN)){
////    Serial.println("Down");
//    driveMotor(-BASE_SPEED_L, -BASE_SPEED_R);
//  } 
//  else if(ps2x.Button(PSB_PAD_LEFT)){
////    Serial.println("Left");
//    driveMotor(-BASE_SPEED_L, BASE_SPEED_R);
//  } 
//  else if (ps2x.Button(PSB_PAD_RIGHT)){
////    Serial.println("Right");
//    driveMotor(BASE_SPEED_L, -BASE_SPEED_R);
//  } 
//  else if (ps2x.Button(PSB_L1)){
////    Serial.println("L1");
//    SERVO_HAND_MANUAL += 2;
//    controlServo(SERVO_HAND_MANUAL, SERVO_ARM_DOWN_VALUE, SERVO_BRACKET_CLOSE_VALUE);
//  }
//  else if (ps2x.Button(PSB_L2)){
////    Serial.println("L2");
//    SERVO_ARM_MANUAL += 2;
//    controlServo(SERVO_HAND_MANUAL, SERVO_ARM_MANUAL, SERVO_BRACKET_CLOSE_VALUE);
//  }
//  else if (ps2x.Button(PSB_R1)){
////    Serial.println("R1");
//    SERVO_HAND_MANUAL -= 2;
//    controlServo(SERVO_HAND_MANUAL, SERVO_ARM_DOWN_VALUE, SERVO_BRACKET_CLOSE_VALUE);
//  }
//  else if (ps2x.Button(PSB_R2)){
////    Serial.println("R2");  
//    SERVO_ARM_MANUAL -= 2;
//    controlServo(SERVO_HAND_MANUAL, SERVO_ARM_MANUAL, SERVO_BRACKET_CLOSE_VALUE);
//  }
//  else if (ps2x.Button(PSB_CROSS)){
////    Serial.println("Square");
//    if(SERVO_BRACKET_STATE){
//      controlServo(0, 0, 0, SERVO_BRACKET_OPEN_STATE);
//      SERVO_BRACKET_STATE = !SERVO_BRACKET_STATE;
//    }
//    else{
//      controlServo(0, 0, 0, SERVO_BRACKET_CLOSE_STATE);
//      SERVO_BRACKET_STATE = !SERVO_BRACKET_STATE;
//    }
//  } 
//  else if (ps2x.Button(PSB_CIRCLE)){
////    Serial.println("Circle");
//    if(SERVO_HAND_STATE){
//      controlServo(0, 0, 0, SERVO_HAND_OPEN_STATE);
//      SERVO_HAND_STATE = !SERVO_HAND_STATE;
//    }
//    else{
//      controlServo(0, 0, 0, SERVO_HAND_CLOSE_STATE);
//      SERVO_HAND_STATE = !SERVO_HAND_STATE;
//    }
//  }  
//  else if (ps2x.ButtonPressed(PSB_SQUARE)){
////    Serial.println("Square");
//    if(SERVO_ARM_STATE){
//      controlServo(0, 0, 0, SERVO_ARM_UP_STATE);
//      SERVO_ARM_STATE = !SERVO_ARM_STATE;
//    }
//    else{
//      controlServo(0, 0, 0, SERVO_ARM_DOWN_STATE);
//      SERVO_ARM_STATE = !SERVO_ARM_STATE;
//    }
//  } 
//  else if (ps2x.Button(PSB_TRIANGLE)){
////    Serial.println("Triangle");
//    driveMotor(MAX_SPEED_L, MAX_SPEED_R);
//  } 
//  else if (ps2x.ButtonPressed(PSB_START)){
//    Serial.println("Start");
//  }
//  else if(ps2x.ButtonPressed(PSB_SELECT)){
//    Serial.println("Select");
//  }
//  else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
////    Serial.println("Stop");
//    driveMotor(MIN_SPEED_L, MIN_SPEED_R);
//  }
//  else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
////    Serial.println("North");
//    driveMotor(BASE_SPEED_L, BASE_SPEED_R);
//  }
//  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
////    Serial.println("North East");
//    driveMotor(BASE_SPEED_L, MIN_SPEED_R);
//  }
//  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
////    Serial.println("East");
//    driveMotor(BASE_SPEED_L, -BASE_SPEED_R);
//  }
//  else if(PSS_LX_VALUE > PSS_LX_MEAN+PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
////    Serial.println("South East");
//    driveMotor(-BASE_SPEED_L, MIN_SPEED_R);
//  }
//    else if(abs(PSS_LX_VALUE-PSS_LX_MEAN) < PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
////    Serial.println("South");
//    driveMotor(-BASE_SPEED_L, -BASE_SPEED_R);
//  }
//  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && PSS_LY_VALUE < PSS_LY_MEAN-PSS_LY_THRESHOLD){
////    Serial.println("South West");
//    driveMotor(MIN_SPEED_L, -BASE_SPEED_R);
//  }
//  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && abs(PSS_LY_VALUE-PSS_LY_MEAN) < PSS_LY_THRESHOLD){
////    Serial.println("West");
//    driveMotor(-BASE_SPEED_L, BASE_SPEED_R);
//  }
//  else if(PSS_LX_VALUE < PSS_LX_MEAN-PSS_LX_THRESHOLD && PSS_LY_VALUE > PSS_LY_MEAN+PSS_LY_THRESHOLD){
////    Serial.println("North West");
//    driveMotor(MIN_SPEED_L, BASE_SPEED_R);
//  }
//}

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
