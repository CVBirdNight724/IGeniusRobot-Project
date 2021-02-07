#define MOTOR_L_A_CHANNEL     2
#define MOTOR_L_B_CHANNEL     3
#define MOTOR_L_PWM_CHANNEL   5
#define MOTOR_R_A_CHANNEL     4
#define MOTOR_R_B_CHANNEL     7
#define MOTOR_R_PWM_CHANNEL   6

#define MOTOR_L_INVERSE   true 
#define MOTOR_R_INVERSE   false
#define MOTOR_L_MIN_SPEED   0
#define MOTOR_R_MIN_SPEED   0
#define MOTOR_L_BASE_SPEED  45
#define MOTOR_R_BASE_SPEED  45
#define MOTOR_ROTATE_SPEED  50
#define MOTOR_L_MAX_SPEED   100
#define MOTOR_R_MAX_SPEED   100
#define MOTOR_PWM_MIN     0
#define MOTOR_PWM_MAX     100
#define MOTOR_ANALOG_MIN  0
#define MOTOR_ANALOG_MAX  255
#define REMOTE_CHANNEL_1_MIN    1050
#define REMOTE_CHANNEL_2_MIN    1050
#define REMOTE_CHANNEL_3_MIN    1050
#define REMOTE_CHANNEL_1_BASE   1500
#define REMOTE_CHANNEL_2_BASE   1500
#define REMOTE_CHANNEL_3_BASE   1500
#define REMOTE_CHANNEL_1_MAX    1950
#define REMOTE_CHANNEL_2_MAX    1950
#define REMOTE_CHANNEL_3_MAX    1950
#define REMOTE_CHANNEL_1_THRESHOLD  50
#define REMOTE_CHANNEL_2_THRESHOLD  50
#define REMOTE_CHANNEL_3_THRESHOLD  50
#define REMOTE_CHANNEL_1_INVERSE  true
#define REMOTE_CHANNEL_2_INVERSE  true
#define REMOTE_CHANNEL_3_INVERSE  false

int REMOTE_CHANNEL_1_VALUE;
int REMOTE_CHANNEL_2_VALUE;
int REMOTE_CHANNEL_3_VALUE;
int timer_1, timer_2, timer_3;
bool last_channel_1, last_channel_2, last_channel_3;

ISR(PCINT0_vect){
  if(last_channel_1 == false && (PINB & B00000001)){
    last_channel_1 = !last_channel_1;
    timer_1 = micros();
  }
  else if(last_channel_1 == true && !(PINB & B00000001)){
    last_channel_1 = !last_channel_1;
    REMOTE_CHANNEL_1_VALUE = micros() - timer_1;
  }  
  if(last_channel_2 == false && (PINB & B00000010)){
    last_channel_2 = !last_channel_2;
    timer_2 = micros();
  }
  else if(last_channel_2 == true && !(PINB & B00000010)){
    last_channel_2 = !last_channel_2;
    REMOTE_CHANNEL_2_VALUE = micros() - timer_2;
  }  
  if(last_channel_3 == false && (PINB & B00000100)){
    last_channel_3 = !last_channel_3;
    timer_3 = micros();
  }
  else if(last_channel_3 == true && !(PINB & B00000100)){
    last_channel_3 = !last_channel_3;
    REMOTE_CHANNEL_3_VALUE = micros() - timer_3;
  } 
}

void readRemoteRC(){
  Serial.print("CH1: ");
  Serial.print(REMOTE_CHANNEL_1_VALUE);
  Serial.print(" CH2: ");
  Serial.print(REMOTE_CHANNEL_2_VALUE);
  Serial.print(" CH3: ");
  Serial.println(REMOTE_CHANNEL_3_VALUE);
  delay(100);
}

