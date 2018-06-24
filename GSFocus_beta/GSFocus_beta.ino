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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(OLED);

#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); // Fast I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

#include <Stepper.h>
#include <DRV8825.h>
DRV8825 stepper(MOTOR_STEPS, DIR, STEP, MS0, MS1, MS2);

#include <EEPROM.h>

#include <DHT.h>;
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;

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

void setup() { 

  Serial.begin(9600); // 시리얼통신
  dht.begin();  // DHT
  
  //pinMode(ledPin, OUTPUT); // LED 테스트할 때만 설정
  // 버튼들을 INPUT_PULLUP으로 설정
  pinMode(btn0, INPUT_PULLUP); 
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  
  // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
  stepper.begin(1, 1);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  display.clearDisplay();  //OLED 화면 지우기
  updateOLED_Welcome();   //OLED 환영 메세지
}

void loop() {
  // 스위치의 값을 읽어서 btnState 변수에 저장한다.
  btn0State = digitalRead(btn0); 
  btn1State = digitalRead(btn1);
  btn2State = digitalRead(btn2);
  btn3State = digitalRead(btn3);
  //updateSerialMonitor_AWS();//AWS를 시리얼 모니터로 출력
  
  updateOLED_AWS1();   //OLED 상단에 AWS 값을 출력
  //updateOLED_TEST();   //OLED 테스트 출력

  /*
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
  */
  //btn_OLED_TEST();  //버튼 동작 테스트 함수

   u8g.firstPage();
    do {  
      //draw_AWS(); 
      updateOLED_TEST1();   
    } while( u8g.nextPage() );
    delay(1000); 
  u8g.firstPage();
    do {  
      //draw2();    
  } while( u8g.nextPage() ); 
 delay(1000); 
 
}

void btn_OLED_TEST(void) {  //버튼 동작 테스트 함수
  if (btn0State == LOW) { // 만약, 버튼을 누르면 !
  //  digitalWrite(ledPin, HIGH); // LED는 1(5V)를 출력한다.
    display.print("Button0 1 ");
    Serial.print("Button0 1 ");
  }
  else { // 버튼을 누르지 않으면 !
  //  digitalWrite(ledPin, LOW); // LED는 0(0V)를 출력한다.
    display.print("Button0 0 ");
    Serial.print("Button0 0 ");
  }
  
  if (btn1State == LOW) { // 만약, 버튼을 누르면 !
  //  digitalWrite(ledPin, HIGH); // LED는 1(5V)를 출력한다.
    display.print("Button1 1 ");
    Serial.print("Button1 1 ");
    display.println("");
    Serial.println("");
  }
  else { // 버튼을 누르지 않으면 !
  //  digitalWrite(ledPin, LOW); // LED는 0(0V)를 출력한다.
    display.print("Button1 0 ");
    Serial.print("Button1 0 ");
    display.println("");
    Serial.println("");
  }
  if (btn2State == LOW) { // 만약, 버튼을 누르면 !
  //  digitalWrite(ledPin, HIGH); // LED는 1(5V)를 출력한다.
    display.print("Button2 1 ");
    Serial.print("Button2 1 ");
  }
  else { // 버튼을 누르지 않으면 !
  //  digitalWrite(ledPin, LOW); // LED는 0(0V)를 출력한다.
    display.print("Button2 0 ");
    Serial.print("Button2 0 ");
  }
    
  if (btn3State == LOW) { // 만약, 버튼을 누르면 !
  //  digitalWrite(ledPin, HIGH); // LED는 1(5V)를 출력한다.
    display.print("Button3 1 ");
    Serial.print("Button3 1 ");
    display.println("");
    Serial.println("");
  }
  else { // 버튼을 누르지 않으면 !
  //  digitalWrite(ledPin, LOW); // LED는 0(0V)를 출력한다.
    display.print("Button3 0 ");
    Serial.print("Button3 0 ");
    display.println("");
    Serial.println("");
  }
}

void updateOLED_AWS1(void) {  //OLED 상단에 AWS 값을 출력 1306
  float val_hum = dht.readHumidity();
  float val_temp= dht.readTemperature();
  //u8g.drawStr(0, 15, "Temp:" + String(val_temp, 1)+ "C");
  //u8g.drawStr(0, 30, "RH:" + String(val_hum, 1) + "%");
  u8g.drawStr(0, 15, "aaaa");
//  u8g.drawStr(0, 30, String(val_hum, 1));
  delay(ssdelay);
}
/*
void updateOLED_AWS(void) {  //OLED 상단에 AWS 값을 출력 1306
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
  delay(ssdelay);
  display.clearDisplay();
}
*/
void updateOLED_TEST(void) {   //OLED 테스트 출력 1306
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("1abcdefghijklmnopqrstuvwxyz");
  display.println("2abcdefghijklmnopqrstuvwxyz");
  display.println("3abcdefghijklmnopqrstuvwxyz");
  display.println("4abcdefghijklmnopqrstuvwxyz");
  display.display();
  delay(ssdelay);
  display.clearDisplay();
}

void updateOLED_TEST1(void) {   //OLED 테스트 출력 u8g
  u8g.drawStr(0, 15, "1abcdefghijklmnopqrstuvwxyz");
  u8g.drawStr(0, 35, "2abcdefghijklmnopqrstuvwxyz");
  u8g.drawStr(0, 65, "3abcdefghijklmnopqrstuvwxyz");
  delay(ssdelay);
}

void updateOLED_Welcome(void) {  //OLED 환영 메세지 1306
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println("Welcome!!");
  //display.setCursor(0,15);
  display.println("GS Focus.. Ver." + String(FirmwareNumber));
  display.display();
  delay(ssdelay);
  display.clearDisplay();
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
