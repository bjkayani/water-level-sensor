//Water Tank Level Monitor
//Transmitter Code
//6.12.19

#include <SoftwareSerial.h>
SoftwareSerial HC12(5, 6); // HC-12 TX Pin, HC-12 RX Pin

#include <HCSR04.h>

UltraSonicDistanceSensor distanceSensor(3, 2);

#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

float temp;
int hum;
float distance;

String sndString;
char sndMsg[12];

void setup() {
  Serial.begin(9600);
  HC12.begin(9600); 
  dht.begin();
  }

void loop() {
  distance = distanceSensor.measureDistanceCm();
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  sndString = "t" + String(temp) + "h" + String(hum) + "d" + String(distance);
  sndString.toCharArray(sndMsg, 15);
  Serial.println(sndMsg);
  HC12.write(sndMsg);
  delay(1000);
  }
