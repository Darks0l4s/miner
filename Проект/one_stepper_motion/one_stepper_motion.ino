//Пины управления шаговиком
#define STEP_PIN         2
#define DIR_PIN          3
#define ENABLE_PIN       4

//Здесь можно настроить время поворота и паузы
#define move_forward_time 40    //время прямого хода в мсек
#define move_back_time 40     //время обратного хода в мсек
#define frequency 4000            //Время между импульсами в мксек. 1000 мксек = 1 мсек = 0.001 сек. Частота следования импульсов 1/0.001 = 1 кГц, 
//Не рекомендуется устанавливать время меньше 100 мсек, т.к. частота будет 10 кГц
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

float ch=0,v=33.83;
float timer3=0;
float k=0.0947311670845656, b=-5.44190333682796;
//Таймер для millis()
uint32_t timer1 = 0, timer2=0;
int i,sum1=0,a=0,q=0,sum2=0;
//Логический флаг для рабочего режима
bool flag = 0;
int mas[2000];

void setup() {

  pinMode(STEP_PIN   , OUTPUT);       //Настраиваем пины управления
  pinMode(DIR_PIN    , OUTPUT);
  pinMode(ENABLE_PIN , OUTPUT);

 digitalWrite(ENABLE_PIN , LOW);    //Включаем мотор, чтобы не грелся


    Serial.begin(9600);

  // Настраиваем на вход пин, к которому подключен датчик линии.
  // Здесь нет необходимости включать внутреннюю подтяжку пина к VCC, т.к.
  // модуль сам заботится о формировании логической "1" при отсутствии цели
  pinMode(7, INPUT);
  z:
  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Не удалось обнаружить и инициализировать датчик!");
    goto z;
  }

#if defined LONG_RANGE
  // понижает предел скорости обратного сигнала (по умолчанию 0,25 MCPS (мчип/с))
  sensor.setSignalRateLimit(100);
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
digitalWrite(DIR_PIN    , HIGH);         //Задаем направление вращения                                   //Крутим движок move_forward_time мсек
      
}

void loop () {
    digitalWrite(STEP_PIN    , HIGH);
    delayMicroseconds(frequency);
   digitalWrite(STEP_PIN    , LOW);
  if(digitalRead(7)==0 && millis()-timer2>4000) 
  {
    timer2=millis();
    if(a==0)
    {
      timer1=millis();
      a=1;
    } 
    else
    { Serial.print("V=");
      Serial.println((millis()-timer1)/1000.0);
      v=(millis()-timer1)/1000.0;
      timer1=millis();
      for(i=0;i<q;i++)
      {
        Serial.print(mas[i]);
        Serial.print(", ");
      }
      Serial.println();
      q=0;
    }   
    
   }
  if(a==1) //
  { 
    ch=sensor.readRangeSingleMillimeters()*k+b;
    if(ch<600 && ch>0) mas[q]=round(ch);
    else mas[q]=200;
    q++;
  }
  }
