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

//상수들은 따로 저장함
#include "Constants.h"

//Libraries
#include <Arduino.h>
#include <Wire.h>

/*
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(OLED);
*/

//#include <U8g2lib.h>
#include "U8glib.h"
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI //okok
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); // Fast I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

#include <Stepper.h>
#include <DRV8825.h>
DRV8825 stepper(MOTOR_STEPS, DIR, STEP, MS0, MS1, MS2);

#include <EEPROM.h>

#include <DHT.h>;
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;

//버튼 핀
//uint8_t uiKeyPrev = 8;
//uint8_t uiKeyNext = 9;
//uint8_t uiKeySelect = 10;
//uint8_t uiKeyBack = 7;
uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;


#define MENU0_ITEMS 3
const char *menu0_strings[MENU0_ITEMS] = { "Focus CTL", "Heat CTL", "Setup"};
#define MENU10_ITEMS 2
const char *menu10_strings[MENU10_ITEMS] = { "Focus IN (∧)", "Focus OUT (∨)"};
#define MENU20_ITEMS 3
const char *menu20_strings[MENU20_ITEMS] = { "Heat PWR UP (∧)", "Heat PWR DOWN (∨)", "Heat PWR AUTO (＞)"};
#define MENU30_ITEMS 4
const char *menu30_strings[MENU30_ITEMS] = { "Menu30 - 1", "Menu30 - 2", "Menu30 - 3", "Menu30 - 4"};

uint8_t menu_current = 0;
uint8_t menu_redraw_required = 0;
uint8_t last_key_code = KEY_NONE;



// 버튼 상태 변수
int btn0State = 0;
int btn1State = 0;
int btn2State = 0;
int btn3State = 0;

//DRV8825 마이크로스텝 변수
int ms0State = 0;
int ms1State = 0;
int ms22State = 0;

float rotateDEG1 = 1.8;
float rotateDEG2 = 3.6;

//delay 값
int ssdelay = 50;
int lldelay = 1000;

void uiSetup(void) {
  // configure input keys 
  pinMode(uiKeyPrev, INPUT_PULLUP);           // set pin to input with pullup
  pinMode(uiKeyNext, INPUT_PULLUP);           // set pin to input with pullup
  pinMode(uiKeySelect, INPUT_PULLUP);           // set pin to input with pullup
  pinMode(uiKeyBack, INPUT_PULLUP);           // set pin to input with pullup
}

void uiStep(void) {
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(uiKeyPrev) == LOW )
    uiKeyCodeFirst = KEY_PREV;
  else if ( digitalRead(uiKeyNext) == LOW )
    uiKeyCodeFirst = KEY_NEXT;
  else if ( digitalRead(uiKeySelect) == LOW )
    uiKeyCodeFirst = KEY_SELECT;
  else if ( digitalRead(uiKeyBack) == LOW )
    uiKeyCodeFirst = KEY_BACK;
  else 
    uiKeyCodeFirst = KEY_NONE;
  
  if ( uiKeyCodeSecond == uiKeyCodeFirst )
    uiKeyCode = uiKeyCodeFirst;
  else
    uiKeyCode = KEY_NONE;
}

void updateOLED_AWS(void) {  //OLED 상단에 AWS 값을 출력  //아직 미완성
  float val_Temp= dht.readTemperature();
  float val_Humi = dht.readHumidity();
  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  String temp = "aaa";
  u8g.drawStr(2, 0, "Temp:");
  u8g.drawStr(66, 0, "Humi:");
  //u8g.drawStr(2, 0, char(val_Temp, 1));
  //u8g.drawStr(66, 0, char(val_Humi, 1));
}

void updateOLED_Welcome(void) {  //OLED 환영 메세지
  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  u8g.drawStr(2, 0, "Welcome!!");
  //u8g.drawStr(2, 20, ("GS Focus.. Ver." + String(FirmwareNumber)));
  delay(lldelay);

}

void updateSerialMonitor_AWS() {  //AWS를 시리얼 모니터로 출력
  //Read data and store it to variables hum and temp
  float val_hum = dht.readHumidity();
  float val_temp = dht.readTemperature();
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(val_hum);
  Serial.print(" %, Temp: ");
  Serial.print(val_temp);
  Serial.println(" Celsius");
  delay(ssdelay);
}

