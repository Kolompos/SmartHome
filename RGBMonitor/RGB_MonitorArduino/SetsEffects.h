void setAll(byte r, byte g, byte b) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    strip.setPixelColor(i, ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b); 
  }
}

void setBottom(byte r, byte g, byte b) {
  int i;
  for(i = 0; i < 15; i++ ) {
    strip.setPixelColor(i, ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b); 
  }
  for(i = 45; i < 60; i++ ) {
    strip.setPixelColor(i, ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b); 
  }
}

void setTop(byte r, byte g, byte b) {
  for(int i = 15; i < 45; i++ ) {
    strip.setPixelColor(i, ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b); 
  }
}

long wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }
  
  return (((uint32_t)c[0] << 16) | ((uint32_t)c[1] <<  8) | c[2]);
}

void rainbowCycle(byte first, byte last) {
  
  int i, j;
  for(j=0; j<256; j++) {
    
    for(i=first; i < last; i++) {
      strip.setPixelColor(i,wheel(((i * 256 / (first-last)) + j) & 255));
    }
    
    strip.show();
    if(Serial.available() || interrupted){
      return;
    }
    tickTime++;
    delay(cycleDelay);
    if(tickTime > sleepTimeout){
      state = 4;
      return;
    }
  }
}

void sleepEffect(byte middle){
  strip.setPixelColor(middle,white);

  //előre
  if(middle == NUM_LEDS - 2){
    strip.setPixelColor(middle + 1,lightgrey);
    strip.setPixelColor(0,darkgrey);
  }
  else if(middle == NUM_LEDS - 1){
    strip.setPixelColor(0,lightgrey);
    strip.setPixelColor(1,darkgrey);
  }else{
    strip.setPixelColor(middle + 1,lightgrey);
    strip.setPixelColor(middle + 2,darkgrey);
  }

  //hátra
  if(middle == 0){
    strip.setPixelColor(59,lightgrey);
    strip.setPixelColor(58,darkgrey);
    strip.setPixelColor(57,black);
  }else if(middle == 1){
    strip.setPixelColor(0,lightgrey);
    strip.setPixelColor(59,darkgrey);
    strip.setPixelColor(58,black);
  }else if(middle == 2){
    strip.setPixelColor(1,lightgrey);
    strip.setPixelColor(0,darkgrey);
    strip.setPixelColor(59,black);
  }else{
    strip.setPixelColor(middle - 1,lightgrey);
    strip.setPixelColor(middle - 2,darkgrey);
    strip.setPixelColor(middle - 3,black);
  }
  strip.show();
}
