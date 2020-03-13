//Connect to port D2 of Wio Link 

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "bmm150.h"
#include "bmm150_defs.h"


const char* ssid = "FIWARE";
const char* password =  "!FIWARE!on!air!";
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

#define LED 2 //the onboard LED of Arduino or Seeeduino
#define COLLISION_SENSOR 13//collision sensor is connected with D2 of Arduino

void turnOnLED()
{
    digitalWrite(LED,HIGH);//the LED is on
}

void turnOffLED()
{
    digitalWrite(LED,LOW);//the LED is off
}

void pins_init()
{
    pinMode(LED,OUTPUT);
    turnOffLED();
    pinMode(COLLISION_SENSOR,INPUT);
}

boolean isTriggered()
{
    if(!digitalRead(COLLISION_SENSOR))
    {
        delay(5);
        if(!digitalRead(COLLISION_SENSOR))
        return true;//the collision sensor triggers
    }
    return false;
}



void setup()
{
  pinMode(15,OUTPUT);
  digitalWrite(15,HIGH);
  pins_init();
 Serial.begin(9600);
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
}

void loop()
{

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  Serial.println("   ");
  
    if(isTriggered())
    {
        turnOffLED();
        client.publish("FIWARE/Wio_Link/Magnet","Triggered",true);
  delay(1000);
        //delay(2000);
    }
    else turnOnLED();
}

