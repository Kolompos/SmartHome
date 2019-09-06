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
#define RANDOMFILL	  9
#define MIDDLEFILL	  10
#define SIDEFILL	    11
#define YINYANG       12

// EFFECT DEFINES
#define BALL_COUNT    3

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
        _seed /= effectDelay;
        switch(effectCode)
        {
          case OFF:
            adafruitStrip.clear();
            renderable = false;
            break; 
          case STATIC:
            adafruitStrip.fill(effectColor, 0, ledChipCount);
            adafruitStrip.setBrightness( effectBrightness );
            renderable = false;
            break; 
          case BREATHE:
            adafruitStrip.fill(effectColor, 0, ledChipCount);
            // math behind this:  1 + on the start makes sure that setB...(0) never gets called. Sets all led to off...
            //                    / 1000.0 scales down the changeing speed of the millis() function
            //                    + 1 offsets from sine's -1 .. +1 interval to 0 .. +2
            //                    * 127 scales up to full 254 interval
            adafruitStrip.setBrightness( map( 1 + ( sin( _seed / 10000.0) + 1) * 127 , 0 , 255 , 0 , effectBrightness));
            break;
          case ANTIBREATHE:
            adafruitStrip.fill(effectColor, 0, ledChipCount);
            adafruitStrip.setBrightness( map( 1 + ( -sin( _seed / 10000.0) + 1) * 127 , 0 , 255 , 0 , effectBrightness));
            break;
          case RAINBOW:
            rainbowEffect(_seed);
            break; 
          case WAVE:
            waveEffect(_seed);
            break;
          case METEOR:
            meteorRain(_seed, 6, 64, true); //uint32_t _seed, uint8_t meteorSize, uint8_t meteorTrailDecay, boolean meteorRandomDecay
            break;
          case BOUNCINGBALL:
            bouncingColoredBalls(_seed);
            break;
          case FIRE:
            fireEffect(55,120); //uint8_t _cooldownRate, uint8_t _sparkProbability
            break;
    		  case RANDOMFILL:
    		    randomFillDrain(_seed); // pick a random LED to light up until entire strip is lit, then reverse
    		    break;
    		  case MIDDLEFILL:
    			  middleFillDrain(_seed); // Light up the strip starting from the middle, then reverse
    		    break;
    		  case SIDEFILL:
            sideFillDrain(_seed); // Light up the strip starting from the sides, then reverse
            break;
          case YINYANG:
            yinYang(_seed);
            break;
          default:  //when unconfigured set the leds off
            adafruitStrip.clear();
            renderable = false;
        }
        adafruitStrip.show();
      }
    }
    
    // ------------------------------------------------------------------------------------------ EFFECT FUNCTIONS
    // ------------------------------------------------------------------------------------------ rainbowEffect
    void rainbowEffect(uint32_t _seed)
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
    }
    
    // ------------------------------------------------------------------------------------------ waveEffect
    uint16_t *sinTable;
    uint8_t rgb[3] = {0, 0, 0};
    
    void waveEffect(uint32_t _seed)
    {
      _seed = _seed >> 2;
      
      if(firstRender)
      {
        rgb[0] = effectColor >> 16;
        rgb[1] = effectColor >> 8;
        rgb[2] = effectColor;
        firstRender = false;
      }
      
      // you can add more offset and modify the parameters 111 and 169, they are random
      uint8_t sinOffset0 = (_seed / 111) & 255;
      uint8_t sinOffset1 = (_seed / 169) & 255;

      for(uint16_t sinTime = 0; sinTime < ledChipCount; sinTime++)
      {
        // in this example you have 2 sine functions parametered like sin(w*t+fi) (general high school stuff)
        
        // those offsets from above are used in the parametering of these sine functions
        // more sine functions can be multiplied together but you have to divide back with each of them at the end
        // sinTable is a uint16_t array, so the '>> 8' makes them uint8_t again.
        // each multiplication means an addition in bit length, so uint8_t * uint8_t makes uint16_t (dont forget to increase variable size when adding more functions)
        uint16_t sumFunction = (sinTable[(2 * sinTime + sinOffset0) & 255] >> 8) * (sinTable[-(5 * sinTime + sinOffset1) & 255] >> 8); // i named it sum functions, for you math savvys, i know it's a product

        // here you multiply that uint16_t and the uint8_t R G and B values (which would make a 24 bit number), so to make them uint8_t again you make an '>> 16'
        adafruitStrip.setPixelColor(sinTime, (rgb[0] * sumFunction) >> 16, (rgb[1] * sumFunction) >> 16, (rgb[2] * sumFunction) >> 16);
      }
    }

    // ------------------------------------------------------------------------------------------ meteorRain
    void meteorRain(uint32_t _seed, uint8_t meteorSize, uint8_t meteorTrailDecay, boolean meteorRandomDecay) 
    {
      if(firstRender)
      {
        adafruitStrip.clear();
        effectIndex = 0;
        lastRender = _seed - 1;
        firstRender = false;
      }
      
      if(_seed > lastRender)
      {
        // fade brightness all LEDs one step
        for(uint16_t j = 0; j < ledChipCount; j++)
        {
          if( (!meteorRandomDecay) || (random( 10 ) > 5)) //80000 and 78000 good candidates
          {
            fadeToBlack( j, meteorTrailDecay );
          }
        }
        
        // draw meteor
        for(uint8_t j = 0; j < meteorSize; j++)
        {
          if( ( effectIndex - j < ledChipCount) && (effectIndex - j>=0) )
          {
            adafruitStrip.setPixelColor(effectIndex - j, effectColor);
          } 
        }
        effectIndex++;
        if(effectIndex == ledChipCount + ledChipCount) effectIndex = 0;
        lastRender = _seed + 1000;
      }
    }
    
    void fadeToBlack(uint16_t ledNo, uint8_t fadeValue)
    {
      uint32_t oldColor;
      uint8_t r, g, b;
      
      oldColor = adafruitStrip.getPixelColor(ledNo);
      r = (oldColor & 0x00ff0000UL) >> 16;
      g = (oldColor & 0x0000ff00UL) >> 8;
      b = (oldColor & 0x000000ffUL);
      
      r=(r<=10)? 0 : (uint8_t) r-(r*fadeValue/256);
      g=(g<=10)? 0 : (uint8_t) g-(g*fadeValue/256);
      b=(b<=10)? 0 : (uint8_t) b-(b*fadeValue/256);
      
      adafruitStrip.setPixelColor(ledNo, r,g,b);
    }

    // ------------------------------------------------------------------------------------------ bouncingColoredBalls
    uint8_t colors[BALL_COUNT][3] =     { { 0xff , 0    ,   0  },     // red
                                          { 0xff , 0xff , 0xff },     // white
                                          { 0    , 0    , 0xff } };   // blue
    float Gravity = -9.81;
    uint8_t StartHeight = 1;
    
    float     Height[BALL_COUNT];
    float     ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
    float     ImpactVelocity[BALL_COUNT];
    float     TimeSinceLastBounce[BALL_COUNT];
    uint8_t   Position[BALL_COUNT];
    uint32_t  ClockTimeSinceLastBounce[BALL_COUNT];
    float     Dampening[BALL_COUNT];
    
    void bouncingColoredBalls(uint32_t _seed)
    {
      adafruitStrip.clear();
      _seed = _seed >> 8;

      if(firstRender)
      {
        for (uint8_t i = 0 ; i < BALL_COUNT ; i++)
        {   
          ClockTimeSinceLastBounce[i] = _seed;
          Height[i] = StartHeight;
          Position[i] = 0; 
          ImpactVelocity[i] = ImpactVelocityStart;
          TimeSinceLastBounce[i] = 0;
          Dampening[i] = 0.90 - float(i)/pow(BALL_COUNT,2); 
        }
        firstRender = false;
      }
      
      for (uint8_t i = 0 ; i < BALL_COUNT ; i++)
      {
        TimeSinceLastBounce[i] =  _seed - ClockTimeSinceLastBounce[i];
        Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
    
        if ( Height[i] < 0 )
        {                      
          Height[i] = 0;
          ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
          ClockTimeSinceLastBounce[i] = _seed;
    
          if ( ImpactVelocity[i] < 0.01 )
          {
            ImpactVelocity[i] = ImpactVelocityStart;
          }
        }
        Position[i] = round( Height[i] * (ledChipCount - 1) / StartHeight);
      }
    
      for (int i = 0 ; i < BALL_COUNT ; i++)
      {
        adafruitStrip.setPixelColor(Position[i],colors[i][0],colors[i][1],colors[i][2]);
      }
    }

    // ------------------------------------------------------------------------------------------ fireEffect
    uint8_t *heat;
    uint16_t cooldown;
     
    void fireEffect(uint8_t _cooldownRate, uint8_t _sparkProbability)
    {
      // Step 1.  Cool down every cell a little
      for( int i = 0; i < ledChipCount; i++)
      {
        cooldown = random(0, ((_cooldownRate * 10) / ledChipCount) + 2);
        
        if( cooldown > heat[i] )
        {
          heat[i] = 0;
        }
        else
        {
          heat[i] = heat[i] - cooldown;
        }
      }
      
      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for(uint16_t k = ledChipCount - 1; k >= 2; k--)
      {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
      }
        
      // Step 3.  Randomly ignite new 'sparks' near the bottom
      if( random(255) < _sparkProbability )
      {
        uint8_t y = random(7);
        heat[y] = heat[y] + random(160,255);
        //heat[y] = random(160,255);
      }
    
      // Step 4.  Convert heat to LED colors
      for( uint16_t j = 0; j < ledChipCount; j++)
      {
        setPixelHeatColor(j, heat[j]);
      }
      
      delay(effectDelay);
    }
    
    void setPixelHeatColor (uint16_t Pixel, uint8_t temperature)
    {
      // Scale 'heat' down from 0-255 to 0-191
      uint8_t t192 = round((temperature/255.0)*191);
     
      // calculate ramp up from
      uint8_t heatramp = t192 & 0x3F;       // 0..63
      heatramp <<= 2;                       // scale up to 0..252
     
      // figure out which third of the spectrum we're in:
      if( t192 > 0x80)                                    // hottest
      {
        adafruitStrip.setPixelColor(Pixel, 255, 255, heatramp);
      }
      else if( t192 > 0x40 )                              // middle
      {
        adafruitStrip.setPixelColor(Pixel, 255, heatramp, 0);
      }
      else                                                // coolest
      {
        adafruitStrip.setPixelColor(Pixel, heatramp, 0, 0);
      }
    }
	
    // ------------------------------------------------------------------------------------------ randomFillDrain
    // pick a random LED to light up until entire strip is lit

    bool *litLEDs;        // array to keep track of lit LEDs
    uint16_t randomPick;
    
    void randomFillDrain(uint32_t _seed)
    {
      _seed = _seed >> 6;  // microseconds divided by 1024 is miliseconds
      
      if(firstRender)
      {
        adafruitStrip.clear();
        effectIndex = 0;
        memset(litLEDs, 0, ledChipCount);
        lastRender = _seed - 1;
        filling = true;
        firstRender = false;
      }

      if(_seed > lastRender)
      {
        if(filling)
        {
          while(true)
          {
            randomPick = random(0 , ledChipCount); // pick a random LED
            if(litLEDs[randomPick] != true)  // if LED not already lit, proceed
            { 
              adafruitStrip.setPixelColor(randomPick, effectColor);
              litLEDs[randomPick] = true; // update array to remember it is lit
              effectIndex++;
              break;
            }
          }
          if(effectIndex == ledChipCount) filling = false;
        }
        else
        {
          while(true)
          {
            randomPick = random(0 , ledChipCount); // pick a random LED
            if(litLEDs[randomPick] != false)  // if LED already lit, proceed
            { 
              adafruitStrip.setPixelColor(randomPick, 0);
              litLEDs[randomPick] = false; // update array to remember it is lit
              effectIndex--;
              break;
            }
          }
          if(effectIndex == 0) filling = true;
        }
        lastRender = _seed + 100;
      }
    }
	
  	// ------------------------------------------------------------------------------------------ middleFillDrain
  	// Light up the strip starting from the middle
  	void middleFillDrain(uint32_t _seed)
  	{
      _seed = _seed >> 6;
      
      if(firstRender)
      {
        adafruitStrip.clear();
        effectIndex = 0;
        lastRender = _seed - 1;
        filling = true;
        firstRender = false;
      }
      
  	  if(_seed > lastRender)
      {
        if(filling)
        {
          adafruitStrip.setPixelColor(ledChipCount / 2 + effectIndex, effectColor);
          adafruitStrip.setPixelColor(ledChipCount / 2 - effectIndex, effectColor);
          effectIndex++;
          if(effectIndex == ledChipCount / 2) filling = false;
        }
        else
        {
          adafruitStrip.setPixelColor(effectIndex, 0);
          adafruitStrip.setPixelColor(ledChipCount - effectIndex, 0);
          effectIndex--;
          if(effectIndex == 0) filling = true;
        }
        lastRender = _seed + 100;
      }
  	}
  	
  	// ------------------------------------------------------------------------------------------ sideFillDrain
  	// Light up the strip starting from the sides
  	void sideFillDrain(uint32_t _seed)
  	{
      _seed = _seed >> 6;
      
      if(firstRender)
      {
        adafruitStrip.clear();
        effectIndex = 0;
        lastRender = _seed - 1;
        filling = true;
        firstRender = false;
      }
      
      if(_seed > lastRender)
      {
        if(filling)
        {
          adafruitStrip.setPixelColor(effectIndex, effectColor);
          adafruitStrip.setPixelColor(ledChipCount - effectIndex, effectColor);
          effectIndex++;
          if(effectIndex == ledChipCount / 2) filling = false;
        }
        else
        {
          adafruitStrip.setPixelColor(ledChipCount / 2 + effectIndex, 0);
          adafruitStrip.setPixelColor(ledChipCount / 2 - effectIndex, 0);
          effectIndex--;
          if(effectIndex == 0) filling = true;
        }
        lastRender = _seed + 100;
      }
  	}

    // ------------------------------------------------------------------------------------------ yinYang
    void yinYang(uint32_t _seed)
    {
      _seed = _seed >> 6;
      
      if(firstRender)
      {
        adafruitStrip.clear();
        effectIndex = 0;
        lastRender = _seed - 1;
        filling = true;
        firstRender = false;
      }
      
      if(_seed > lastRender)
      {
        
        lastRender = _seed + 100;
      }
    }
    
    // ------------------------------------------------------------------------------------------ SET CONFIG FUNCTIONS
    void setEffect(uint8_t _effect)
    {
      effectCode = _effect;
    }
    
    void setSpeed(uint8_t _speed, bool fromEEPROM)
    {
      if(fromEEPROM)
        effectDelay = _speed;
      else
        effectDelay = constrain(255 - _speed, 5, 255);
        
    }

    void setBrightness(uint8_t _effectBrightness)
    {
      effectBrightness = _effectBrightness;
      adafruitStrip.setBrightness(_effectBrightness);
    }
    
    void setColor(uint8_t _r, uint8_t _g, uint8_t _b)
    {
      effectColor = _r << 16 | _g << 8 | _b ;
    }
    
    void setColor(uint32_t _color)
    {
      effectColor = _color;
    }
    
    void setColorR(uint8_t _color)
    {
      effectColor &= 0x00FFFF;
      effectColor |= _color << 16;
    }
    void setColorG(uint8_t _color)
    {
      effectColor &= 0xFF00FF;
      effectColor |= _color << 8;
    }
    void setColorB(uint8_t _color)
    {
      effectColor &= 0xFFFF00;
      effectColor |= _color;
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
      #ifdef VERBOSE_MODE
        Serial.print("LOADED VALUES FROM EEPROM FOR STRIP ");
        Serial.println(indexOfThisStrip);
      #endif
      preEffectFunction();
    }
    
    void saveConfigToEEPROM()
    {
      // this runs after every config change so it is practical to use this function for such activities
      preEffectFunction();
      afterNewConfigChecks();  // for some effects this is needed to make user experience maximal
      
      EEPROM.write(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + EFFECT_ADDRESS, effectCode);
      EEPROM.write(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + SPEED_ADDRESS, effectDelay);
      EEPROM.write(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + BRIGHTNESS_ADDRESS, effectBrightness);
      EEPROMWrite_uint32_t(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + COLOR_ADDRESS, effectColor);
      EEPROM.commit();
      #ifdef VERBOSE_MODE
        Serial.print("SAVED VALUES TO EEPROM FOR STRIP ");
        Serial.println(indexOfThisStrip);
      #endif
    }

    void setDefaultConfig()
    {
      setEffect(1);
      setSpeed(200, false);
      setBrightness(150);
      setColor(0xFFFFFF);
      #ifdef VERBOSE_MODE
        Serial.print("DEFAULT CONFIG SET FOR STRIP ");
        Serial.println(indexOfThisStrip);
      #endif
      preEffectFunction();
    }

    void preEffectFunction()
    {
      firstRender = true;
      renderable = true;

      // these are here to save memory
      // later on when technology goes by me and my old views, you could technically make these reserved the whole time... 
      // but to know me, I like to save every byte of RAM
      if(effectCode == FIRE && heat == NULL)
      {
        heat = new uint8_t[ledChipCount];
      }
      else if(effectCode != FIRE && heat != NULL)
      {
        delete[] heat;
        heat = NULL;
      }
      
      if(effectCode == WAVE && sinTable == NULL)
      {
        sinTable = new uint16_t[256];
        for(uint16_t i = 0; i < 256; i++)
        {
          sinTable[i] = sin(3.1415 / 128 * i) * 0x7fff + 0x8000;
        }
      }
      else if(effectCode != WAVE && sinTable != NULL)
      {
        delete[] sinTable;
        sinTable = NULL;
      }
      
      if(effectCode == RANDOMFILL && litLEDs == NULL)
      {
        litLEDs = new bool[ledChipCount];
      }
      else if(effectCode != RANDOMFILL && litLEDs != NULL)
      {
        delete[] litLEDs;
        litLEDs = NULL;
      }



      
    }

    void afterNewConfigChecks()
    {
      if(effectCode == BOUNCINGBALL){ effectDelay = constrain( effectDelay, 5, 50 );}
      if(effectCode == FIRE){ effectDelay = constrain( effectDelay, 5, 200 );}
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
    uint16_t        effectIndex;      // keeps track of the index of the led each effect is at
    bool            renderable, firstRender, filling;
    uint32_t        lastRender;
    
};
