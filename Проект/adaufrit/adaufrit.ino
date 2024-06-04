#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

float i,ch=0;
float k=0.095737164502371, b=-4.09493187477935;
void setup()
{
  Serial.begin(9600);
  z:
  Wire.begin();

  sensor.setTimeout(200);
  if (!sensor.init())
  {
    Serial.println("Не удалось обнаружить и инициализировать датчик!");
    goto z;
  }

#if defined LONG_RANGE
  // понижает предел скорости обратного сигнала (по умолчанию 0,25 MCPS (мчип/с))
  sensor.setSignalRateLimit(0.1);
  // увеличить периоды лазерного импульса (по умолчанию 14 и 10 PCLK)
  // * - PCLK — это частота периферии
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // уменьшить тайминг до 20 мс (по умолчанию около 33 мс)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // увеличить тайминг 200 мс
  sensor.setMeasurementTimingBudget(200000);
#endif
}

void loop()
{
  ch=sensor.readRangeSingleMillimeters()*k+b;
  if(ch<600)Serial.println(round(ch));
}