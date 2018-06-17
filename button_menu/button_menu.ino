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
 * Description: Full featured stepper motor telescope focus controller.
 *
 * Author: Kiehyun Kevin Park
 * 
 *   Kiehyun.Park@gmail.com
 */


#define FirmwareDate   "May. 31. 2018"
#define FirmwareNumber "1.0b"
#define FirmwareName   "GSFocus"
#define FirmwareTime   "14:00:00"

#include "Constants.h"

//List of Libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//Adafruit_SSD1306 display(OLED);
#include <U8x8lib.h>
//#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send ACK

#include <DHT.h>;
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;

#define MENU_ITEMS 4
char *menu_strings[MENU_ITEMS] = { "First Line", "Second Item", "3333333", "abcdefg" };

uint8_t menu_current = 0;
uint8_t menu_redraw_required = 0;
uint8_t last_key_code = KEY_NONE;


//////////

void setup(){
Serial.begin(9600);
dht.begin();

// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)

//welcomeOLED();
display.clearDisplay();
delay(50);
  
uiSetup();                      // setup key detection and debounce algorithm
  menu_redraw_required = 1;     // force initial redraw
}

void loop(){
/*
float val_hum = dht.readHumidity();
float val_temp = dht.readTemperature();
disp_AWS();
updateAWS_OLED();
display.clearDisplay();
delay(2000);

welcomeOLED()
display.clearDisplay();
delay(100);
*/

uiStep();                                     // check for key press
  if (  menu_redraw_required != 0 ) {
    u8g.firstPage();
    do  {
      drawMenu();
    } while( u8g.nextPage() );
    menu_redraw_required = 0;
  }
updateMenu();                            // update menu bar
}

void disp_AWS(){
delay(100);
float val_hum = dht.readHumidity();
float val_temp= dht.readTemperature();
//Print temp and humidity values to serial monitor
Serial.print("Humidity: ");
Serial.print(val_hum);
Serial.print(" %, Temp: ");
Serial.print(val_temp);
Serial.println(" Celsius");
delay(100); //Delay 2 sec.
}

void updateAWS_OLED(void) {
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
/*
void welcomeOLED() {
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.clearDisplay();
display.println("Welcome!!");
display.setCursor(0,15);
display.println("GS Focus.. Ver." + String(FirmwareNumber));
display.println("    (" + String(FirmwareDate) + ")");
display.display();
delay(100);
}
*/

void uiStep(void) {
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(pinU) == LOW )
    uiKeyCodeFirst = KEY_PREV;
  else if ( digitalRead(pinD) == LOW )
    uiKeyCodeFirst = KEY_NEXT;
  else if ( digitalRead(pinR) == LOW )
    uiKeyCodeFirst = KEY_SELECT;
  else if ( digitalRead(pinL) == LOW )
    uiKeyCodeFirst = KEY_BACK;
  else 
    uiKeyCodeFirst = KEY_NONE;
  
  if ( uiKeyCodeSecond == uiKeyCodeFirst )
    uiKeyCode = uiKeyCodeFirst;
  else
    uiKeyCode = KEY_NONE;
}

void drawMenu(void) {
  uint8_t i, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, i*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, i*h, menu_strings[i]);
  }
}

void updateMenu(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      menu_current++;
      if ( menu_current >= MENU_ITEMS )
        menu_current = 0;
      menu_redraw_required = 1;
      break;
    case KEY_PREV:
      if ( menu_current == 0 )
        menu_current = MENU_ITEMS;
      menu_current--;
      menu_redraw_required = 1;
      break;
  }
}

void uiSetup(void) {
  // configure input keys   
  pinMode(pinU, INPUT);           // set pin to input
  digitalWrite(pinU, HIGH);       // turn on pullup resistors
  pinMode(pinD, INPUT);           // set pin to input
  digitalWrite(pinD, HIGH);       // turn on pullup resistors
  pinMode(pinR, INPUT);           // set pin to input
  digitalWrite(pinR, HIGH);       // turn on pullup resistors
  pinMode(pinL, INPUT);           // set pin to input
  digitalWrite(pinL, HIGH);       // turn on pullup resistors
}

