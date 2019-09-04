#include <Adafruit_NeoPixel.h>

// ------------------------------------------------------------------------------------------ DEFINES
// EFFECT CODES
#define OFF           0
#define STATIC        1
#define BREATHE       2
#define ANTIBREATHE   3
#define RAINBOW       4
#define WAVE          5
#define METEOR        6
#define BOUNCINGBALL  7
#define FIRE          8

// COLORS
#define R   0
#define G   1
#define B   2

#define URL_INDEX_OF_EFFECT             1
#define URL_INDEX_OF_SPEED              2
#define URL_INDEX_OF_BRIGHTNESS         3

#define URL_INDEX_OF_R                  5
#define URL_INDEX_OF_G                  6
#define URL_INDEX_OF_B                  7
#define URL_OFFSET_TO_NEXT_STRIP        10

// ------------------------------------------------------------------------------------------ CLASS
// TODO: add fade effect https://github.com/bitluni/bitluniHomeAutomation/blob/master/Fader.h
class Strip
{
  public:
    // ------------------------------ VARIABLES

    // ------------------------------ FUNCTIONS
    Strip(uint8_t _pinNumber, uint16_t _ledChipCount, uint8_t _indexOfThisStrip)
    {
      adafruitStrip = Adafruit_NeoPixel(_ledChipCount, (uint16_t)_pinNumber, NEO_GRB + NEO_KHZ800);
      ledChipCount = _ledChipCount;
      indexOfThisStrip = _indexOfThisStrip;
      currentRGBValues = new uint8_t*[3];
      for(uint8_t i = 0; i < 3; ++i)
          currentRGBValues[i] = new uint8_t[_ledChipCount];
          
      adafruitStrip.begin();
      adafruitStrip.setBrightness(255);
      adafruitStrip.clear();
      adafruitStrip.show();
    }
    
    ~Strip()
    {
      delete currentRGBValues;
    }
    
    void render(uint32_t _seed)
    {
      if(renderable)
      {
        _seed /= effectDelay ;
        switch(effectCode)
        {
          case OFF:
            adafruitStrip.clear();
            adafruitStrip.show();
            renderable = false;
            break; 
          case STATIC:
            adafruitStrip.fill(effectColor, 0, ledChipCount);
            adafruitStrip.setBrightness( effectBrightness );
            adafruitStrip.show();
            renderable = false;
            break; 
          case BREATHE:
            adafruitStrip.fill(effectColor, 0, ledChipCount);
            // math behind this:  1 + on the start makes sure that setB...(0) never gets called. Sets all led to off...
            //                    / 1000.0 scales down the changeing speed of the millis() function
            //                    + 1 offsets from sine's -1 .. +1 interval to 0 .. +2
            //                    * 127 scales up to full 254 interval
            adafruitStrip.setBrightness( 1 + (sin( _seed / 10000.0) + 1) * 127 );
            adafruitStrip.show();
            break;
          case ANTIBREATHE:
            adafruitStrip.fill(effectColor, 0, ledChipCount);
            // math behind this:  1 + on the start makes sure that setB...(0) never gets called. Sets all led to off...
            //                    / 1000.0 scales down the changeing speed of the millis() function
            //                    + 1 offsets from sine's -1 .. +1 interval to 0 .. +2
            //                    * 127 scales up to full 254 interval
            adafruitStrip.setBrightness( 1 + (-sin( _seed / 10000.0) + 1) * 127 );
            adafruitStrip.show();
            break;
          case RAINBOW:
            renderRainbow(_seed);
            break; 
          case WAVE:
            // TODO: implement
            adafruitStrip.show();
            break;
          case METEOR:
            meteorRain(10, 64, true, 30);
            break;
          case BOUNCINGBALL:
            byte colors[3][3] = { {0xff, 0,0}, 
                                  {0xff, 0xff, 0xff}, 
                                  {0   , 0   , 0xff} };
            BouncingColoredBalls(3, colors);
            break;
           case FIRE:
            FireEffect(55,120,15);
            break;
        }
      }
    }
    
// ------------------------------------------------------------------------------------------ EFFECT FUNCTIONS

    void renderRainbow(uint32_t _seed)
    {
    	uint8_t j = (_seed >> 12) & 255;
      uint8_t ij;
    	for(uint16_t i = 0; i < ledChipCount; i++)
    	{
    	  ij = 255 - ((i + j) & 255);
    	  if(ij < 85)
    		adafruitStrip.setPixelColor(i, 255 - ij * 3, 0, ij * 3);
    	  else if(ij < 170)
    		adafruitStrip.setPixelColor(i, 0, (ij - 85) * 3, 255 - (ij - 85) * 3);
    	  else
    		adafruitStrip.setPixelColor(i, (ij -170) * 3, 255 - (ij -170) * 3, 0);
    	}
      adafruitStrip.show();
    }

