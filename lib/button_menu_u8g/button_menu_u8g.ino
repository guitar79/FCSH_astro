/*
  Menu.pde
  
  Simple Menu Selection
  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).
  
  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.
  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/
#include "U8glib.h"
// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported 
// devices with all constructor calls is here: https://github.com/olikraus/u8glib/wiki/device
//U8GLIB_NHD27OLED_BW u8g(13, 11, 10, 9);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_BW u8g(13, 11, 10, 9);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_GR u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_GR u8g(13, 11, 10, 9);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_BW u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_BW u8g(13, 11, 10, 9);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_GR u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_GR u8g(13, 11, 10, 9);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGS102 u8g(13, 11, 10, 9, 8);   // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM132 u8g(13, 11, 10, 9);    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM128 u8g(13, 11, 10, 9);    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM128_2X u8g(13, 11, 10, 9);   // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_ST7920_128X64_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64_1X u8g(18, 16, 17);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_128X64_4X u8g(18, 16, 17);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_192X32_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_192X32_1X u8g(18, 16, 17);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_4X u8g(18, 16, 17);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_1X u8g(13, 11, 10);  // SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10
//U8GLIB_ST7920_192X32_4X u8g(10);    // SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10, HW SPI
//U8GLIB_ST7920_202X32_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_202X32_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_202X32_1X u8g(18, 16, 17);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_202X32_4X u8g(18, 16, 17);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_LM6059 u8g(13, 11, 10, 9);   // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_LM6063 u8g(13, 11, 10, 9);   // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_BW u8g(10, 9);    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_GR u8g(13, 11, 10, 9);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_BW u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_GR u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8);   // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);   // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 14, 15, 17, 16);    // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16
//U8GLIB_LC7981_160X80 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16);  // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X64 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16);  // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X128 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_ILI9325D_320x240 u8g(18,17,19,U8G_PIN_NONE,16 );       // 8Bit Com: D0..D7: 0,1,2,3,4,5,6,7 en=wr=18, cs=17, rs=19, rd=U8G_PIN_NONE, reset = 16
//U8GLIB_SBN1661_122X32 u8g(8,9,10,11,4,5,6,7,14,15, 17, U8G_PIN_NONE, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 cs1=14, cs2=15,di=17,rw=16,reset = 16
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X64 u8g(4, 5, 6, 7);  // SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7 (new white HalTec OLED)
//U8GLIB_SSD1306_128X64 u8g(10, 9);   // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X64 u8g(A4, A5);   // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI //okok
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); // Fast I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC
//U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(13, 11, 10, 9);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(10, 9);    // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X32 u8g(13, 11, 10, 9); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X32 u8g(10, 9);             // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 
//U8GLIB_SSD1306_64X48 u8g(13, 11, 10, 9);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_64X48 u8g(10, 9);             // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_64X48 u8g(U8G_I2C_OPT_NONE); // I2C / TWI 
//U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SH1106_128X64 u8g(4, 5, 6, 7); // SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7 (new blue HalTec OLED)
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE); // I2C / TWI 
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_FAST); // Dev 0, Fast I2C / TWI
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK); // Display which does not send ACK
//U8GLIB_SSD1309_128X64 u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1327_96X96_GR u8g(U8G_I2C_OPT_NONE);  // I2C
//U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE); // I2C
//U8GLIB_UC1611_DOGM240 u8g(U8G_I2C_OPT_NONE);  // I2C
//U8GLIB_UC1611_DOGM240 u8g(13, 11, 10, 9); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_UC1611_DOGM240 u8g(10, 9);   // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_UC1611_DOGM240 u8g(10, 9);   // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_UC1611_DOGM240 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 3, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=3, di/a0=17,rw=16
//U8GLIB_UC1611_DOGXL240 u8g(U8G_I2C_OPT_NONE); // I2C
//U8GLIB_UC1611_DOGXL240 u8g(13, 11, 10, 9);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_UC1611_DOGXL240 u8g(10, 9);    // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_UC1611_DOGXL240 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 3, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=3, di/a0=17,rw=16
//U8GLIB_NHD_C12864 u8g(13, 11, 10, 9, 8);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_NHD_C12832 u8g(13, 11, 10, 9, 8);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_LD7032_60x32 u8g(13, 11, 10, 9, 8);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_LD7032_60x32 u8g(11, 12, 9, 10, 8);  // SPI Com: SCK = 11, MOSI = 12, CS = 9, A0 = 10, RST = 8  (SW SPI Nano Board)
//U8GLIB_UC1608_240X64 u8g(13, 11, 10, 9, 8); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_UC1608_240X64_2X u8g(13, 11, 10, 9, 8);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_UC1608_240X64 u8g(10, 9, 8); // HW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_UC1608_240X64_2X u8g(10, 9, 8);  // HW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_UC1608_240X u8g(13, 11, 10, 9, 8); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_UC1608_240X64_2X u8g(13, 11, 10, 9, 8);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_UC1608_240X64 u8g(10, 9, 8); // HW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_UC1608_240X64_2X u8g(10, 9, 8);  // HW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_T6963_240X128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
//U8GLIB_T6963_128X128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
//U8GLIB_T6963_240X64 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
//U8GLIB_T6963_128X64 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
//U8GLIB_HT1632_24X16 u8g(3, 2, 4);   // WR = 3, DATA = 2, CS = 4
//U8GLIB_SSD1351_128X128_332 u8g(13, 11, 8, 9, 7); // Arduino UNO: SW SPI Com: SCK = 13, MOSI = 11, CS = 8, A0 = 9, RESET = 7 (http://electronics.ilsoft.co.uk/ArduinoShield.aspx)
//U8GLIB_SSD1351_128X128_332 u8g(76, 75, 8, 9, 7); // Arduino DUE: SW SPI Com: SCK = 13, MOSI = 11, CS = 8, A0 = 9, RESET = 7 (http://electronics.ilsoft.co.uk/ArduinoShield.aspx)
//U8GLIB_SSD1351_128X128_332 u8g(8, 9, 7); // Arduino: HW SPI Com: SCK = 13, MOSI = 11, CS = 8, A0 = 9, RESET = 7 (http://electronics.ilsoft.co.uk/ArduinoShield.aspx)
//U8GLIB_SSD1351_128X128_HICOLOR u8g(76, 75, 8, 9, 7); // Arduino DUE, SW SPI Com: SCK = 76, MOSI = 75, CS = 8, A0 = 9, RESET = 7 (http://electronics.ilsoft.co.uk/ArduinoShield.aspx)
//U8GLIB_SSD1351_128X128_HICOLOR u8g(8, 9, 7); // Arduino, HW SPI Com: SCK = 76, MOSI = 75, CS = 8, A0 = 9, RESET = 7 (http://electronics.ilsoft.co.uk/ArduinoShield.aspx)
//U8GLIB_SSD1351_128X128GH_332 u8g(8, 9, 7); // Arduino, HW SPI Com: SCK = 76, MOSI = 75, CS = 8, A0 = 9, RESET = 7 (Freetronics OLED)
//U8GLIB_SSD1351_128X128GH_HICOLOR u8g(8, 9, 7); // Arduino, HW SPI Com: SCK = 76, MOSI = 75, CS = 8, A0 = 9, RESET = 7 (Freetronics OLED)
#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4

#define ssdelay 100

//Constants
#define DHTPIN 2 // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302)

#include <DHT.h>;
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


uint8_t uiKeyPrev = 8;
uint8_t uiKeyNext = 9;
uint8_t uiKeySelect = 10;
uint8_t uiKeyBack = 7;
uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;

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

#define MENU0_ITEMS 3
const char *menu0_strings[MENU0_ITEMS] = { "Focus CTL", "Heat CTL", "Setup"};
#define MENU10_ITEMS 4
const char *menu10_strings[MENU10_ITEMS] = { "Menu10 - 1", "Menu10 - 2", "Menu10 - 3", "Menu10 - 4"};
#define MENU20_ITEMS 4
const char *menu20_strings[MENU20_ITEMS] = { "Menu20 - 1", "Menu20 - 2", "Menu20 - 3", "Menu20 - 4"};
#define MENU30_ITEMS 4
const char *menu30_strings[MENU30_ITEMS] = { "Menu30 - 1", "Menu30 - 2", "Menu30 - 3", "Menu30 - 4"};

uint8_t menu_current = 0;
uint8_t menu_redraw_required = 0;
uint8_t last_key_code = KEY_NONE;


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
        if ( menu_current >= ( 10 + MENU10_ITEMS) )
          menu_current = 10;
        menu_redraw_required = 1;
        break;
      case KEY_PREV:
        if ( menu_current == 10 )
          menu_current = ( 10 + MENU10_ITEMS ) ;
        menu_current--;
        menu_redraw_required = 1;
        break;
      case KEY_SELECT:
        //menu_current = (menu_current+1) * 10 ;
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
        if ( menu_current >= ( 20 + MENU20_ITEMS) )
          menu_current = 20;
        menu_redraw_required = 1;
        break;
      case KEY_PREV:
        if ( menu_current == 20 )
          menu_current = ( 20 + MENU20_ITEMS ) ;
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


void updateOLED_AWS(void) {  //OLED 상단에 AWS 값을 출력  //아직 미완성
  float val_Temp= dht.readTemperature();
  float val_Humi = dht.readHumidity();
  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  u8g.drawStr(2, 0, "Temp:");
  u8g.drawStr(66, 0, "Humi:");
  //u8g.drawStr(2, 0, char(val_Temp, 1));
  //u8g.drawStr(66, 0, char(val_Humi, 1));
}

void setup() {
  // rotate screen, if required
  // u8g.setRot180();
  
  uiSetup();                    // setup key detection and debounce algorithm
  menu_redraw_required = 1;     // force initial redraw
}


void loop() {  

  uiStep();                                     // check for key press

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
  
  updateMenu();                            // update menu bar 
  
  }
