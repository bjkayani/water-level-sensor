
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

SoftwareSerial HC12(14, 12); // HC-12 TX Pin, HC-12 RX Pin

String incomMsg;

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

String temp;
String hum;
String distance;
String water_level;

float distance_f;
float water_level_f;

char auth[] = "";

char ssid[] = "";
char pass[] = "";

BlynkTimer timer;

const int min_dist = 30;
const int max_dist = 140;

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  Wire.begin(D4,D3);
  lcd.begin();  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
  while (HC12.available()) {       
    incomMsg = HC12.readString();
  }

}

void myTimerEvent()
{
  temp = (incomMsg.substring(incomMsg.indexOf("t")+1,incomMsg.indexOf("h")-1));
  hum = (incomMsg.substring(incomMsg.indexOf("h")+1,incomMsg.indexOf("d")));
  if((incomMsg.substring(incomMsg.indexOf("d")+1,incomMsg.indexOf("t",5))).toFloat() != -1){
      distance = (incomMsg.substring(incomMsg.indexOf("d")+1,incomMsg.indexOf("t",5)));
  }
  distance_f = distance.toFloat();
  water_level_f = (1-(distance_f-min_dist)/(max_dist - min_dist))*100;
  lcd.setCursor(0,0);
  lcd.print("Level: ");
  lcd.print(int(water_level_f));
  lcd.print("%");
  lcd.print("      ");
  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print("C");
  lcd.print(" H:");
  lcd.print(hum);
  lcd.print("%   ");
  Serial.println(incomMsg);

  Blynk.virtualWrite(V0, int(water_level_f));
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, temp);
}