void drawMenu0(void) {
  uint8_t i, h;
  u8g_uint_t w, d;
  //u8g.setFont(u8g_font_6x13);
  //u8g.setFontRefHeightText();
  //u8g.setFontPosTop();

  updateOLED_AWS();   //OLED 상단에 AWS 값을 출력
    
  h = u8g.getFontAscent()-u8g.getFontDescent()+1;
  w = u8g.getWidth();
  for( i = 0; i < MENU0_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu0_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, (i+2)*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, (i+2)*h, menu0_strings[i]);
  }
}

void drawMenu10(void) {
  uint8_t i, h;
  u8g_uint_t w, d;
  //u8g.setFont(u8g_font_6x13);
  //u8g.setFontRefHeightText();
  //u8g.setFontPosTop();

  updateOLED_AWS();   //OLED 상단에 AWS 값을 출력
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU10_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu10_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == (menu_current - 10) ) {
      u8g.drawBox(0, (i+2)*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, (i+2)*h, menu10_strings[i]);
  }
  u8g.drawStr(d, (i+3)*h, "msg");
}

void drawMenu20(void) {
  uint8_t i, h;
  u8g_uint_t w, d;
  //u8g.setFont(u8g_font_6x13);
  //u8g.setFontRefHeightText();
  //u8g.setFontPosTop();

  updateOLED_AWS();   //OLED 상단에 AWS 값을 출력
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU20_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu20_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == (menu_current - 20) ) {
      u8g.drawBox(0, (i+2)*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, (i+2)*h, menu20_strings[i]);
  }
}

void drawMenu30(void) {
  uint8_t i, h;
  u8g_uint_t w, d;
  //u8g.setFont(u8g_font_6x13);
  //u8g.setFontRefHeightText();
  //u8g.setFontPosTop();

  updateOLED_AWS();   //OLED 상단에 AWS 값을 출력
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU30_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu30_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == (menu_current - 30) ) {
      u8g.drawBox(0, (i+2)*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, (i+2)*h, menu30_strings[i]);
  }
}


void updateMenu(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;

  if ( menu_current < 10 ) {
    switch ( uiKeyCode ) {
      case KEY_NEXT:
        menu_current++;
        if ( menu_current >= MENU0_ITEMS )
          menu_current = 0;
        menu_redraw_required = 1;
        break;
      case KEY_PREV:
        if ( menu_current == 0 )
          menu_current = MENU0_ITEMS;
        menu_current--;
        menu_redraw_required = 1;
        break;
      case KEY_SELECT:
        menu_current = (menu_current + 1) * 10 ;
        menu_redraw_required = 1;
        break;
      case KEY_BACK:
        menu_current = uint8_t((menu_current)/10) ;
        menu_redraw_required = 1;
        break;
    }
  }
  
  if ( menu_current >= 10 & menu_current < 20 ) {
    switch ( uiKeyCode ) {
      case KEY_NEXT:
        menu_current++;
        if ( menu_current >= ( 10 + MENU10_ITEMS - 1 ))
          menu_current = ( 10 + MENU10_ITEMS - 1 );
        stepper.rotate(rotateDEG1);
        //const char msg = "Motor is rotated toword inside";
        menu_redraw_required = 1;
        break;
      case KEY_PREV:
        menu_current--;
        if ( menu_current < 10 )
          menu_current = 10 ;
        stepper.rotate(rotateDEG1);
        //const char msg = "Motor is rotated toword outside";
        menu_redraw_required = 1;
        break;
      case KEY_SELECT:
        menu_redraw_required = 1;
        break;
      case KEY_BACK:
        menu_current = uint8_t((menu_current)/10 - 1) ;
        menu_redraw_required = 1;
        break;
    }
  }

  if ( menu_current >= 20 & menu_current < 30 ) {
    switch ( uiKeyCode ) {
      case KEY_NEXT:
        menu_current++;
        if ( menu_current >= ( 20 + MENU20_ITEMS - 1 ))
          menu_current = ( 20 + MENU20_ITEMS - 1 );
        menu_redraw_required = 1;
        break;
      case KEY_PREV:
        menu_current--;
        if ( menu_current < 20 )
          menu_current = 20 ;
        menu_redraw_required = 1;
        break;
      case KEY_SELECT:
        menu_current--;
        if ( menu_current == ( 20 + MENU20_ITEMS - 1 ))
          // Set heat auto ;
        menu_redraw_required = 1;
        break;
      case KEY_BACK:
        menu_current = uint8_t((menu_current)/10 - 1 ) ;
        menu_redraw_required = 1;
        break;
    }
  }

  if ( menu_current >= 30 & menu_current < 40 ) {
    switch ( uiKeyCode ) {
      case KEY_NEXT:
        menu_current++;
        if ( menu_current >= ( 30 + MENU30_ITEMS) )
          menu_current = 30;
        menu_redraw_required = 1;
        break;
      case KEY_PREV:
        if ( menu_current == 30 )
          menu_current = ( 30 + MENU30_ITEMS ) ;
        menu_current--;
        menu_redraw_required = 1;
        break;
      case KEY_SELECT:
        //menu_current = (menu_current+1) * 10 ;
        menu_redraw_required = 1;
        break;
      case KEY_BACK:
        menu_current = uint8_t((menu_current)/10 - 1 ) ;
        menu_redraw_required = 1;
        break;
    }
  }
}

