//Connect to port D2 of Wio Link

#include<Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
/*macro definitions of PIR motion sensor pin and LED pin*/
#define PIR_MOTION_SENSOR 2//Use pin 2 to receive the signal from the module

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
    pinMode(PIR_MOTION_SENSOR, INPUT);
    Serial.begin(9600); 
}

void loop()
{
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  Serial.println("   ");
    if(digitalRead(PIR_MOTION_SENSOR))//if it detects the moving people?
        client.publish("FIWARE/Wio_Link/Magnet","People Around",true);
    else
        client.publish("FIWARE/Wio_Link/Magnet","Nobody around or nobody moving",true);

 delay(1000);
}