    void meteorRain(byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) 
    {  
      adafruitStrip.clear();
      
      for(int i = 0; i < ledChipCount+ledChipCount; i++) {
        
        
        // fade brightness all LEDs one step
        for(int j=0; j<ledChipCount; j++) {
          if( (!meteorRandomDecay) || (random(10)>5) ) {
            fadeToBlack(j, meteorTrailDecay );        
          }
        }
        
        // draw meteor
        for(int j = 0; j < meteorSize; j++) {
          if( ( i-j <ledChipCount) && (i-j>=0) ) {
            adafruitStrip.setPixelColor(i-j, effectColor);
          } 
        }
       
        adafruitStrip.show();
        delay(SpeedDelay);
      }
    }
    
    void fadeToBlack(int ledNo, byte fadeValue) {
     #ifdef ADAFRUIT_NEOPIXEL_H 
        // NeoPixel
        uint32_t oldColor;
        uint8_t r, g, b;
        int value;
        
        oldColor = adafruitStrip.getPixelColor(ledNo);
        r = (oldColor & 0x00ff0000UL) >> 16;
        g = (oldColor & 0x0000ff00UL) >> 8;
        b = (oldColor & 0x000000ffUL);
    
        r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
        g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
        b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
        
        adafruitStrip.setPixelColor(ledNo, r,g,b);
     #endif
     #ifndef ADAFRUIT_NEOPIXEL_H
       // FastLED
       leds[ledNo].fadeToBlackBy(fadeValue);
     #endif  
    }

    void BouncingColoredBalls(int BallCount, byte colors[][3]) {
      float Gravity = -9.81;
      int StartHeight = 1;
      
      float Height[BallCount];
      float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
      float ImpactVelocity[BallCount];
      float TimeSinceLastBounce[BallCount];
      int   Position[BallCount];
      long  ClockTimeSinceLastBounce[BallCount];
      float Dampening[BallCount];
      
      for (int i = 0 ; i < BallCount ; i++) {   
        ClockTimeSinceLastBounce[i] = millis();
        Height[i] = StartHeight;
        Position[i] = 0; 
        ImpactVelocity[i] = ImpactVelocityStart;
        TimeSinceLastBounce[i] = 0;
        Dampening[i] = 0.90 - float(i)/pow(BallCount,2); 
      }
      
      while (true) {
        for (int i = 0 ; i < BallCount ; i++) {
          TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
          Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
      
          if ( Height[i] < 0 ) {                      
            Height[i] = 0;
            ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
            ClockTimeSinceLastBounce[i] = millis();
      
            if ( ImpactVelocity[i] < 0.01 ) {
              ImpactVelocity[i] = ImpactVelocityStart;
            }
          }
          Position[i] = round( Height[i] * (ledChipCount - 1) / StartHeight);
        }
      
        for (int i = 0 ; i < BallCount ; i++) {
          adafruitStrip.setPixelColor(Position[i],colors[i][0],colors[i][1],colors[i][2]);
        }
        
        adafruitStrip.show();
        adafruitStrip.clear();
      }
    }

    void FireEffect(int Cooling, int Sparking, int SpeedDelay) {
      static byte heat*;
      int cooldown;
      
      // Step 1.  Cool down every cell a little
      for( int i = 0; i < ledChipCount; i++) {
        cooldown = random(0, ((Cooling * 10) / ledChipCount) + 2);
        
        if(cooldown>heat[i]) {
          heat[i]=0;
        } else {
          heat[i]=heat[i]-cooldown;
        }
      }
      
      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for(int k = ledChipCount - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
      }
        
      // Step 3.  Randomly ignite new 'sparks' near the bottom
      if( random(255) < Sparking ) {
        int y = random(7);
        heat[y] = heat[y] + random(160,255);
        //heat[y] = random(160,255);
      }
    
      // Step 4.  Convert heat to LED colors
      for( int j = 0; j < ledChipCount; j++) {
        setPixelHeatColor(j, heat[j]);
      }
    
      adafruitStrip.show();
      delay(SpeedDelay);
    }
    
    void setPixelHeatColor (int Pixel, byte temperature) {
      // Scale 'heat' down from 0-255 to 0-191
      byte t192 = round((temperature/255.0)*191);
     
      // calculate ramp up from
      byte heatramp = t192 & 0x3F; // 0..63
      heatramp <<= 2; // scale up to 0..252
     
      // figure out which third of the spectrum we're in:
      if( t192 > 0x80) {                     // hottest
        adafruitStrip.setPixelColor(Pixel, 255, 255, heatramp);
      } else if( t192 > 0x40 ) {             // middle
        adafruitStrip.setPixelColor(Pixel, 255, heatramp, 0);
      } else {                               // coolest
        adafruitStrip.setPixelColor(Pixel, heatramp, 0, 0);
      }
    }