void clearOLED(){
    u8g.firstPage();  
    do {
    } while( u8g.nextPage() );
}

void setup() { 

  Serial.begin(9600); // 시리얼통신
  dht.begin();  // DHT
  
  //pinMode(ledPin, OUTPUT); // LED 테스트할 때만 설정

  uiSetup();                    // setup key detection and debounce algorithm
  menu_redraw_required = 1;  
  
  // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
  stepper.begin(1, 1);
  clearOLED(); 
  u8g.firstPage();
    do  {
    updateOLED_Welcome();   //OLED 환영 메세지
    } while ( u8g.nextPage() );
  void clearOLED();
}

void loop() {
  uiStep();                   // check for key press
  if (  menu_redraw_required != 0 & menu_current < 10 ) {
    u8g.firstPage();
    do  {
      //updateOLED_AWS();   //OLED 상단에 AWS 값을 출력
      drawMenu0();
    } while ( u8g.nextPage() );
    menu_redraw_required = 1;
  }
  if (  menu_redraw_required != 0 & menu_current >= 10 & menu_current < 20) {
    u8g.firstPage();
    do  {
      //updateOLED_AWS();   //OLED 상단에 AWS 값을 출력
      drawMenu10();
    } while ( u8g.nextPage() );
    menu_redraw_required = 1;
  }
  if (  menu_redraw_required != 0 & menu_current >= 20 & menu_current < 30) {
    u8g.firstPage();
    do  {
      //updateOLED_AWS();   //OLED 상단에 AWS 값을 출력
      drawMenu20();
    } while ( u8g.nextPage() );
    menu_redraw_required = 1;
  }
   if (  menu_redraw_required != 0 & menu_current >= 30 & menu_current < 40) {
    u8g.firstPage();
    do  {
      //updateOLED_AWS();   //OLED 상단에 AWS 값을 출력
      drawMenu30();
    } while ( u8g.nextPage() );
    menu_redraw_required = 1;
  }
  
  updateMenu();  
  
  /*
  // 스위치의 값을 읽어서 btnState 변수에 저장한다.
  btn0State = digitalRead(btn0); 
  btn1State = digitalRead(btn1);
  btn2State = digitalRead(btn2);
  btn3State = digitalRead(btn3);
  //updateSerialMonitor_AWS();//AWS를 시리얼 모니터로 출력
  
  // Tell motor to rotate any degrees. That's it.
  if (btn1State == LOW) { // 만약, 버튼0을 누르면 !
    stepper.rotate(rotateDEG1);
    display.println("Motor is rotated " + String(rotateDEG1) + " degrees ");
    Serial.println("Motor is rotated " + String(rotateDEG1) + " degrees ");
  }

  // Tell motor to rotate any degrees. That's it.
  if (btn2State == LOW) { // 만약, 버튼0을 누르면 !
    stepper.rotate(rotateDEG2);
    display.println("Motor is rotated " + String(rotateDEG2) + " degrees ");
    Serial.println("Motor is rotated " + String(rotateDEG2) + " degrees ");
  }
  
  //btn_OLED_TEST();  //버튼 동작 테스트 함수
  
  display.display();
  display.clearDisplay();
  */
}

