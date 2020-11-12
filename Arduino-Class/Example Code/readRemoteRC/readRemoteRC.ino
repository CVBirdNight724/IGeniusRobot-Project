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

void setup() {
  PCICR |= (1 << PCIE0);          // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0);        // set PCINT0 (pin 8) to trigger interrupt
  PCMSK0 |= (1 << PCINT1);        // set PCINT1 (pin 9) to trigger interrupt
  PCMSK0 |= (1 << PCINT2);        // set PCINT2 (pin 10) to trigger interrupt
  PCMSK0 |= (1 << PCINT3);        // set PCINT3 (pin 11) to trigger interrupt
  Serial.begin(115200);
}

void loop() {
  Serial.print("CH1: ");
  Serial.print(REMOTE_CHANNEL_1_VALUE);
  Serial.print(" CH2: ");
  Serial.print(REMOTE_CHANNEL_2_VALUE);
  Serial.print(" CH3: ");
  Serial.println(REMOTE_CHANNEL_3_VALUE);
  delay(100);
}
