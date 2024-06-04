#define STEP_PIN         8
#define DIR_PIN          9
#define ENABLE_PIN       10

int frequency=1500;           //Время между импульсами в мксек. 1000 мксек = 1 мсек = 0.001 сек. Частота следования импульсов 1/0.001 = 1 кГц, 
//Не рекомендуется устанавливать время меньше 100 мсек, т.к. частота будет 10 кГц

void setup() {
  pinMode(14, INPUT);
  Serial.begin(9600);
  pinMode(STEP_PIN   , OUTPUT);       
  pinMode(DIR_PIN    , OUTPUT);
  pinMode(ENABLE_PIN , OUTPUT);
  digitalWrite(ENABLE_PIN , LOW);
  digitalWrite(DIR_PIN    , HIGH); 
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  while(digitalRead(14)!=0)
  {
    digitalWrite(DIR_PIN    , LOW); 
    digitalWrite(STEP_PIN    , HIGH);
    delayMicroseconds(frequency);
    digitalWrite(STEP_PIN    , LOW);
  }
  delay(2000);
  Serial.println("Great");
  frequency=9999;
  int timer=millis();
  while(millis()<=timer+200)
  {
    digitalWrite(DIR_PIN    , LOW); 
    digitalWrite(STEP_PIN    , HIGH);
    delayMicroseconds(frequency);
    digitalWrite(STEP_PIN    , LOW);
  }
  while((millis()/2000)%2!=1)
  {

  }
}
int i=0;
int val,n;
void loop() {
  if (n==2)
  {
    if((millis()/2000)%2==0)
    {
      digitalWrite(DIR_PIN    , LOW); 
      digitalWrite(STEP_PIN    , HIGH);
      delayMicroseconds(frequency);
      digitalWrite(STEP_PIN    , LOW);
    }
    else
    {
      digitalWrite(DIR_PIN    , HIGH); 
      digitalWrite(STEP_PIN    , HIGH);
      delayMicroseconds(frequency);
      digitalWrite(STEP_PIN    , LOW);
    }
  }
   if (Serial.available()) {
    val=Serial.parseInt();
    if(val!=0) n=val;
    if(val==1) 
    {
      for (i=0;i<500;i++)
      {
      digitalWrite(6, HIGH);
      analogWrite(7, -100);
      }
    }
    if(val==2) 
    {
      for (i=0;i<500;i++)
      {
      digitalWrite(6, LOW);
      analogWrite(7, 100);
      }
    }
  }
  else{
    digitalWrite(6, LOW); analogWrite(7, 0);
  }
}
