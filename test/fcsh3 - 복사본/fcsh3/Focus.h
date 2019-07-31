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


// disable the stepper motor outputs - coil power off
void disableoutput()
{
    digitalWrite(myEnable, HIGH);
    // control via ENABLE pin, but this turns off indexer inside DRV8825
    // which means the stepper motor will loose power and move position to the nearest full step
    // not an issue if using full steps but major issue if using microstepping as will cause change
    // in focus position
}
void enableoutput()
{
   digitalWrite(myEnable, LOW);
}


// Move stepper 방향에 따라서 결정
void anticlockwise()
{
(!GSfocus.ReverseDirection) ? digitalWrite(myDir, LOW ) : digitalWrite(myDir, HIGH ); //myDir으로 방향 결정
digitalWrite(myStep, 1 ); //myStep으로 움직이는지 결정
delayMicroseconds(stepontime);
digitalWrite(myStep, 0 );
}
void clockwise()
{
        (!GSfocus.ReverseDirection) ? digitalWrite(myDir, HIGH ) : digitalWrite(myDir, LOW );
        digitalWrite(myStep, 1 );
        delayMicroseconds(stepontime);
        digitalWrite(myStep, 0 );
}


// converting to long/int
long hexstr2long(char *line)
{
    long ret = 0;
    ret = strtol(line, NULL, 16);
    return (ret);
}
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
    GSfocus.stepmode = stepmode;
}


void ResetFocuserDefaults()
{
    GSfocus.validdata = 99;
    GSfocus.fposition = 5000L;
    GSfocus.maxstep = 10000L;
    GSfocus.stepmode = 1;
    GSfocus.ReverseDirection = 0;
    GSfocus.coilPwr = 1;
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
        Serial.print("send : ");
        Serial.println(str);
    }


}

void sendresponsenl()
{
    if (Serial)
    {
      Serial.print("get : ");
        Serial.println();
    }

}

