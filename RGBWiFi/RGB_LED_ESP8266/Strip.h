#include <Adafruit_NeoPixel.h>

// ------------------------------------------------------------------------------------------ DEFINES
// EFFECT CODES
#define OFF       0
#define STATIC    1
#define BREATHE   2
#define RAINBOW   3
#define WAVE      4

// COLORS
#define R   0
#define G   1
#define B   2

// ------------------------------------------------------------------------------------------ CLASS
// TODO: add fade effect https://github.com/bitluni/bitluniHomeAutomation/blob/master/Fader.h
class Strip
{
  public:
    // ------------------------------ VARIABLES
    uint8_t indexOfThisStrip;
    uint16_t ledChipCount;
    // a reference for a strip
    Adafruit_NeoPixel adafruitStrip;
    uint8_t** currentRGBValues;

    uint8_t effectCode;
    uint8_t effectSpeed;
    uint8_t effectBrightness;
    uint32_t effectColor;

    // ------------------------------ CONSTS
    
    // ------------------------------ FUNCTIONS
    Strip(uint8_t _pinNumber, uint16_t _ledChipCount, uint8_t _indexOfThisStrip)
    {
      adafruitStrip = Adafruit_NeoPixel(_ledChipCount, (uint16_t)_pinNumber, NEO_GRB + NEO_KHZ800);
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
    
    void render()
    {
      if(renderable)
      {
        switch(effectCode)
        {
          case OFF:
            adafruitStrip.clear();
            adafruitStrip.show();
            renderable = false;
            break; 
          case STATIC:
            adafruitStrip.show();
            renderable = false;
            break; 
          case BREATHE:
            // math behind this:  1 + on the start makes sure that setB...(0) never gets called. Sets all led to off...
            //                    / 1000.0 scales down the changeing speed of the millis() function
            //                    + 1 offsets from sine's -1 .. +1 interval to 0 .. +2
            //                    * 127 scales up to full 254 interval
            // TODO: make that 1000.0 variabel, so that breathe intervall can be adjusted. also make a function for changeing that variable or use setSpeed() funtion
            adafruitStrip.setBrightness( 1 + (sin( millis() / 1000.0) + 1) * 127 );
            adafruitStrip.show();
            break; 
          case RAINBOW:
            // TODO: implement
            adafruitStrip.show();
            break; 
          case WAVE:
            // TODO: implement
            adafruitStrip.show();
            break;
          
        }
      }
    }

    void setEffect(uint8_t _effect)
    {
      effectCode = _effect;
      EEPROM.write(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + EFFECT_ADDRESS, _effect);
      this->setBrightness(255);
      renderable = true;
    }
    
    void setColor(uint8_t _r, uint8_t _g, uint8_t _b)
    {
      effectColor = _r << 16 | _g << 8 | _b ;
      EEPROMWrite_uint32_t(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + COLOR_ADDRESS, effectColor);
      adafruitStrip.fill(effectColor, 0, ledChipCount);
      renderable = true;
    }
    
    void setColor(uint32_t _color)
    {
      effectColor = _color;
      EEPROMWrite_uint32_t(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + COLOR_ADDRESS, effectColor);
      adafruitStrip.fill(effectColor, 0, ledChipCount);
      renderable = true;
    }
    
    void setSpeed(uint8_t _speed)
    {
      effectSpeed = _speed;
      EEPROM.write(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + SPEED_ADDRESS, _speed);
    }
    
    void setBrightness(uint8_t _effectBrightness)
    {
      effectBrightness = _effectBrightness;
      EEPROM.write(indexOfThisStrip * OFFSET_TO_NEXT_STRIP_DATA + BRIGHTNESS_ADDRESS, _effectBrightness);
      adafruitStrip.setBrightness(_effectBrightness);
    }
    
    String getEffect()
    {
      return String(effectCode);
    }
    
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
    
  private:
  
    bool renderable;
    
};
