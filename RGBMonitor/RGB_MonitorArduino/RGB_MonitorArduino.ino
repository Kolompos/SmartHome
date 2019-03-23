#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include "EEPROMstuff.h"
#include "DefinesVars.h"
#include "SEvent.h"
#include "SetsEffects.h"

/*  Állapotok
 *  0 Olvasólámpa
 *  1 lámpa
 *  2 Olvasólámpa+rainbow
 *  3 RainbowWheel
 *  4 Sleep mode
 */
 
/*  Parancsok
 *  s set state pl: s0
 *  d set delay pl: d23
 *  b set brightness pl: b200
 *  t set timeout to sleep pl: t50000
 *  p set sleep brightness
 *  e set sleep speed
 */

void setup() {
  Serial.begin(115200);
  strip.begin();

  state = EEPROM.read(STATEADDRESS);
  if(state == 4)
    state = 0;
  cycleDelay = EEPROM.read(DELAYADDRESS);
  if(cycleDelay == 0){
    cycleDelay = 1;
    EEPROM.write(DELAYADDRESS, cycleDelay);
  }
  brightness = EEPROM.read(BRIGHTADDRESS);
  if(brightness == 0){
    brightness = 100;
    EEPROM.write(BRIGHTADDRESS, brightness);
  }
  sleepTimeout = EEPROMReadlong(TIMEOUTADDRESS);
  if(sleepTimeout == 0){
    sleepTimeout = 60000 / cycleDelay;
    EEPROMWritelong(TIMEOUTADDRESS, sleepTimeout);
  }
  sleepbrightness = EEPROM.read(SLPBRGHTADDRESS);
  if(sleepbrightness == 0){
    sleepbrightness = 15;
    EEPROM.write(SLPBRGHTADDRESS, sleepbrightness);
  }
  sleepcycleDelay = EEPROM.read(SLPDELAYADDRESS);
  if(sleepcycleDelay == 0){
    sleepcycleDelay = 150;
    EEPROM.write(SLPDELAYADDRESS, sleepcycleDelay);
  }
  
  Serial.println("START!");
}

void loop() {
  
  strip.setBrightness(brightness);
  
  switch (state){
    case 0:
      setTop(0,0,0);
      setBottom(255,255,255);
      strip.show();
      while(true){
        if(Serial.available() || interrupted)
          return;
        tickTime++;
        delay(cycleDelay);
        if(tickTime > sleepTimeout){
          state = 4;
          return;
        }
      }
      break;
    case 1:
      setAll(255,255,255);
      strip.show();
      while(true){
        if(Serial.available() || interrupted)
          return;
        tickTime++;
        delay(cycleDelay);
        if(tickTime > sleepTimeout){
          state = 4;
          return;
        }
      }
      break;
    case 2:
      setBottom(255,255,255);
      while(!(Serial.available() || interrupted)){
        rainbowCycle(15,45);
        if(state == 4)
          return;
      }
      break;
    case 3:
      while(!(Serial.available() || interrupted)){
        rainbowCycle(0,NUM_LEDS);
        if(state == 4)
          return;
      }
      break;
    case 4:
      Serial.println("Going to sleep");
      setAll(0,0,0);
      strip.setBrightness(sleepbrightness);
      int i;
      while(true){
        for(i = 0; i < 60; i++){
          sleepEffect(i);
          delay(sleepcycleDelay);
          if(Serial.available() || interrupted){
            return;
          }
        }
      }
      break;
  }
}
