#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(48,49);  // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // возможные номера труб

void setup() {
  Serial.begin(9600);       // открываем порт для связи с ПК
  radio.begin();            // активировать модуль
  radio.setAutoAck(1);      // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);  // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload(); // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32); // размер пакета, в байтах

  radio.openReadingPipe(1, address[3]); // хотим слушать трубу 0
  radio.setChannel(0x60);   // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS);   // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  // должна быть одинакова на приёмнике и передатчике!
  // при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  // ВНИМАНИЕ!!! enableAckPayload НЕ РАБОТАЕТ НА СКОРОСТИ 250 kbps!

  radio.powerUp();        // начать работу
  radio.startListening(); // начинаем слушать эфир, мы приёмный модуль
}

void loop(void) {
  byte pipeNo, gotByte;
  while ( radio.available(&pipeNo)) {  // слушаем эфир со всех труб
    radio.read( &gotByte, 1 );         // чиатем входящий сигнал
    // отправляем обратно то что приняли
    radio.writeAckPayload(pipeNo, &gotByte, 1 );
    Serial.print("Recieved: ");
    Serial.println(gotByte);
  }
}