// this example is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

// clion junk
//#include <Arduino.h>
//#include <SoftwareSerial/SoftwareSerial.h>
//#include <HardwareSerial.h>
// end clion junk

#include "max6675a.h"
#include "stopwatch.h"

const int thermoDO = 4; // is MISO
const int thermoCS = 5;
const int thermoCLK = 6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
int vccPin = 3;
int gndPin = 2;
double t_read;
Stopwatch time_temp_reader(200000); // microseconds
// Currently, there is an issue with SPI (presumably synchronization) that prevents going faster than about 5 Hz

void get_and_print_temp(void) {
   t_read = thermocouple.readCelsius();
   Serial.println(t_read);
   //Serial.println(time_temp_reader.last_time());
   //Serial.println(micros());
   
}

  
void setup() {
  Serial.begin(9600);
  // use Arduino pins 
  pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);
  
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
   time_temp_reader.call(&get_and_print_temp);
//   delay(100);
}
