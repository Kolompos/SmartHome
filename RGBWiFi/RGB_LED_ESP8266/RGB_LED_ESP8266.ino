#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "EEPROMstuff.h"
#include <FS.h>

// ------------------------------------------------------------------------------------------ OBJECTS
ESP8266WebServer server(80);
WiFiUDP timeUDPclient;
NTPClient timeClient(timeUDPclient, "europe.pool.ntp.org", 7200); //europe.pool.ntp.org

// ------------------------------------------------------------------------------------------ DEFINES
// requires support on windows 10 which is not state of the art yet...
//#define MDNS_ADDRESS "ESPI"

//#define VERBOSE_MODE

#define HEARTBEAT_PERIOD 120000 // in ms

// EEPROM
#define RETAIN_COMMAND_FOR              86400   // in seconds
#define LAST_COMMAND_EPOCH_ADDRESS      0       // 4 byte
#define EFFECT_ADDRESS                  4       // 1 byte
#define COLOR_ADDRESS                   5       // 4 byte
#define SPEED_ADDRESS                   9       // 1 byte
#define BRIGHTNESS_ADDRESS              10      // 1 byte
#define OFFSET_TO_NEXT_STRIP_DATA       10      // SUM 11 byte EEPROM / strip

// ------------------------------------------------------------------------------------------ VARIABLES
uint32_t lastCommandEpochTick, seedGlobal, seedOverflowTeamFortress2;     // you know its way better ;)
uint32_t timer = HEARTBEAT_PERIOD;

bool isCommandValid;

