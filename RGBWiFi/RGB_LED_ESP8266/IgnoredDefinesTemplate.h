#define NTP_UPDATE

#define WIFI_SSID "---"
#define WIFI_PW  "---"

IPAddress staticIP(192,168,1,91);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress dns(8, 8, 8, 8);

// strips settings
#define NUMBER_OF_STRIPS                3

#define LOGO_PIN                        D4
#define LOGO_LED_COUNT                  14

#define FRAME_PIN                       D3
#define FRAME_LED_COUNT                 82

#define LAMP_PIN                        D2
#define LAMP_LED_COUNT                  22

#include "Strip.h"
Strip strips[NUMBER_OF_STRIPS] = { { LOGO_PIN,LOGO_LED_COUNT,0 } , { FRAME_PIN,FRAME_LED_COUNT,1 }, { LAMP_PIN,LAMP_LED_COUNT,2 } };
#include "Handles.h"
