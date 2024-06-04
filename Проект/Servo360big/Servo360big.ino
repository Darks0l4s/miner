#define pin 10
//340 1730  2500
char incomingByte;
int i=200;
int k=0;
void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
}
 
void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if(incomingByte!='\n') 
    {
      Serial.println(i);
      if(incomingByte=='+') 
      {
        i+=10;
      }
      if(incomingByte=='-') 
      {
        i-=10;
      }
    }
  }
  digitalWrite(pin,HIGH);
  delayMicroseconds(i);
  digitalWrite(pin,LOW);
  delayMicroseconds(i);
}
