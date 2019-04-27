/*
   Title       Focuser controller (bipolar stepmotor)
   by          Kiehyun Kevin Park


   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Description:

   Author: Kiehyun Kevin Park
     Kiehyun.Park@gmail.com
*/

#include "Config.Kevin_touch.h"

void software_Reboot()
{
    // jump to the start of the program
    asm volatile ( "jmp 0");
}

////////
// disable the stepper motor outputs - coil power off
void disableoutput()
{
    digitalWrite(myEnable, HIGH);
    // control via ENABLE pin, but this turns off indexer inside DRV8825
    // which means the stepper motor will loose power and move position to the nearest full step
    // not an issue if using full steps but major issue if using microstepping as will cause change
    // in focus position
}

// enable the stepper motor outputs - coil power on
void enableoutput()
{
   digitalWrite(myEnable, LOW);
}

// Move stepper anticlockwise
void anticlockwise()
{
#ifdef INOUTLEDS
    (!GSfocus.ReverseDirection) ? digitalWrite( gledOUT, 1) : digitalWrite( bledIN, 1);
#endif
(!GSfocus.ReverseDirection) ? digitalWrite(myDir, LOW ) : digitalWrite(myDir, HIGH );
digitalWrite(myStep, 1 );
delayMicroseconds(stepontime);
digitalWrite(myStep, 0 );

#ifdef INOUTLEDS
    (!GSfocus.ReverseDirection) ? digitalWrite( gledOUT, 0 ) : digitalWrite( bledIN, 0);
#endif
}

// Move stepper clockwise
void clockwise()
{
    #ifdef INOUTLEDS
        (!GSfocus.ReverseDirection) ? digitalWrite( bledIN, 1) : digitalWrite( gledOUT, 1);
    #endif
        (!GSfocus.ReverseDirection) ? digitalWrite(myDir, HIGH ) : digitalWrite(myDir, LOW );
        digitalWrite(myStep, 1 );
        delayMicroseconds(stepontime);
        digitalWrite(myStep, 0 );
    #ifdef INOUTLEDS
        (!GSfocus.ReverseDirection) ? digitalWrite( bledIN, 0 ) : digitalWrite( gledOUT, 0);
    #endif
}


// convert hex string to long int
long hexstr2long(char *line)
{
    long ret = 0;
    ret = strtol(line, NULL, 16);
    return (ret);
}

// convert string to int
int decstr2int(char *line)
{
    int ret = 0;
    String Str(line);
    ret = Str.toInt();
    return ret;
}


// set the microstepping mode
void setstepmode(byte stepmode)
{
    switch ( stepmode )
    {
        case 1:      // full step
            digitalWrite(myM0, 0);
            digitalWrite(myM1, 0);
            digitalWrite(myM2, 0);
            break;
        case 2:      // half step
            digitalWrite(myM0, 1);
            digitalWrite(myM1, 0);
            digitalWrite(myM2, 0);
            break;
        case 4:
            digitalWrite(myM0, 0);
            digitalWrite(myM1, 1);
            digitalWrite(myM2, 0);
            break;
        case 8:
            digitalWrite(myM0, 1);
            digitalWrite(myM1, 1);
            digitalWrite(myM2, 0);
            break;
        case 16:
            digitalWrite(myM0, 0);
            digitalWrite(myM1, 0);
            digitalWrite(myM2, 1);
            break;
        case 32:
            digitalWrite(myM0, 1);
            digitalWrite(myM1, 0);
            digitalWrite(myM2, 1);
            break;
        default:      // half step
            digitalWrite(myM0, 1);
            digitalWrite(myM1, 0);
            digitalWrite(myM2, 0);
            stepmode = 2;
            break;
    }
    GSfocus.stepmode = stepmode;
}

