#include <Arduino.h>

class metal
{
  public:
    metal(int pin);

    int search();
  private:
    int _pin;
    int _pulse;
    int _pulse_ms;
    int _i;
    int _j;
};