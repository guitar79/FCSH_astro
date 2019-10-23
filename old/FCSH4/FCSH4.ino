// GSfocusPro 
// ONLY FOR USE WITH STEPPER MOTORS ONLY
// LEDS, TEMPERATURE PROBE, OLED
// NO PUSH BUTTONS
// REQUIRES 12V 3A Power Supply

// (c) Copyright Kiehyun Park 2014-2019. All Rights Reserved.

// ----------------------------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------------------------

//기능별로 제작한 함수 라이브러리 파일들
#include "Focus.h"

// ----------------------------------------------------------------------------------------------------------
// FIRMWARE CHANGE LOG

// 1.00
// ----------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------
// CODE START

// Setup
void setup()
{
    int nlocations;    // number of storage locations available in EEPROM
    byte datasize;     // will hold size of the struct GSfocus - 6 bytes
    byte found;        // did we find any stored values?
  
    Serial.begin(SerialPortSpeed);

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
    
    currentaddr = 0;
    found = 0;
    writenow = 0;
    datasize = sizeof( GSfocus );
    nlocations = EEPROMSIZE / datasize;
    
    for (int lp1 = 0; lp1 < nlocations; lp1++ )
    {
        int addr = lp1 * datasize;
        EEPROM_readAnything( addr, GSfocus );
        // check to see if the data is valid
        if ( GSfocus.validdata == 99 )
        {
            // data was erased so write some default values
            currentaddr = addr;
            found = 1;
        }
    }
    if ( found == 1 )
    {
        EEPROM_readAnything( currentaddr, GSfocus );
        GSfocus.validdata = 0;
        EEPROM_writeAnything(currentaddr, GSfocus);
        currentaddr += datasize;
        if ( currentaddr >= (nlocations * datasize) )
            currentaddr = 0;
        GSfocus.validdata = 99;
        EEPROM_writeAnything(currentaddr, GSfocus);
    }
    else
    {
        ResetFocuserDefaults();
    }

    currentPosition = GSfocus.fposition;  // Set focuser defaults from saved values in EEPROM.
    targetPosition = GSfocus.fposition;
    maxSteps = GSfocus.maxstep;
    
    ch1tempval  = 20.0;
    lasttempval = 20.0;
    tprobe1 = 0;

    #ifdef DHT22
    DHT dht(DHTPIN, DHTTYPE);
    
    if ( tprobe1 == 1 )
    {
        //requesttemp();
        delay(600 / (1 << (12 - GSfocus.ds18b20resolution)));           // should enough time to wait
        //readtemp();
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
    if ( GSfocus.coilPwr == 1 )
        enableoutput();
    else
        disableoutput();
    stepontime = PULSETIME;
    TSWTHRESHOLD = MTRTHRESHHOLD;
    motorspeedchange = 0;
    setstepmode(GSfocus.stepmode);
    motorSpeed = FAST;
    savedmotorSpeed = FAST;
    updatemotorSpeedDelay();
    
    if ( GSfocus.updatedisplayintervalNotMoving < 2000 )
        GSfocus.updatedisplayintervalNotMoving = 2000;
    if ( GSfocus.updatedisplayintervalNotMoving > 4000 )
        GSfocus.updatedisplayintervalNotMoving = 4000;
    
    writenow = 0;
    
}

// Main Loop
void loop()
{
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
        if ( GSfocus.lcdupdateonmove == 1 )
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
        if ( ((currentMillis - olddisplaytimestampNotMoving) > GSfocus.updatedisplayintervalNotMoving) || (currentMillis < olddisplaytimestampNotMoving) )
        {
            olddisplaytimestampNotMoving = currentMillis;    // update the timestamp
            displaylcd();                         // update ALL the display values - takes about 2s
        }
        #endif

        #ifdef DHT22
        long tempnow = millis();
            // see if the temperature needs updating - done automatically every 5s
            if ( ((tempnow - lasttempconversion) > TEMPREFRESHRATE) || (tempnow < lasttempconversion) )
            {
                lasttempconversion = millis();    // update
                if ( requesttempflag == 0 )
                {
                    readDHT();
                    requesttempflag = 1;
                }
                else
                {
                    //requesttemp();
                    requesttempflag = 0;
                }
            } // end of check to see if it is time to get new temperature reading
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
                GSfocus.validdata = 99;
                GSfocus.fposition = currentPosition;
                GSfocus.maxstep = maxSteps;
                EEPROM_writeAnything(currentaddr, GSfocus);     // update values in EEPROM
                writenow = 0;
            }
        }
        if ( GSfocus.coilPwr == 0 )
            disableoutput();                                    // release the stepper coils to save power
    }  // end of else
}
    
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
