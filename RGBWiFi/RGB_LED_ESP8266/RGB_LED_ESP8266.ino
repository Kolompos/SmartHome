#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "EEPROMstuff.h"
#include <FS.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// ------------------------------------------------------------------------------------------ OBJECTS
ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);     // protocol, poolServerName, timeOffset in seconds, updateInterval in millis

// ------------------------------------------------------------------------------------------ DEFINES
#define WIFI_SSID "Mha Hart Mah Sole"
//#define WIFI_PW  "aezakmi123"
// requires support on windows 10 which is not state of the art yet...
//#define MDNS_ADDRESS "ESPI"

//#define HEARTBEAT_PERIOD 120000 // in ms - All of a sudden started causing CPU crash after implementing EEPROM read on startup:/
#ifdef HEARTBEAT_PERIOD
  uint32_t timer;
#endif

// EEPROM
#define RETAIN_COMMAND_FOR              86400   // in seconds
#define LAST_COMMAND_EPOCH_ADDRESS      0
#define EFFECT_ADDRESS                  4
#define COLOR_ADDRESS                   5
#define SPEED_ADDRESS                   9
#define BRIGHTNESS_ADDRESS              10
#define OFFSET_TO_NEXT_STRIP_DATA       10

// strips settings
#define NUMBER_OF_STRIPS                2

#define LOGO_PIN                        D4
#define LOGO_LED_COUNT                  14

#define FRAME_PIN                       D3
#define FRAME_LED_COUNT                 104

// ------------------------------------------------------------------------------------------ VARIABLES
uint32_t lastCommandEpochTick, seedGlobal;

bool isCommandValid;

// ------------------------------------------------------------------------------------------ OWN SOURCE FILES
#include "Strip.h"
Strip strips[NUMBER_OF_STRIPS] = { { LOGO_PIN,LOGO_LED_COUNT,0 } , { FRAME_PIN,FRAME_LED_COUNT,1 } };
#include "Handles.h"

// ------------------------------------------------------------------------------------------ SETUP
void setup(void) 
{
  // ------------------------------ SERIAL
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PW);
  Serial.println("");

  // ------------------------------ WIFI
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address is ");
  Serial.println(WiFi.localIP());

  // ------------------------------ MDNS
  #ifdef MDNS_ADDRESS
    if (MDNS.begin(MDNS_ADDRESS))
    {
      Serial.print("MDNS responder started on http://");
      Serial.print(MDNS_ADDRESS);
      Serial.println(".local");
  
      MDNS.addService("http", "tcp", 80);
    }
  #endif
  
  // ------------------------------ HANDLES
  server.onNotFound(handleNotFound);

  server.on("/command", handleCommand);
  
  // ------------------------------ BEGINS
  SPIFFS.begin(); 
  EEPROM.begin(64);   //start eeprom with 64byte
  server.begin();
  Serial.println("HTTP server started");
  timeClient.begin();
  timeClient.forceUpdate();

  // ------------------------------ EEPROM
  lastCommandEpochTick = EEPROMRead_uint32_t(LAST_COMMAND_EPOCH_ADDRESS);
  if(lastCommandEpochTick + RETAIN_COMMAND_FOR > timeClient.getEpochTime())
  {
    // if command is valid
    isCommandValid = true;
    // load last command's parameters
    for(uint8_t index = 0; index < NUMBER_OF_STRIPS; index++)
    {
      strips[index].loadConfigFromEEPROM();
    }
  }
  else
  {
    // use default values since last command is now invalid
    for(uint8_t index = 0; index < NUMBER_OF_STRIPS; index++)
    {
      strips[index].setDefaultConfig();
    }

  }
}

// ------------------------------------------------------------------------------------------ LOOP
void loop(void)
{
  #ifdef MDNS_ADDRESS
    MDNS.update();
  #endif
  
  server.handleClient();
  timeClient.update();

  seedGlobal = micros();
  
  for(uint8_t index = 0; index < NUMBER_OF_STRIPS; index++)
  {
    strips[index].render(seedGlobal);
  }

  // ------------------------------ DEBUG AND MAINTENANCE
  if (Serial.available())
  {
    uint8_t command = Serial.read();
    if (command == (uint8_t)'r')
      ESP.restart();
    else if (command == (uint8_t)'m')
      Serial.println(system_get_free_heap_size());
    else if (command == (uint8_t)'t')
    {
      Serial.print("Ellapsed minutes since boot: ");
      Serial.println(millis()/60000);
      Serial.print("Time from NTP server: ");
      Serial.println(timeClient.getFormattedTime());
    }
    else if (command == (uint8_t)'u')
    {
      for(uint8_t index = 0; index < NUMBER_OF_STRIPS; index++)
      {
        Serial.print("Strip number ");
        Serial.print(index);
        Serial.print("'s URL is ");
        Serial.println(strips[index].getConfigAsURL());
      }
    }
    else if (command == (uint8_t)'b')
    {
      for(uint8_t index = 0; index < NUMBER_OF_STRIPS; index++)
      {
        Serial.print("Strip number ");
        Serial.print(index);
        Serial.print("'s renderable is  ");
        Serial.println(strips[index].renderable);
      }
    }
    
  }
  
  #ifdef HEARTBEAT_PERIOD
    if(millis() > timer)
    {
      timer = millis() + HEARTBEAT_PERIOD;
      Serial.print("Elapsed minutes: ");
      Serial.println(millis()/60000);
      Serial.print("Free bytes in RAM: ");
      Serial.println(system_get_free_heap_size());
    }
   #endif
}
