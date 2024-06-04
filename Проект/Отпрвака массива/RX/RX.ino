/*   Данный скетч делает следующее: передатчик (TX) отправляет массив
     данных, который генерируется согласно показаниям с кнопки и с
     двух потенциомтеров. Приёмник (RX) получает массив, и записывает
     данные на реле, сервомашинку и генерирует ШИМ сигнал на транзистор.
    by AlexGyver 2016
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(2, 10);  // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги
#define D1 3          // Направление вращение двигателя 1
#define M1 5          // ШИМ вывод для управления двигателем 1
#define D2 6        // Направление вращение двигателя 2
#define M2 9         // ШИМ вывод для управления двигателем 2
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

void setup() {
  Serial.begin(9600);         // открываем порт для связи с ПК
  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openReadingPipe(1, address[0]);   // хотим слушать трубу 0
  radio.setChannel(0x77);     // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_2MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();        // начать работу
  radio.startListening(); // начинаем слушать эфир, мы приёмный модуль
  pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
}
int v=0;
int vx=0;
int gotByte;
int a[2];
float c;
void loop() {
  byte pipeNo;
  
  while (radio.available(&pipeNo)) {        // слушаем эфир со всех труб
    radio.read(&a, sizeof(a));  // чиатем входящий сигнал
    if(a[0]==-1 && a[1]==-1)
    {
    digitalWrite(D1, 1);       // Задаем скорость вращения
    digitalWrite(D2, 0);
    analogWrite(M1,0);
    analogWrite(M2,250);
    }
    else
    {
      if(a[0]==-2 && a[1]==-2)
    {
    digitalWrite(D2, 1);       // Задаем скорость вращения
    digitalWrite(D1, 0);
    analogWrite(M2,0);
    analogWrite(M1,250);
    }
    else go();
    }
    
    }
  }


void go()
{
      vx=a[0];
    v=a[1];
    if(v==528) v==527;
    v=(v-527)/2;
    if(vx==511) vx==512;
    vx=(vx-512)/2;

    if(v>250) v=249;
    if(v<-250) v=-225;
    if(vx>=250) vx=249;
    if(vx<=-250) vx=-249;
    c=100-abs(vx*100/249);
    Serial.println((225-abs(v)*c/100));
    if (vx<0){
    if(v<0){
    digitalWrite(D1, 1);       // Задаем скорость вращения
    digitalWrite(D2, 1);
    analogWrite(M2, 225-abs(v));
    analogWrite(M1, (225-abs(v)*c/100));
    }
    else {
    digitalWrite(D1, 0);       // Задаем скорость вращения
    digitalWrite(D2, 0);
    analogWrite(M1,v);
    analogWrite(M2,v*c/100);
    
    }}
    else
    {
    if(v<0){
    digitalWrite(D1, 1);       // Задаем скорость вращения
    digitalWrite(D2, 1);
    analogWrite(M2, (225-abs(v)*c/100));
    analogWrite(M1, 225-abs(v));
    }
    else {
      digitalWrite(D1, 0);       // Задаем скорость вращения
    digitalWrite(D2, 0);
    analogWrite(M1,v*c/100);
    analogWrite(M2,v);
    
    }
    }
}