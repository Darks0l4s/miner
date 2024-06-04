#include "metal.h"
#include <Wire.h>
#include "CG_RadSens.h"
#include <TroykaDHT.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define STEP_PIN         8
#define DIR_PIN          9
#define ENABLE_PIN       10

TinyGPS gps;
SoftwareSerial gpsSerial(18,19);
RF24 radio(48, 49);
DHT dht(3, DHT21);    
metal metal1(2);
CG_RadSens radSens(RS_DEFAULT_I2C_ADDRESS);

int incomingByte = 0;
bool found;
int RadID, dhtH, dhtT;

bool A_metal=false, A_dht=false, A_rad=false, A_gps=false,A_metal2=false;

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};

bool newdata = false;
unsigned long start;
long lat, lon;
unsigned long time, date;
int frequency=1500;

void setup() {
  gpsSerial.begin(9600);
  Wire.begin();
  Serial.begin(9600);
  dht.begin();

  radio.begin(); 
  radio.setAutoAck(0); 
  radio.setRetries(0, 15);
  radio.enableAckPayload(); 
  radio.setPayloadSize(32);

  radio.openWritingPipe(address[2]);
  radio.setChannel(0x76); 

  radio.setPALevel (RF24_PA_MAX);
  radio.setDataRate (RF24_2MBPS);

  radio.powerUp();
  radio.stopListening();

  radSens.setLedState(true);

  pinMode(14, INPUT);
  pinMode(STEP_PIN   , OUTPUT);       
  pinMode(DIR_PIN    , OUTPUT);
  pinMode(ENABLE_PIN , OUTPUT);
  digitalWrite(ENABLE_PIN , LOW);
  digitalWrite(DIR_PIN    , HIGH); 
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}
int b[8];
int i;
void loop() {

  radio.openWritingPipe(address[2]);
  radio.stopListening();
  scan();
  radio.openReadingPipe(1, address[0]);
  radio.startListening();
  delay(2);
  if (radio.available(0)) 
  {        // слушаем эфир со всех труб
    radio.read(&b, sizeof(b));  // чиатем входящий сигнал
    Serial.print(b[4]);
    Serial.print(" ");
    Serial.print(b[5]);
    Serial.print(" ");
    Serial.print(b[6]);
    Serial.print(" ");
    Serial.println(b[7]);
  }
  A_metal2=A_metal;
  A_dht=b[4];
  A_metal=b[5];
  A_gps=b[6];
  A_rad=b[7];
  if(A_metal==1 && A_metal2!=A_metal)
  {
    while(digitalRead(14)!=0)
  {
    digitalWrite(DIR_PIN    , LOW); 
    digitalWrite(STEP_PIN    , HIGH);
    delayMicroseconds(frequency);
    digitalWrite(STEP_PIN    , LOW);
  }
  frequency=9999;
  int timer=millis();
  while(millis()<=timer+200)
  {
    digitalWrite(DIR_PIN    , LOW); 
    digitalWrite(STEP_PIN    , HIGH);
    delayMicroseconds(frequency);
    digitalWrite(STEP_PIN    , LOW);
  }
      digitalWrite(6, LOW);
      analogWrite(7, 100);
      delay(1000);
      digitalWrite(6, LOW);
      analogWrite(7, 0);
  }

  if(A_metal==0 && A_metal2!=A_metal)
  {
      digitalWrite(6, HIGH);
      analogWrite(7, -100);
      delay(1000);
      digitalWrite(6, LOW);
      analogWrite(7, 0);
  }
}
long long gradus=-200;
float a[6];
void scan()
{
 // Serial.println(a[1]);
  if(A_dht)
  {
    dht.read();
    dhtH=dht.getHumidity();
    dhtT=dht.getTemperatureC();
    a[0]=dhtT;
    a[1]=dhtH;
  }
  else
  {
    a[0]=-100;
    a[1]=-100;
  }
  if(A_rad)
  {
    RadID=radSens.getRadIntensyDynamic();
    a[2]=RadID;
  }
  else a[2]=-100;
  if(A_metal)
  {
    if(gradus<0)
    {
      digitalWrite(DIR_PIN    , HIGH); 
      digitalWrite(STEP_PIN    , HIGH);
      delayMicroseconds(frequency);
      digitalWrite(STEP_PIN    , LOW);
    }
    if (gradus>=0) {
      digitalWrite(DIR_PIN    , LOW); 
      digitalWrite(STEP_PIN    , HIGH);
      delayMicroseconds(frequency);
      digitalWrite(STEP_PIN    , LOW);
    }
    if(gradus==200) gradus=-200;
    gradus++;
    found=metal1.search();
    a[3]=found;
  }
  else a[3]=-100;
  if(A_gps)
  {

    newdata = readgps();
    if (newdata)
    {
      gps.get_position(&lat, &lon);
      a[4]=lat;
      a[5]=lon;
    }
  }
  else
  {
    a[4]=-100;
    a[5]=-100;
  }
  radio.write(&a, sizeof(a));
}

bool readgps()
{
while (gpsSerial.available())
{
int b = gpsSerial.read();

if('\r' != b)
{
if (gps.encode(b))
return true;
}
}
return false;
}