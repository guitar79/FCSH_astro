// GSfocusPro 
// ONLY FOR USE WITH STEPPER MOTORS ONLY
// BUZZER, LEDS, TEMPERATURE PROBE, OLED, BT
// NO PUSH BUTTONS
// REQUIRES 12V 3A Power Supply

// (c) Copyright Robert Brown 2014-2019. All Rights Reserved.
// YOU MAY NOT SELL CONTROLLERS OR PCB'S BASED ON THIS PROJECT
// for commercial gain without express written permission granted from the author.
// Schematics, Code, Firmware, Ideas, Applications, Layout are protected by International Copyright Law.

// Permission is NOT granted to any person to redistribute, market, manufacture or sell for commercial gain the GSfocus
// products, ideas, PCB's, circuits, builds, variations and units described or discussed herein or on this site.
// Permission is granted for personal and Academic/Educational use only.

// THIS MEANS THAT YOU CANNOT RE-DESIGN AND MAKE AVAILABLE KITS OR PCB BASED ON THIS PROJECT AND
// OFFER THOSE KITS FOR SALE TO OTHERS. THAT IS A BREACH OF COPYRIGHT.

// CONTRIBUTIONS
// If you wish to make a small contribution in thanks for this project, please use PayPal and send the amount
// to user rbb1brown@gmail.com (Robert Brown). All contributions are gratefully accepted.

// ----------------------------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------------------------

//기능별로 제작한 함수 라이브러리 파일들
#include "Focus.h"

// ----------------------------------------------------------------------------------------------------------
// FIRMWARE CHANGE LOG

// 1.00

// based on some portions of original code by Dave Wells, and orly.andico@gmail.com (moonlite focuser example)
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
    Serial.println("GSfocus_Stepper#");
    
    
    previousMillis = millis();
    
    
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
    
    
    currentaddr = 0;
    found = 0;
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
    
    if ( GSfocus.updatedisplayintervalNotMoving < 2000 )
        GSfocus.updatedisplayintervalNotMoving = 2000;
    if ( GSfocus.updatedisplayintervalNotMoving > 4000 )
        GSfocus.updatedisplayintervalNotMoving = 4000;
    
    
    
}



// Main Loop
void loop()
{

  
    //만약 입력된 것이 있으면...
    if ( queue.count() >= 1 )                 // check for serial command
    {
        processCommand();
    }
    //지금 필요없음
//    // Move the position by a single step if target <> current position (ismoving 판별)
//    if ((targetPosition != currentPosition) && (gotonewposition == 1))
//    {
//        // Slow down if approaching home position
//        if ( motorspeedchange == 1 )
//        {
//            // Slow down if approaching home position
//            long nearinghomepos = currentPosition - targetPosition;
//            nearinghomepos = abs(nearinghomepos);
//            if ( nearinghomepos < TSWTHRESHOLD )
//            {
//                #ifdef DEBUG
//                Serial.println("Speed change to Slow because nearing position");
//                #endif
//                motorSpeed = SLOW;                    // slow
//            }
//        }
//
//        enableoutput();                           // have to enable driver board
//        isMoving = 1;                             // focuser is 
//        previousMillis = millis();                // keep updating previousMillis whilst focuser is moving
//
//        // Going Anticlockwise to lower position
//        if (targetPosition < currentPosition)
//        {
//            anticlockwise();
//            currentPosition--;
//        }
//
//        // Going Clockwise to higher position
//        if (targetPosition > currentPosition)
//        {
//            // do not need to check if > maximumPosition as its done when a target command is receieved
//            clockwise();
//            currentPosition++;
//        }
//
//        
//    delayMicroseconds(motorSpeedDelay);     // microseconds, 1uS = 1000mS
//    }
//    else
//    {
//        // focuser is NOT moving now, move is completed
//        isMoving = 0;
//        gotonewposition = 0;
//        
//        // reset motorSpeed
//        motorSpeed = savedmotorSpeed;
//
//    }
}

      

        
    
    
void clearSerialPort()
{
    while ( Serial.available() )
        Serial.read(); //저장을 안하니까 버퍼에 아무것도 남지 않는다
}

