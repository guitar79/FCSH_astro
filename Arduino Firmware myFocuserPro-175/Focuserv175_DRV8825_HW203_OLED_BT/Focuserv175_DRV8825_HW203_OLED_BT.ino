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
// DEFINABLE FEATURES
// Caution: Do not enable a feature if you have not added the associated hardware circuits to support that feature
// With this version you can enable or disable specific add-ons and build the controller with the options you want
// using a single firmware file

// To enable bluetooth, uncomment the next line -  - Do not use with Rotary Encoder
#define BLUETOOTH 1

// To enable the OLED DISPLAY uncomment the next line
#define OLEDDISPLAY 1

// To enable temperature probe, uncomment next line
#define TEMPERATUREPROBE 1

// To enable the buzzer, uncomment the next line
#define BUZZER 1

// To enable the IN-OUT LEDS, uncomment the next line
#define INOUTLEDS 1

// do not change
//#define DEBUG

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
// FIRMWARE START
#include <Arduino.h>
#include <myQueue.h>                    // By Steven de Salas
#include <myEEPROM.h>                   // needed for EEPROM
#include <myeepromanything.h>           // needed for EEPROM
#ifdef TEMPERATUREPROBE
#include <OneWire.h>                    // needed for DS18B20 temperature probe, see https://github.com/PaulStoffregen/OneWire
#include <myDallasTemperature.h>        // needed for DS18B20 temperature probe, see https://github.com/milesburton/Arduino-Temperature-Control-Library
#endif
#ifdef OLEDDISPLAY
#include <Wire.h>                       // needed for I2C, installed when installing the Arduino IDE
#include <mySSD1306Ascii.h>             // oled
#include <mySSD1306AsciiWire.h>         // oled
#endif
#ifdef BLUETOOTH
#include <SoftwareSerial.h>         // needed for bt adapter - this library is already included when you install the Arduino IDE
#endif

// these values saved in EEPROM and restored on boot
struct config_t {
  byte validdata;                       // if this is 99 then data is valid
  long fposition;                       // last focuser position
  long maxstep;                         // max steps
  long updatedisplayintervalNotMoving;  // refresh rate of display - time each page is displayed for
  double stepsize;                      // the step size in microns, ie 7.2
  byte stepmode;                        // indicates stepmode, full, half, 1/4, 1/8. 1/16. 1/32 [1.2.4.8.16.32]
  byte ds18b20resolution;               // resolution of DS18B20 temperature probe
  byte ReverseDirection;                // reverse direction
  byte coilPwr;                         // coil pwr
  byte tempmode;                        // temperature display mode, Celcius=1, Fahrenheit=0
  byte stepsizeenabled;                 // if 1, controller returns step size
  byte lcdupdateonmove;                 // update position on lcd when moving
} myfocuser;

#define TEMPREFRESHRATE     1000L       // refresh rate between temperature conversions unless an update is requested via serial command
#define LCDUPDATESTEPCOUNT  15          // the number of steps moved which triggers an lcd update when moving, do not make too small
#define OLEDADDRESS         0x3C        // some OLED displays maybe at 0x3F, use I2Cscanner to find the correct address
#define OLED_SDA            A4          // connected to SDA pin on OLED
#define OLED_SCL            A5          // connected to SCL pin on OLED, and connect OLED VCC pin to 5V and OLED GND pin to GND
#define bledIN              A1
#define gledOUT             A2
#define Buzzer              A3
#define ch1temp             2
#define myDir               3
#define myStep              4
#define myEnable            8
#define myM2                5         // microstepping lines
#define myM1                6         // M0/M1/M2 sets stepping mode 000=F, 100=1/2, 010=1/4, 110=1/8, 001=1/16, 101=1/16, 110=1/16 etc
#define myM0                7         // steps per revolution = 200, 400, 800, 1600, 6400
#define BTRX                11        // bluetooth adapter HC05
#define BTTX                10

