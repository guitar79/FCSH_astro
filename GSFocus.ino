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
#define FirmwareDate   "01 09 18"
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

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH 16

DHT dht(DHTPIN, DHTTYPE);



//Variables
int chk;
float hum; //Stores humidity value
float temp; //Stores temperature value

void setup()
{
Serial.begin(9600);
dht.begin();
}

void loop()
{
delay(2000);
//Read data and store it to variables hum and temp
hum = dht.readHumidity();
temp= dht.readTemperature();
//Print temp and humidity values to serial monitor
Serial.print("Humidity: ");
Serial.print(hum);
Serial.print(" %, Temp: ");
Serial.print(temp);
Serial.println(" Celsius");
delay(10000); //Delay 2 sec.
}

void setup() { 
Serial.begin(9600);

// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
// init done

display.clearDisplay();
delay(2000);
updateText();
display.clearDisplay();
}

void loop() {
updateText();
display.clearDisplay();
}

void updateText(void) {
float h = dht.readHumidity();
float t = dht.readTemperature();
display.setTextSize(2);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.clearDisplay();
display.println("T: " + String(t, 2));
display.setCursor(0,16);
display.println("H: " + String(h, 2));
display.display();
Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.println(" *C ");
delay(400);
}
