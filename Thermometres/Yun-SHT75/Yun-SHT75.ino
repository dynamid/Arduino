#include <Sensirion.h>
#include <Bridge.h>

#define DEBUG 0

//============================== SHT75 ==============================
#define SHT75dataPin  8
#define SHT75clockPin 9
float SHT75temp;
float SHT75humidity;
float SHT75dewpoint;


//unsigned long uptime;
Sensirion SHT75Sensor = Sensirion(SHT75dataPin, SHT75clockPin);


/*
SHT75
=====

   +-—+
   |  |
   \  /
    \/
    |
 +-+++-+
 | | | |
 C V G D
 8 1 5 9

C = Clock   -> Jaune
V = Vcc= 5V -> Rouge
G = GND     -> Noir
D = Data    -> Vert

Connecteur au bout de câble :
    +---+
+---+   +---+
| 1 2 3 4 5 |
| 6 7 8 9 10|
+-----------+

Placement des couleurs :
    +---+
+---+   +---+
| R . . . N |
| . . J V . |
+-----------+

Branchements sur Arduino : 
  Rouge : 5V
  Noir  : GND
  Vert  : 8
  Jaune : 9

*/




//==============================  MISC  ==============================
#include <Process.h>
String ID;




void setup()
{
  // Initializing serials
  Bridge.begin();
  if(DEBUG==1) {
    Serial.begin(9600);
  }

  // Don't make any measurement at the starting moment.
  delay(4000);

  // SHT75 init
  SHT75Sensor.measure(&SHT75temp, &SHT75humidity, &SHT75dewpoint);


  // Getting the MAC address and using it as an ID
  Process getMACProcess;
  getMACProcess.runShellCommand(F("ifconfig wlan0 | grep HWaddr | sed s/.*HWaddr//"));
  while (getMACProcess.available() > 0) {
    ID=String(getMACProcess.readStringUntil("\n"));
    ID.trim();
    Serial.println("MAC address : "+ID);
  }


  // Getting the MAC address and using it as an ID
  getMACProcess.runShellCommand(F("ifconfig wlan0 | grep 134.214 | sed s/.*addr:// | sed s/Bcast:.*//"));
  while (getMACProcess.available() > 0) {
    String IP=String(getMACProcess.readStringUntil("\n"));
    IP.trim();
    Serial.println("IP address : "+IP);
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
    Serial.print("Temperature: ");
    Serial.print(SHT75temp);
    Serial.print(" C, Humidity: ");
    Serial.print(SHT75humidity);
    Serial.print(" %, Dewpoint: ");
    Serial.print(SHT75dewpoint);
    Serial.println(" C");
  }

  
  // ==================== Publish values =======================
  Bridge.put("uptime",String(millis()/1000));
  Bridge.put("Point de rosee",String(SHT75dewpoint));
  Bridge.put(String("hell/SHT75/0"),String("Temp=")+String(SHT75temp)+"&Hum="+String(SHT75humidity));
  Bridge.put(String("hell//timeStamp"),timeStamp);
  Bridge.put(String("hell//ID"),ID);
  


  // ====================     Waiting    =======================
  if(DEBUG==1) {
    delay(2000); // Mise à jour chaque 2 seconde
  } else {
    delay(600000); // Mise à jour chaque 10 minutes
  }


  // ==================== READ new values =======================
  SHT75Sensor.measure(&SHT75temp, &SHT75humidity, &SHT75dewpoint);
}