#define EEPROMSIZE        1024      // ATMEGA328P 1024 EEPROM
#define TIMEINTERVAL      10000L
#define MAXSTEPLIMIT      65000
#define STARTMAXPOS       10000
#define STARTPOS          5000
#define MAXINC            1000
#define PULSETIME         5             // expressed in microseconds
#define MOTORSPEEDSLOW0   80000
#define MOTORSPEEDSLOW2   40000
#define MOTORSPEEDSLOW4   4000
#define MOTORSPEEDSLOW8   2000
#define MOTORSPEEDSLOW16  1000
#define MOTORSPEEDSLOW32  1000
#define MOTORSPEEDMED0    22000
#define MOTORSPEEDMED2    3250
#define MOTORSPEEDMED4    1500
#define MOTORSPEEDMED8    800
#define MOTORSPEEDMED16   400
#define MOTORSPEEDMED32   400
#define MOTORSPEEDFAST0   3000
#define MOTORSPEEDFAST2   1500
#define MOTORSPEEDFAST4   600
#define MOTORSPEEDFAST8   400
#define MOTORSPEEDFAST16  200
#define MOTORSPEEDFAST32  200
#define SLOW              0
#define MED               1
#define FAST              2
#define MAXCOMMAND        15
#define TEMP_PRECISION    10
#define MTRTHRESHHOLD     200

char programName[]    = "myFP-DRV8825HW203-OLED";
char programVersion[] = "175";
char ProgramAuthor[]  = "(c) R BROWN 2018";
char ontxt[]          = "ON ";
char offtxt[]         = "OFF";
char coilpwrtxt[]     = "Coil power  =";
char revdirtxt[]      = "Reverse Dir =";

long currentPosition;       // current position
long targetPosition;        // target position
long maxFocuserLimit;       // arbitary focuser limit
long maxSteps;              // maximum position of focuser
long maxIncrement;          // maximum number of steps permitted in one move
long minimumPosition;       // minimum position to avoid focuser damage
long pos;
int currentaddr;            // will be address in eeprom of the data stored
byte writenow;              // should we update values in eeprom
long previousMillis;        // used as a delay whenever the EEPROM settings need to be updated
long myinterval;            // interval in milliseconds to wait after a move before writing settings to EEPROM, 10s
byte stepdelay;             // used by moonlite
byte tempcomp;              // used by moonlite
double tempoffsetval;       // used by moonlite
byte stepontime;            // stepontime - time in microseconds that coil power is ON for one step, board requires 2us pulse
int motorSpeedDelay;        // the delay between steps
byte motorSpeed;            // the motorspeed setting 0=slow, 1=medium, 2=fast, default=slow on startup only
byte savedmotorSpeed;       // used to save original speed if slowing down when nearing target position
byte gotonewposition;       // used by moonlite after an SN command followed by a FG
byte isMoving;              // is the motor currently moving
byte TSWTHRESHOLD;          // position at which stepper slows down at it approaches home position
byte motorspeedchange;
String fastreply;

#define SerialPortSpeed     9600
#define BTPORTSPEED         9600   // define bluetooth speed, DO NOT CHANGE

Queue<String> queue(10);            // receive serial queue of commands
char line[MAXCOMMAND];
byte eoc;                           // end of command
byte idx;                           // index into command string

byte displayenabled = 1;            // used to enable and disable the display
long olddisplaytimestampNotMoving;  // timestamp of last display update
byte lcdupdatestepcount;            // the number of steps moved which triggers an lcd update when moving
int updatecount;                    // loop variable used in updating lcd when moving

#ifdef BLUETOOTH
char btline[MAXCOMMAND];
byte bteoc;
byte btidx;
SoftwareSerial btSerial( BTTX, BTRX);
#endif

#ifdef OLEDDISPLAY
SSD1306AsciiWire myoled;
char tempString[12];
#endif

byte tprobe1;                       // indicate if there is a probe attached to that channel
double ch1tempval;                  // temperature value for probe
double lasttempval;                 // holds previous temperature value - used if ismoving and if temp request < 10s apart

#ifdef TEMPERATUREPROBE
OneWire oneWirech1(ch1temp);        // setup temperature probe 1
DallasTemperature sensor1(&oneWirech1);
DeviceAddress tpAddress;            // used to send precision setting to specific sensor
double starttempval;
long lasttempconversion;            // holds time of last conversion
byte requesttempflag;
#endif

// ----------------------------------------------------------------------------------------------------------
// CODE START

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

// enable the stepper motor outputs - coil power on
void enableoutput()
{
  digitalWrite(myEnable, LOW);
}

// Move stepper anticlockwise
void anticlockwise()
{
#ifdef INOUTLEDS
  (!myfocuser.ReverseDirection) ? digitalWrite( gledOUT, 1) : digitalWrite( bledIN, 1);
#endif
  (!myfocuser.ReverseDirection) ? digitalWrite(myDir, LOW ) : digitalWrite(myDir, HIGH );
  digitalWrite(myStep, 1 );
  delayMicroseconds(stepontime);
  digitalWrite(myStep, 0 );
#ifdef INOUTLEDS
  (!myfocuser.ReverseDirection) ? digitalWrite( gledOUT, 0 ) : digitalWrite( bledIN, 0);
#endif
}

