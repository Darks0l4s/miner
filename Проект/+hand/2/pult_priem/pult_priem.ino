#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля
#include <GyverOLED.h>

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

RF24 radio(9, 10);

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

void setup() {
  Serial.begin(9600);         // открываем порт для связи с ПК
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openReadingPipe(1, address[2]);
  radio.setChannel(0x76);             // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_2MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();        // начать работу
  radio.startListening();  // не слушаем радиоэфир, мы передатчик

  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(1);   // масштаб текста (1..4)
  oled.home();
  oled.print("Температура ");
  oled.setCursor(0, 1);
  oled.print("Влажность ");
  oled.setCursor(0, 2);
  oled.print("Радиация ");
  oled.setCursor(0, 3);
  oled.print("Металл ");
  oled.setCursor(0, 4);
  oled.print("Широта ");
  oled.setCursor(0, 5);
  oled.print("Долгота ");
}
float b[6];
void loop() {
  if (radio.available(2)) 
  {        // слушаем эфир со всех труб
    radio.read(&b, sizeof(b));  // чиатем входящий сигнал
    oledik();
    Serial.println("Принял");
  }

}

void oledik()
{
  if(b[0]!=-100) 
    {
      oled.home();
      oled.print("Температура ");
      oled.print(b[0]);
      oled.print("                                          ");
    }
    else
    {
      oled.home();
      oled.print("Температура ");
      oled.print("none                                          ");
    }
    if(b[1]!=-100) 
    {
      oled.setCursor(0, 1);
      oled.print("Влажность ");
      oled.print(b[1]);
      oled.print("                                          ");
    }
    else
    {
      oled.setCursor(0, 1);
      oled.print("Влажность ");
      oled.print("none                                          ");
    }
    if(b[2]!=-100) 
    {
      oled.setCursor(0, 2);
      oled.print("Радиация ");
      oled.print(b[2]);
      oled.print("                                          ");
    }
    else
    {
      oled.setCursor(0, 2);
      oled.print("Радиация ");
      oled.print("none                                          ");
    }
    if(b[3]!=-100) 
    {
      oled.setCursor(0, 3);
      oled.print("Металл ");
      oled.print(b[3]);
      oled.print("                                          ");
    }
    else
    {
      oled.setCursor(0, 3);
      oled.print("Металл ");
      oled.print("none                                          ");
    }
    if(b[4]!=-100) 
    {
      oled.setCursor(0, 4);
      oled.print("Широта ");
      oled.print(b[4]);
      oled.print("                                          ");
    }
    else
    {
      oled.setCursor(0, 4);
      oled.print("Широта ");
      oled.print("none                                          ");
    }
    if(b[5]!=-100) 
    {
      oled.setCursor(0, 5);
      oled.print("Долгота ");
      oled.print(b[5]);
      oled.print("                                          ");
    }
    else
    {
      oled.setCursor(0, 5);
      oled.print("Долгота ");
      oled.print("none                                          ");
    }
}