//나중에 복구해
// Serial Commands
//void processCommand()
//{
//    byte len;
//    int tmp;
//    long pos;
//    int cmdval;
//    String replystr = "";
//    String tmpstr;
//    char mycmd[MAXCOMMAND];
//    char param[MAXCOMMAND];
//    char tempString[8];
//  
//    memset(mycmd, 0, MAXCOMMAND);
//    memset(param, 0, MAXCOMMAND);
//    replystr = queue.pop();
//    len = replystr.length();
//    if ( len == 1 )
//    {
//        mycmd[0] = replystr[0];
//        #ifdef DEBUG
//        Serial.print("tmpstr cmd = "); Serial.println(mycmd);
//        #endif
//    }
//    if ( len == 2 )
//    {
//        tmpstr = replystr.substring(0, 3);
//        tmpstr.toCharArray(mycmd, 3);
//        #ifdef DEBUG
//        Serial.print("tmpstr cmd = "); Serial.println(tmpstr);
//        #endif
//    }
//    else if ( len > 2 )
//    {
//        tmpstr = replystr.substring(0, 3);
//        tmpstr.toCharArray(mycmd, 3);
//        #ifdef DEBUG
//        Serial.print("tmpstr cmd = "); Serial.println(tmpstr);
//        #endif
//        tmpstr = replystr.substring(2, len + 1);
//        tmpstr.toCharArray(param, tmpstr.length() + 1);
//        #ifdef DEBUG
//        Serial.print("tmpstr param = "); Serial.println(tmpstr);
//        #endif
//    }
//    else return;
//  
//    if ( len > 1)
//        cmdval = (int) mycmd[0] + ((int) mycmd[1] * 256);
//    else
//        cmdval = (int) mycmd[0];
//  
//    #ifdef DEBUG
//        Serial.print("replystr = "); Serial.println(replystr);
//        Serial.print("len = "); Serial.println(len);
//        Serial.print("mycmd = "); Serial.println(mycmd);
//        Serial.print("param = "); Serial.println(param);
//        Serial.print("cmdval = "); Serial.println(cmdval);
//    #endif
//  
//    switch ( cmdval )
//    {
//        case 20551:   // :GP# get the current focuser position
//            sprintf(tempString, "%04X", (unsigned int) currentPosition);
//            replystr = String(tempString) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 18759 : // :GI# motor is moving - 1 if moving, 0 otherwise
//            replystr = "0" + String(isMoving) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 21575: // :GT# get the current temperature - moonlite compatible
//            tmp = (ch1tempval * 2);
//            sprintf(tempString, "%04X", (int) tmp);
//            replystr = String(tempString) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 23111: // :GZ# get the current temperature
//            dtostrf(ch1tempval, 4, 3, tempString);
//            replystr = String(tempString) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 22087: // :GV# firmware value Moonlite
//            sendresponsestr("10#");
//            break;
//        case 17991: // :GF# firmware value
//            sendresponsestr(String(programName));
//            sendresponsenl();
//            sendresponsestr(String(programVersion) + "#");
//            break;
//        case 19783: // :GM# get the MaxSteps
//            sprintf(tempString, "%04X", (unsigned int) maxSteps);
//            replystr = String(tempString) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 22855: // :GY# get the maxIncrement - set to MaxSteps
//            sprintf(tempString, "%04X", (unsigned int) maxIncrement);
//            replystr = String(tempString) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 18503: // whether half-step is enabled or not, moonlite always return "00"
//            if ( GSfocus.stepmode == 2 )
//                replystr = "FF#";
//            else
//                replystr = "00#";
//            sendresponsestr(replystr);
//            break;
//        case 21319: // get stepmode
//            sprintf(tempString, "%02X", GSfocus.stepmode);
//            replystr = String(tempString) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 20295: // :GO# get the coilPwr setting
//            replystr = String(GSfocus.coilPwr) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 21063: // :GR# get the Reverse Direction setting
//            replystr = String(GSfocus.ReverseDirection) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 21069: // :MR# get Motor Speed
//            replystr = String(motorSpeed) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 21837: // :MU# Get the MotorSpeed Threshold
//            replystr = String(TSWTHRESHOLD) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 22349: // :MW#         xxx#      Get if motorspeedchange enabled/disabled
//            replystr = String(motorspeedchange) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 16967: // :GB# LED backlight value, always return "00" - moonlite
//            // not implemented in INDI driver
//            sendresponsestr("00#");
//            break;
//        case 20039: // :GN# get the new motor position (target)
//            // not implemented in INDI driver
//            sprintf(tempString, "%04X", (unsigned int) targetPosition);
//            replystr = String(tempString) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 17479: // :GD# get the current motor step delay, only values of 02, 04, 08, 10, 20
//            // not used so just return 02
//            sprintf(tempString, "%02X", stepdelay);
//            replystr = String(tempString) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 17223: // :GC# get temperature co-efficient XX
//            // do nothing, ignore
//            sprintf(tempString, "%02X", tempcomp);
//            replystr = String(tempString) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 18244: // :DG# get display state on or off
//            replystr = String(displayenabled) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 22599: // :GXxxxxx#          get the time that an LCD screen is displayed for (in milliseconds, eg 2500 = 2.5seconds
//            sprintf(tempString, "%04X", (unsigned int) GSfocus.updatedisplayintervalNotMoving);
//            replystr = String(tempString) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 18256: // :PG    Get temperature precision (9-12)
//            tmp = TEMP_PRECISION;
//            
//           
//            replystr = String(tmp) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 20048: // :PN# xx#     get update of position on lcd when moving (00=disable, 01=enable)
//            replystr = String(GSfocus.lcdupdateonmove) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 20816: // :PQ#    None      Get if stepsize is enabled in controller (1 or 0, 0/1)
//            replystr = String(GSfocus.stepsizeenabled) + "#";
//            sendresponsestr(replystr);
//            break;
//        case 21072: // :PR#    xxxxx#    Get step size in microns (if enabled by controller)
//            replystr = String(GSfocus.stepsize) + "#";
//            sendresponsestr(replystr);
//            break;
//        
//        case 22872: // :XY# troubleshooting only - print currentaddr value, use in serial monitor mode is best
//            sendresponsestr("-#");
//            break;
//        case 18246: // :FG# initiate a move to the target position
//            gotonewposition = 1;
//            isMoving = 1;
//            break;
//        case 20806: // :FQ# stop a move - HALT
//            gotonewposition = 0;
//            isMoving = 0;
//            targetPosition = currentPosition;
//            previousMillis = millis();   // start time interval
//            break;
//        case 20051: // :SNxxxx# set new target position SNXXXX - this is a move command
//            // but must be followed by a FG command to start the move
//            pos = hexstr2long(param);
//            if ( pos < 0 )
//                pos = 0;
//            else if ( pos > GSfocus.maxstep)
//                pos = GSfocus.maxstep;
//            targetPosition = pos;
//            gotonewposition = 0;
//            isMoving = 0;
//            break;
//        case 18512: // :PH# home the motor, hard-coded, ignore parameters
//            // not implemented in INDI driver
//            gotonewposition = 1;
//            isMoving = 1;
//            targetPosition = 0;
//            break;
//        case 20563: // :SPxxxx# set current position to received position - no move SPXXXX
//            // in INDI driver, only used to set to 0 SP0000 in reset()
//            pos = hexstr2long(param);
//            targetPosition = pos;
//            if ( pos < 0 )
//                pos = 0;
//            else if ( pos > GSfocus.maxstep)
//                pos = GSfocus.maxstep;
//            previousMillis = millis();
//            gotonewposition = 0;
//            isMoving = 0;
//            break;
//        case 18003: // set fullstep mode
//            setstepmode(1);
//            previousMillis = millis();
//            break;
//        case 18515: // set halfstep mode
//            setstepmode(2);
//            previousMillis = millis();
//            break;
//        case 21331: // set stepmode
//            // GSfocus command
//            tmp = (byte) hexstr2long(param);
//            setstepmode((byte) tmp );
//            previousMillis = millis();
//            break;
//        case 20307: // :SOxxxx# set the coilPwr setting
//            GSfocus.coilPwr = (byte) decstr2int(param) & 0x01;
//            previousMillis = millis();
//            break;
//        case 21075: // :SRxx# set the Reverse Direction setting
//            GSfocus.ReverseDirection = (byte) decstr2int(param) & 0x01;
//            previousMillis = millis();
//            break;
//        
//        case 21325: // set motorSpeed - time delay between pulses, acceptable values are 00, 01 and 02 which
//            // correspond to a slow, med, high
//            // GSfocus command
//            tmp = (byte) decstr2int(param) & 0x03;
//            motorSpeed = (byte) tmp;
//            savedmotorSpeed = motorSpeed;        // remember the speed setting
//            break;
//        case 21581: // :MTxxx# set the MotorSpeed Threshold
//            // GSfocus command
//            tmp = decstr2int(param);
//            if ( tmp < 50 )
//                tmp = 50;
//            else if ( tmp > 200 )
//                tmp = 200;
//            TSWTHRESHOLD = (byte) tmp;
//            break;
//        case 22093: // :MVx#       None         Set Enable/Disable motorspeed change when moving
//            motorspeedchange = (byte) decstr2int(param) & 0x01;
//            break;
//        case 22605: // :MX#          None        Save settings to EEPROM
//            // copy current settings and write the data to EEPROM
//            GSfocus.validdata = 99;
//            GSfocus.fposition = currentPosition;
//            GSfocus.maxstep = maxSteps;
//            EEPROM_writeAnything(currentaddr, GSfocus);    // update values in EEPROM
//            break;
//        case 17491: // :SDxx# set step delay, only acceptable values are 02, 04, 08, 10, 20 which
//            // correspond to a stepping delay of 250, 125, 63, 32 and 16 steps
//            // per second respectively. Moonlite only
//            stepdelay = (int) hexstr2long(param);
//            break;
//        case 17235: // :SCxx# set temperature co-efficient XX
//            // do nothing, ignore
//            tempcomp = (int) hexstr2long(param);
//            break;
//        case 43: // + activate temperature compensation focusing
//            // ignore
//            break;
//        case 45: // - disable temperature compensation focusing
//            // ignore
//            break;
//        case 20304:  // :PO# temperature calibration offset POXX in 0.5 degree increments (hex)
//        {
//            // Moonlite only
//            // this adds/subtracts an offset from the temperature reading in 1/2 degree C steps
//            // FA -3, FB -2.5, FC -2, FD -1.5, FE -1, FF -.5, 00 0, 01 0.5, 02 1.0, 03 1.5, 04 2.0, 05 2.5, 06 3.0
//            tempoffsetval = 0.0;
//            // param is a char []
//            String parm1 = String(param);
//            if ( parm1 == "FA" )
//                tempoffsetval = -3.0;
//            else if ( parm1 == "FB")
//                tempoffsetval = -2.5;
//            else if ( parm1 == "FC")
//                tempoffsetval = -2.0;
//            else if ( parm1 == "FD")
//                tempoffsetval = -1.5;
//            else if ( parm1 == "FE")
//                tempoffsetval = -1.0;
//            else if ( parm1 == "FF")
//                tempoffsetval = -0.5;
//            else if ( parm1 == "00")
//                tempoffsetval = 0.0;
//            else if ( parm1 == "01")
//                tempoffsetval = 0.5;
//            else if ( parm1 == "02")
//                tempoffsetval = 1.0;
//            else if ( parm1 == "03")
//                tempoffsetval = 1.5;
//            else if ( parm1 == "04")
//                tempoffsetval = 2.0;
//            else if ( parm1 == "05")
//                tempoffsetval = 2.5;
//            else if ( parm1 == "06")
//                tempoffsetval = 3.0;
//            break;
//        }
//        case 19795: // :SMxxx# set new maxSteps position SMXXXX
//            // GSfocus command
//            pos = hexstr2long(param);
//            if ( pos > maxFocuserLimit )
//                pos = maxFocuserLimit;
//            // avoid setting maxSteps below current position
//            if ( pos < currentPosition )
//                pos = currentPosition;
//            // for NEMA17 at 400 steps this would be 5 full rotations of focuser knob
//            // for 28BYG-28 this would be less than 1/2 a revolution of focuser knob
//            maxSteps = pos;
//            // check maxIncement in case its larger
//            if ( maxIncrement > maxSteps )
//                maxIncrement = maxSteps;
//            // signal that the focuser position has changed and should be saved to eeprom
//            previousMillis = millis();   // start time interval
//            break;
//          case 22867: // :SYxxxx# set new maxIncrement SYXXXX
//              // GSfocus command
//              // pos = hexstr2long(param);
//              // ignore
//              maxIncrement = maxSteps;
//              break;
//       
//        case 22611: // :SXxxxx# None    Set updatedisplayNotMoving (length of time an LCD page is displayed for in milliseconds
//            pos = hexstr2long(param);
//            // bounds check to 2000-4000 2s-4s
//            if ( pos < 2000L )
//                pos = 2000L;
//            if ( pos > 4000L )
//                pos = 4000L;
//            GSfocus.updatedisplayintervalNotMoving = pos;
//            previousMillis = millis();   // start time interval
//            break;
//        case 16724: // :TA#  Reboot Arduino
//            software_Reboot();
//            break;
//        case 21328: // :PS    Set temperature precision (9-12 = 0.5, 0.25, 0.125, 0.0625)
//            
//
//            break;
//        case 19792: // :PMxx#    None    set update of position on lcd when moving (00=disable, 01=enable)
//            GSfocus.lcdupdateonmove = (byte) decstr2int(param) & 0x01;
//            previousMillis = millis();   // start time interval
//            break;
//        case 23120: // :PZxx#   None      Set the return of user specified stepsize to be OFF - default (0) or ON (1)
//            GSfocus.stepsizeenabled = (byte) decstr2int(param) & 0x01;
//            previousMillis = millis();   // start time interval
//            break;
//        case 20560: // :PPxxxx#  None     Set the step size value - double type, eg 2.1
//            // convert param to float
//            tmpstr = param;
//            tmpstr = tmpstr + "";  // add end of string terminator
//            GSfocus.stepsize = (double) tmpstr.toFloat();
//            if ( GSfocus.stepsize < 0 )
//            {
//                GSfocus.stepsize = 0;
//            }
//            previousMillis = millis();   // start time interval
//            break;
//        case 23128: // :XZ# troubleshooting only - reset focuser defaults
//            // GSfocus command
//            currentaddr = 0;
//            ResetFocuserDefaults();
//            // Set focuser defaults.
//            currentPosition = GSfocus.fposition;
//            targetPosition = GSfocus.fposition;
//            maxSteps = GSfocus.maxstep;
//            break;
//    }
//}


