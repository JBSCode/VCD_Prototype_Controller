#include <SoftPWM_Diff.h>


static volatile uint8_t v = 0;

int swooshSimpleValue;
int swooshSmoothWaveLength;

int mode = -1;


void loopGBCModes()
{
//  Serial.print(" in ====> ");
//  Serial.println( (int)((millis() / 1000) / 30) );
  int modeNew = (int)((millis() / 1000) / 30) % 4;
  if( modeNew != mode ){
    mode = modeNew;
    Serial.print(" MODE : ");
    Serial.println(mode);  
    Serial.println(millis());  
    showMode( mode );
    swooshSimpleValue = 10;
    swooshSmoothWaveLength = 256;
  }

  switch( mode ){
    case 0:
//      for( int i=10;i<100;i+=10){
        swooshSimple( swooshSimpleValue );
//      }
      swooshSimpleValue += 10;
      swooshSimpleValue %= 100;
      break;
    case 1:
//      for( int waveLength=256;waveLength<30*256;waveLength+=200){
        swooshSmooth( 40 , swooshSmoothWaveLength );
//      }   
      swooshSmoothWaveLength += 200;
      swooshSmoothWaveLength %= 30*256;      
      break;
    case 2:
      wave( 0.005f );
      break;    
    case 3:
      randomFeed( .06f );
      break;    
  }
  
//  for( int waveLength=256;waveLength<30*256;waveLength+=200){
//    swooshSmooth( 40 , waveLength );
//  }
  
//  swooshSmooth( 40 );

//  randomFeed( .06f );

//  wave( 0.005f );
  
}

void showMode( int mode ){
//  for (uint8_t i = 0;i < AMOUNT_LCDS;++i){
//    AMOUNT_LCDS
//    SoftPWM.set( i, 0 );
//  }
  memset(LCDStates,0,AMOUNT_LCDS);
  for( int j=0; j<10;j++){
    int val = ((j%2) == 0 ? 255 : 0);
    for (uint8_t i = 0;i < (mode+1)*2;++i){
//      int val = 10;
      LCDStates[i+16] = val;
      LCDStates[i+24] = val;

//      SoftPWM.set( i+16, val );
//      SoftPWM.set( i+24, val );
    }
    updateLCDs();
    delay(500);
  }
//  delay(5000);
  for (uint8_t i = 0;i < AMOUNT_LCDS;++i){
//    SoftPWM.set( i, 0 );
    LCDStates[i] = 0;
  }
    updateLCDs();
}

// ----------------------------------------
// Simple Flash
//

void flash( uint8_t pin ){
  flash( pin, 1000 );
}

void flash( uint8_t pin, int dur ){
//  SoftPWM.set( pin, 255 );
    LCDStates[pin] = 255;
    updateLCDs();
    delay(dur);
    LCDStates[pin] = 0;
    updateLCDs();
    //    SoftPWM.set( pin, 0 );
}

// ----------------------------------------
// Swooshis
//
void swooshSimple(){
  swooshSimple( 500 );
}

void swooshSimple( int dur ){
//  for (uint8_t i = 0;i < SoftPWM.size();++i){
//    SoftPWM.set( i, 0 );
//    
//  }
  memset(LCDStates,0,AMOUNT_LCDS);
  for (uint8_t i = 0;i < 16;++i){
//    SoftPWM.set( i, 255 );
//    SoftPWM.set( i+16, 255 );    
    LCDStates[i] = 255;
    LCDStates[i+16] = 255;    
    updateLCDs();    
    delay( dur );
  }
  delay( dur*5 );
  for (uint8_t i = 0;i < 16;++i){
//    SoftPWM.set( i, 0 );
//    SoftPWM.set( i+16, 0 );    
    LCDStates[i] = 255;
    LCDStates[i+16] = 255;    
    updateLCDs();    
    delay(dur);
  }
}

//

