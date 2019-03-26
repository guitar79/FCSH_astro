/*
 * Title       Focuser, Optic Cover, Switch controller
 * by          Kiehyun Kevin Park
 *
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
 *   Full featured stepper motor telescope microcontroller for Equatorial and
 *   Alt-Azimuth mounts, with the LX200 derived command set.
 *
 * Author: Kiehyun Kevin Park
 *   Kiehyun.Park@gmail.com
 */
// Use Config.xxx.h to configure OnStep to your requirements

#define FirmwareDate   "Feb. 18. 2019"
#define FirmwareNumber "0.1"
#define FirmwareName   "KevinFCS"
#define FirmwareTime   "12:00:00"

//상수들은 따로 저장함
#include "Constants.h"

//기능별로 제작한 함수 라이브러리 파일들
//#include "Menu.h"
//#include "Focus.h"
//#include "Cover.h"
//#include "Switch.h"
#include "Focuserv175.h"

//pcb 보드별 파일들
//해당 파일을 열어 하나만 ON으로 설정.
//#include "Config.FCS_teensy3.2.h"
//#include "Config.FCS_GSnano.h"
//#include "Config.F_nanoJS.h"
//#include "Config.Kevin_touch.h"

//사용한 라이브러리들
//#include <DHT.h>
//#include "DHT.h"
//DHT dht(DHTPIN, DHTTYPE);

//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//Adafruit_SSD1306 display(OLED_RESET);

//#include <SyncDriver.h>
//#include <Stepper.h>
//#include <BasicStepperDriver.h>
//#include <DRV8825.h>
//DRV8825 stepper(MOTOR_STEPS, DIR, STEP, MS0, MS1, MS2);

//#include <Servo.h>
//Servo myservo;

//#include <EEPROM.h>

//#include <U8g2lib.h>
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0);

// Setup
void setup()
{
  int nlocations;    // number of storage locations available in EEPROM
  byte datasize;     // will hold size of the struct myfocuser - 6 bytes
  byte found;        // did we find any stored values?

  Serial.begin(SerialPortSpeed);

#ifdef BLUETOOTH
  btSerial.begin(BTPORTSPEED);                    // start bt adapter
  clearbtPort();
#endif

#ifdef BUZZER
  pinMode(Buzzer, OUTPUT);
  digitalWrite( Buzzer, 1);
#endif

#ifdef INOUTLEDS
  pinMode( bledIN, OUTPUT);
  pinMode( gledOUT, OUTPUT);
  digitalWrite( bledIN, 1 );
  digitalWrite( gledOUT, 1 );
#endif

  previousMillis = millis();

#ifdef TEMPERATUREPROBE
  lasttempconversion = previousMillis;
#endif
  myinterval = TIMEINTERVAL;
  stepdelay = 0;
  tempcomp = 0;
  tempoffsetval = 0;
  eoc = 0;
  idx = 0;
  isMoving = 0;
  gotonewposition = 0;
  memset(line, 0, MAXCOMMAND);
  targetPosition = currentPosition = STARTPOS;
  maxFocuserLimit = MAXSTEPLIMIT;
  maxSteps = STARTMAXPOS;
  maxIncrement = MAXINC;
  minimumPosition = 0L;

  displayenabled = 0;
  olddisplaytimestampNotMoving = 0L;
  updatecount = 0;
  lcdupdatestepcount = 15;

#ifdef OLEDDISPLAY
  displayenabled = 1;
  // Setup the OLED
  Wire.begin();
  myoled.begin(&Adafruit128x64, OLEDADDRESS);
  myoled.set400kHz();
  myoled.setFont(Adafruit5x7);
  myoled.clear();                           // clrscr OLED
  myoled.Display_Normal();                  // black on white
  myoled.Display_On();                      // display ON
  myoled.Display_Rotate(0);                 // portrait, not rotated
  myoled.Display_Bright();

  // The screen size is 128 x 64, so using characters at 6x8 this gives 21chars across and 8 lines down
  myoled.println(programName);              // print startup screen
  myoled.println(programVersion);
  myoled.InverseCharOn();
  myoled.println(ProgramAuthor);
  myoled.InverseCharOff();
#endif

#ifdef BLUETOOTH
  bteoc = 0;
  btidx = 0;
  memset(btline, 0, MAXCOMMAND);
#endif

  currentaddr = 0;
  found = 0;
  writenow = 0;
  datasize = sizeof( myfocuser );
  nlocations = EEPROMSIZE / datasize;

  for (int lp1 = 0; lp1 < nlocations; lp1++ )
  {
    int addr = lp1 * datasize;
    EEPROM_readAnything( addr, myfocuser );
    // check to see if the data is valid
    if ( myfocuser.validdata == 99 )
    {
      // data was erased so write some default values
      currentaddr = addr;
      found = 1;
    }
  }
  if ( found == 1 )
  {
    EEPROM_readAnything( currentaddr, myfocuser );
    myfocuser.validdata = 0;
    EEPROM_writeAnything(currentaddr, myfocuser);
    currentaddr += datasize;
    if ( currentaddr >= (nlocations * datasize) )
      currentaddr = 0;
    myfocuser.validdata = 99;
    EEPROM_writeAnything(currentaddr, myfocuser);
  }
  else
  {
    ResetFocuserDefaults();
  }

  currentPosition = myfocuser.fposition;  // Set focuser defaults from saved values in EEPROM.
  targetPosition = myfocuser.fposition;
  maxSteps = myfocuser.maxstep;

  ch1tempval  = 20.0;
  lasttempval = 20.0;
  tprobe1 = 0;

#ifdef TEMPERATUREPROBE
  sensor1.begin();                                  // start the temperature sensor1
  sensor1.getDeviceCount();                         // should return 1 if probe connected
  findds18b20address();
  if ( tprobe1 == 1 )
  {
    sensor1.setResolution( tpAddress, myfocuser.ds18b20resolution );  // set probe resolution
    requesttemp();
    delay(600 / (1 << (12 - myfocuser.ds18b20resolution)));           // should enough time to wait
    readtemp();
  }
  requesttempflag = 0;
#endif

#ifdef OLEDDISPLAY
  myoled.clear();                           // clrscr OLED
#endif
  pinMode(  myDir, OUTPUT );
  pinMode(  myStep, OUTPUT );
  pinMode(  myM0, OUTPUT );
  pinMode(  myM1, OUTPUT );
  pinMode(  myM2, OUTPUT );
  digitalWrite( myDir, 0 );
  digitalWrite( myStep, 0 );
  pinMode( myEnable, OUTPUT );
  if ( myfocuser.coilPwr == 1 )
    enableoutput();
  else
    disableoutput();
  stepontime = PULSETIME;
  TSWTHRESHOLD = MTRTHRESHHOLD;
  motorspeedchange = 0;
  setstepmode(myfocuser.stepmode);
  motorSpeed = FAST;
  savedmotorSpeed = FAST;
  updatemotorSpeedDelay();

  if ( myfocuser.updatedisplayintervalNotMoving < 2000 )
    myfocuser.updatedisplayintervalNotMoving = 2000;
  if ( myfocuser.updatedisplayintervalNotMoving > 4000 )
    myfocuser.updatedisplayintervalNotMoving = 4000;

  writenow = 0;

#ifdef INOUTLEDS
  digitalWrite( bledIN, 0 );
  digitalWrite( gledOUT, 0 );
#endif
#ifdef BUZZER
  digitalWrite( Buzzer, 0);
#endif
}

