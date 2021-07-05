//Created by: Slavko Zdravevski

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
s
const char* ssid = "NAME"; // your name of your car's wifi
const char* password = "PASSWORD"; // your password of your car's wifi
ESP8266WebServer server(80);

int lockingOptocoupler = D5;
int unlockingOptocoupler = D6;
bool serverStateChanged = false;
bool carIsLocked = false;

void setup() {
  Serial.begin(115200);
  
  pinMode(lockingOptocoupler, OUTPUT);
  pinMode(unlockingOptocoupler, OUTPUT);
  turnOffOptocouplers();
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on ( "/", HTTP_handleRoot );
  server.onNotFound ( HTTP_handleRoot );
  server.begin();  
}

void loop() {
  if (serverStateChanged) {
    carIsLocked = !carIsLocked;
    lockOrUnlockCar();
    serverStateChanged = false;
  }

  server.handleClient();
}

void lockOrUnlockCar () {
  Serial.println("Lock or unlock the car");
  if (carIsLocked) {
    digitalWrite(unlockingOptocoupler, HIGH);
  } else {
    digitalWrite(lockingOptocoupler, HIGH);
  }
  delay(1000);
  turnOffOptocouplers();
  delay(1000);
}

void turnOffOptocouplers () {
  digitalWrite(unlockingOptocoupler, LOW);
  digitalWrite(lockingOptocoupler, LOW);
}

void HTTP_handleRoot(void) {
    if(server.hasArg("state")){
      serverStateChanged = true;
    }
  delay(1);
}
