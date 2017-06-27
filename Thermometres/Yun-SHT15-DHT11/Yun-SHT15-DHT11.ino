#include <Bridge.h>
#include <Sensirion.h>
#include "DHT.h"
 
 

#define DEBUG 0

//============================== SHT15 ==============================
#define SHT15dataPin  8
#define SHT15clockPin 9
float SHT15temp;
float SHT15humidity;
float SHT15dewpoint;


//unsigned long uptime;
Sensirion SHT15Sensor = Sensirion(SHT15dataPin, SHT15clockPin);
/*
SHT15
=====
            +-—+
 Ground  6 -|  |- NC
   Data  2 -|  |- NC
  Clock  3 -|  |- NC
    Vcc 10 -|  |- NC
            \__/

Vcc : 3,3 normalement (supporte de 2,4 à 5,5V)


Connecteur du PCB :

+--------------------+
|                    |
|       PCB          |
|                    |
|                    |
|      +-----------+ |
|      | 1 2 3 4 5 | |
|      | 6 7 8 9 10| |
|      +-----------+ |
+--------------------+
 */


//============================== DHT11 ==============================
#define DHTPIN        2
#define DHTTYPE DHT11   
float DHT11humidity;
float DHT11temp;
float DHT11HI;

DHT dht(DHTPIN, DHTTYPE);


//==============================  MISC  ==============================
#include <Process.h>
String ID;



void setup()
{
  Bridge.begin();
  if(DEBUG==1) {
    Serial.begin(9600);
  }

 
  delay(4000); // Attente initiale de 4secondes

 

  // DHT11 (AM2302)
  dht.begin();
  DHT11humidity = dht.readHumidity();
  DHT11temp = dht.readTemperature();
  DHT11HI = dht.computeHeatIndex(DHT11temp, DHT11humidity, false);


  // SHT15 init
  SHT15Sensor.measure(&SHT15temp, &SHT15humidity, &SHT15dewpoint);



  // Get an ID  
  Process getMACProcess;
  getMACProcess.runShellCommand(F("ifconfig wlan0 | grep HWaddr | sed s/.*HWaddr//"));
  while (getMACProcess.available() > 0) {
    ID=String(getMACProcess.readStringUntil("\n"));
    ID.trim();
    Serial.println("MAC address : "+ID);
  }

}




void loop() {
  // ================== Getting time stamp =====================
  Process getTimeStampProcess;
  getTimeStampProcess.runShellCommand(F("date -R"));
  String timeStamp;
  while (getTimeStampProcess.available() > 0) {
    timeStamp=String(getTimeStampProcess.readStringUntil("\n"));
    timeStamp.trim();
    Serial.println("timeStamp : "+timeStamp);
  }




  // ====================      DEBUG     =======================
  if(DEBUG==1) {
    Serial.print("DHT11: ");
    Serial.print(DHT11temp);
    Serial.print(" C  /  ");
    Serial.print(DHT11humidity);
    Serial.print(" %  /  ");
    Serial.print(DHT11HI);
    Serial.println(" C ressentis");

    
    Serial.print("SHT15 : ");
    Serial.print(SHT15temp);
    Serial.print(" C, Humidity: ");
    Serial.print(SHT15humidity);
    Serial.print(" %, Dewpoint: ");
    Serial.print(SHT15dewpoint);
    Serial.println(" C");
  }


  // ==================== Publish values =======================
  Bridge.put("uptime",String(millis()/1000));
  Bridge.put("Point de rosee",String(SHT15dewpoint));
  Bridge.put(String("hell/SHT15/0"),String("Temp=")+String(SHT15temp)+"&Hum="+String(SHT15humidity));
  Bridge.put(String("hell/DHT11/0"),String("Temp=")+String(DHT11temp)+"&Hum="+String(DHT11humidity));
  Bridge.put(String("hell//timeStamp"),timeStamp);
  Bridge.put(String("hell//ID"),ID);

  Bridge.put("DHT11 heat index",String(DHT11HI));


  
  // ====================     Waiting    =======================
  if(DEBUG==1) {
    delay(2000); // Mise à jour chaque 2 seconde
  } else {
    delay(600000); // Mise à jour chaque 10 minutes
  }


  // ==================== READ new values =======================

  // DHT11
  DHT11humidity = dht.readHumidity();
  DHT11temp = dht.readTemperature();
  DHT11HI = dht.computeHeatIndex(DHT11temp, DHT11humidity, false);

  // SHT15
  SHT15Sensor.measure(&SHT15temp, &SHT15humidity, &SHT15dewpoint);
}



