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
// not to be confused with a stripper
class Strip
{
  public:
    // ------------------------------ VARIABLES
    String fantasyName;
    uint16_t ledChipCount;
    // a reference for a strip
    Adafruit_NeoPixel adafruitStrip;
    uint8_t** currentRGBValues;

    uint8_t effectCode;
    uint32_t color;

    // ------------------------------ CONSTS
    
    // ------------------------------ FUNCTIONS
    Strip(uint8_t _pinNumber, uint16_t _ledChipCount, String _fantasyName)
    {
      adafruitStrip = Adafruit_NeoPixel(_ledChipCount, (uint16_t)_pinNumber, NEO_GRB + NEO_KHZ800);
      fantasyName = _fantasyName;
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
            adafruitStrip.setBrightness( 1 + (sin( millis() / 100) + 1) * 127 );
            adafruitStrip.show();
            break; 
          case RAINBOW:
            adafruitStrip.show();
            break; 
          case WAVE:
            adafruitStrip.show();
            break;
          
        }
      }
    }

    void setEffect(uint8_t _effect)
    {
      effectCode = _effect;
      EEPROM.write(EFFECT_ADDRESS, _effect);
      EEPROMWrite_uint32_t(LAST_COMMAND_EPOCH_ADDRESS, timeClient.getEpochTime());
      renderable = true;
    }
    
    void setColor(uint8_t _r, uint8_t _g, uint8_t _b)
    {
      color = _r << 16 | _g << 8 | _b ;
      //EEPROM.write(EFFECT_ADDRESS, _effect);
      //EEPROMWrite_uint32_t(LAST_COMMAND_EPOCH_ADDRESS, timeClient.getEpochTime());
      adafruitStrip.fill(color, 0, ledChipCount);
      renderable = true;
    }
    
  private:
  
    bool renderable;
    
};
