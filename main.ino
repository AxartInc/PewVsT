#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>  
#include <SSD1306.h>
#include <time.h>
#include <YoutubeApi.h>  

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

SSD1306  display(0x3c, D1, D2); //the display I2C address, SCL and SDA pins
 

const char* ssid     = "name";                 // SSID of local network
const char* password = "password";       // Password on network

#define API_KEY "api key"

#define CHANNEL_ID "UC-lHJZR3Gqxm24_Vd_AJ5Yw" //PEW
#define CHANNEL_ID_T "UCq-Fj5jknLsUf-MWSy4_brA" //T

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);


unsigned long api_mtbs = 5000; //mean time between api requests
unsigned long api_lasttime;

long sub = 0;
long subs = 0;
long subs_t = 0;

void setup() {
  Serial.begin(115200);
  
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  Serial.setDebugOutput(true);
  display.clear();
  display.drawString(0, 19, "Booting...");
  display.display();
  delay(250);
  display.clear();
  display.drawString(0, 6, "Searching");
  display.drawString(0, 32, "Wi-Fi");
  display.display();
  display.clear();
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 6, "Retrying");
    display.drawString(0, 32, "Wi-Fi");
    delay(5000);
    
  } 

  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  
    display.clear();
    display.setFont(ArialMT_Plain_24);
    Serial.println("Wi-Fi Connected");
    display.drawString(0, 6, "Wi-Fi");
    display.drawString(0, 32, "Connected");
    delay(500);
    display.display();

}

void loop() 
{  


  if (millis() > api_lasttime + api_mtbs)  {


    if(api.getChannelStatistics(CHANNEL_ID))
    {

      Serial.println("---------Stats PEW---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      subs = api.channelStats.subscriberCount;
      Serial.println("------------------------");
     }
     
     if(api.getChannelStatistics(CHANNEL_ID_T))
     {


  
      Serial.println("---------Stats T SERIEs---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      subs_t = api.channelStats.subscriberCount;
      Serial.println("------------------------");
     }
     
      
      display.clear();
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 2, "Sub Gap Pew/T-");
      sub = subs - subs_t;
      display.setFont(ArialMT_Plain_24);
      display.drawString(0, 24, String(sub));
      display.display();
 
    
    
    api_lasttime = millis();
    
  }
}
