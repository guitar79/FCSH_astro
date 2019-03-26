// myfocuserPro DRV8825-HW203-OLED-BT
// myFocuserPro2M PCB and Case - Fully Assembled and Ready to go
// ONLY FOR USE WITH NEMA1417 BIPOLAR STEPPER MOTORS ONLY
// BUZZER, LEDS, TEMPERATURE PROBE, OLED, BT
// NO PUSH BUTTONS
// PCB SIZE IS 51mm x 50mm
// REQUIRES 12V 3A Power Supply

// IF USING MOONLITE ASCOM DRIVER
// MAKE SURE THAT POWER ON RESET CIRCUIT IS ENABLED
// ELSE MOONLITE WILL REPORT A TIMEOUT ERROR
// TO REPROGRAM THE CONTROLLER DISABLE THE POWER ON RESET PREVENT CIRCUIT

// (c) Copyright Robert Brown 2014-2019. All Rights Reserved.
// YOU MAY NOT SELL CONTROLLERS OR PCB'S BASED ON THIS PROJECT
// for commercial gain without express written permission granted from the author.
// Schematics, Code, Firmware, Ideas, Applications, Layout are protected by International Copyright Law.

// Permission is NOT granted to any person to redistribute, market, manufacture or sell for commercial gain the myFocuser
// products, ideas, PCB's, circuits, builds, variations and units described or discussed herein or on this site.
// Permission is granted for personal and Academic/Educational use only.

// THIS MEANS THAT YOU CANNOT RE-DESIGN AND MAKE AVAILABLE KITS OR PCB BASED ON THIS PROJECT AND
// OFFER THOSE KITS FOR SALE TO OTHERS. THAT IS A BREACH OF COPYRIGHT.

// CONTRIBUTIONS
// If you wish to make a small contribution in thanks for this project, please use PayPal and send the amount
// to user rbb1brown@gmail.com (Robert Brown). All contributions are gratefully accepted.

// ----------------------------------------------------------------------------------------------------------
// Order here - Full assembled working controller
// https://www.dropbox.com/s/vgr8glsjfi5omom/OrderForm-myFocuserPro2M.pdf?dl=0
//
// Order PCB
// https://aisler.net/brown_rb/my-project-repository/myfocuserpro2mdrv8825

// Stripboard
// Not designed, PCB Only
//
// ----------------------------------------------------------------------------------------------------------

//기능별로 제작한 함수 라이브러리 파일들

#include "Focus.h"

// ----------------------------------------------------------------------------------------------------------
// FIRMWARE CHANGE LOG

// 1.75 21022019
// rewrite for oled
// major changes to commands and processing
// speed improvements
// code space reductions
// major changes to focuser struct
// rewrite temperature code
// Compatible with myFocuser ASCOM driver
// Compatible with myFocuser Application
// Compatible with Moonlite ASCOM driver and application
// Add Blue Tooth

// based on some portions of original code by Dave Wells, and orly.andico@gmail.com (moonlite focuser example)
// heavily modified by RBB to work with myFocuser
// ----------------------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------------------
// CODE START

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