void updatemotorSpeedDelay()
{
    switch ( motorSpeed )
    {
        case 0: // slow
            switch (GSfocus.stepmode)
                {
                    case 1: // full steps
                        motorSpeedDelay = (unsigned int) MOTORSPEEDSLOW0; break;
                    case 2: // half steps
                        motorSpeedDelay = MOTORSPEEDSLOW2; break;
                    case 4: // 1/4 steps
                        motorSpeedDelay = MOTORSPEEDSLOW4; break;
                    case 8: // 1/8 steps
                        motorSpeedDelay = MOTORSPEEDSLOW8; break;
                    case 16: // 1/16 steps
                        motorSpeedDelay = MOTORSPEEDSLOW16; break;
                    case 32: // 1/32 steps
                        motorSpeedDelay = MOTORSPEEDSLOW32; break;
                    default:
                        motorSpeedDelay = MOTORSPEEDSLOW2; break;
                }
            break;
        case 1: // medium
            switch (GSfocus.stepmode)
                {
                    case 1: // full steps
                        motorSpeedDelay = MOTORSPEEDMED0; break;
                    case 2: // half steps
                        motorSpeedDelay = MOTORSPEEDMED2; break;
                    case 4: // 1/4 steps
                        motorSpeedDelay = MOTORSPEEDMED4; break;
                    case 8: // 1/8 steps
                        motorSpeedDelay = MOTORSPEEDMED8; break;
                    case 16: // 1/16 steps
                        motorSpeedDelay = MOTORSPEEDMED16; break;
                    case 32: // 1/32 steps
                        motorSpeedDelay = MOTORSPEEDMED32; break;
                    default:
                        motorSpeedDelay = MOTORSPEEDMED2; break;
                }
            break;
        case 2: // fast
            switch (GSfocus.stepmode)
                {
                    case 1: // full steps
                        motorSpeedDelay = MOTORSPEEDFAST0; break;
                    case 2: // half steps
                        motorSpeedDelay = MOTORSPEEDFAST2; break;
                    case 4: // 1/4 steps
                        motorSpeedDelay = MOTORSPEEDFAST4; break;
                    case 8: // 1/8 steps
                        motorSpeedDelay = MOTORSPEEDFAST8; break;
                    case 16: // 1/16 steps
                        motorSpeedDelay = MOTORSPEEDFAST16; break;
                    case 32: // 1/32 steps
                        motorSpeedDelay = MOTORSPEEDFAST32; break;
                    default:
                        motorSpeedDelay = MOTORSPEEDFAST2; break;
                }
            break;
    }
    #ifdef DEBUG
        Serial.print("motorSpeed = "); Serial.println(motorSpeed);
        Serial.print("stepmode = "); Serial.println(GSfocus.stepmode);
        Serial.print("motorSpeedDelay = "); Serial.println(motorSpeedDelay);
    #endif
}

#ifdef TEMPERATUREPROBE
void requesttemp()
{
    sensor1.requestTemperatures();
}

void readtemp()
{
    double result = sensor1.getTempCByIndex(0);             // get channel 1 temperature, always in celsius
    // sometimes sensor returns -127, not sure why, so check and use last reading if this is the case
    // range of sensor is -55c to +125c
    // limit to values of -40c to +80c (-40f to 176f)
    int tempval = (int) result;
    if ( tempval < -40 )
        ch1tempval = lasttempval;
    else if ( tempval > 80 )
        ch1tempval = lasttempval;
    else
    {
        ch1tempval = result;
        lasttempval = ch1tempval;             // remember last reading
    }
}
#endif

void ResetFocuserDefaults()
{
    GSfocus.validdata = 99;
    GSfocus.fposition = 5000L;
    GSfocus.maxstep = 10000L;
    GSfocus.stepmode = 1;
    GSfocus.ReverseDirection = 0;
    GSfocus.coilPwr = 1;
    GSfocus.tempmode = 1; // celsius
    GSfocus.updatedisplayintervalNotMoving = 2500L;
    GSfocus.ds18b20resolution = TEMP_PRECISION;
    GSfocus.stepsizeenabled = 0;
    GSfocus.stepsize = 10;
    GSfocus.lcdupdateonmove = 0;
    // now write the data to EEPROM
    EEPROM_writeAnything(currentaddr, GSfocus);    // update values in EEPROM
}

