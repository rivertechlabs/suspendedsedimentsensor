/******************************************************************************** 
  This code reads turbidity (in Lux) from two light detectors, temperature and pressure
  and saves the data onto an SD card. The code is meant to be loaded onto the Smart Water
  Sensor: Version Rhône. The code enables the sensor to go into extreme low power mode
  (0.07mA during sleep).

  For more information, please visit www.rivertechlabs.org or contact @rivertechjess

******************************************************************************* */

#include "ArduinoLowPower.h"
#include <MKRWAN.h>
#include <Wire.h>
//#include <Arduino_PMIC.h>
#include <Adafruit_Sensor.h>
#include "wiring_private.h"
#include "Adafruit_TSL2591.h"
#include <MS5803_05.h>
#include <SPI.h>
#include <SD.h>
#include <RTCZero.h>

MS_5803 sensor = MS_5803(512);
File myFile; //log file
String filename = "Rhone1.txt"; // name of .txt file with data
int count = 0;
RTCZero rtc;

#define PCF85263address 0x51   //define I2C address 
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
String days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

/******************************************************************************* 
  Define the custom TwoWire instance for SAMD21 since the TSL2591 has one 
  address and we have two TSLs (so we need two I2C busses)
********************************************************************************/
TwoWire myWire(&sercom3, 0, 1);  // Create the new wire instance assigning it to pin 0 and 1
extern "C"{
  void SERCOM3_Handler(void);

  void SERCOM3_Handler(void) {
    myWire.onService();
  }
}

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)
Adafruit_TSL2591 myTSL = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

/******************************************************************************* 
  Defining two functions to read the IR value from each TSL sensor on each 
  I2C bus
********************************************************************************/
void readASensor(uint16_t& ir){
  uint32_t lum = tsl.getFullLuminosity();
  ir = lum >> 16;
}

void readBSensor(uint16_t& ir){
  uint32_t lum = myTSL.getFullLuminosity();
  ir = lum >> 16;
}

void readBothChannels(uint16_t& a_ir,uint16_t& b_ir){
  readASensor(a_ir);
  delay(1);
  readBSensor(b_ir);
  delay(1);
}

/******************************************************************************* 
  Function to print time from internal RTC
********************************************************************************/
void print2digits(int number) {
  if (number < 10) {
    Serial.print("0"); // print a 0 before if the number is < than 10
  }
  Serial.print(number);
}

void print2myFile(int number) {
  if (number < 10) {
    myFile.print("0"); // print a 0 before if the number is < than 10
  }
  myFile.print(number);
}

/******************************************************************************* 
  Read date and time from PCF RTC
********************************************************************************/
byte bcdToDec(byte value){
 return ((value / 16) * 10 + value % 16);
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

/******************************************************************************* 
  Print PCF RTC time to serial monitor
********************************************************************************/
void print_PCFtime() {
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
}

/******************************************************************************* 
  Print MKR RTC time to serial monitor
********************************************************************************/
void print_MKRtime() {
  print2digits(rtc.getDay());
  Serial.print("/");
  print2digits(rtc.getMonth());
  Serial.print("/");
  print2digits(rtc.getYear());
  Serial.print(" ");

  // ...and time
  print2digits(rtc.getHours());
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());

  Serial.println();
}

/******************************************************************************* 
  Write all sensor data to the SD card
********************************************************************************/
void writeToSD(uint16_t& temp, uint16_t& pres, uint16_t& ir_a, uint16_t& ir_b, uint16_t& ir_a_off, uint16_t& ir_b_off){
  if(!myFile)
    return;

   myFile.print(dayOfMonth, DEC);myFile.print("/");
   myFile.print(month, DEC);myFile.print("/20");myFile.print(year, DEC);
   myFile.print(" ");
   myFile.print(hour, DEC);myFile.print(":");
   if (minute < 10){
      myFile.print("0");
    }
   myFile.print(minute, DEC);myFile.print(":");  
   if (second < 10){
      myFile.print("0");
    }  
   myFile.print(second, DEC); myFile.print(", ");

  print2myFile(rtc.getDay());myFile.print("/");
  print2myFile(rtc.getMonth());myFile.print("/");
  print2myFile(rtc.getYear());
  myFile.print(" ");
  print2myFile(rtc.getHours());myFile.print(":");
  print2myFile(rtc.getMinutes());myFile.print(":");
  print2myFile(rtc.getSeconds()); myFile.print(", ");

  myFile.print(temp); myFile.print(", ");
  myFile.print(pres); myFile.print(", ");
  myFile.print(ir_a); myFile.print(", ");
  myFile.print(ir_b); myFile.print(", ");
  myFile.print(ir_a_off); myFile.print(", ");
  myFile.println(ir_b_off);
  
}

