#include "metal.h"
#include <Wire.h>
#include "CG_RadSens.h"
#include <TroykaDHT.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial gpsSerial(18,19);
RF24 radio(48, 49);
DHT dht(3, DHT21);    
metal metal1(2);
CG_RadSens radSens(RS_DEFAULT_I2C_ADDRESS);

int incomingByte = 0;
bool found;
int RadID, dhtH, dhtT;

bool A_metal=false, A_dht=false, A_rad=false, A_gps=false;

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};

bool newdata = false;
unsigned long start;
long lat, lon;
unsigned long time, date;

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

  radio.openWritingPipe(address[0]);
  radio.setChannel(0x76); 

  radio.setPALevel (RF24_PA_MAX);
  radio.setDataRate (RF24_2MBPS);

  radio.powerUp();
  radio.stopListening();

  radSens.setLedState(true);
}

void loop() {
  if (Serial.available() > 0) 
  {
    reception();
  }
  scan();
}

void reception()
{
    incomingByte = Serial.read();
    incomingByte-='0';
    if (incomingByte>=0)
    {
        if(incomingByte==0) A_metal=!A_metal;
        if(incomingByte==1) A_dht=!A_dht;
        if(incomingByte==2) A_rad=!A_rad;
        if(incomingByte==3) A_gps=!A_gps;
    }
}
float a[6];
void scan()
{
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
    found=metal1.search();
    a[3]=found;
  }
  else a[3]=-100;
  if(A_gps)
  {
    if (millis() - start > 1000)
  {
    newdata = readgps();
    if (newdata)
    {
      start = millis();
      gps.get_position(&lat, &lon);
      a[4]=lat;
      a[5]=lon;
    }
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