void sendresponsestr(String str)
{
    if (Serial)
    {
        Serial.print(str);
    }

    #ifdef BLUETOOTH
        btSerial.print(str);
    #endif
}

void sendresponsenl()
{
    if (Serial)
    {
        Serial.println();
    }
    #ifdef BLUETOOTH
        btSerial.println();
    #endif
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
        case 18759 : // :GI# motor is moving - 1 if moving, 0 otherwise
            replystr = "0" + String(isMoving) + "#";
            sendresponsestr(replystr);
            break;
        case 21575: // :GT# get the current temperature - moonlite compatible
            tmp = (ch1tempval * 2);
            sprintf(tempString, "%04X", (int) tmp);
            replystr = String(tempString) + "#";
            sendresponsestr(replystr);
            break;
        case 23111: // :GZ# get the current temperature
            dtostrf(ch1tempval, 4, 3, tempString);
            replystr = String(tempString) + "#";
            sendresponsestr(replystr);
            break;
        case 22087: // :GV# firmware value Moonlite
            sendresponsestr("10#");
            break;
        case 17991: // :GF# firmware value
            sendresponsestr(String(programName));
            sendresponsenl();
            sendresponsestr(String(programVersion) + "#");
            break;
        case 19783: // :GM# get the MaxSteps
            sprintf(tempString, "%04X", (unsigned int) maxSteps);
            replystr = String(tempString) + "#";
            sendresponsestr(replystr);
            break;
        case 22855: // :GY# get the maxIncrement - set to MaxSteps
            sprintf(tempString, "%04X", (unsigned int) maxIncrement);
            replystr = String(tempString) + "#";
            sendresponsestr(replystr);
            break;
        case 18503: // whether half-step is enabled or not, moonlite always return "00"
            if ( GSfocus.stepmode == 2 )
                replystr = "FF#";
            else
                replystr = "00#";
            sendresponsestr(replystr);
            break;
        case 21319: // get stepmode
            sprintf(tempString, "%02X", GSfocus.stepmode);
            replystr = String(tempString) + "#";
            sendresponsestr(replystr);
            break;
        case 20295: // :GO# get the coilPwr setting
            replystr = String(GSfocus.coilPwr) + "#";
            sendresponsestr(replystr);
            break;
        case 21063: // :GR# get the Reverse Direction setting
            replystr = String(GSfocus.ReverseDirection) + "#";
            sendresponsestr(replystr);
            break;
        case 21069: // :MR# get Motor Speed
            replystr = String(motorSpeed) + "#";
            sendresponsestr(replystr);
            break;
        case 21837: // :MU# Get the MotorSpeed Threshold
            replystr = String(TSWTHRESHOLD) + "#";
            sendresponsestr(replystr);
            break;
        case 22349: // :MW#         xxx#      Get if motorspeedchange enabled/disabled
            replystr = String(motorspeedchange) + "#";
            sendresponsestr(replystr);
            break;
        case 16967: // :GB# LED backlight value, always return "00" - moonlite
            // not implemented in INDI driver
            sendresponsestr("00#");
            break;
        case 20039: // :GN# get the new motor position (target)
            // not implemented in INDI driver
            sprintf(tempString, "%04X", (unsigned int) targetPosition);
            replystr = String(tempString) + "#";
            sendresponsestr(replystr);
            break;
        case 17479: // :GD# get the current motor step delay, only values of 02, 04, 08, 10, 20
            // not used so just return 02
            sprintf(tempString, "%02X", stepdelay);
            replystr = String(tempString) + "#";
            sendresponsestr(replystr);
            break;
        case 17223: // :GC# get temperature co-efficient XX
            // do nothing, ignore
            sprintf(tempString, "%02X", tempcomp);
            replystr = String(tempString) + "#";
            sendresponsestr(replystr);
            break;
        case 18244: // :DG# get display state on or off
            replystr = String(displayenabled) + "#";
            sendresponsestr(replystr);
            break;
        case 22599: // :GXxxxxx#          get the time that an LCD screen is displayed for (in milliseconds, eg 2500 = 2.5seconds
            sprintf(tempString, "%04X", (unsigned int) GSfocus.updatedisplayintervalNotMoving);
            replystr = String(tempString) + "#";
            sendresponsestr(replystr);
            break;
        case 18256: // :PG    Get temperature precision (9-12)
            tmp = TEMP_PRECISION;
            
            #ifdef TEMPERATUREPROBE
                if ( tprobe1 == 1 )
                {
                    GSfocus.ds18b20resolution = (byte) sensor1.getResolution(tpAddress);
                    tmp = GSfocus.ds18b20resolution;
                    writenow = 1;             // updating of EEPROM ON
                    previousMillis = millis();   // start 30s time interval
                }
            #endif
            replystr = String(tmp) + "#";
            sendresponsestr(replystr);
            break;
        case 20048: // :PN# xx#     get update of position on lcd when moving (00=disable, 01=enable)
            replystr = String(GSfocus.lcdupdateonmove) + "#";
            sendresponsestr(replystr);
            break;
        case 20816: // :PQ#    None      Get if stepsize is enabled in controller (1 or 0, 0/1)
            replystr = String(GSfocus.stepsizeenabled) + "#";
            sendresponsestr(replystr);
            break;
        case 21072: // :PR#    xxxxx#    Get step size in microns (if enabled by controller)
            replystr = String(GSfocus.stepsize) + "#";
            sendresponsestr(replystr);
            break;
        case 19782: // :FM#    x#      Get Display temp mode (Celsius=1, Fahrenheit=0)
            replystr = String(GSfocus.tempmode) + "#";
            sendresponsestr(replystr);
            break;
        case 22872: // :XY# troubleshooting only - print currentaddr value, use in serial monitor mode is best
            sendresponsestr("-#");
            break;
        case 18246: // :FG# initiate a move to the target position
            gotonewposition = 1;
            isMoving = 1;
            break;
        case 20806: // :FQ# stop a move - HALT
            gotonewposition = 0;
            isMoving = 0;
            targetPosition = currentPosition;
            writenow = 1;             // updating of EEPROM ON
            previousMillis = millis();   // start time interval
            break;
        case 20051: // :SNxxxx# set new target position SNXXXX - this is a move command
            // but must be followed by a FG command to start the move
            pos = hexstr2long(param);
            if ( pos < 0 )
                pos = 0;
            else if ( pos > GSfocus.maxstep)
                pos = GSfocus.maxstep;
            targetPosition = pos;
            gotonewposition = 0;
            isMoving = 0;
            break;
        case 18512: // :PH# home the motor, hard-coded, ignore parameters
            // not implemented in INDI driver
            gotonewposition = 1;
            isMoving = 1;
            targetPosition = 0;
            break;
        case 20563: // :SPxxxx# set current position to received position - no move SPXXXX
            // in INDI driver, only used to set to 0 SP0000 in reset()
            pos = hexstr2long(param);
            targetPosition = pos;
            if ( pos < 0 )
                pos = 0;
            else if ( pos > GSfocus.maxstep)
                pos = GSfocus.maxstep;
            currentPosition = targetPosition;
            writenow = 1;
            previousMillis = millis();
            gotonewposition = 0;
            isMoving = 0;
            break;
        case 18003: // set fullstep mode
            setstepmode(1);
            updatemotorSpeedDelay();
            writenow = 1;
            previousMillis = millis();
            break;
        case 18515: // set halfstep mode
            setstepmode(2);
            updatemotorSpeedDelay();
            writenow = 1;
            previousMillis = millis();
            break;
        case 21331: // set stepmode
            // GSfocus command
            tmp = (byte) hexstr2long(param);
            setstepmode((byte) tmp );
            updatemotorSpeedDelay();
            writenow = 1;
            previousMillis = millis();
            break;
        case 20307: // :SOxxxx# set the coilPwr setting
            GSfocus.coilPwr = (byte) decstr2int(param) & 0x01;
            writenow = 1;
            previousMillis = millis();
            break;
        case 21075: // :SRxx# set the Reverse Direction setting
            GSfocus.ReverseDirection = (byte) decstr2int(param) & 0x01;
            writenow = 1;
            previousMillis = millis();
            break;
        case 19780: // :DMx# set displaystate C or F
            GSfocus.tempmode = (byte) decstr2int(param) & 0x01;
            writenow = 1;
            previousMillis = millis();
            break;
        case 21325: // set motorSpeed - time delay between pulses, acceptable values are 00, 01 and 02 which
            // correspond to a slow, med, high
            // GSfocus command
            tmp = (byte) decstr2int(param) & 0x03;
            motorSpeed = (byte) tmp;
            savedmotorSpeed = motorSpeed;        // remember the speed setting
            updatemotorSpeedDelay();
            break;
        case 21581: // :MTxxx# set the MotorSpeed Threshold
            // GSfocus command
            tmp = decstr2int(param);
            if ( tmp < 50 )
                tmp = 50;
            else if ( tmp > 200 )
                tmp = 200;
            TSWTHRESHOLD = (byte) tmp;
            break;
        case 22093: // :MVx#       None         Set Enable/Disable motorspeed change when moving
            motorspeedchange = (byte) decstr2int(param) & 0x01;
            break;
        case 22605: // :MX#          None        Save settings to EEPROM
            // copy current settings and write the data to EEPROM
            GSfocus.validdata = 99;
            GSfocus.fposition = currentPosition;
            GSfocus.maxstep = maxSteps;
            EEPROM_writeAnything(currentaddr, GSfocus);    // update values in EEPROM
            writenow = 0;
            break;
        case 17491: // :SDxx# set step delay, only acceptable values are 02, 04, 08, 10, 20 which
            // correspond to a stepping delay of 250, 125, 63, 32 and 16 steps
            // per second respectively. Moonlite only
            stepdelay = (int) hexstr2long(param);
            break;
        case 17235: // :SCxx# set temperature co-efficient XX
            // do nothing, ignore
            tempcomp = (int) hexstr2long(param);
            break;
        case 43: // + activate temperature compensation focusing
            // ignore
            break;
        case 45: // - disable temperature compensation focusing
            // ignore
            break;
        case 20304:  // :PO# temperature calibration offset POXX in 0.5 degree increments (hex)
        {
            // Moonlite only
            // this adds/subtracts an offset from the temperature reading in 1/2 degree C steps
            // FA -3, FB -2.5, FC -2, FD -1.5, FE -1, FF -.5, 00 0, 01 0.5, 02 1.0, 03 1.5, 04 2.0, 05 2.5, 06 3.0
            tempoffsetval = 0.0;
            // param is a char []
            String parm1 = String(param);
            if ( parm1 == "FA" )
                tempoffsetval = -3.0;
            else if ( parm1 == "FB")
                tempoffsetval = -2.5;
            else if ( parm1 == "FC")
                tempoffsetval = -2.0;
            else if ( parm1 == "FD")
                tempoffsetval = -1.5;
            else if ( parm1 == "FE")
                tempoffsetval = -1.0;
            else if ( parm1 == "FF")
                tempoffsetval = -0.5;
            else if ( parm1 == "00")
                tempoffsetval = 0.0;
            else if ( parm1 == "01")
                tempoffsetval = 0.5;
            else if ( parm1 == "02")
                tempoffsetval = 1.0;
            else if ( parm1 == "03")
                tempoffsetval = 1.5;
            else if ( parm1 == "04")
                tempoffsetval = 2.0;
            else if ( parm1 == "05")
                tempoffsetval = 2.5;
            else if ( parm1 == "06")
                tempoffsetval = 3.0;
            break;
        }
        case 19795: // :SMxxx# set new maxSteps position SMXXXX
            // GSfocus command
            pos = hexstr2long(param);
            if ( pos > maxFocuserLimit )
                pos = maxFocuserLimit;
            // avoid setting maxSteps below current position
            if ( pos < currentPosition )
                pos = currentPosition;
            // for NEMA17 at 400 steps this would be 5 full rotations of focuser knob
            // for 28BYG-28 this would be less than 1/2 a revolution of focuser knob
            maxSteps = pos;
            // check maxIncement in case its larger
            if ( maxIncrement > maxSteps )
                maxIncrement = maxSteps;
            // signal that the focuser position has changed and should be saved to eeprom
            writenow = 1;             // updating of EEPROM ON
            previousMillis = millis();   // start time interval
            break;
          case 22867: // :SYxxxx# set new maxIncrement SYXXXX
              // GSfocus command
              // pos = hexstr2long(param);
              // ignore
              maxIncrement = maxSteps;
              break;
        case 21316: // :DSx# disable or enable the display setting
            #ifdef LCDDISPLAY
            if ( decstr2int(param) == 0 )
            {
                myoled.Display_Off();
                displayenabled = 0;
            }
            else
            {
                myoled.Display_On();
                displayenabled = 1;
            }
            #endif
            break;
        case 22611: // :SXxxxx# None    Set updatedisplayNotMoving (length of time an LCD page is displayed for in milliseconds
            pos = hexstr2long(param);
            // bounds check to 2000-4000 2s-4s
            if ( pos < 2000L )
                pos = 2000L;
            if ( pos > 4000L )
                pos = 4000L;
            GSfocus.updatedisplayintervalNotMoving = pos;
            writenow = 1;             // updating of EEPROM ON
            previousMillis = millis();   // start time interval
            break;
        case 16724: // :TA#  Reboot Arduino
            software_Reboot();
            break;
        case 21328: // :PS    Set temperature precision (9-12 = 0.5, 0.25, 0.125, 0.0625)
            #ifdef TEMPERATUREPROBE
            tmp = decstr2int(param);
            if ( tprobe1 == 1 )
            {
                sensor1.setResolution( tpAddress, tmp );
                GSfocus.ds18b20resolution = (byte) tmp;        // save setting in EEPROM
                writenow = 1;             // updating of EEPROM ON
                previousMillis = millis();   // start time interval
            }
            else
            {
                // no probe, set to default
                GSfocus.ds18b20resolution = TEMP_PRECISION;
            }
            #endif
            break;
        case 19792: // :PMxx#    None    set update of position on lcd when moving (00=disable, 01=enable)
            GSfocus.lcdupdateonmove = (byte) decstr2int(param) & 0x01;
            writenow = 1;             // updating of EEPROM ON
            previousMillis = millis();   // start time interval
            break;
        case 23120: // :PZxx#   None      Set the return of user specified stepsize to be OFF - default (0) or ON (1)
            GSfocus.stepsizeenabled = (byte) decstr2int(param) & 0x01;
            writenow = 1;             // updating of EEPROM ON
            previousMillis = millis();   // start time interval
            break;
        case 20560: // :PPxxxx#  None     Set the step size value - double type, eg 2.1
            // convert param to float
            tmpstr = param;
            tmpstr = tmpstr + "";  // add end of string terminator
            GSfocus.stepsize = (double) tmpstr.toFloat();
            if ( GSfocus.stepsize < 0 )
            {
                GSfocus.stepsize = 0;
            }
            writenow = 1;             // updating of EEPROM ON
            previousMillis = millis();   // start time interval
            break;
        case 23128: // :XZ# troubleshooting only - reset focuser defaults
            // GSfocus command
            currentaddr = 0;
            ResetFocuserDefaults();
            // Set focuser defaults.
            currentPosition = GSfocus.fposition;
            targetPosition = GSfocus.fposition;
            maxSteps = GSfocus.maxstep;
            break;
    }
}

