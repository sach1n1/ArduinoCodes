//Connect to I2C port of Wio_Link board
//For Generic connect to 5v,GND and SCL, SDA pins
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SeeedOLED.h>

const char* ssid = "FIWARE";
const char* password = "!FIWARE!on!air!";

#define mqtt_server "192.168.20.185"
WiFiClient espClient;
PubSubClient client(espClient);
char k='1';
  void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to ("ESP8266Client","username","password")
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  k=(char)payload[0];
  if(k=='1'){
    SeeedOled.clearDisplay();          
    SeeedOled.setNormalDisplay();      
    SeeedOled.setPageMode();           
    SeeedOled.setTextXY(0, 0);         
    SeeedOled.putString("Switch is on!"); //Message for Switch on 
  }else{
    SeeedOled.clearDisplay();          
    SeeedOled.setNormalDisplay();      
    SeeedOled.setPageMode();           
    SeeedOled.setTextXY(0, 0);         
    SeeedOled.putString("Switch is off!"); //Message for off
  }
  Serial.print(k); //Checking payload on Serial Monitor 
}

bool checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
    (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}

void setup()
{
  pinMode(15, OUTPUT);
  digitalWrite(15, 1);
  Serial.begin(9600);
  Wire.begin(4,5);
  SeeedOled.init();
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
  pinMode(13, INPUT); 
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