    // ------------------------------------------------------------------------------------------ SET CONFIG FUNCTIONS
    void setEffect(uint8_t _effect)
    {
      effectCode = _effect;
      renderable = true;
    }
    
    void setSpeed(uint8_t _speed, bool fromEEPROM)
    {
      if(fromEEPROM)
        effectDelay = _speed;
      else
        effectDelay = constrain(255 - _speed, 1, 254);
    }

    void setBrightness(uint8_t _effectBrightness)
    {
      effectBrightness = _effectBrightness;
      adafruitStrip.setBrightness(_effectBrightness);
    }
    
    void setColor(uint8_t _r, uint8_t _g, uint8_t _b)
    {
      effectColor = _r << 16 | _g << 8 | _b ;
      renderable = true;
    }
    
    void setColor(uint32_t _color)
    {
      effectColor = _color;
      renderable = true;
    }
    
    void setColorR(uint8_t _color)
    {
      effectColor &= 0x00FFFF;
      effectColor |= _color << 16;
      renderable = true;
    }
    void setColorG(uint8_t _color)
    {
      effectColor &= 0xFF00FF;
      effectColor |= _color << 8;
      renderable = true;
    }
    void setColorB(uint8_t _color)
    {
      effectColor &= 0xFFFF00;
      effectColor |= _color;
      renderable = true;
    }
    
    void setConfigTo(uint8_t _configNumber, uint8_t _configValue)
    {
      switch(_configNumber)
      {
        case URL_INDEX_OF_EFFECT:
          setEffect(_configValue);
          break;
        case URL_INDEX_OF_SPEED:
          setSpeed(_configValue, false);
          break;
        case URL_INDEX_OF_BRIGHTNESS:
          setBrightness(_configValue);
          break;
        case URL_INDEX_OF_R:
          setColorR(_configValue);
          break;
        case URL_INDEX_OF_G:
          setColorG(_configValue);
          break;
        case URL_INDEX_OF_B:
          setColorB(_configValue);
          break;
      }
    }
    
    // ------------------------------------------------------------------------------------------ GET CONFIG FUNCTIONS
    String getColor(uint8_t _color)
    {
      switch(_color)
      {
        case R:
          return String((uint8_t)(effectColor >> 16));
        case G:
          return String((uint8_t)(effectColor >> 8));
        case B:
          return String((uint8_t)(effectColor));
      }
    }
    
    String getConfigAsURL()
    {
      String ret;
      ret += String(indexOfThisStrip * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_EFFECT) + "=" + String(effectCode);
      ret += "&" + String(indexOfThisStrip * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_SPEED) + "=" + String(255 - effectDelay);
      ret += "&" + String(indexOfThisStrip * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_BRIGHTNESS) + "=" + String(effectBrightness);
      ret += "&" + String(indexOfThisStrip * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_R) + "=" + getColor(R);
      ret += "&" + String(indexOfThisStrip * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_G) + "=" + getColor(G);
      ret += "&" + String(indexOfThisStrip * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_B) + "=" + getColor(B);
      return ret; 
    }
    
    // ------------------------------------------------------------------------------------------ OTHER FUNCTIONS
    void loadConfigFromEEPROM()
    {
      setEffect(EEPROM.read(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + EFFECT_ADDRESS));
      setSpeed(EEPROM.read(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + SPEED_ADDRESS), true);
      setBrightness(EEPROM.read(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + BRIGHTNESS_ADDRESS));
      setColor(EEPROMRead_uint32_t(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + COLOR_ADDRESS));
      Serial.print("LOADED VALUES FROM EEPROM FOR STRIP ");
      Serial.println(indexOfThisStrip);
    }
    
    void saveConfigToEEPROM()
    {
      EEPROM.write(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + EFFECT_ADDRESS, effectCode);
      EEPROM.write(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + SPEED_ADDRESS, effectDelay);
      EEPROM.write(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + BRIGHTNESS_ADDRESS, effectBrightness);
      EEPROMWrite_uint32_t(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + COLOR_ADDRESS, effectColor);
      EEPROM.commit();
    }

    void setDefaultConfig()
    {
      setEffect(1);
      setSpeed(200, false);
      setBrightness(100);
      setColor(0xFFFFFF);
      Serial.print("DEFAULT CONFIG SET FOR STRIP ");
      Serial.println(indexOfThisStrip);
    }
    
  private: 
    // ------------------------------ VARIABLES
    Adafruit_NeoPixel adafruitStrip;
    uint8_t         indexOfThisStrip;
    uint16_t        ledChipCount;
    uint8_t**       currentRGBValues;
    uint8_t         effectCode;
    uint8_t         effectDelay;
    uint8_t         effectBrightness;
    uint32_t        effectColor;
    bool            renderable;
    
};
