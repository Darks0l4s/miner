#include "metal.h"

metal::metal(int pin)
{
  pinMode(pin, INPUT);
  _pin = pin;
}

int metal::search()
{
  _pulse=0;
  _pulse_ms=0;
  for(_j=0;_j<80;_j++)
  {
    for(_i=0;_i<20;_i++)
    {
      _pulse_ms+=digitalRead(_pin);
    }
    _pulse = _pulse_ms/20;
  }
   if(_pulse!=0)return 1;
   else return 0;
}