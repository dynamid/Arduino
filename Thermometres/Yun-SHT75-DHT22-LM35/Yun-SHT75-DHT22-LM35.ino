/* Pour activiation IPV6, tuto suii sur Y005 : 
 *    https://www.rivy.org/2012/09/enable-ipv6-on-openwrt/
 * 
 * opkg update
 * opkg install kmod-ipv6 ip6tables radvd ip
 */

#include <Sensirion.h>
#include <Bridge.h>
#include <HttpClient.h>
#include "DHT.h"


#define DEBUG 0
String SERVEUR = String("http://hell.citi.insa-lyon.fr/monitoring/publish/");
//String SERVEUR = String("http://cyrrus.citi.insa-lyon.fr:666/publish/");
//String SERVEUR = String("http://192.168.240.225:6666/publish/");
//curl http://[fe80::224:e8ff:fe73:5bd8%25eth1]:666/publish/11:22:33:44:55:66/SHT75/0?Temp=30&Hum=80 

//curl -6 http://hell.citi.insa-lyon.fr/monitoring/publish/11:22:33:44:55:66/SHT75/0?Temp=30\&Hum=80


// ping6 fe80::224:e8ff:fe73:5bd8 -c 3 ---> ça marche
// ping6 fe80::224:e8ff:fe73:5bd8 -c 3 -I eth1 ---> ça marche


//============================== SHT75 ==============================
#define SHT75dataPin  8
#define SHT75clockPin 9
float SHT75temp;
float SHT75humidity;
float SHT75dewpoint;


float SHT75mintemp;
float SHT75minhum;
float SHT75mindew;

float SHT75maxtemp;
float SHT75maxhum;
float SHT75maxdew;

//unsigned long uptime;
Sensirion SHT75Sensor = Sensirion(SHT75dataPin, SHT75clockPin);



//============================== DHT22 ==============================
#define DHTPIN        2 
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

DHT dht(DHTPIN, DHTTYPE);


//==============================  LM35  ==============================
/*
 * 11 : +5V
 * A0 : mesure température analogique
 */

#define LM35valuePin  A0
long LM35Val=0;
float LM35temp=0;




//==============================  MISC  ==============================
#include <Process.h>
String ID;



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


  // Get an ID  
  Process getMACProcess;
  getMACProcess.runShellCommand(F("ifconfig wlan0 | grep HWaddr | sed s/.*HWaddr//"));
  while (getMACProcess.available() > 0) {
    ID=String(getMACProcess.readStringUntil("\n"));
    ID.trim();
    Serial.println("MAC address : "+ID);
  }

  SERVEUR = SERVEUR + ID+"/";


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




  // Publication vers un serveur

  //HttpClient client;
  
  //Bridge.put("Temperature",String(SHT75temp));
  //Bridge.put("Humidity",String(SHT75humidity));
  //Bridge.put("Point de rosee",String(SHT75dewpoint));
  //put("Analogic Temperature",String(LM35temp));
  //put("DHT22 Temp",String(DHT22temp));
  //put("DHT22 Hum",String(DHT22humidity));
  //put("DHT22 heat index",String(DHT22HI));

  //client.get(SERVEUR+"SHT75/0?Temp="+SHT75temp+"&Hum="+SHT75humidity);
  //client.get(SERVEUR+"DHT22/0?Temp="+DHT22temp+"&Hum="+DHT22humidity);
  //client.get(SERVEUR+"LM35/0?Temp="+LM35temp);

  
//http://127.0.0.1:666/publish/11:22:33:44:55:66/SHT75/0?Temp=30\&Hum=80



  Process getTimeStampProcess;
  getTimeStampProcess.runShellCommand(F("date -R"));
  String timeStamp;
  while (getTimeStampProcess.available() > 0) {
    timeStamp=String(getTimeStampProcess.readStringUntil("\n"));
    timeStamp.trim();
    Serial.println("timeStamp : "+timeStamp);
  }

  Bridge.put(String("hell/SHT75/0"),String("Temp=")+String(SHT75temp)+"&Hum="+String(SHT75humidity));
  Bridge.put(String("hell/DHT22/0"),String("Temp=")+String(DHT22temp)+"&Hum="+String(DHT22humidity));
  Bridge.put(String("hell//timeStamp"),timeStamp);
  Bridge.put(String("hell//ID"),ID);
  
    //curl -6 http://hell.citi.insa-lyon.fr/monitoring/publish/11:22:33:44:55:66/SHT75/0?Temp=30\&Hum=80



  // DEBUG  
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
