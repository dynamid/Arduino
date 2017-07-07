#include <OneWire.h>
#include <DallasTemperature.h>
#include <Process.h>
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 8
#include <Bridge.h>
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

String ID;

 
void setup(void)
{
  // start serial port
  Serial.begin(9600);
 // delay(2000);
  Serial.println("Dallas Temperature IC Control Library Demo");
  digitalWrite(12,HIGH);
  
   // Getting the MAC address and using it as an ID

  Bridge.begin();

  Process getMACProcess;
  Serial.println("ifconfig wlan0 | grep HWaddr | sed s/.*HWaddr//");
  getMACProcess.runShellCommand(F("ifconfig wlan0 | grep HWaddr | sed s/.*HWaddr//"));
  while (getMACProcess.available() > 0) {
    ID=String(getMACProcess.readStringUntil("\n"));
    ID.trim();
    Serial.println("MAC address : "+ID);
  }

  
  getMACProcess.runShellCommand(F("ifconfig wlan0 | grep 134.214 | sed s/.*addr:// | sed s/Bcast:.*//"));
  while (getMACProcess.available() > 0) {
    String IP=String(getMACProcess.readStringUntil("\n"));
    IP.trim();
    Serial.println("IP address : "+IP);
  }
  
  // Start up the library
  sensors.begin();
}
 
 
void loop(void)
{
  // ================== Getting time stamp =====================
  
  Process getTimeStampProcess;
  getTimeStampProcess.runShellCommand(F("date -R"));
  String timeStamp=String("");
  while (getTimeStampProcess.available() > 0) {
    timeStamp.concat(String(getTimeStampProcess.readStringUntil("\n")));
  }
  timeStamp.trim();
  Serial.println("timeStamp : "+timeStamp);

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("Temperature is: ");
  float temp=sensors.getTempCByIndex(0);
  Serial.println(String(temp)); // Why "byIndex"? 
    // You can have more than one IC on the same bus. 
    // 0 refers to the first IC on the wire

  Serial.println("Publish on bridge");
  Bridge.put(String("hell/DS18B20/0"),String("Temp=")+String(temp));
  Bridge.put(String("hell//timeStamp"),timeStamp);
  Bridge.put(String("hell//ID"),ID);
  
  delay(600000);
}

