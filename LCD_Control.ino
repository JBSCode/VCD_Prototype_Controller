/* define this before including SoftPWM.h for printInterruptLoad()
 * to work. */
//#define __DEBUG_SOFTPWM__ 1

#include <SoftPWM_Diff.h>


/* pins_arduino.h defines the pin-port/bit mapping as PROGMEM so
 * you have to read them with pgm_read_xxx(). That's generally okay
 * for ordinary use, but really bad when you're writing super fast
 * codes because the compiler doesn't treat them as constants and
 * cannot optimize them away with sbi / cbi instructions.
 *
 * Therefore we have to tell the compiler the PORT and BIT here.
 * Hope someday we can find a way to workaround this.
 *
 * check the manuel of your MCU for port / bit mapping. */
 
// LEFT TOP 
SOFTPWM_DEFINE_CHANNEL( 8 + 8, DDRA, PORTA, PORTA0 );    // 22
SOFTPWM_DEFINE_CHANNEL( 9 + 8, DDRA, PORTA, PORTA1 );    // 23
SOFTPWM_DEFINE_CHANNEL( 10 + 8, DDRA, PORTA, PORTA2 );    // 24
SOFTPWM_DEFINE_CHANNEL( 11 + 8, DDRA, PORTA, PORTA3 );    // 25
SOFTPWM_DEFINE_CHANNEL( 12 + 8, DDRA, PORTA, PORTA4 );    // 26
SOFTPWM_DEFINE_CHANNEL( 13 + 8, DDRA, PORTA, PORTA5 );    // 27
SOFTPWM_DEFINE_CHANNEL( 14 + 8, DDRA, PORTA, PORTA6 );    // 28
SOFTPWM_DEFINE_CHANNEL( 15 + 8, DDRA, PORTA, PORTA7 );    // 29

// LEFT BOTTOM
SOFTPWM_DEFINE_CHANNEL( 0, DDRC, PORTC, PORTC0 );    // 30
SOFTPWM_DEFINE_CHANNEL( 1, DDRC, PORTC, PORTC1 );    // 31
SOFTPWM_DEFINE_CHANNEL( 2, DDRC, PORTC, PORTC2 );   // 32
SOFTPWM_DEFINE_CHANNEL( 3, DDRC, PORTC, PORTC3 );   // 33
SOFTPWM_DEFINE_CHANNEL( 4, DDRC, PORTC, PORTC4 );   // 34
SOFTPWM_DEFINE_CHANNEL( 5, DDRC, PORTC, PORTC5 );   // 35
SOFTPWM_DEFINE_CHANNEL( 6, DDRC, PORTC, PORTC6 );   // 36
SOFTPWM_DEFINE_CHANNEL( 7, DDRC, PORTC, PORTC7 );   // 37

// RIGHT BOTTOM
SOFTPWM_DEFINE_CHANNEL( 0 + 8, DDRD, PORTD, PORTD7 );   // 38
SOFTPWM_DEFINE_CHANNEL( 1 + 8, DDRG, PORTG, PORTG2 );   // 39
SOFTPWM_DEFINE_CHANNEL( 2 + 8, DDRG, PORTG, PORTG1 );   // 40
SOFTPWM_DEFINE_CHANNEL( 3 + 8, DDRB, PORTB, PORTB3 );   // 50
SOFTPWM_DEFINE_CHANNEL( 4 + 8, DDRL, PORTL, PORTL7 );   // 42
SOFTPWM_DEFINE_CHANNEL( 5 + 8, DDRL, PORTL, PORTL6 );   // 43
SOFTPWM_DEFINE_CHANNEL( 6 + 8, DDRL, PORTL, PORTL5 );   // 44
SOFTPWM_DEFINE_CHANNEL( 7 + 8, DDRL, PORTL, PORTL4 );   // 45

// RIGHT TOP
SOFTPWM_DEFINE_CHANNEL( 8 + 16, DDRB, PORTB, PORTB0 );    // 53
SOFTPWM_DEFINE_CHANNEL( 9 + 16, DDRB, PORTB, PORTB1 );    // 52
SOFTPWM_DEFINE_CHANNEL( 10 + 16, DDRB, PORTB, PORTB2 );   // 51
SOFTPWM_DEFINE_CHANNEL( 11 + 16, DDRG, PORTG, PORTG0 );   // 41
SOFTPWM_DEFINE_CHANNEL( 12 + 16, DDRL, PORTL, PORTL0 );   // 49
SOFTPWM_DEFINE_CHANNEL( 13 + 16, DDRL, PORTL, PORTL1 );   // 48
SOFTPWM_DEFINE_CHANNEL( 14 + 16, DDRL, PORTL, PORTL2 );   // 47
SOFTPWM_DEFINE_CHANNEL( 15 + 16, DDRL, PORTL, PORTL3 );   // 46


/* Here you make an instance of desired channel counts you want
 * with the default 256 brightness levels (0 ~ 255) */
SOFTPWM_DEFINE_OBJECT( 32 );

/* Or you can make one with only 100 brightness levels (0 ~ 99).
 * by using less brightness levels, you may be able to use higher
 * pwm frequencies. */

/* if you want to use the SoftPWM object outside where it's defined,
 * add the following line to the file. */
SOFTPWM_DEFINE_EXTERN_OBJECT( 32 );

//uint8_t LCDStates[AMOUNT_LCDS];

void setupLCDs()
{
  Serial.begin( 115200 );

  /* begin with 60hz pwm frequency */
  SoftPWM.begin( 60 );
  
  /* print interrupt load for diagnostic purpose */
//  SoftPWM.printInterruptLoad();
  
  for( int i=0;i<AMOUNT_LCDS;i++ ){
    LCDStates[i] = 0*10;
  }
  updateLCDs();
}

void updateLCDs(){
  for (uint8_t i = 0;i < SoftPWM.size();++i){
    SoftPWM.set( i, LCDStates[i] );
  }
}

void setLCDS( unsigned char* values, int len ){
  int l = min(len,SoftPWM.size());
  Serial.print("SET LCDs: ");
  for(int i=0;i<l;i++){
    Serial.print(" ");
    Serial.print( values[i] , DEC);
    LCDStates[i] = values[i];
  }
  Serial.println();
}

uint8_t* getLCDStates(){
  return LCDStates;
} 



