#define STATEADDRESS 0
#define DELAYADDRESS 1
#define BRIGHTADDRESS 2
#define TIMEOUTADDRESS 3 //4 5 6 (long ami 4 byteos)
#define SLPBRGHTADDRESS 7
#define SLPDELAYADDRESS 8

#define PIN 12
#define NUM_LEDS 60

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
const long black = strip.Color(0, 0, 0);
const long darkgrey = strip.Color(85, 85, 85);
const long lightgrey = strip.Color(171, 171, 171);
const long white = strip.Color(255, 255, 255);
      
bool interrupted;
byte command, state, cycleDelay, brightness, sleepbrightness, sleepcycleDelay;
long sleepTimeout, tickTime;
