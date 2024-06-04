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
#define D2 6         // Направление вращение двигателя 2
#define M2 9          // ШИМ вывод для управления двигателем 2
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

void loop() {
  byte pipeNo;
  int gotByte;
  while (radio.available(&pipeNo)) {        // слушаем эфир со всех труб
    radio.read(&gotByte, sizeof(gotByte));  // чиатем входящий сигнал
    Serial.print("Recieved: ");
    Serial.println(gotByte);
    if (gotByte==0) 
    {
    digitalWrite(D1,0);  // Задаем направление вращения
    digitalWrite(D2, 0);
    analogWrite(M1, 0);       // Задаем скорость вращения
    analogWrite(M2, 0);
    }
    if (gotByte==1) 
    {
    digitalWrite(D1,0);  // Задаем направление вращения
    digitalWrite(D2, 0);
    analogWrite(M1, 255);       // Задаем скорость вращения
    analogWrite(M2, 255);
    }
    if (gotByte==2) 
    {
    digitalWrite(D1,0);  // Задаем направление вращения
    digitalWrite(D2, 0);
    analogWrite(M1, 255);       // Задаем скорость вращения
    analogWrite(M2, 100);
    }
    if (gotByte==3) 
    {
    digitalWrite(D1,0);  // Задаем направление вращения
    digitalWrite(D2, 1);
    analogWrite(M1, 200);       // Задаем скорость вращения
    analogWrite(M2, -200);
    }
    if (gotByte==4) 
    {
    digitalWrite(D1,1);  // Задаем направление вращения
    digitalWrite(D2, 1);
    analogWrite(M1, -225);       // Задаем скорость вращения
    analogWrite(M2, -175);
    }
    if (gotByte==5) 
    {
    digitalWrite(D1,1);  // Задаем направление вращения
    digitalWrite(D2, 1);
    analogWrite(M1, -255);       // Задаем скорость вращения
    analogWrite(M2, -255);
    }
    if (gotByte==6) 
    {
    digitalWrite(D1,1);  // Задаем направление вращения
    digitalWrite(D2, 1);
    analogWrite(M1, -175);       // Задаем скорость вращения
    analogWrite(M2, -225);
    }
    if (gotByte==7) 
    {
    digitalWrite(D1,1);  // Задаем направление вращения
    digitalWrite(D2, 0);
    analogWrite(M1, -200);       // Задаем скорость вращения
    analogWrite(M2, 200);
    }
    if (gotByte==8) 
    {
    digitalWrite(D1,0);  // Задаем направление вращения
    digitalWrite(D2, 0);
    analogWrite(M1, 100);       // Задаем скорость вращения
    analogWrite(M2, 225);
    }
  }
}
