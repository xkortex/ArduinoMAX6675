// this example is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

// clion junk
//#include <Arduino.h>
//#include <SoftwareSerial/SoftwareSerial.h>
//#include <HardwareSerial.h>
// end clion junk

#include <SPI.h>
#include "max6675a.h"
#include "stopwatch.h"

const int8_t thermoDO = 12; // is MISO
const int8_t thermoCS = 10;
const int8_t thermoCLK = 13;

// Globals
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
int vccPin = 3;
int gndPin = 2;
volatile double G_t_read;
volatile double G_t_filt;
double IIR_LAMBDA;
double buffer[10];
int i;
unsigned long start, now, dt, time_to_read_temp;
Stopwatch time_temp_reader(200000); // microseconds - MAX6675 specifies Conversion Time: TYPICAL: 0.17 s MAXIMUM: 0.22 s
Stopwatch time_temp_printer(200000); // microseconds

/* By specification, the MAX6675 can only operate at a maximum read rate of about 5 Hz
*/

void get_temp(void) {
  start = micros();
  G_t_read = GetTemperature();
  now = micros();
  dt = now - start;
  G_t_filt = IIR_LAMBDA * G_t_read + (1 - IIR_LAMBDA) * G_t_filt; // IIR low pass filter
  time_to_read_temp = IIR_LAMBDA * dt + (1 - IIR_LAMBDA) * time_to_read_temp;
}

void get_and_print_temp(void) {
   //G_t_read = thermocouple.readCelsius();
   
   //G_t_read = GetTemperature();
   Serial.println(G_t_filt); // testing to see if it's just serial interrupts slowing things down. Apparently not. 
   //Serial.println(time_to_read_temp);
   
   //Serial.println(time_temp_reader.last_time());
   //Serial.println(micros());
   
}


//-------------------------
double GetTemperature (void)
{
unsigned int temp_reading;
  double tempC;

 // stop any conversion process
 delayMicroseconds(5000);
 digitalWrite(thermoCS,LOW); // Set MAX7765 /CS Low
 delayMicroseconds(5000);
 
 // initiate a new conversion
 digitalWrite(thermoCS,HIGH); // Set MAX7765 /CS High
 delayMicroseconds(250000); // wait for conversion to finish..
 
 noInterrupts();
 // read result
 digitalWrite(thermoCS,LOW); // Set MAX7765 /CS Low
 delayMicroseconds(1000);
 
 temp_reading = SPI.transfer(0xff) << 8;  
 temp_reading += SPI.transfer(0xff);  

 digitalWrite(thermoCS,HIGH); // Set MAX7765 /CS High
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

//-------------------------

  
void setup() {
  Serial.setTimeout(50); // something to help with the buffer pileup that occurs when reading too fast
  Serial.begin(57600); // 9600 is stable (obviously). 115200 seems to be stable though a bit dodgy at times. 
  // use Arduino pins 
  pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);
  
  pinMode(thermoCS,OUTPUT); // MAX6675/6674 /CS Line must be an output for hardware SPI
  digitalWrite(thermoCS,HIGH); // Set MAX7765 /CS High

  SPI.begin(); // Init SPI
  SPI.setBitOrder(MSBFIRST); // Sets the order of the bits shifted out of and into the SPI bus
  SPI.setDataMode(SPI_MODE1); // Base value of clock is 1, data is captured on clock's falling edge.
  SPI.setClockDivider(SPI_CLOCK_DIV32); // Set SPI data rate to 16mhz/4. IE: 4mhz.
  
  IIR_LAMBDA = 0.5;
  G_t_filt = 25;
  time_to_read_temp = 100000;
  
  Serial.println("MAX6675 test");
  //long unsigned int size_of_double = sizeof(double);
  Serial.println("Sizeof double: ");
  Serial.print(sizeof(double));
  Serial.print(sizeof(float));
  Serial.println();
  // wait for MAX chip to stabilize
  delay(500);
  Serial.println("GO!");
}

void loop() {
  // basic readout test, just print the current temp
  
//   Serial.print("C = "); 
//   get_and_print_temp();
   time_temp_reader.call(&get_temp);
   time_temp_printer.call(&get_and_print_temp);
//   delay(100);
}