// Main Loop
void loop()
{
#ifdef BLUETOOTH
  btSerialEvent();                          // check for command from bt adapter
#endif

  if ( queue.count() >= 1 )                 // check for serial command
  {
    processCommand();
  }

  // Move the position by a single step if target <> current position
  if ((targetPosition != currentPosition) && (gotonewposition == 1))
  {
    // Slow down if approaching home position
    if ( motorspeedchange == 1 )
    {
      // Slow down if approaching home position
      long nearinghomepos = currentPosition - targetPosition;
      nearinghomepos = abs(nearinghomepos);
      if ( nearinghomepos < TSWTHRESHOLD )
      {
#ifdef DEBUG
        Serial.println("Speed change to Slow because nearing position");
#endif
        motorSpeed = SLOW;                    // slow
        updatemotorSpeedDelay();              // set the correct delay
      }
    }

    enableoutput();                           // have to enable driver board
    isMoving = 1;                             // focuser is moving
    writenow = 1;                             // updating of EEPROM ON
    previousMillis = millis();                // keep updating previousMillis whilst focuser is moving

    // Going Anticlockwise to lower position
    if (targetPosition < currentPosition)
    {
      anticlockwise();
      currentPosition--;
    }

    // Going Clockwise to higher position
    if (targetPosition > currentPosition)
    {
      // do not need to check if > maximumPosition as its done when a target command is receieved
      clockwise();
      currentPosition++;
    }

#ifdef OLEDDISPLAY                          // check if lcd needs updating during move
    if ( myfocuser.lcdupdateonmove == 1 )
    {
      updatecount++;
      if ( updatecount > LCDUPDATESTEPCOUNT )
      {
        updatepositionoled();
        updatecount = 0;
      }
    }
#endif
    delayMicroseconds(motorSpeedDelay);     // microseconds, 1uS = 1000mS
  }
  else
  {
    // focuser is NOT moving now, move is completed
    isMoving = 0;
    gotonewposition = 0;

    // reset motorSpeed
    motorSpeed = savedmotorSpeed;
    updatemotorSpeedDelay();

#ifdef OLEDDISPLAY
    // see if the display needs updating
    long currentMillis = millis();
    if ( ((currentMillis - olddisplaytimestampNotMoving) > myfocuser.updatedisplayintervalNotMoving) || (currentMillis < olddisplaytimestampNotMoving) )
    {
      olddisplaytimestampNotMoving = currentMillis;    // update the timestamp
      displaylcd();                         // update ALL the display values - takes about 2s
    }
#endif

#ifdef TEMPERATUREPROBE
    // if there is a temperature probe
    if ( tprobe1 == 1)
    {
      long tempnow = millis();
      // see if the temperature needs updating - done automatically every 5s
      if ( ((tempnow - lasttempconversion) > TEMPREFRESHRATE) || (tempnow < lasttempconversion) )
      {
        lasttempconversion = millis();    // update
        if ( requesttempflag == 0 )
        {
          readtemp();
          requesttempflag = 1;
        }
        else
        {
          requesttemp();
          requesttempflag = 0;
        }
      } // end of check to see if it is time to get new temperature reading
    } // end of check for temperature probe present
#endif

    // is it time to update EEPROM settings?
    if ( writenow == 1 )
    {
      // decide if we have waited 10s after the last move, if so, update the EEPROM
      long currentMillis = millis();
      if ( ((currentMillis - previousMillis) > myinterval) || (currentMillis < previousMillis) )
      {
        previousMillis = currentMillis;
        // copy current settings and write the data to EEPROM
        myfocuser.validdata = 99;
        myfocuser.fposition = currentPosition;
        myfocuser.maxstep = maxSteps;
        EEPROM_writeAnything(currentaddr, myfocuser);     // update values in EEPROM
        writenow = 0;
      }
    }
    if ( myfocuser.coilPwr == 0 )
      disableoutput();                                    // release the stepper coils to save power
  }  // end of else
}

