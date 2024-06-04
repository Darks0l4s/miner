#include "metal.h"

metal metal1(2);

void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.println(metal1.search());

}
