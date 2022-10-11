/* PCF85263AT RTC write/read 
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!To set the time, you need to run this code TWICE!!!!!! 
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * The first time to set the time, the second time with the line
 * setPCF85263(); commented out so that it doesn't reset the time 
 * every time the arduino restarts
  */


#include "ArduinoLowPower.h"
#include <MKRWAN.h>

#include "Wire.h"              //Enable I2C library
#define PCF85263address 0x51   //define I2C address 
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
String days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

byte bcdToDec(byte value){
 return ((value / 16) * 10 + value % 16);
}

byte decToBcd(byte value){
 return (value / 10 * 16 + value % 10);
}

void setPCF85263() {         // this function sets the time and date to the PCF85263
   Wire.beginTransmission(PCF85263address);
   Wire.write(0x01);
   Wire.write(decToBcd(second));  
   Wire.write(decToBcd(minute));
   Wire.write(decToBcd(hour));     
   Wire.write(decToBcd(dayOfMonth));
   Wire.write(decToBcd(dayOfWeek));  
   Wire.write(decToBcd(month));
   Wire.write(decToBcd(year));
   Wire.endTransmission();
 }
 
void readPCF85263() {          // this gets the time and date from the PCF85263
   Wire.beginTransmission(PCF85263address);
   Wire.write(0x01);
   Wire.endTransmission();
   Wire.requestFrom(PCF85263address, 7);
   second     = bcdToDec(Wire.read() & B01111111); // remove VL error bit
   minute     = bcdToDec(Wire.read() & B01111111); // remove unwanted bits from MSB
   hour       = bcdToDec(Wire.read() & B00111111); 
   dayOfMonth = bcdToDec(Wire.read() & B00111111);
   dayOfWeek  = bcdToDec(Wire.read() & B00000111);  
   month      = bcdToDec(Wire.read() & B00011111); // remove century bit, 1999 is over
   year       = bcdToDec(Wire.read());
 }

void setup(){
   Wire.begin();
   Serial.begin(9600);
   // must open serial monitor
   while (!Serial) {}

//------------- change the following to set your initial date
   dayOfWeek = 1;
   dayOfMonth = 11;
   month = 7;
   year = 22;

   // time is automatically taken from compile time
   String compile_date = __DATE__ " " __TIME__;
   Serial.print("Right now the computer says: ");
   Serial.println(compile_date);
   String new_hour =  compile_date.substring(12, 14);
   String new_min =  compile_date.substring(15, 17);
   String new_sec =  compile_date.substring(18, 20);
 
   second = new_sec.toInt();
   minute = new_min.toInt();
   hour = new_hour.toInt();

//------------- comment out the next line on second upload to keep the time from resetting every reset
 setPCF85263();
 }
 
void loop(){
   readPCF85263();
   Serial.print(days[dayOfWeek]); 
   Serial.print(" ");  
   Serial.print(month, DEC);
   Serial.print("/");
   Serial.print(dayOfMonth, DEC);
   Serial.print("/20");
   Serial.print(year, DEC);
   Serial.print(" - ");
   Serial.print(hour, DEC);
   Serial.print(":");
   if (minute < 10)
 {
   Serial.print("0");
 }
   Serial.print(minute, DEC);
   Serial.print(":");  
   if (second < 10)
 {
   Serial.print("0");
 }  
   Serial.println(second, DEC);  
   delay(1000);
}