// SerialEvent occurs whenever new data comes in the serial RX. (가장 중요)
void serialEvent()
{
  Serial.println("chk");
    // : starts the command, # ends the command, do not store these in the command buffer
    // read the command until the terminating # character
    while (Serial.available() && !eoc)//eoc(end of command)가 0이면
    {
        char inChar = Serial.read(); //한글자만 읽어온다
        if (inChar != '#' && inChar != ':')
        {
            line[idx++] = inChar;
            if (idx >= MAXCOMMAND)
               idx = MAXCOMMAND - 1;
        } //처음과 끝이 아닌부분만 line에 넣어두고, maxcommand를 넘어가면 모두 거르고 마지막 글자만 넣는다
        else
        {
            if (inChar == '#')
            {
              Serial.print("inchar = #, line = ");
              Serial.println(line);
                eoc = 1; //command 하나가 끝났다.
              Serial.print("eoc = ");
                idx = 0; //새로 입력받을 수 있도록 index를 초기화한다.
                if ( strcmp(line, "00") == 0 )   // get current focuser position
                {
                    Serial.println("0K : line = 00");
                    Serial.print("currentPosition :");
                    Serial.println(currentPosition);
                    fastreply = "P" + String(currentPosition) + "#";
                    sendresponsestr(fastreply);
                }
                    else
                        {
                            queue.push(String(line));
                        }
                eoc = 0;
                Serial.print("eoc is 0");
                //이거 사이에는 뭐 입력해도 아무것도 없다
                memset( line, 0, MAXCOMMAND);
            }
        }
    }
}


// Serial Commands
void processCommand()
{
    byte len;
    int tmp;
    long pos;
    int cmdval;
    String replystr = "";
    String tmpstr;
    char mycmd[MAXCOMMAND];
    char param[MAXCOMMAND];
    char tempString[8];
  
    memset(mycmd, 0, MAXCOMMAND);
    memset(param, 0, MAXCOMMAND);
    replystr = queue.pop();
    len = replystr.length();
    if ( len == 1 )
    {
        mycmd[0] = replystr[0];
        #ifdef DEBUG
        Serial.print("tmpstr cmd = "); Serial.println(mycmd);
        #endif
    }
    if ( len == 2 )
    {
        tmpstr = replystr.substring(0, 3);
        tmpstr.toCharArray(mycmd, 3);
        #ifdef DEBUG
        Serial.print("tmpstr cmd = "); Serial.println(tmpstr);
        #endif
    }
    else if ( len > 2 )
    {
        tmpstr = replystr.substring(0, 3);
        tmpstr.toCharArray(mycmd, 3);
        #ifdef DEBUG
        Serial.print("tmpstr cmd = "); Serial.println(tmpstr);
        #endif
        tmpstr = replystr.substring(2, len + 1);
        tmpstr.toCharArray(param, tmpstr.length() + 1);
        #ifdef DEBUG
        Serial.print("tmpstr param = "); Serial.println(tmpstr);
        #endif
    }
    else return;
  
    if ( len > 1)
        cmdval = (int) mycmd[0] + ((int) mycmd[1] * 256);
    else
        cmdval = (int) mycmd[0];
  
    #ifdef DEBUG
        Serial.print("replystr = "); Serial.println(replystr);
        Serial.print("len = "); Serial.println(len);
        Serial.print("mycmd = "); Serial.println(mycmd);
        Serial.print("param = "); Serial.println(param);
        Serial.print("cmdval = "); Serial.println(cmdval);
    #endif
  
    switch ( cmdval )
    {
        case 20551:   // :GP# get the current focuser position
            sprintf(tempString, "%04X", (unsigned int) currentPosition);
            replystr = String(tempString) + "#";
            sendresponsestr(replystr);
            break;
    }  
}
