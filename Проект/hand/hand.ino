#include <ServoSmooth.h>

#define ButPin 0
#define ButDown 2
#define ButUp 8
#define stickX A5
#define stickY A4

ServoSmooth servo1;
ServoSmooth servo2;
ServoSmooth servo3;
ServoSmooth servo4;

bool ButPinState=false;
int i;
int pos1[2]={0,170}, pos2[2]={40,155}, pos3[2]={0,140}, pos4[2]={40,170};
int stat1=90, stat2=90, stat3=90, statX, statY;
void setup() {
  Serial.begin(9600);

  servo1.attach(3,90);
  servo1.smoothStart();
  servo1.setSpeed(90);

  servo2.attach(5,90);
  servo2.smoothStart();
  servo2.setSpeed(90);

  servo3.attach(6,90);
  servo3.smoothStart();
  servo3.setSpeed(90);

  servo4.attach(9,pos4[1]);
  servo4.smoothStart();
  servo4.setSpeed(90);

  pinMode(ButUp, INPUT_PULLUP);
  pinMode(ButDown, INPUT_PULLUP);
  pinMode(ButPin, INPUT);
  pinMode(stickX, INPUT);
  pinMode(stickY, INPUT);
}
long long timer1=0, timer2=0,timer3=0, timer4=0;
void loop() {

  statX=map(analogRead(stickX), 1025, 45, -5, 5);
  if(statX==6) statX=5;
  if(statX==7) statX=5;
  if((statX>1 || statX<-1) && timer3+100/abs(statX)<=millis()){
  if(stat1+statX>=pos1[0] && stat1+statX<=pos1[1]) stat1+=statX/abs(statX);
  servo1.write(stat1);
  timer3=millis();
  }

  statY=map(analogRead(stickY), 1025, 45, -5, 5);
  if(statY==6) statY=5;
  if(statY==7) statY=5;
  if((statY>1 || statY<-1) && timer4+100/abs(statY)<=millis()){
  if(stat2+statY>=pos2[0] && stat2+statY<=pos2[1])stat2+=statY/abs(statY);
  servo2.write(stat2);
  timer4=millis();
  }
  Serial.println(statX);
  if(digitalRead(ButPin)==0 && timer1+400<=millis())
  {
    ButPinState=!ButPinState;
    timer1=millis();
  }
  if(digitalRead(ButUp)==0 && stat3<pos3[1])
  {
    stat3++;
    servo3.write(stat3);
    delay(20);
  }
  if(digitalRead(ButDown)==0 && stat3>pos3[0])
  {
    stat3--;
    servo3.write(stat3);
    delay(20);
  }
  if(ButPinState)
  {
    servo4.write(pos4[0]);
  }
  else 
  {
    servo4.write(pos4[1]);
  }
}

