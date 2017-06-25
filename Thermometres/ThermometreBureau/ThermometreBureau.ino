#include <Sensirion.h>
#include <Bridge.h>
#include "DHT.h"

#define SHT75dataPin  8
#define SHT75clockPin 9
#define LM35valuePin  A0
#define DHTPIN        2 

#define DEBUG 0

float SHT75temp;
float SHT75humidity;
float SHT75dewpoint;


float SHT75mintemp;
float SHT75minhum;
float SHT75mindew;

float SHT75maxtemp;
float SHT75maxhum;
float SHT75maxdew;


#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
float DHT22humidity;
float DHT22temp;
float DHT22HI;


float DHT22mintemp;
float DHT22minhum;
float DHT22minHI;

float DHT22maxtemp;
float DHT22maxhum;
float DHT22maxHI;



/*
 * 11 : +5V
 * A0 : mesure température analogique
 */


//unsigned long uptime;

Sensirion SHT75Sensor = Sensirion(SHT75dataPin, SHT75clockPin);

long LM35Val=0;
float LM35temp=0;

DHT dht(DHTPIN, DHTTYPE);


void setup()
{
  Bridge.begin();
  if(DEBUG==1) {
    Serial.begin(9600);
  }
  
  // Génération de +5V pour sonde analogique
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);
 
  delay(4000); // Attente initiale de 4secondes

  // SHT75
  SHT75Sensor.measure(&SHT75temp, &SHT75humidity, &SHT75dewpoint);
  SHT75mintemp=SHT75temp;
  SHT75maxtemp=SHT75temp;
  SHT75minhum=SHT75humidity;
  SHT75maxhum=SHT75humidity;
  SHT75mindew=SHT75dewpoint;
  SHT75maxdew=SHT75dewpoint;

  // LM35 
  LM35Val=analogRead(LM35valuePin);


  // DHT22 (AM2302)
  dht.begin();
  DHT22humidity = dht.readHumidity();
  DHT22temp = dht.readTemperature();
  DHT22HI = dht.computeHeatIndex(DHT22temp, DHT22humidity, false);

  DHT22mintemp=DHT22temp;
  DHT22maxtemp=DHT22temp;
  DHT22maxhum=DHT22humidity;
  DHT22minhum=DHT22humidity;
  DHT22minHI=DHT22HI;
  DHT22maxHI=DHT22HI;
}

void loop() {

  // ==================== Publish values =======================

  Bridge.put("uptime",String(millis()/1000));

  // SHT75
  Bridge.put("Temperature",String(SHT75temp));
  Bridge.put("Humidity",String(SHT75humidity));
  Bridge.put("Point de rosee",String(SHT75dewpoint));

  Bridge.put("Min Temperature",String(SHT75mintemp));
  Bridge.put("Min Humidity",String(SHT75minhum));
  Bridge.put("Min Point de rosee",String(SHT75mindew));

  Bridge.put("Max Temperature",String(SHT75maxtemp));
  Bridge.put("Max Humidity",String(SHT75maxhum));
  Bridge.put("Max Point de rosee",String(SHT75maxdew));

  if(DEBUG==1) {
    Serial.print("Temperature: ");
    Serial.print(SHT75temp);
    Serial.print(" C, Humidity: ");
    Serial.print(SHT75humidity);
    Serial.print(" %, Dewpoint: ");
    Serial.print(SHT75dewpoint);
    Serial.println(" C");
  }


  // LM35
  LM35temp=LM35Val*0.48828125;
  Bridge.put("Analogic Temperature",String(LM35temp));

  if(DEBUG==1) {
    Serial.print("Temperature analogique: ");
    Serial.print(LM35temp);
    Serial.println(" C");
  }


  // DHT22
  Bridge.put("DHT22 Temp",String(DHT22temp));
  Bridge.put("DHT22 Hum",String(DHT22humidity));
  Bridge.put("DHT22 heat index",String(DHT22HI));

  Bridge.put("DHT22 min Temp",String(DHT22mintemp));
  Bridge.put("DHT22 min Hum",String(DHT22minhum));
  Bridge.put("DHT22 min heat index",String(DHT22minHI));
  Bridge.put("DHT22 max Temp",String(DHT22maxtemp));
  Bridge.put("DHT22 max Hum",String(DHT22maxhum));
  Bridge.put("DHT22 max heat index",String(DHT22maxHI));

  if(DEBUG==1) {
    Serial.print("DHT22: ");
    Serial.print(DHT22temp);
    Serial.print(" C  /  ");
    Serial.print(DHT22humidity);
    Serial.print(" %  /  ");
    Serial.print(DHT22HI);
    Serial.println(" C ressentis");
  }


  

  if(DEBUG==1) {
    delay(2000); // Mise à jour chaque 2 seconde
  } else {
    delay(600000); // Mise à jour chaque 10 minutes
  }


  // ==================== READ new values =======================

  // SHT75
  SHT75Sensor.measure(&SHT75temp, &SHT75humidity, &SHT75dewpoint);

  if(SHT75mintemp>SHT75temp) {SHT75mintemp=SHT75temp; }
  if(SHT75maxtemp<SHT75temp) {SHT75maxtemp=SHT75temp; }
  if(SHT75minhum>SHT75humidity) {SHT75minhum=SHT75humidity; }
  if(SHT75maxhum<SHT75humidity) {SHT75maxhum=SHT75humidity; }
  if(SHT75mindew>SHT75dewpoint) {SHT75mindew=SHT75dewpoint; }
  if(SHT75maxdew<SHT75dewpoint) {SHT75maxdew=SHT75dewpoint; }

  // LM35
  LM35Val=analogRead(A0);

  // DHT22
  DHT22humidity = dht.readHumidity();
  DHT22temp = dht.readTemperature();
  DHT22HI = dht.computeHeatIndex(DHT22temp, DHT22humidity, false);

  if(DHT22mintemp>DHT22temp)    {DHT22mintemp=DHT22temp; }
  if(DHT22maxtemp<DHT22temp)    {DHT22maxtemp=DHT22temp; }
  if(DHT22minhum>DHT22humidity) {DHT22minhum=DHT22humidity; }
  if(DHT22maxhum<DHT22humidity) {DHT22maxhum=DHT22humidity; }
  if(DHT22minHI>DHT22HI)        {DHT22minHI=DHT22HI; }
  if(DHT22maxHI<DHT22HI)        {DHT22maxHI=DHT22HI; }
}
