//Connect to digital port D2 of Wio Link

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Ultrasonic.h"

Ultrasonic ultrasonic(13);
const char* ssid = "";
const char* password =  "";
#define mqtt_server "192.168.20.185"
WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
bool checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
         (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}

void setup()
{
    pinMode(15,OUTPUT);
    digitalWrite(15,1);
    WiFi.begin(ssid, password);
    Serial.print("Connecting");
   while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
     Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqtt_server, 1883);
    Serial.begin(9600);
}
void loop()
{
    long RangeInInches;
      if (!client.connected()) {
        reconnect();
     }
   client.loop();
   Serial.println("   ");
   
   delay(1000);

    Serial.println("The distance to obstacles in front is: ");
    RangeInInches = ultrasonic.MeasureInInches();
    Serial.print(RangeInInches);//0~157 inches
    Serial.println(" inch");
    client.publish("FIWARE/Wio_Link/Magnet",String(RangeInInches).c_str(),true);
   // delay(250);

    //RangeInCentimeters = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
    //Serial.print(RangeInCentimeters);//0~400cm
    //Serial.println(" cm");
    delay(1000);
}