// Move stepper clockwise
void clockwise()
{
#ifdef INOUTLEDS
  (!myfocuser.ReverseDirection) ? digitalWrite( bledIN, 1) : digitalWrite( gledOUT, 1);
#endif
  (!myfocuser.ReverseDirection) ? digitalWrite(myDir, HIGH ) : digitalWrite(myDir, LOW );
  digitalWrite(myStep, 1 );
  delayMicroseconds(stepontime);
  digitalWrite(myStep, 0 );
#ifdef INOUTLEDS
  (!myfocuser.ReverseDirection) ? digitalWrite( bledIN, 0 ) : digitalWrite( gledOUT, 0);
#endif
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
  myfocuser.stepmode = stepmode;
}

void updatemotorSpeedDelay()
{
  switch ( motorSpeed )
  {
    case 0: // slow
      switch (myfocuser.stepmode)
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
      switch (myfocuser.stepmode)
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
      switch (myfocuser.stepmode)
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
  //Serial.print("motorSpeed = "); Serial.println(motorSpeed);
  //Serial.print("stepmode = "); Serial.println(myfocuser.stepmode);
  //Serial.print("motorSpeedDelay = "); Serial.println(motorSpeedDelay);
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
  myfocuser.validdata = 99;
  myfocuser.fposition = 5000L;
  myfocuser.maxstep = 10000L;
  myfocuser.stepmode = 1;
  myfocuser.ReverseDirection = 0;
  myfocuser.coilPwr = 1;
  myfocuser.tempmode = 1; // celsius
  myfocuser.updatedisplayintervalNotMoving = 2500L;
  myfocuser.ds18b20resolution = TEMP_PRECISION;
  myfocuser.stepsizeenabled = 0;
  myfocuser.stepsize = 10;
  myfocuser.lcdupdateonmove = 0;
  // now write the data to EEPROM
  EEPROM_writeAnything(currentaddr, myfocuser);    // update values in EEPROM
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
      if ( myfocuser.stepmode == 2 )
        replystr = "FF#";
      else
        replystr = "00#";
      sendresponsestr(replystr);
      break;
    case 21319: // get stepmode
      sprintf(tempString, "%02X", myfocuser.stepmode);
      replystr = String(tempString) + "#";
      sendresponsestr(replystr);
      break;
    case 20295: // :GO# get the coilPwr setting
      replystr = String(myfocuser.coilPwr) + "#";
      sendresponsestr(replystr);
      break;
    case 21063: // :GR# get the Reverse Direction setting
      replystr = String(myfocuser.ReverseDirection) + "#";
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
      sprintf(tempString, "%04X", (unsigned int) myfocuser.updatedisplayintervalNotMoving);
      replystr = String(tempString) + "#";
      sendresponsestr(replystr);
      break;
    case 18256: // :PG    Get temperature precision (9-12)
      tmp = TEMP_PRECISION;
#ifdef TEMPERATUREPROBE
      if ( tprobe1 == 1 )
      {
        myfocuser.ds18b20resolution = (byte) sensor1.getResolution(tpAddress);
        tmp = myfocuser.ds18b20resolution;
        writenow = 1;             // updating of EEPROM ON
        previousMillis = millis();   // start 30s time interval
      }
#endif
      replystr = String(tmp) + "#";
      sendresponsestr(replystr);
      break;
    case 20048: // :PN# xx#     get update of position on lcd when moving (00=disable, 01=enable)
      replystr = String(myfocuser.lcdupdateonmove) + "#";
      sendresponsestr(replystr);
      break;
    case 20816: // :PQ#    None      Get if stepsize is enabled in controller (1 or 0, 0/1)
      replystr = String(myfocuser.stepsizeenabled) + "#";
      sendresponsestr(replystr);
      break;
    case 21072: // :PR#    xxxxx#    Get step size in microns (if enabled by controller)
      replystr = String(myfocuser.stepsize) + "#";
      sendresponsestr(replystr);
      break;
    case 19782: // :FM#    x#      Get Display temp mode (Celsius=1, Fahrenheit=0)
      replystr = String(myfocuser.tempmode) + "#";
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
      else if ( pos > myfocuser.maxstep)
        pos = myfocuser.maxstep;
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
      else if ( pos > myfocuser.maxstep)
        pos = myfocuser.maxstep;
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
      // myfocuser command
      tmp = (byte) hexstr2long(param);
      setstepmode((byte) tmp );
      updatemotorSpeedDelay();
      writenow = 1;
      previousMillis = millis();
      break;
    case 20307: // :SOxxxx# set the coilPwr setting
      myfocuser.coilPwr = (byte) decstr2int(param) & 0x01;
      writenow = 1;
      previousMillis = millis();
      break;
    case 21075: // :SRxx# set the Reverse Direction setting
      myfocuser.ReverseDirection = (byte) decstr2int(param) & 0x01;
      writenow = 1;
      previousMillis = millis();
      break;
    case 19780: // :DMx# set displaystate C or F
      myfocuser.tempmode = (byte) decstr2int(param) & 0x01;
      writenow = 1;
      previousMillis = millis();
      break;
    case 21325: // set motorSpeed - time delay between pulses, acceptable values are 00, 01 and 02 which
      // correspond to a slow, med, high
      // myfocuser command
      tmp = (byte) decstr2int(param) & 0x03;
      motorSpeed = (byte) tmp;
      savedmotorSpeed = motorSpeed;        // remember the speed setting
      updatemotorSpeedDelay();
      break;
    case 21581: // :MTxxx# set the MotorSpeed Threshold
      // myfocuser command
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
      myfocuser.validdata = 99;
      myfocuser.fposition = currentPosition;
      myfocuser.maxstep = maxSteps;
      EEPROM_writeAnything(currentaddr, myfocuser);    // update values in EEPROM
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
      // myfocuser command
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
      // myfocuser command
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
      myfocuser.updatedisplayintervalNotMoving = pos;
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
        myfocuser.ds18b20resolution = (byte) tmp;        // save setting in EEPROM
        writenow = 1;             // updating of EEPROM ON
        previousMillis = millis();   // start time interval
      }
      else
      {
        // no probe, set to default
        myfocuser.ds18b20resolution = TEMP_PRECISION;
      }
#endif
      break;
    case 19792: // :PMxx#    None    set update of position on lcd when moving (00=disable, 01=enable)
      myfocuser.lcdupdateonmove = (byte) decstr2int(param) & 0x01;
      writenow = 1;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
      break;
    case 23120: // :PZxx#   None      Set the return of user specified stepsize to be OFF - default (0) or ON (1)
      myfocuser.stepsizeenabled = (byte) decstr2int(param) & 0x01;
      writenow = 1;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
      break;
    case 20560: // :PPxxxx#  None     Set the step size value - double type, eg 2.1
      // convert param to float
      tmpstr = param;
      tmpstr = tmpstr + "";  // add end of string terminator
      myfocuser.stepsize = (double) tmpstr.toFloat();
      if ( myfocuser.stepsize < 0 )
      {
        myfocuser.stepsize = 0;
      }
      writenow = 1;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
      break;
    case 23128: // :XZ# troubleshooting only - reset focuser defaults
      // myfocuser command
      currentaddr = 0;
      ResetFocuserDefaults();
      // Set focuser defaults.
      currentPosition = myfocuser.fposition;
      targetPosition = myfocuser.fposition;
      maxSteps = myfocuser.maxstep;
      break;
  }
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
  if ( myfocuser.coilPwr )
    myoled.print(ontxt);
  else
    myoled.print(offtxt);
  myoled.clearToEOL();
  myoled.println();

  myoled.print(revdirtxt);
  if ( myfocuser.ReverseDirection )
    myoled.print(ontxt);
  else
    myoled.print(offtxt);
  myoled.clearToEOL();
  myoled.println();

  // stepmode setting
  if ( myfocuser.stepmode == 1 )
    myoled.print("Step Mode   =Full");
  else if ( myfocuser.stepmode == 2 )
    myoled.print("Step Mode   =Half");
  else if ( myfocuser.stepmode == 4 )
    myoled.print("Step Mode   =1/4");
  else if ( myfocuser.stepmode == 8 )
    myoled.print("Step Mode   =1/8");
  else if ( myfocuser.stepmode == 16 )
    myoled.print("Step Mode  =1/16");
  else if ( myfocuser.stepmode == 32 )
    myoled.print("Step Mode   =1/32");

  myoled.clearToEOL();
  myoled.println();

  //Temperature
  char tempstr[8];
  if ( myfocuser.tempmode == 1 )
  {
    // no need to convert, already in C
    dtostrf(ch1tempval, 4, 3, tempstr);
    myoled.print  ("Temperature =");
    myoled.print(tempstr);
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
  ltoa(myfocuser.maxstep, tempString, 10);
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

