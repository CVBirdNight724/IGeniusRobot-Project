#include<ATX2.h>

#define left in(29)
#define right in(30)

#define baseSpeed 30
#define maxSpeed  100

#define irCenter    analog(2)
#define irLeftFront   analog(1)
#define irLeftBack    analog(0)
#define irRightFront  analog(3)
#define irRightBack   analog(4)

int colorwhite = 0;
int colorblack = 1;

int chk_leftfront = 250;
int chk_leftback = 220;
int chk_cen = 200;
int chk_rightback = 200;
int chk_rightfront = 190;

int modeRun;  //modeRun 1 = Left ,2 = Right

void setup()
{
  XIO();
  menuRun();
  function();
}

void loop() 
{
//  Read_Sensors();
  track(); 
}

void menuRun(void){  
  modeRun = 1;
  while(!sw_OK()){
    if (irLeftBack > 600) modeRun = 1;
    if (irRightBack > 600) modeRun = 2;
//    else modeRun = 3;
    glcd (0,0,"%d",modeRun);
  }
  while(sw_OK());

}

void function()
{
  if (modeRun == 1)
  {
    delay(5000);
    motorControl(-baseSpeed,baseSpeed);
    delay(130);
    motorControl(0,0);
    delay(10);
  }
  else if(modeRun == 2)
  {
    delay(5000);
    motorControl(baseSpeed,-baseSpeed);
    delay(130);
    motorControl(0,0);
    delay(10);
  }
}

void Read_Sensors()
{
  glcd(0,0, "irLB : %d",irLeftBack);
  glcd(1,0, "irLF : %d",irLeftFront);
  glcd(2,0, "irC : %d",irCenter);
  glcd(3,0, "irRF : %d",irRightBack);
  glcd(4,0, "irRB : %d",irRightBack);
  delay(180);
}

void motorControl(int LeftSpeeds, int Rightpeeds)
{
  motor(1,LeftSpeeds);
  motor(2,LeftSpeeds);
  motor(3,Rightpeeds);
  motor(4,Rightpeeds);
}

void track()
{
  if(left == colorwhite && right == colorwhite)
  {
    motorControl(-baseSpeed,-baseSpeed);
    delay(220);
    motorControl(baseSpeed,-baseSpeed);
    delay(120);
  }
  else if(left == colorwhite && right == colorblack)
  {
    motorControl(-baseSpeed,-baseSpeed);
    delay(220);
    motorControl(baseSpeed,-baseSpeed);
    delay(120);
  }
  else if(left == colorblack && right == colorwhite)
  {
    motorControl(-baseSpeed,-baseSpeed);
    delay(220);
    motorControl(-baseSpeed,baseSpeed);
    delay(120);
  }
  else if(left == colorblack && right == colorblack)
  {
//    motorControl(0,0);
    check_ss();
//    motorControl(baseSpeed,baseSpeed);
    delay(10);
  }
}

void check_ss ()
{
  if (irCenter > chk_cen )
  {
    motorControl(maxSpeed,maxSpeed);
  }
  else if (irLeftFront > chk_leftfront )
  {
    motorControl( -baseSpeed , baseSpeed );
    delay(20);
  }
  else if ( irRightFront > chk_rightfront )
  {
    motorControl(baseSpeed , -baseSpeed );
    delay(20);
  }
  else if ( irLeftBack > chk_leftback )
  {
    motorControl(-baseSpeed,baseSpeed);
    delay(40);    
  }
  else if (irRightBack > chk_rightback )
  {
    motorControl(baseSpeed,-baseSpeed);
    delay(40);
  }
  else motorControl(baseSpeed,baseSpeed);
}
