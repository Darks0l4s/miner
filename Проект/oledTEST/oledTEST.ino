#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
void setup() {
  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(1);   // масштаб текста (1..4)
  oled.home();        // курсор в 0,0
}
void loop() {
  oled.home();
  oled.print("Температура ");
  oled.print(23);
  oled.print("                                          ");
  oled.setCursor(0, 1);
  oled.print("Влажность ");
  oled.print(65);
  oled.print("                                          ");
  oled.setCursor(0, 2);
  oled.print("Радиация ");
  oled.print(2);
  oled.print("                                                                             ");
  oled.setCursor(0, 3);
  oled.print("Широта ");
  oled.print(43235465);
  oled.print("                                                                             ");
  oled.setCursor(0, 4);
  oled.print("Долгота ");
  oled.print(23455234);
  oled.print("                                                                             ");
  oled.setCursor(0, 5);
  oled.print("металлоискатель ");
  oled.print(0);
  oled.print("                                                                             ");
}