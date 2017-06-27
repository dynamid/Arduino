void setup() // Uno : READER
{
  // Initializing serials
  Serial.begin(9600);

  delay(1000);
}




void loop() {
  Serial.println("Serial");

  while (Serial.available() > 0) {
    String s = Serial.readString();
    Serial.println("Lu : " + s);
  }

  delay(500);
}


