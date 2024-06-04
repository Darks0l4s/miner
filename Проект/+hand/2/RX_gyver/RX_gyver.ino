/*   Данный скетч делает следующее: передатчик (TX) отправляет массив
     данных, который генерируется согласно показаниям с кнопки и с
     двух потенциомтеров. Приёмник (RX) получает массив, и записывает
     данные на реле, сервомашинку и генерирует ШИМ сигнал на транзистор.
    by AlexGyver 2016
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <GyverMotor.h>

RF24 radio(2, 8);  // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

#define MIN_DUTY 5
#define RA 4          // Направление вращение двигателя 1
#define RV 5          // ШИМ вывод для управления двигателем 1
#define LA 6        // Направление вращение двигателя 2
#define LV 9         // ШИМ вывод для управления двигателем 2
#define servo1 10
#define servo2 3

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
int povorot=1730;
GMotor motorR(DRIVER2WIRE, RA, RV, HIGH);
GMotor motorL(DRIVER2WIRE, LA, LV, HIGH);

void setup() {
  pinMode(3, OUTPUT);

  Serial.begin(9600);         // открываем порт для связи с ПК
  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openReadingPipe(1, address[0]);   // хотим слушать трубу 0
  radio.setChannel(0x76);     // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_2MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();        // начать работу
  radio.startListening(); // начинаем слушать эфир, мы приёмный модуль


  motorR.setMode(AUTO);
  motorL.setMode(AUTO);

  motorR.setDirection(REVERSE);
  motorL.setDirection(NORMAL);

  // мин. сигнал вращения
  motorR.setMinDuty(MIN_DUTY);
  motorL.setMinDuty(MIN_DUTY);

  // плавность скорости моторов
  motorR.setSmoothSpeed(1000);
  motorL.setSmoothSpeed(1000);

  pinMode(servo1, OUTPUT);
  pinMode(servo2, OUTPUT);
  digitalWrite(servo2,HIGH);
  delayMicroseconds(povorot);
  digitalWrite(servo2,LOW);
  delayMicroseconds(povorot);
}
int v = 0, vx = 0, speedR = 0, speedL = 0;
int gotByte;
int a[4];
float c;
int transmission=1;
long long timer4=0;
void loop() {
  byte pipeNo=0;
  
  while (radio.available(0)) {        // слушаем эфир со всех труб
    radio.read(&a, sizeof(a));  // чиатем входящий сигнал
    Serial.println(a[0]);
    if(a[0]==-1 && a[1]==-1)
    {
      if(transmission==5) transmission=1;
      else transmission++;
    }
    else go();



    if(a[2]<-1) 
    {
      digitalWrite(servo1,HIGH);
      delayMicroseconds(1400);
      digitalWrite(servo1,LOW);
      delayMicroseconds(1400);
    }
    if(a[2]>1) 
    {
      digitalWrite(servo1,HIGH);
      delayMicroseconds(1530);
      digitalWrite(servo1,LOW);
      delayMicroseconds(1530);
    }
    if(abs(a[2])==1 || a[2]==0) 
    {
      digitalWrite(servo1,HIGH);
      delayMicroseconds(1450);
      digitalWrite(servo1,LOW);
      delayMicroseconds(1450);
    }


    if(a[3]<-1) 
    {
      povorot+=2;
      digitalWrite(servo2,HIGH);
      delayMicroseconds(povorot);
      digitalWrite(servo2,LOW);
      delayMicroseconds(povorot);
    }
    if(a[3]>1) 
    {
      povorot-=2;
      digitalWrite(servo2,HIGH);
      delayMicroseconds(povorot);
      digitalWrite(servo2,LOW);
      delayMicroseconds(povorot);
    }
    }
  }


void go()
{
    motorL.setMode(AUTO);
    motorR.setMode(AUTO);
    vx=a[0]+40;
    v=a[1]+30;
    int LX = -map(vx, 1024, 0, -255, 255);
    int LY = -map(v, 1024, 0, -255, 255);
    if (LX>=-30 && LX<=30) LX=0;
    if (LY>=-30 && LY<=30) LY=0;
    speedR = LX+LY;
    speedL = LY-LX;
    if(transmission==1)
    {
      speedR = constrain(speedR, -50, 50);
      speedL = constrain(speedL, -50, 50);
    }
    if(transmission==2)
    {
      speedR = constrain(speedR, -100, 100);
      speedL = constrain(speedL, -100, 100);
    }
    if(transmission==3)
    {
      speedR = constrain(speedR, -150, 150);
      speedL = constrain(speedL, -150, 150);
    }
    if(transmission==4)
    {
      speedR = constrain(speedR, -200, 200);
      speedL = constrain(speedL, -200, 200);
    }
    if(transmission==5)
    {
      speedR = constrain(speedR, -225, 225);
      speedL = constrain(speedL, -225, 225);
    }
    if(speedL>=-2 && speedL<=2 ) motorL.smoothTick(0);
    else motorL.smoothTick(speedR);
    if(speedR>=-2 && speedR<=2) motorR.smoothTick(0);
    else motorR.smoothTick(speedL);
    // Serial.print(LX);
    // Serial.print(" ");
    // Serial.println(LY);
}