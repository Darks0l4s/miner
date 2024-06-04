#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги
#define ButPin 0
#define ButDown 3
#define ButUp 2
#define stickX A2
#define stickY A3
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

byte counter;

int pos1[2]={0,170}, pos2[2]={40,155}, pos3[2]={0,140}, pos4[2]={40,170};

bool ButPinState=false;

int stat1=90, stat2=90, stat3=90, statX, statY;

void setup() {
  Serial.begin(9600);         // открываем порт для связи с ПК

  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openWritingPipe(address[0]);  // мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x77);             // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();        // начать работу
  radio.stopListening();  // не слушаем радиоэфир, мы передатчик
  
  pinMode(ButUp, INPUT_PULLUP);
  pinMode(ButDown, INPUT_PULLUP);
  pinMode(ButPin, INPUT);
  pinMode(stickX, INPUT);
  pinMode(stickY, INPUT);
}

int a[4];
long long timer1=0;
void loop() {
  statX=map(analogRead(stickX), 1025, 45, -5, 5);
  if(statX==6) statX=5;
  if(statX==7) statX=5;
  a[0]=statX;
  statY=map(analogRead(stickY), 1025, 45, -5, 5);
  if(statY==6) statY=5;
  if(statY==7) statY=5;
  a[1]=statY;
  if(digitalRead(ButUp)==0)
  {
    stat3=1;
    delay(20);
  }
  else
  if(digitalRead(ButDown)==0)
  {
    stat3=-1;
    delay(20);
  }
  else stat3=0;
  a[2]=stat3;
    if(digitalRead(ButPin)==0 && timer1+400<=millis())
  {
    ButPinState=!ButPinState;
    timer1=millis();
  }
  a[3]=ButPinState;
  radio.write(&a, sizeof(a));
  Serial.print(a[0]);
  Serial.print(" ");
  Serial.print(a[1]);
  Serial.print(" ");
  Serial.print(a[2]);
  Serial.print(" ");
  Serial.println(a[3]);
}