void driveMotor(int PWM_L_VALUE, int PWM_R_VALUE){
  PWM_L_VALUE = map(PWM_L_VALUE, MOTOR_PWM_MIN, MOTOR_PWM_MAX, MOTOR_ANALOG_MIN, MOTOR_ANALOG_MAX);
  PWM_R_VALUE = map(PWM_R_VALUE, MOTOR_PWM_MIN, MOTOR_PWM_MAX, MOTOR_ANALOG_MIN, MOTOR_ANALOG_MAX);
  if(MOTOR_L_INVERSE){
    PWM_L_VALUE = -PWM_L_VALUE;
  }
  if(MOTOR_R_INVERSE){
    PWM_R_VALUE = -PWM_R_VALUE;
  }
  if(PWM_L_VALUE >= MOTOR_PWM_MAX){
    digitalWrite(MOTOR_L_A_CHANNEL, HIGH);
    digitalWrite(MOTOR_L_B_CHANNEL, LOW);
    digitalWrite(MOTOR_L_PWM_CHANNEL, HIGH);
  }
  else if(PWM_L_VALUE > MOTOR_PWM_MIN){
    digitalWrite(MOTOR_L_A_CHANNEL, HIGH);
    digitalWrite(MOTOR_L_B_CHANNEL, LOW);
    analogWrite(MOTOR_L_PWM_CHANNEL, PWM_L_VALUE);
  }
  else if(PWM_L_VALUE < -MOTOR_PWM_MIN){
    digitalWrite(MOTOR_L_A_CHANNEL, LOW);
    digitalWrite(MOTOR_L_B_CHANNEL, HIGH);
    analogWrite(MOTOR_L_PWM_CHANNEL, abs(PWM_L_VALUE));
  }
  else if(PWM_L_VALUE <= -MOTOR_PWM_MAX){
    digitalWrite(MOTOR_L_A_CHANNEL, LOW);
    digitalWrite(MOTOR_L_B_CHANNEL, HIGH);
    digitalWrite(MOTOR_L_PWM_CHANNEL, HIGH);
  }
  else if(abs(PWM_L_VALUE) <= MOTOR_PWM_MIN){
    digitalWrite(MOTOR_L_A_CHANNEL, HIGH);
    digitalWrite(MOTOR_L_B_CHANNEL, HIGH);
    digitalWrite(MOTOR_L_PWM_CHANNEL, LOW); 
  }
  if(PWM_R_VALUE >= MOTOR_PWM_MAX){
    digitalWrite(MOTOR_R_A_CHANNEL, HIGH);
    digitalWrite(MOTOR_R_B_CHANNEL, LOW);
    digitalWrite(MOTOR_R_PWM_CHANNEL, HIGH);
  }
  else if(PWM_R_VALUE > MOTOR_PWM_MIN){
    digitalWrite(MOTOR_R_A_CHANNEL, HIGH);
    digitalWrite(MOTOR_R_B_CHANNEL, LOW);
    analogWrite(MOTOR_R_PWM_CHANNEL, PWM_R_VALUE);
  }
  else if(PWM_R_VALUE < -MOTOR_PWM_MIN){
    digitalWrite(MOTOR_R_A_CHANNEL, LOW);
    digitalWrite(MOTOR_R_B_CHANNEL, HIGH);
    analogWrite(MOTOR_R_PWM_CHANNEL, abs(PWM_R_VALUE));
  }
  else if(PWM_R_VALUE <= -MOTOR_PWM_MAX){
    digitalWrite(MOTOR_R_A_CHANNEL, LOW);
    digitalWrite(MOTOR_R_B_CHANNEL, HIGH);
    digitalWrite(MOTOR_R_PWM_CHANNEL, HIGH);
  }
  else if(abs(PWM_R_VALUE) <= MOTOR_PWM_MIN){
    digitalWrite(MOTOR_R_A_CHANNEL, HIGH);
    digitalWrite(MOTOR_R_B_CHANNEL, HIGH);
    digitalWrite(MOTOR_R_PWM_CHANNEL, LOW); 
  }
}

void testMotor(){
  driveMotor(MOTOR_PWM_MAX, MOTOR_PWM_MIN);
  delay(1000);
  driveMotor(-MOTOR_PWM_MAX, MOTOR_PWM_MIN);
  delay(1000);
  driveMotor(MOTOR_PWM_MIN, MOTOR_PWM_MAX);
  delay(1000);
  driveMotor(MOTOR_PWM_MIN, -MOTOR_PWM_MAX);
  delay(1000);
}

