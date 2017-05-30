
/*
 Testing the Max6675
 03/03/2011
*/

#include <SPI.h>

#define CS_MAX 10 // MAX6675 CS Line

//-------------------------
double GetTemperature (void)
{
unsigned int temp_reading;
  double tempC;

 // stop any conversion process
 delay(5);
 digitalWrite(CS_MAX,LOW); // Set MAX7765 /CS Low
 delay(5);
 
 // initiate a new conversion
 digitalWrite(CS_MAX,HIGH); // Set MAX7765 /CS High
 delay(250); // wait for conversion to finish..
 
 // read result
 digitalWrite(CS_MAX,LOW); // Set MAX7765 /CS Low
 delay(1);
 
 temp_reading = SPI.transfer(0xff) << 8;  
 temp_reading += SPI.transfer(0xff);  

 digitalWrite(CS_MAX,HIGH); // Set MAX7765 /CS High
 delay(1);
 
 // check result
 if(temp_reading & 0x4) {
     return(-1); // Failed / NoCOnnection Error
 }
  temp_reading >>= 3; // Don't fully understand this bitshift, must be a MAX thing
  tempC = temp_reading * 0.25;
  return tempC; //Convert to Degc
   
}

//-------------------------
//-------------------------
void setup()
{
  Serial.begin(57600);
  Serial.println("start");

  pinMode(CS_MAX,OUTPUT); // MAX6675/6674 /CS Line must be an output for hardware SPI
  digitalWrite(CS_MAX,HIGH); // Set MAX7765 /CS High

  SPI.begin(); // Init SPI
  SPI.setBitOrder(MSBFIRST); // Sets the order of the bits shifted out of and into the SPI bus
  SPI.setDataMode(SPI_MODE1); // Base value of clock is 1, data is captured on clock's falling edge.
  SPI.setClockDivider(SPI_CLOCK_DIV32); // Set SPI data rate to 16mhz/4. IE: 4mhz.
}

//-------------------------

void loop()
{
double temperature = 0;

while(1)
 {
   delay(200);
   temperature = GetTemperature(); 
   if(temperature == -1)
     {
       Serial.println("No Connection");
     }
     else
     {
       Serial.println(temperature, 2); 
//       Serial.print("\n");
     }
 }
}

