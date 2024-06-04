#include "SerialString.h"
char a;
String b;
int speed;
SerialString stroka(9600);
void setup() {
}

void loop() {
    // if (Serial.available() > 0) {
    //     a=Serial.read();
    //     b+=a;
    //     if(a=='\n')
    //     {
    //       b.replace("\n","");
    //       Serial.println(b);
    //       b="";
    //     }
    // }
    Serial.println(stroka.readString());
}
