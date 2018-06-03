/*
 * Title       GSFocus
 * by          Kiehyun Kevin Park
 *
 * Copyright (C) 2012 to 2018 Kiehyun Kevin Park.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Description:
 *   Full featured stepper motor telescope focus controller.
 *
 * Author: Kiehyun Kevin Park
 * 
 *   Kiehyun.Park@gmail.com
 */

// Use Config.xxxxx.h to configure OnStep to your requirements (아직 미확인)

// firmware info, these are returned by the ":GV?#" commands (아직 미확인)
#define FirmwareDate   "Jun. 1st. 2018"
#define FirmwareNumber "1.0b"
#define FirmwareName   "GSFocus"
#define FirmwareTime   "12:00:00"

#include "Constants.h"


/* How to use the DHT-22 sensor with Arduino uno
Temperature and humidity sensor
*/

//Libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <DHT.h>;
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//Variables
int chk;
float hum; //Stores humidity value
float temp; //Stores temperature value

void setup() { 
Serial.begin(9600);
dht.begin();

// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
// init done

display.clearDisplay();
delay(100);
updateOLED_Welcome();
display.clearDisplay();
}

void loop() {
updateSM();
updateOLED_AWS();
updateOLED_AWS1();
display.clearDisplay();
}

void updateOLED_AWS(void) {
float val_hum = dht.readHumidity();
float val_temp= dht.readTemperature();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.clearDisplay();
display.println("Temp:" + String(val_temp, 1)+ "C");
display.setCursor(76,0);
display.println("RH:" + String(val_hum, 1) + "%");
display.display();
delay(2000);
}

void updateOLED_AWS1(void) {
float val_hum = dht.readHumidity();
float val_temp= dht.readTemperature();
display.setTextSize(1);
display.setTextColor(WHITE);
display.println("1abcdefghijklmnopqrstuvwxyz");
display.println("2abcdefghijklmnopqrstuvwxyz");
display.println("3abcdefghijklmnopqrstuvwxyz");
display.println("4abcdefghijklmnopqrstuvwxyz");
display.display();
delay(2000);
}

void updateOLED_Welcome(void) {
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.clearDisplay();
display.println("Welcome!!");
//display.setCursor(0,15);
display.println("GS Focus.. Ver." + String(FirmwareNumber));
display.println("    (" + String(FirmwareDate) + ")");
display.display();
delay(1000);
}

void updateSM() {
//Read data and store it to variables hum and temp
float val_hum = dht.readHumidity();
float val_temp = dht.readTemperature();
//Print temp and humidity values to serial monitor
Serial.print("Humidity: ");
Serial.print(val_hum);
Serial.print(" %, Temp: ");
Serial.print(val_temp);
Serial.println(" Celsius");
delay(50); //Delay 2 sec.
}