/******************************************************************************* 
  Set up the code; to be run upon reset or initialization
********************************************************************************/
void setup() {
  Serial.begin(9600);

  //----------- Initialize internal RTC sensor
  rtc.begin(); // initialize RTC

  rtc.setTime(0, 0, 0);
  rtc.setDate(1, 1, 00);

  //----------- Initialize the IR sensor
  if (tsl.begin()) {
    Serial.println(F("Found a TSL2591 sensor"));
  } else {
    Serial.println(F("No sensor found ... check your wiring?"));
  }

  //----------- Initialize the MS5803 sensor
  if (sensor.initializeMS_5803(false)) { // used "false" here so all coefs. aren't printed
    Serial.println( "MS5803 CRC check OK." );
  } else {
    Serial.println( "MS5803 CRC check FAILED!" );
  }

  //----------- Initialize the second I2C channels
  pinPeripheral(0, PIO_SERCOM);  // PAD[0]   //Assign SDA function to pin 0
  pinPeripheral(1, PIO_SERCOM);  // PAD[1]   //Assign SCL function to pin 1

  if (myTSL.begin(&myWire)){
    Serial.println(F("Found a TSL2591 sensor"));
  } else {
    Serial.println(F("No sensor found ... check your wiring?"));
  }

  //----------- Initialize the SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) { // initialize
    Serial.println("initialization failed!");
  }
  Serial.println("initialization done.");

  //----------- Set up and write first line to .csv file
  myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
    Serial.println("test.txt - Created");
    Serial.print("Writing to test.txt...");
    myFile.println("DateTimePCF, DateTimeMKR, Temperature, Pressure, IR_A, IR_B, IR_A_OFF, IR_B_OFF");
    Serial.println("done.");
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }

  //----------- Initialize LED
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  
  //----------- Set all other pins to INPUT_PULLUP so lowest current draw in sleep
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(0, INPUT_PULLUP); //SDA2
  //pinMode(1, INPUT_PULLUP); //SCL2
  pinMode(2, INPUT_PULLUP); //motor step
  pinMode(3, INPUT_PULLUP); //motor dir
  //pinMode(4, INPUT_PULLUP); //CS of SD card
  //pinMode(5, INPUT_PULLUP); // IR LED
  //pinMode(6, INPUT_PULLUP); //isRecording LED (also connected to on board LED)
  pinMode(7, INPUT_PULLUP); //isRecording switch
  /*pinMode(8, INPUT_PULLUP); // next three are for SD card
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP); // next two are SDA1 and SCL1
  pinMode(12, INPUT_PULLUP);*/
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
}

void loop() {
  
  uint16_t degree, pascals, high_300_full_a, high_300_ir_a, high_300_full_b, high_300_ir_b;
  uint16_t high_300_full_a_avg, high_300_ir_a_avg, high_300_full_b_avg, high_300_ir_b_avg;
  uint16_t high_300_full_a_avg_off, high_300_ir_a_avg_off, high_300_full_b_avg_off, high_300_ir_b_avg_off;

  readPCF85263();
  Serial.print("PCF time: ");
  print_PCFtime();
  Serial.print("MKR time: ");
  print_MKRtime();
  
  sensor.readSensor(); // get pressure and temperature reading. 

  pascals = sensor.pressure();
  Serial.print("Pressure = ");
  Serial.print(pascals);
  Serial.println(" mbar");
  
  degree = sensor.temperature();
  Serial.print("Temperature = ");
  Serial.print(degree);
  Serial.println("C");

  // set gain and integration time for IR detectors
  myTSL.setGain(TSL2591_GAIN_HIGH);
  myTSL.setTiming(TSL2591_INTEGRATIONTIME_300MS);

  tsl.setGain(TSL2591_GAIN_HIGH);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);

  // turn on LED
  digitalWrite(5, HIGH);
  delay(100); //allow LED to heat up

  int sen = 1;
  long high_300_ir_a_tot = 0;
  long high_300_ir_b_tot = 0;
      
  while (sen < 16){ //average of 15 measurements
    readBothChannels(high_300_ir_a, high_300_ir_b);
    high_300_ir_a_tot = high_300_ir_a_tot + high_300_ir_a;
    high_300_ir_b_tot = high_300_ir_b_tot + high_300_ir_b;
    sen = sen + 1;
  }

  high_300_ir_a_avg = high_300_ir_a_tot / 15;
  high_300_ir_b_avg = high_300_ir_b_tot / 15;

  // turn off LED
  digitalWrite(5, LOW);

  int sen_off = 1;
  high_300_ir_a_tot = 0;
  high_300_ir_b_tot = 0;
      
  while (sen_off < 3){
    readBothChannels(high_300_ir_a, high_300_ir_b);
    high_300_ir_a_tot = high_300_ir_a_tot + high_300_ir_a;
    high_300_ir_b_tot = high_300_ir_b_tot + high_300_ir_b;
    sen_off = sen_off + 1;
  }

  high_300_ir_a_avg_off = high_300_ir_a_tot / 2;
  high_300_ir_b_avg_off = high_300_ir_b_tot / 2;
  
  Serial.print(high_300_ir_a_avg_off);Serial.print(",");
  Serial.print(high_300_ir_b_avg_off);Serial.print(",");
  
  Serial.print(high_300_ir_a_avg);Serial.print(",");
  Serial.println(high_300_ir_b_avg);

  myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
    Serial.println("File opened");
  }

  writeToSD(degree,pascals,high_300_ir_a_avg, high_300_ir_b_avg, high_300_ir_a_avg_off, high_300_ir_b_avg_off);

  tsl.disable();
  
  myFile.close();
  Serial.println("File closed");
  Serial.println("");

  LowPower.deepSleep(3600000);
  // sensor enters deep sleep for 1h: 3 600 000  = 60min*60sec*1000mili
}
