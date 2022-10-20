#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#define sensorId  1 //identifier
#define sensorLoc  "weird sensor //identifier
"
#define analogPin A0
int adcValue = 0;

const char* serverName = "YOUR_SERVER_URL_ENDPOINT";  //Might look something like http://rpi.local:5001/Home/PostTemp if you set up the project on a raspberry pi similarly

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to WiFi!");
}

void loop() {
  adcValue = analogRead(analogPin);

  if ((millis() - lastTime) > timerDelay) {

    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);

      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String httpRequestData = "Id=" + String(sensorId) + "&Location=" + String(sensorLoc) + "&RawReading=" + String(adcValue);           

      Serial.println(httpRequestData);
      int httpResponseCode = http.POST(httpRequestData);

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();

      delay(600000); //10min
    }
    else {
      Serial.println("WiFi Disconnected");
      WiFi.begin(ssid, password);
      Serial.println("Connecting");
      while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.print("Connected to WiFi!");
    }
    lastTime = millis();
  }
}