#ifdef BLUETOOTH
void clearbtPort()
{
  while (btSerial.available())
  {
    btSerial.read();
  }
}

void btSerialEvent()
{
  while (btSerial.available() && (bteoc == 0) )
  {
    char btinChar = (char) btSerial.read();
    if ((btinChar != '#') && (btinChar != ':'))
    {
      btline[btidx++] = btinChar;
      if (btidx >= MAXCOMMAND)
        btidx = MAXCOMMAND - 1;
    }
    else
    {
      if (btinChar == '#')
      {
        bteoc = 1;
        btidx = 0;
        if ( strcmp(line, ":00#") == 0 )   // get current focuser position
        {
          fastreply = "P" + String(currentPosition) + "#";
          sendresponsestr(fastreply);
        }
        else if ( strcmp( line, ":01#") == 0) // get motor moving status - 01 if moving, 00 otherwise
        {
          if (isMoving == 1)
            sendresponsestr("I01#");
          else
            sendresponsestr("I00#");
        }
        else if ( strcmp( line, ":27#") == 0) // Halt
        {
          // :27#   None      stop a move - like a Halt
          isMoving = false;
          targetPosition = currentPosition;
          writenow = 1;             // updating of EEPROM ON
          previousMillis = millis();   // start time interval
        }
        else
        {
          queue.push(String(btline));
        }
        bteoc = 0;
        memset( btline, 0, MAXCOMMAND);
      }
    }
  }
}
#endif

void clearSerialPort()
{
  while ( Serial.available() )
    Serial.read();
}

// SerialEvent occurs whenever new data comes in the serial RX.
void serialEvent()
{
  // : starts the command, # ends the command, do not store these in the command buffer
  // read the command until the terminating # character
  while (Serial.available() && !eoc)
  {
    char inChar = Serial.read();
    if (inChar != '#' && inChar != ':')
    {
      line[idx++] = inChar;
      if (idx >= MAXCOMMAND)
        idx = MAXCOMMAND - 1;
    }
    else
    {
      if (inChar == '#')
      {
        eoc = 1;
        idx = 0;
        if ( strcmp(line, ":00#") == 0 )   // get current focuser position
        {
          fastreply = "P" + String(currentPosition) + "#";
          sendresponsestr(fastreply);
        }
        else if ( strcmp( line, ":01#") == 0) // get motor moving status - 01 if moving, 00 otherwise
        {
          if (isMoving == 1)
            sendresponsestr("I01#");
          else
            sendresponsestr("I00#");
        }
        else if ( strcmp( line, ":27#") == 0) // Halt
        {
          // :27#   None      stop a move - like a Halt
          isMoving = false;
          targetPosition = currentPosition;
          writenow = 1;             // updating of EEPROM ON
          previousMillis = millis();   // start time interval
        }
        else
        {
          queue.push(String(line));
        }
        eoc = 0;
        memset( line, 0, MAXCOMMAND);
      }
    }
  }
}

