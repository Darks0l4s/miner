#include "SerialString.h"

SerialString::SerialString(int speed)
{
  _speed=speed;
  Serial.begin(_speed);
}

String SerialString::readString() 
{
  _i=0;
  while(_i==0)
  {
  if (Serial.available() > 0) {
        _a=Serial.read();
        _b+=_a;
        if(_a=='\n')
        {
          _b.replace("\n","");
          return _b;
          _b="";
        }
    }
  }
}