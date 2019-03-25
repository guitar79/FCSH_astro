/* Test program 3, test the DS18B20 sensor probes
requires  probes connected and serial port monitor
*/

#include <Arduino.h>
#include <math.h>
#include <OneWire.h>                    // needed for DS18B20 temperature probe
#include <myDallasTemperature.h>        // needed for DS18B20 temperature probe

#define ch1temp 2     // temperature probe on pin 2, remember to use 4.7k pullup
OneWire oneWirech1(ch1temp);           // setup temperature probe 1

// Pass our oneWire reference to Dallas Temperature library
DallasTemperature sensor1(&oneWirech1); // probe ch1

// DB18B20 info
#define MaxProbes 2
#define TEMP_PRECISION 10     // Set the DS18B20s precision (10bit =0.25degrees) 
DeviceAddress tpAddress;
int tprobe1 = 0;              // these indicate if there is a probe attached to that channel
float ch1tempval;             // temperature from sensor is not float

// --------------------------------------------------------
void setup() {
  Serial.begin(9600);    // initialise serial port
  Serial.println("DS18B20 probe test and identification");

  // start temperature sensor DS18B20
  tprobe1 = 0;  // set probe indicator OFF
  sensor1.begin();
  sensor1.getDeviceCount();    // should return 1 if probe connected
  for (int i = 0; i < MaxProbes; i++) {
    // Search the wire for address
    if (sensor1.getAddress(tpAddress, i)) { // founnd?
      Serial.print("Found probe ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tpAddress);
      Serial.println();
      tprobe1 = 1;  // there is a probe1
      Serial.println("Setting resultion of sensor to 10bit");
      sensor1.setResolution(tpAddress, TEMP_PRECISION);
    }
  }

  if( tprobe1 == 1 ) Serial.println("Temp probe 1 found");
  else Serial.println("Temp probe 1 NOT found"); 
  delay(1000);
}

// --------------------------------------------------------
void loop() {
  Serial.print("#Temp probes found:");
  Serial.println(tprobe1);
  if( tprobe1 == 1 ) {
      Serial.println("Requesting temperature");
      sensor1.requestTemperatures();
      delay(750 / (1 << (12 - TEMP_PRECISION))); // should be long enough to wait
      ch1tempval = 0;
      if( tprobe1 == 1 ) 
        ch1tempval = sensor1.getTempCByIndex(0);    // get channel 1 temperature
      Serial.print("Temperature is ");
      Serial.print(ch1tempval,1);
      Serial.println(" Celsius");
      Serial.println("This program will repeat in 5s.");
      delay(5000);
  }
  else {
      Serial.println("No temperature probe found");
      delay(5000);
  }
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++)   {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}


