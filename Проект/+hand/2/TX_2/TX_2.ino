#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля

RF24 radio(9, 10);

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

#define pinX    A1  // ось X джойстика
#define pinY    A0  // ось Y джойстика
#define pinX2    A2  // ось X джойстика
#define pinY2    A3  // ось Y джойстика
#define swPin    7  // кнопка джойстика

int counter;

void setup() {
  Serial.begin(9600);         // открываем порт для связи с ПК
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openWritingPipe(address[0]);  // мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x76);             // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_2MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();        // начать работу
  radio.stopListening();
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  pinMode(pinX, INPUT);
  pinMode(pinX2, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinY2, INPUT);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

}
int Y;
int X;
int a[4];
long long  timer=0, timer2=0,timer3=0;
byte pipeNo=1;
int t=50;
void loop() 
{
  radio.openWritingPipe(address[0]);
  radio.stopListening();
  a[0] = analogRead(pinX);              // считываем значение оси Х
  a[1]= analogRead(pinY);              // считываем значение оси Y
  // a[4]= analogRead(pinX2);
  a[3]=map(analogRead(pinY2), 1020, 20, -5, 5);
  a[2]=map(analogRead(pinX2), 1020,50, -5, 5);
  radio.write(&a, sizeof(a));
  if(digitalRead(7)==0 && timer+300<=millis())
  {
    timer=millis();
    a[0]=-1;
    a[1]=-1;
    radio.write(&a, sizeof(a));
  }
}