#ifdef OLEDDISPLAY
// displaylcd screen
void displaylcd()
{
    char tempString[20];
  
    // display values
    //myoled.clear();
    myoled.home();
    myoled.print("Current Pos =");
    myoled.print(currentPosition);
    myoled.clearToEOL();
    myoled.println();
    myoled.print("Target Pos  =");
    myoled.print(targetPosition);
    myoled.clearToEOL();
    myoled.println();
  
    myoled.print(coilpwrtxt);
    if ( GSfocus.coilPwr )
        myoled.print(ontxt);
    else
        myoled.print(offtxt);
    myoled.clearToEOL();
    myoled.println();
  
    myoled.print(revdirtxt);
    if ( GSfocus.ReverseDirection )
        myoled.print(ontxt);
    else
        myoled.print(offtxt);
    myoled.clearToEOL();
    myoled.println();
  
    // stepmode setting
    if ( GSfocus.stepmode == 1 )
        myoled.print("Step Mode   =Full");
    else if ( GSfocus.stepmode == 2 )
        myoled.print("Step Mode   =Half");
    else if ( GSfocus.stepmode == 4 )
        myoled.print("Step Mode   =1/4");
    else if ( GSfocus.stepmode == 8 )
        myoled.print("Step Mode   =1/8");
    else if ( GSfocus.stepmode == 16 )
        myoled.print("Step Mode  =1/16");
    else if ( GSfocus.stepmode == 32 )
        myoled.print("Step Mode   =1/32");
  
    myoled.clearToEOL();
    myoled.println();
  
    //Temperature
    char tempstr[8];
    if ( GSfocus.tempmode == 1 )
    {
        // no need to convert, already in C
        dtostrf(ch1tempval, 4, 3, tempstr);
        myoled.print  ("Temperature =");
        //myoled.print(tempstr);
        myoled.print(dht_temp_read);
        myoled.print(" c");
        myoled.clearToEOL();
        myoled.println();
    }
    else
    {
        // convert to F for display
        float tempvalf;
        tempvalf = (ch1tempval * 1.8) + 32;
        dtostrf(tempvalf, 4, 3, tempstr);
        myoled.print("Temperature =");
        myoled.print(tempstr);
        myoled.print(" f");
        myoled.clearToEOL();
        myoled.println();
    }
  
    //Motor Speed
    if ( motorSpeed == 0 )
       myoled.print("Motor Speed =Slow");
    else if (motorSpeed == 1 )
       myoled.print("Motor Speed =Med");
    else if ( motorSpeed == 2 )
       myoled.print("Motor Speed =Fast");
    myoled.clearToEOL();
    myoled.println();
  
    //MaxSteps
    myoled.print("MaxSteps    =");
    ltoa(GSfocus.maxstep, tempString, 10);
    myoled.print(tempString);
    myoled.clearToEOL();
    myoled.println();
}
      
void updatepositionoled()
{
    myoled.setCursor(0, 0);
    myoled.print("Current Pos =");
    myoled.print(currentPosition);
    myoled.clearToEOL();
    myoled.println();
    myoled.print("Target Pos  =");
    myoled.print(targetPosition);
    myoled.clearToEOL();
    myoled.println();
}
#endif

#ifdef TEMPERATUREPROBE
// find the address of the DS18B20 sensor probe
void findds18b20address()
{
    // look for probes
    // Search the wire for address
    if (sensor1.getAddress(tpAddress, 0))
    {
        tprobe1 = 1;  // there is a probe1
    }
    // device address is now saved in tpAddress
}
#endif

#ifdef DHT22
void playDHT(void)
{
    dht_humi_read = dht.readHumidity();
    dht_temp_read = dht.readTemperature();
}
#endif