const char daysOfTheWeek[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

// ------------------------------------------------------------------------------------------ IGNORED PART
#include "IgnoredDefines.h"

// ------------------------------------------------------------------------------------------ SETUP
void setup(void) 
{
  // ------------------------------ SERIAL
  Serial.begin(115200);

  // ------------------------------ WIFI
  Serial.println("WIFI CONNECTING...");
  WiFi.setAutoReconnect(false);
  //TODO: implement sleep mode while we have trees left...
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  //Serial.println(WiFi.hostname("ESP_" + ESP.getChipId()) ? "Hostname changed" : "Hostname not changed" );
  WiFi.config(staticIP, gateway, subnet, dns);
  WiFi.begin(WIFI_SSID, WIFI_PW);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  getWifiInfo();

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
  server.on("/ota", OTAModeOn);
  server.on("/rst", []()
  {
    server.send(200, "text/plain", "RESTARTING IN 1 SEC");
    Serial.println("RESTARTING IN 1 SEC");
    delay(1000);
    ESP.restart();
  });
  server.on("/time", []()
  {
    server.send(200, "text/plain", timeClient.getFormattedTime());
    Serial.println("/time requested from Client");
    Serial.println(timeClient.getFormattedTime());
  });

  // ------------------------------ OTA
  ArduinoOTA.onStart([]() 
  {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else  // U_SPIFFS
    {
      type = "filesystem";
    }
    EEPROM.end();
    SPIFFS.end();
    timeClient.end();
    Serial.println("Start updating " + type);
  });

  // These are not used at the time, but there is the opportunity
  /* 
  ArduinoOTA.onEnd([]()
  {
    Serial.println("\nEnd");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("End Failed");
    }
  });
  */
  
  // ------------------------------ BEGINS
  ArduinoOTA.begin(false);  // false to begin without mDNS support
  SPIFFS.begin(); 
  EEPROM.begin(64);         //start eeprom with 64byte
  server.begin();
  timeClient.setUpdateInterval(100 * HEARTBEAT_PERIOD);
  timeClient.begin();
  timeClient.forceUpdate();

  // ------------------------------ EEPROM
  lastCommandEpochTick = EEPROMRead_uint32_t(LAST_COMMAND_EPOCH_ADDRESS);
  if((lastCommandEpochTick + RETAIN_COMMAND_FOR > timeClient.getEpochTime()) && (lastCommandEpochTick < timeClient.getEpochTime()))
  {
    // if command is valid, meaning not older than RETAIN_COMMAND_FOR and not later than this moment(later occurs when red EEPROM was newer written before)
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
  Serial.println("Booted successfully");
  getDateInfo();
  getTimeInfo();
}

// ------------------------------------------------------------------------------------------ LOOP
void loop(void)
{
  #ifdef MDNS_ADDRESS
    MDNS.update();
  #endif
  
  #ifdef VERBOSE_MODE
    //Serial.print('/');    // spams '/' every loop
    //delay(10);
  #endif
  
  server.handleClient();

  // TODO: decide to keep this or OTAModeOn()
  ArduinoOTA.handle();
  
  seedGlobal = micros();
  
  for(uint8_t index = 0; index < NUMBER_OF_STRIPS; index++)
  {
    strips[index].render(seedGlobal);
  }
  
  // -------------------------------------------------------------------------- HARTBEAT
  if(millis() > timer)
  {
    // ------------------------------ OVERFLOW GUARDS
    if(millis() > 4276800000) ESP.restart();  //49,5 days, almost a millis overflow (max is 4294967295)
    timer = millis() + HEARTBEAT_PERIOD;      //based on line abowe HB_PERIOD can not be greater than 18167295 (~5hours)
    if(micros() < seedOverflowTeamFortress2)
    {
      for(uint8_t index = 0; index < NUMBER_OF_STRIPS; index++)
      {
        strips[index].resetLastRender();
      }
    }
    seedOverflowTeamFortress2 = micros();
    
    #ifdef NTP_UPDATE
      timeClient.update();
      checkAndForceUpdateTime();
    #endif
    
    // ------------------------------ BASIC HARTBEAT INFO
    #ifdef VERBOSE_MODE
      getTimeInfo();
      getMemoryInfo();
    #endif
    
    // ------------------------------ WIFI RECONNECT IN CASE LOST CONNECTION
    if(WiFi.status() != WL_CONNECTED)
    {
      Serial.println("WIFI CONNECTION LOST, RECONNECTING...");
      WiFi.reconnect();
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(1000);
        Serial.print(".");
      }
      Serial.println("");
      getWifiInfo();
    }
  }
  
  // -------------------------------------------------------------------------- SERIAL COMMAND LINE
  if (Serial.available())
  {
    uint8_t command = Serial.read();
    if (command == (uint8_t)'r')
    {
      Serial.println("Restart initiated! Restarting in 1 sec!");
      delay(1000);
      ESP.restart();
    }
    else if (command == (uint8_t)'m')
    {
     getMemoryInfo();
    }
    else if (command == (uint8_t)'c')
    {
      checkAndForceUpdateTime();
    }
    else if (command == (uint8_t)'t')
    {
      getTimeInfo();
    }
    else if (command == (uint8_t)'d')
    {
      getDateInfo();
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
    else if (command == (uint8_t)'o')
    {
      for(uint8_t index = 0; index < NUMBER_OF_STRIPS; index++)
      {
        strips[index].setEffect(0);
        strips[index].preEffectFunction();
        Serial.print("Strip number ");
        Serial.print(index);
        Serial.println(" is set to OFF");
      }
    }
    else if (command == (uint8_t)'n')
    {
      Serial.print("Wifi status is: ");
      switch(WiFi.status())
      {
        case WL_CONNECTED:
          Serial.println("WL_CONNECTED");
          break;
        case WL_NO_SHIELD:
          Serial.println("WL_NO_SHIELD");
          break;
        case WL_IDLE_STATUS:
          Serial.println("WL_IDLE_STATUS");
          break;
        case WL_NO_SSID_AVAIL:
          Serial.println("WL_NO_SSID_AVAIL");
          break;
        case WL_SCAN_COMPLETED:
          Serial.println("WL_SCAN_COMPLETED");
          break;
        case WL_CONNECT_FAILED:
          Serial.println("WL_CONNECT_FAILED");
          break;
        case WL_CONNECTION_LOST:
          Serial.println("WL_CONNECTION_LOST");
          break;
        case WL_DISCONNECTED:
          Serial.println("WL_DISCONNECTED");
          break;
        default:
          Serial.println(WiFi.status());
          Serial.println("No available define for that status code");
      }
      getWifiInfo();
    }
    else if (command == (uint8_t)'f')
    {
      OTAModeOn();
    }
    else if (command == (uint8_t)'a')
    {
      getAdvancedInfo();
    }
    else if (command == (uint8_t)'h')
    {
      // Print help aboud available commands
      Serial.println("-------------------------------------------------------------------------");
      Serial.println("Available commands, send the character of the command you wish to execute");
      Serial.println("'r' - restart the ESP");
      Serial.println("'m' - display memory stats");
      Serial.println("'c' - check and force update time");
      Serial.println("'t' - print the time of the ESP");
      Serial.println("'d' - print the date of the ESP");
      Serial.println("'u' - print the generated URLs, also these are the active settings for the strips");
      Serial.println("'o' - set all strips to OFF effect");
      Serial.println("'n' - print network info");
      Serial.println("'f' - firmware upgrade, OTA mode on");
      Serial.println("'a' - advanced info on ESP");
      Serial.println("");
    }
  }
}

// ------------------------------------------------------------------------------------------ MISC FUNCTIONS
void OTAModeOn()
{
  server.send(200, "text/plain", "OTA MODE ON, waiting for fw for 30 sec. You can close this tab.");
  Serial.println("OTA MODE ON, waiting for fw for 30 sec");
  
  uint32_t startTime = millis() + 30000;
  while(startTime > millis())
  {
    ArduinoOTA.handle();
    delay(100);
  }
  Serial.println("Failed to upgrade, restarting...");
  delay(100);
  ESP.restart();
}

void getMemoryInfo()
{
  Serial.println("ESP memory stats:");
  uint32_t freeMemory;
  uint16_t maxBlock;
  uint8_t fragmentation;
  
  //freeMemory = ESP.getFreeHeap();
  //maxBlock = ESP.getMaxFreeBlockSize();
  //fragmentation = ESP.getHeapFragmentation();
  
  ESP.getHeapStats(&freeMemory, &maxBlock, &fragmentation);
  Serial.print("  Free heap size[byte]:       ");
  Serial.println(freeMemory);
  Serial.print("  Max block size[byte]:       ");
  Serial.println(maxBlock);
  Serial.print("  Fragmentation[%]:           ");
  Serial.println(fragmentation);
}

void getTimeInfo()
{
  Serial.println("Time Info:");
  Serial.print("  Ellapsed mins. since boot:  ");
  Serial.println(millis() / 60000);
  Serial.print("  Time from NTP server:       ");
  Serial.println(timeClient.getFormattedTime());
  Serial.print("  Epoch from NTP server:      ");
  Serial.println(timeClient.getEpochTime());
}

void getDateInfo()
{
  // TODO: print date in number format
  Serial.print("Date:                         ");
  Serial.print(timeClient.getEpochTime() / 31556926 + 1970);
  Serial.print(".");
  uint8_t month = (timeClient.getEpochTime() / 2629743) % 12 + 1;
  Serial.print((month < 10 ) ? "0" + String(month) : month);
  Serial.print(".");
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.println(". ");
}

void checkAndForceUpdateTime()
{
  if(timeClient.getEpochTime() < 1546300800) // 2019.01.01 00:00:00
  {
    timeClient.forceUpdate();
    Serial.println("Forced time update!");
    Serial.println(timeClient.getEpochTime() < 1546300800 ? "Failure" : "Success");
  }
}
    
void getWifiInfo()
{ 
  Serial.println("Wifi Info:");
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("  Connected to:               ");
    Serial.println(WIFI_SSID);
    Serial.print("  IP address is:              ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("No info available, disconnected from WiFi");
  }
}

void getAdvancedInfo()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    getWifiInfo();
    uint8_t macAddr[6];
    WiFi.macAddress(macAddr);
    Serial.printf("MAC address:                  %02x:%02x:%02x:%02x:%02x:%02x\n", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
    Serial.print("Subnet mask:                  ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway IP:                   ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DHCP hostname:                ");
    Serial.println(WiFi.hostname());
    Serial.print("RSSI[dB]:                     ");
    Serial.println(WiFi.RSSI());
  }
  else
  {
    Serial.println("No info available, disconnected from WiFi");
  }
  // TODO convert to volt
  Serial.print("3V3 supply rail at:           ");
  Serial.println(ESP.getVcc());
  Serial.print("Chip ID:                      ");
  Serial.println(ESP.getChipId());
  Serial.print("SDK version:                  ");
  Serial.println(ESP.getSdkVersion());
  Serial.print("Core version:                 ");
  Serial.println(ESP.getCoreVersion());
  Serial.print("Full version:                 ");
  Serial.println(ESP.getFullVersion());
  Serial.print("Boot version:                 ");
  Serial.println(ESP.getBootVersion());
  Serial.print("Boot mode:                    ");
  Serial.println(ESP.getBootMode());
  Serial.print("CPU frequency[MHz]:           ");
  Serial.println(ESP.getCpuFreqMHz());
  Serial.print("Flash chip ID:                ");
  Serial.println(ESP.getFlashChipId());
  Serial.print("Flash chip vendor ID:         ");
  Serial.println(ESP.getFlashChipVendorId());
  Serial.print("Flash chip real size[byte]:   ");
  Serial.println(ESP.getFlashChipRealSize());
  Serial.print("Flash chip size[byte]:        ");
  Serial.println(ESP.getFlashChipSize());
  Serial.print("Flash chip speed[HZ]:         ");
  Serial.println(ESP.getFlashChipSpeed());
  Serial.print("Flash chip mode:              ");
  Serial.println(ESP.getFlashChipMode());
  Serial.print("Flash chip size by ID[byte]:  ");
  Serial.println(ESP.getFlashChipSizeByChipId());
  Serial.print("Sketch size[byte]:            ");
  Serial.println(ESP.getSketchSize());
  Serial.print("Sketch MD5:                   ");
  Serial.println(ESP.getSketchMD5());
  Serial.print("Free sketch space[byte]:      ");
  Serial.println(ESP.getFreeSketchSpace());
}
