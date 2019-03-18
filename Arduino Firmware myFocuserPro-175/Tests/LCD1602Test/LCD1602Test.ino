/* Test program 5 - LCD test
requires lcd1602 I2C, uses SDA/SCL A4/A5
*/
#include <Arduino.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  Serial.begin(57600);    // initialise serial port

  Serial.println("Test Program 1: LCDXXXX+I2C");
  // initialise the lcd display
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.print("Focuser");
  lcd.setCursor( 0, 1 );              // col, row
  lcd.print("Test");
  delay(2000);
  Serial.println("If the LCD appears on - you should see some light - then adjust the backlight setting");
  Serial.println("which is a small potentiometer on the I2C board that you turn with a little screwdriver");
  Serial.println();
  Serial.println("If there is no light at all, check all your connections to the LCD, VCC, GND, SDA=A4, SCL=A5");
}

// --------------------------------------------------------
void loop() {
  lcd.clear();
  lcd.setCursor( 0, 0 );
  lcd.print("Focuser");
  lcd.setCursor( 0, 1 );              // col, row
  lcd.print("Test");
  delay(2000);
}