void controlSumo(){
  if(REMOTE_CHANNEL_1_INVERSE){
    REMOTE_CHANNEL_1_VALUE = REMOTE_CHANNEL_1_MAX + REMOTE_CHANNEL_1_MIN - REMOTE_CHANNEL_1_VALUE;
  }
  if(REMOTE_CHANNEL_2_INVERSE){
    REMOTE_CHANNEL_2_VALUE = REMOTE_CHANNEL_2_MAX + REMOTE_CHANNEL_2_MIN - REMOTE_CHANNEL_2_VALUE;
  }
  if(REMOTE_CHANNEL_3_INVERSE){
    REMOTE_CHANNEL_3_VALUE = REMOTE_CHANNEL_3_MAX + REMOTE_CHANNEL_3_MIN - REMOTE_CHANNEL_3_VALUE;
  }
  if(abs(REMOTE_CHANNEL_1_VALUE - REMOTE_CHANNEL_1_BASE) < REMOTE_CHANNEL_1_THRESHOLD && abs(REMOTE_CHANNEL_2_VALUE - REMOTE_CHANNEL_2_BASE) < REMOTE_CHANNEL_2_THRESHOLD){
    Serial.println("Remote RC: Stop");
    driveMotor(MOTOR_L_MIN_SPEED, MOTOR_R_MIN_SPEED);
  }
  else if(abs(REMOTE_CHANNEL_1_VALUE - REMOTE_CHANNEL_1_BASE) < REMOTE_CHANNEL_1_THRESHOLD && REMOTE_CHANNEL_2_VALUE - REMOTE_CHANNEL_2_BASE > REMOTE_CHANNEL_2_THRESHOLD){
    if(REMOTE_CHANNEL_3_VALUE >= REMOTE_CHANNEL_3_BASE){
      int SPEED = map(REMOTE_CHANNEL_2_VALUE, REMOTE_CHANNEL_2_BASE, REMOTE_CHANNEL_2_MAX, MOTOR_L_MIN_SPEED, MOTOR_L_MAX_SPEED);
      driveMotor(SPEED, SPEED); 
    }
    else{
      int SPEED = map(REMOTE_CHANNEL_2_VALUE, REMOTE_CHANNEL_2_BASE, REMOTE_CHANNEL_2_MAX, MOTOR_L_MIN_SPEED, MOTOR_L_BASE_SPEED);
      driveMotor(SPEED, SPEED); 
    }
    Serial.println("Remote RC: Forward");
//    Serial.println(SPEED);
  }
  else if(REMOTE_CHANNEL_1_VALUE - REMOTE_CHANNEL_1_BASE > REMOTE_CHANNEL_1_THRESHOLD && REMOTE_CHANNEL_2_VALUE - REMOTE_CHANNEL_2_BASE > REMOTE_CHANNEL_2_THRESHOLD){
    Serial.println("Remote RC: Forward Turn Right");
    int SPEED_L = map(REMOTE_CHANNEL_2_VALUE, REMOTE_CHANNEL_2_BASE, REMOTE_CHANNEL_2_MAX, MOTOR_L_MIN_SPEED, MOTOR_L_BASE_SPEED);
    int SPEED_R = map(REMOTE_CHANNEL_1_VALUE, REMOTE_CHANNEL_1_BASE, REMOTE_CHANNEL_1_MAX, MOTOR_R_BASE_SPEED, MOTOR_R_MIN_SPEED);
    driveMotor(SPEED_L, SPEED_R);
    Serial.println(SPEED_L);
    Serial.println(SPEED_R);
  }
  else if(REMOTE_CHANNEL_1_VALUE - REMOTE_CHANNEL_1_BASE > REMOTE_CHANNEL_1_THRESHOLD && abs(REMOTE_CHANNEL_2_VALUE - REMOTE_CHANNEL_2_BASE) < REMOTE_CHANNEL_2_THRESHOLD){

//    int SPEED = map(REMOTE_CHANNEL_1_VALUE, REMOTE_CHANNEL_1_MIN, REMOTE_CHANNEL_1_BASE, MOTOR_R_BASE_SPEED, MOTOR_R_MIN_SPEED);
//    driveMotor(-SPEED, SPEED);
    Serial.println("Remote RC: Rotate CCW");
//    Serial.println(SPEED);
//    Serial.println("Remote RC: Rotate CW");
    int SPEED = map(REMOTE_CHANNEL_1_VALUE, REMOTE_CHANNEL_1_BASE, REMOTE_CHANNEL_1_MAX, MOTOR_R_MIN_SPEED, MOTOR_ROTATE_SPEED);
    driveMotor(-SPEED, SPEED);
    Serial.println(SPEED);
  }
  else if(REMOTE_CHANNEL_1_VALUE - REMOTE_CHANNEL_1_BASE > REMOTE_CHANNEL_1_THRESHOLD && REMOTE_CHANNEL_2_VALUE - REMOTE_CHANNEL_2_BASE < -REMOTE_CHANNEL_2_THRESHOLD){
    int SPEED_L = map(REMOTE_CHANNEL_1_VALUE, REMOTE_CHANNEL_1_BASE, REMOTE_CHANNEL_1_MAX, -MOTOR_L_BASE_SPEED, MOTOR_L_MIN_SPEED);
    int SPEED_R = map(REMOTE_CHANNEL_2_VALUE, REMOTE_CHANNEL_2_MIN, REMOTE_CHANNEL_2_BASE, -MOTOR_R_BASE_SPEED, MOTOR_R_MIN_SPEED);
    driveMotor(SPEED_L, SPEED_R);
    Serial.println("Remote RC: Backward Turn Left");
    Serial.println(SPEED_L);
    Serial.println(SPEED_R);
  }
  else if(abs(REMOTE_CHANNEL_1_VALUE - REMOTE_CHANNEL_1_BASE) < REMOTE_CHANNEL_1_THRESHOLD && REMOTE_CHANNEL_2_VALUE - REMOTE_CHANNEL_2_BASE < -REMOTE_CHANNEL_2_THRESHOLD){
    int SPEED = map(REMOTE_CHANNEL_2_VALUE, REMOTE_CHANNEL_2_MIN, REMOTE_CHANNEL_2_BASE, -MOTOR_ROTATE_SPEED, MOTOR_R_MIN_SPEED);
    driveMotor(SPEED, SPEED);
    Serial.println("Remote RC: Backward");
    Serial.println(SPEED);
  }
  else if(REMOTE_CHANNEL_1_VALUE - REMOTE_CHANNEL_1_BASE < -REMOTE_CHANNEL_1_THRESHOLD && REMOTE_CHANNEL_2_VALUE - REMOTE_CHANNEL_2_BASE < -REMOTE_CHANNEL_2_THRESHOLD){
    int SPEED_L = map(REMOTE_CHANNEL_2_VALUE, REMOTE_CHANNEL_2_MIN, REMOTE_CHANNEL_2_BASE, -MOTOR_L_BASE_SPEED, MOTOR_L_MIN_SPEED);
    int SPEED_R = map(REMOTE_CHANNEL_1_VALUE, REMOTE_CHANNEL_1_MIN, REMOTE_CHANNEL_1_BASE, MOTOR_R_MIN_SPEED,  -MOTOR_R_BASE_SPEED);
    driveMotor(SPEED_L, SPEED_R);
    Serial.println("Remote RC: Backward Turn Right");
    Serial.println(SPEED_L);
    Serial.println(SPEED_R);
  }
  else if(REMOTE_CHANNEL_1_VALUE - REMOTE_CHANNEL_1_BASE < -REMOTE_CHANNEL_1_THRESHOLD && abs(REMOTE_CHANNEL_2_VALUE - REMOTE_CHANNEL_2_BASE) < REMOTE_CHANNEL_2_THRESHOLD){
    Serial.println("Remote RC: Rotate CW");
//    int SPEED = map(REMOTE_CHANNEL_1_VALUE, REMOTE_CHANNEL_1_BASE, REMOTE_CHANNEL_1_MAX, MOTOR_R_MIN_SPEED, MOTOR_ROTATE_SPEED);
//    driveMotor(SPEED, -SPEED);
//    Serial.println(SPEED);
    int SPEED = map(REMOTE_CHANNEL_1_VALUE, REMOTE_CHANNEL_1_BASE, REMOTE_CHANNEL_1_MAX, MOTOR_R_MIN_SPEED, MOTOR_ROTATE_SPEED);
    driveMotor(-SPEED, SPEED);
    Serial.println(SPEED);
  }
  else if(REMOTE_CHANNEL_1_VALUE - REMOTE_CHANNEL_1_BASE < -REMOTE_CHANNEL_1_THRESHOLD && REMOTE_CHANNEL_2_VALUE - REMOTE_CHANNEL_2_BASE > REMOTE_CHANNEL_2_THRESHOLD){
    
    int SPEED_R = map(REMOTE_CHANNEL_2_VALUE, REMOTE_CHANNEL_2_BASE, REMOTE_CHANNEL_2_MAX, MOTOR_L_MIN_SPEED, MOTOR_L_BASE_SPEED);
    int SPEED_L = map(REMOTE_CHANNEL_1_VALUE, REMOTE_CHANNEL_1_MIN, REMOTE_CHANNEL_1_BASE, MOTOR_R_MIN_SPEED, MOTOR_R_BASE_SPEED);
    driveMotor(SPEED_L, SPEED_R);
    Serial.println("Remote RC: Forward Turn Left");
    Serial.println(SPEED_L);
    Serial.println(SPEED_R);
  }
}


void setup() {
  PCICR |= (1 << PCIE0);          // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0);        // set PCINT0 (pin 8) to trigger interrupt
  PCMSK0 |= (1 << PCINT1);        // set PCINT1 (pin 9) to trigger interrupt
  PCMSK0 |= (1 << PCINT2);        // set PCINT2 (pin 10) to trigger interrupt
  PCMSK0 |= (1 << PCINT3);        // set PCINT3 (pin 11) to trigger interrupt
  pinMode(MOTOR_L_A_CHANNEL, OUTPUT);
  pinMode(MOTOR_L_B_CHANNEL, OUTPUT);
  pinMode(MOTOR_L_PWM_CHANNEL, OUTPUT);
  pinMode(MOTOR_R_A_CHANNEL, OUTPUT);
  pinMode(MOTOR_R_B_CHANNEL, OUTPUT);
  pinMode(MOTOR_R_PWM_CHANNEL, OUTPUT);
  Serial.begin(115200);
}

void loop() {
//  testMotor();
  controlSumo();
//  readRemoteRC();
  delay(100);
}
