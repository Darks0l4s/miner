#include <Servo.h> // подключение библиотеки
Servo test_servo; // создание объекта для сервопривода
void setup() {
  Serial.begin(9600);
  test_servo.attach(6); // указываем управляющий пин
}
  int i=0;
void loop() {
  test_servo.write(113);
  delay(4000);
  test_servo.write(73);
  delay(4000);
}