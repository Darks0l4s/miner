#include <WString.h>
#include <Arduino.h>
#include <HardwareSerial.h>

class SerialString 
{
  public:
    SerialString(int speed);

     String readString();
  private:
    char _a;
    String _b="";
    int _speed;
    int _i;
};