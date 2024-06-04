#include <ServoSmooth.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(8, 10);

bool ButPinState=false;
ServoSmooth servo1;
ServoSmooth servo2;
ServoSmooth servo3;
ServoSmooth servo4;

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};
int pos1[2]={0,170}, pos2[2]={40,155}, pos3[2]={0,140}, pos4[2]={40,170};
int i;

int stat1=90, stat2=90, stat3=90, statX, statY;
void setup() {
  Serial.begin(9600);

  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openReadingPipe(1, address[0]);   // хотим слушать трубу 0
  radio.setChannel(0x77);     // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();        // начать работу
  radio.startListening(); // начинаем слушать эфир, мы приёмный модуль

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
}
long long timer1=0, timer2=0,timer3=0, timer4=0;
int c[4];
void loop() {
  byte pipeNo;
  while (radio.available(&pipeNo)) {        // слушаем эфир со всех труб
    radio.read(&c, sizeof(c));  // чиатем входящий сигнал
    Serial.print(c[0]);
  Serial.print(" ");
  Serial.print(c[1]);
  Serial.print(" ");
  Serial.print(c[2]);
  Serial.print(" ");
  Serial.println(c[3]);
  }
  
  statX=c[0];
  statY=c[1];

  if((statX>1 || statX<-1) && timer3+100/abs(statX)<=millis()){
  if(stat1+statX>=pos1[0] && stat1+statX<=pos1[1]) stat1+=statX/abs(statX);
  stat1+=statX/abs(statX);
  servo1.write(stat1);
  timer3=millis();
  }

  if((statY>1 || statY<-1) && timer4+100/abs(statY)<=millis()){
  if(stat2+statY>=pos2[0] && stat2+statY<=pos2[1])stat2+=statY/abs(statY);
  stat2+=statY/abs(statY);
  servo2.write(stat2);
  timer4=millis();
  }

  if(c[2]>0 && stat3<pos3[1])
  {
    stat3++;
    servo3.write(stat3);
    delay(20);
  }

  if(c[2]<0 && stat3>pos3[0])
  {
    stat3--;
    servo3.write(stat3);
    delay(20);
  }
  if(c[3])
  {
    servo4.write(pos4[0]);
  }
  else 
  {
    servo4.write(pos4[1]);
  }
}