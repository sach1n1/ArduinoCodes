//Connect to Digital port of wio Link

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SeeedOLED.h>

/*macro definition of Speaker pin*/
#define SPEAKER 2
char k='1';
int BassTab[]={1911,1702,1516,1431,1275,1136,1012};//bass 1~7

const char* ssid = "";
const char* password =  "";
#define mqtt_server "192.168.20.185"
WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
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

void pinInit()
{
    pinMode(SPEAKER,OUTPUT);
    digitalWrite(SPEAKER,LOW);
}

void sound(uint8_t note_index)
{
    for(int i=0;i<100;i++)
    {
        digitalWrite(SPEAKER,HIGH);
        delayMicroseconds(BassTab[note_index]);
        digitalWrite(SPEAKER,LOW);
        delayMicroseconds(BassTab[note_index]);
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    k=(char)payload[0];

  if(k=='1'){
    sound(1);
    delay(500);
  }else{
    sound(0);
    delay(500);
  }
}

bool checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
         (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}




void setup()
{
    pinMode(15,OUTPUT);
    digitalWrite(15,HIGH);
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
   client.setCallback(callback);
   pinInit();
}
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  client.subscribe("/Fiware/Wio_Link/ScreenT");
  delay(1000);
}
