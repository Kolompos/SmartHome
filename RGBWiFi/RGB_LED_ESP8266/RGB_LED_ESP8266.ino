#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "EEPROMstuff.h"
#include <FS.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// ------------------------------------------------------------------------------------------ OBJECTS
ESP8266WebServer server(80);
//HTTPClient httpClient;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);

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


// ------------------------------------------------------------------------------------------ VARIABLES

const uint8_t LOGO_PIN = D4;
const uint16_t LOGO_LED_COUNT = 14;

const uint8_t FRAME_PIN = D3;
const uint16_t FRAME_LED_COUNT = 104;

uint32_t lastCommandEpochTick;

bool isCommandValid;

// ------------------------------------------------------------------------------------------ OWN SOURCE FILES
#include "Strip.h"
Strip stripLogo(LOGO_PIN, LOGO_LED_COUNT, 0);
Strip stripFrame(FRAME_PIN, FRAME_LED_COUNT, 1);
#include "Handles.h"

// ------------------------------------------------------------------------------------------ FUNCTIONS
uint16_t getArgValue(String name)
{
  for (uint8_t i = 0; i < server.args(); i++)
    if (server.argName(i) == name)
      return server.arg(i).toInt();
  return -1;
}

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
  timeClient.update();

  // ------------------------------ EEPROM
  lastCommandEpochTick = EEPROMRead_uint32_t(LAST_COMMAND_EPOCH_ADDRESS);
  
  if(lastCommandEpochTick + RETAIN_COMMAND_FOR > timeClient.getEpochTime())
  {
    // if command is valid
    isCommandValid = true;
    // load last command's parameters
    // TODO: create function in strip class that does EEPROM reads.
    stripLogo.setEffect(EEPROM.read(stripLogo.indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + EFFECT_ADDRESS));
    stripLogo.setColor(EEPROMRead_uint32_t(stripLogo.indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + COLOR_ADDRESS));
    stripLogo.setSpeed(EEPROM.read(stripLogo.indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + SPEED_ADDRESS));
    stripLogo.setBrightness(EEPROM.read(stripLogo.indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + BRIGHTNESS_ADDRESS));
    Serial.println("LOADED VALUES FROM EEPROM");
  }
  else
  {
    // use default values since last command is now invalid
    // TODO: create function in strip class that sets default values.
    stripLogo.setEffect(0);
    stripLogo.setColor(0);
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
  
  stripLogo.render();

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
    else if (command == (uint8_t)'t')
    {
      Serial.print("Ellapsed minutes since boot: ");
      Serial.println(millis()/60000);
      Serial.print("Time from NTP server: ");
      Serial.println(timeClient.getFormattedTime());
    }
    else if (command == (uint8_t)'s')
    {
      stripLogo.setColor(200,200,200);
      stripLogo.setEffect(1);
      Serial.println("Effect 1 set");
    }
    else if (command == (uint8_t)'o')
    {
      stripLogo.setEffect(0);
      Serial.println("Effect 0 set");
    }
    else if (command == (uint8_t)'b')
    {
      stripLogo.setEffect(2);
      stripLogo.setColor(200,200,200);
      Serial.println("Effect 2 set");
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
