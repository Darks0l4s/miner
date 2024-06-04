#define PIN_SENSOR 7

int i,sum=0,k=100,min=1000000;
void setup() {
  // Инициализируем работу с серийным портом для вывода отладочной информации
  Serial.begin(9600);

  // Настраиваем на вход пин, к которому подключен датчик линии.
  // Здесь нет необходимости включать внутреннюю подтяжку пина к VCC, т.к.
  // модуль сам заботится о формировании логической "1" при отсутствии цели
  pinMode(PIN_SENSOR, INPUT);
}

void loop() {
  sum=0;
  for(i=0;i<1;i++)
  {
    sum+=digitalRead(7);
  }
  Serial.println(sum);
  //if(sum<=min){Serial.println(sum); min=sum;}
}