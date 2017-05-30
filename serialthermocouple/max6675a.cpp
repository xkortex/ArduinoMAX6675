// this library is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif
#include <SPI.h>
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
  
  SPI.begin(); // Init SPI
  SPI.setBitOrder(MSBFIRST); // Sets the order of the bits shifted out of and into the SPI bus
  SPI.setDataMode(SPI_MODE1); // Base value of clock is 1, data is captured on clock's falling edge.
  SPI.setClockDivider(SPI_CLOCK_DIV32); // Set SPI data rate to 16mhz/4. IE: 4mhz.
}
double MAX6675::readCelsius(void) {
  /* Read the temperature from the SPI interface
  Currently runs at about 375 microseconds per call (2.6 KHz). Not bad, but theoretical is near 1 MHz. 
  Using #defines we get to about 360 microseconds. Not a great improvement. 
  Need to do low level magic to get faster
  */
  //unsigned long start = micros();        // profiling
  unsigned int temp_reading;
  double tempC;

 // stop any conversion process
 delayMicroseconds(5000);
 digitalWrite(cs,LOW); // Set MAX7765 /CS Low
 delayMicroseconds(5000);
 
 // initiate a new conversion
 digitalWrite(cs,HIGH); // Set MAX7765 /CS High
 delayMicroseconds(250000); // wait for conversion to finish..
 
 noInterrupts();
 // read result
 digitalWrite(cs,LOW); // Set MAX7765 /CS Low
 delayMicroseconds(1000);
 
 temp_reading = SPI.transfer(0xff) << 8;  
 temp_reading += SPI.transfer(0xff);  

 digitalWrite(cs,HIGH); // Set MAX7765 /CS High
 delayMicroseconds(1000);
 interrupts();
 
 // check result
 if(temp_reading & 0x4) {
     return(-1); // Failed / NoCOnnection Error
 }
  temp_reading >>= 3; // Don't fully understand this bitshift, must be a MAX thing
  tempC = temp_reading * 0.25;
  return tempC; //Convert to Degc
   
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