void swooshSmooth( int speed, int waveWidth ){
//  int waveWidth = 256*8;
  int allWidth = 16*256;
  int pos = -waveWidth;
  int val;

  unsigned long const WAIT = 1 * 1000000 / 100;
  unsigned long nextMicros = 0;
  
  int tempCnt = 0;
  
  Serial.print("swooshSmooth ");
  Serial.println( waveWidth,DEC );  
  
  for( int i=-waveWidth;i<allWidth+waveWidth;i+=speed ){
    while (micros() < nextMicros);
    nextMicros = micros() + WAIT;
    
    
    tempCnt ++;
    Serial.println(tempCnt);
    
//    Serial.print(" POS ");
//    Serial.print( i );
//    Serial.print(" :     ");    
    for (uint8_t pin = 0;pin < 16;++pin){
//      val = constrain( waveWidth - abs(allWidth-pin*256), 0, waveWidth );
      val = abs((pin*256) - i);
      val = waveWidth - constrain( val, 0, waveWidth );
//      val = val < waveWidth ? 255 : 0;
      val = map( val, 0, waveWidth, 0, 255);
      val = pgm_read_byte_near(exp_gamma+val);
//      SoftPWM.set( pin, val );
//      SoftPWM.set( pin+16, val );      
      LCDStates[i] = val;
      LCDStates[i+16] = val;    
//      Serial.print( i );
//      Serial.print(" :     ");    
    }
      updateLCDs();    
  }
}

//

//void swooshSmooth( int dur ){
//  
//}


// ----------------------------------------
// Wave
//
void wave( float speed ){
  unsigned long const WAIT = 1 * 1000000 / 100;
  unsigned long nextMicros = 0;
  int val;  
  float l = 3.1415926535 * 2;
  for( float i=0;i<l;i+=speed ){
    while (micros() < nextMicros);
    nextMicros = micros() + WAIT;
//      val = sin( i );
//      Serial.println( sin( i ) );
    for (uint8_t pin = 0;pin < 16;++pin){
      val = map( sin( i+(pin*.25f) )*1000, -1000, 1000, 0, 255);
//      val = pgm_read_byte_near(exp_gamma+val);
//       SoftPWM.set( pin, val );
//       SoftPWM.set( pin+16, val );
      LCDStates[pin] = val;
      LCDStates[pin+16] = val;    
    }
      updateLCDs();    
  }
}


// ----------------------------------------
// Random
//
void randomFeed( float speed ){
  unsigned long const WAIT = 1 * 1000000 / 100;
  unsigned long nextMicros = 0;
  int val;  
  float l = 3.1415926535 * 10;
  for( float i=0;i<l;i+=speed ){
    while (micros() < nextMicros);
    nextMicros = micros() + WAIT;
//      val = sin( i );
//      Serial.println( sin( i ) );
    for (uint8_t pin = 0;pin < 32;++pin){
      val = map( sin( i+(pin*10) )*1000, -1000, 1000, 0, 255);
      val = pgm_read_byte_near(exp_gamma+val);
//      SoftPWM.set( pin, val );      
      LCDStates[pin] = val;
    }
      updateLCDs();    
  }
}


void fade( float speed ){
  
  long nextMillis = 0;
//  uint8_t i = 14;
  for (uint8_t i = 0;i < AMOUNT_LCDS;++i)
//  for (;i < SoftPWM.size();++i)
  {
//    Serial.print( micros() );
//    Serial.print( " loop(): " );
//    Serial.print( i );
//    Serial.println();

//    delay(2000);

    unsigned long const WAIT = 1 * 1000000 / 255 / 2;
    unsigned long nextMicros = 0;
    for (int v = 0;v <= 255 - 1; ++v)
    {
      while (micros() < nextMicros);
      nextMicros = micros() + WAIT;
      int brightLevel=v;
      brightLevel=pgm_read_byte_near(exp_gamma+v);  // gamma transform
//      SoftPWM.set( i, brightLevel );
      LCDStates[i] = brightLevel;
      updateLCDs();    
    }
//    delay(2000);
    for (int v = 255 - 1; v >= 0; --v)
    {
      while (micros() < nextMicros);
      nextMicros = micros() + WAIT;
//      SoftPWM.set( i, v );
      int brightLevel=v;
      brightLevel=pgm_read_byte_near(exp_gamma+v);  // gamma transform
//      SoftPWM.set( i, brightLevel );
      LCDStates[i] = brightLevel;
      updateLCDs();    
    }
  }
}
