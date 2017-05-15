// this library is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif
#include <util/delay.h>
#include <stdlib.h>
#include "max6675a.h"

#define delaym() delayMicroseconds(1)
#define P_MISO miso
#define P_CS cs
#define P_SCLK sclk

MAX6675::MAX6675(int8_t SCLK, int8_t CS, int8_t MISO) {
  sclk = SCLK;
  cs = CS;
  miso = MISO;

  //define pin modes
  pinMode(P_CS, OUTPUT);
  pinMode(P_SCLK, OUTPUT); 
  pinMode(P_MISO, INPUT);

  digitalWrite(cs, HIGH);
}
double MAX6675::readCelsius(void) {
  /* Read the temperature from the SPI interface
  Currently runs at about 375 microseconds per call (2.6 KHz). Not bad, but theoretical is near 1 MHz. 
  Using #defines we get to about 360 microseconds. Not a great improvement. 
  Need to do low level magic to get faster
  */
  //unsigned long start = micros();        // profiling
  uint16_t v;

  digitalWrite(P_CS, LOW);
  delaym();

  v = spiread();
  v <<= 8;
  v |= spiread();

  digitalWrite(P_CS, HIGH);

  if (v & 0x4) {
    // uh oh, no thermocouple attached!
    tempC = NAN; 
    //return -100;
  }

  v >>= 3;

  tempC = v*0.25;
  //Serial.print("us ");                  // profiling
  //Serial.print(micros() - start);
  //Serial.print("\n");
  return tempC;
}

byte MAX6675::spiread(void) { 
  int i;
  byte d = 0;

  for (i=7; i>=0; i--)
  {
    digitalWrite(P_SCLK, LOW);
    delaym();
    if (digitalRead(P_MISO)) {
      //set the bit to 0 no matter what
      d |= (1 << i);
    }

    digitalWrite(P_SCLK, HIGH);
    delaym();
  }

  return d;
}

