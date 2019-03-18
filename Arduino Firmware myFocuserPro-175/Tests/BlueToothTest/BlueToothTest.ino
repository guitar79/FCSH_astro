// Test Bluetooth
//
// SUPPORTS LCD

// BlueTooth HC-05 Module wired as follows
// EN/CMD       // do not connect
// RX to middle of 2K and 1K resistor - other end of 2K resistor to D11 and other end of 1K resistor to ground
// TX to D10
// STATE        // do not connect

#include <Arduino.h>
#include <Wire.h>                     // needed for I2C, installed when installing the Arduino IDE
#include <LCD.h>
#include <LiquidCrystal_I2C.h>        // needed for LCD, see https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <SoftwareSerial.h>           // needed for bt adapter

#define btRX    11
#define btTX    10

// define Bluetooth Adapter
SoftwareSerial btSerial( btTX, btRX);

// define serial port speed - valid values are 9600 19200 38400 57600 115200 230400
#define SerialPortSpeed 9600
#define btSerialPortSpeed 9600

char inChar;                  // used to read a character from serial port
char btinChar;                // used to read a character from bt adapter

#define MAXCOMMAND 8
char mycmd[MAXCOMMAND];         // these are for handling and processing serial commands
char param[MAXCOMMAND];
char line[MAXCOMMAND];
int eoc = 0;    // end of command
int idx = 0;    // index into command string
// these for handling bluetooth serial commands
char btcmd[MAXCOMMAND];
char btline[MAXCOMMAND];
int bteoc = 0;
int btidx = 0;
#define SerialCommand 0
#define WifiCommand 1

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Serial Commands - this processes all received serial commands, either Serial Port or BT
void processCommand(String command, int originator)
{
  int len;

  memset(mycmd, 0 , MAXCOMMAND);    // clear command buffer
  memset(param, 0, MAXCOMMAND);    // clear the parameters buffer

  if ( originator == SerialCommand )                  // strip off the command and the paramters into cmd and param respectively
  {
    len = strlen(line);
    if (len >= 2)
    {
      strncpy(mycmd, line, 2);
    }
    if (len > 2)
    {
      strncpy(param, line + 2, len - 2);
    }
  }
  else        // it is a wificommand
  {
    len = strlen(btline);
    if (len >= 2)
    {
      strncpy(mycmd, btline, 2);
    }
    if (len > 2)
    {
      strncpy(param, btline + 2, len - 2);
    }
  }

  if ( originator == SerialCommand ) // if a USB serial command is complete/ready
  {
    memset(line, 0, MAXCOMMAND);     // clear any buffers or index counts
    eoc = 0;
    idx = 0;
  }
  else
  {
    memset(btline, 0, MAXCOMMAND);
    bteoc = 0;
    btidx = 0;
  }
  
  // clear lcd and print command that arrived via BT
  lcd.clear();
  lcd.print("cmd=");
  lcd.print(mycmd);
}


// Setup
void setup()
{
  // initialize serial
  Serial.begin(SerialPortSpeed);

  // start bt adapter
  btSerial.begin(btSerialPortSpeed);
  btSerial.print("00#");

  eoc = 0;
  idx = 0;
  memset(line, 0, MAXCOMMAND);

  bteoc = false;
  btidx = 0;
  memset(btline, 0, MAXCOMMAND);

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.print("START");
  clearSerialPort();
  clearbtPort();
}

// Main Loop
void loop()
{
  // process the command string when a hash arrives:
  if (eoc)
  {
    processCommand(line, SerialCommand);
    memset(line, 0, MAXCOMMAND);
    eoc = false;
  }

  btSerialEvent();    // check for command from bt adapter

  // process the BLUETOOTH command string when a hash arrives
  if (bteoc)
  {
    processCommand( btline, WifiCommand);
    memset( btline, 0, MAXCOMMAND);
    bteoc = false;
  }

}

void clearSerialPort()
{
  while (Serial.available())
  {
    inChar = Serial.read();
  }
}

void clearbtPort()
{
  while (btSerial.available())
  {
    btinChar = btSerial.read();
  }
}

void btSerialEvent()   // not interrupt driven
{
  while (btSerial.available() && (bteoc == 0) ) {
    btinChar = (char) btSerial.read();
    if ((btinChar != '#') && (btinChar != ':')) {
      btline[btidx++] = btinChar;
      if (btidx >= MAXCOMMAND) {
        btidx = MAXCOMMAND - 1;
      }
    }
    else {
      if (btinChar == '#') {
        bteoc = 1;
        idx = 0;
      }
    }
  }
}

// SerialEvent occurs whenever new data comes in the serial RX.
void serialEvent() {
  // : starts the command, # ends the command, do not store these in the command buffer
  // read the command until the terminating # character
  while (Serial.available() && !eoc) {
    inChar = Serial.read();
    if (inChar != '#' && inChar != ':') {
      line[idx++] = inChar;
      if (idx >= MAXCOMMAND) {
        idx = MAXCOMMAND - 1;
      }
    }
    else {
      if (inChar == '#') {
        eoc = 1;
      }
    }
  }
}
