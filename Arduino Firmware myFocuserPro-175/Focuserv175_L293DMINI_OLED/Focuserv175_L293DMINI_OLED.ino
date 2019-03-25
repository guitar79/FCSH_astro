// myfocuser L293D-MINI-OLED
// ONLY FOR USE WITH NEMA and L293D MINI DRIVER BOARD

// This firmware file supports all versions

// IF USING MOONLITE ASCOM DRIVER
// MAKE SURE THAT POWER ON RESET CIRCUIT IS ENABLED
// ELSE MOONLITE WILL REPORT A TIMEOUT ERROR
// TO REPROGRAM THE CONTROLLER DISABLE THE POWER ON RESET PREVENT CIRCUIT

// (c) Copyright Robert Brown 2014-2018. All Rights Reserved.
// YOU MAY NOT SELL CONTROLLERS OR PCB'S BASED ON THIS PROJECT
// for commercial gain without express written permission granted from the author.
// Schematics, Code, Firmware, Ideas, Applications, Layout are protected by International Copyright Law.

// Permission is NOT granted to any person to redistribute, market, manufacture or sell for commercial gain the myFocuser
// products, ideas, PCB's, circuits, builds, variations and units described or discussed herein or on this site.
// Permission is granted for personal and Academic/Educational use only.

// THIS MEANS THAT YOU CANNOT RE-DESIGN AND MAKE AVAILABLE KITS OR PCB BASED ON THIS PROJECT AND
// OFFER THOSE KITS FOR SALE TO OTHERS. THAT IS A BREACH OF COPYRIGHT.

// PCB to use
// L293D-MINI   https://aisler.net/brown_rb/my-project-repository/myfocuser-l293dmini-mmtfbt

// ----------------------------------------------------------------------------------------------------------
// HARDWARE MAPPINGS

// BlueTooth HC-05 Module wired as follows - Do not use with Rotary Encoder
// EN/CMD           // do not connect
// RX to middle of 2K and 1K resistor - other end of 2K resistor to D11 and other end of 1K resistor to ground
// TX to D10
// STATE            // do not connect

// Mapping for PushButtons
// A0

// Mapping for OLED DISPLAY
// VCC    5V
// GND    GND
// SDA    A4
// SCL    A5


// ----------------------------------------------------------------------------------------------------------
// DEFINABLE FEATURES
// Caution: Do not enable a feature if you have not added the associated hardware circuits to support that feature
// With this version you can enable or disable specific add-ons and build the controller with the options you want
// using a single firmware file
// By default, this is a MINIMAL Controller firmware

// For a MINIMAL controller, comment out - TEMPERATUREPROBE, PUSHBUTTONS, DISPLAY
// For a MT controller, comment out - PUSHBUTTONS, DISPLAY
// For a FULL controller, uncomment TEMPERATUREPROBE, PUSHBUTTONS, OLEDDISPLAY, BUZZER, INOUTLEDS
// To enable Bluetooth on any controller, uncomment BLUETOOTH

// To enable bluetooth, uncomment the next line
//#define BLUETOOTH 1

// To enable temperature probe, uncomment next line
#define TEMPERATUREPROBE 1

// To enable the Push Buttons for manual focusing, uncomment the next line
#define PUSHBUTTONS 1

// To enable the LCD DISPLAY uncomment the next line (LCD1602, I2C)
#define OLEDDISPLAY 1

// To enable the buzzer, uncomment the next line
#define BUZZER 1

// To enable the IN-OUT LEDS, uncomment the next line
#define INOUTLEDS 1

// do not change
//#define DEBUG

// ----------------------------------------------------------------------------------------------------------
// FIRMWARE CHANGE LOG

// 1.75 16122018
// rewrite for oled
// major changes to commands and processing
// speed improvements
// code space reductions
// major changes to focuser struct
// rewrite temperature code
// Compatible with myFocuser ASCOM driver
// Compatible with myFocuser Application
// Compatible with Moonlite ASCOM driver and application
//
// based on some portions of original code by Dave Wells, and orly.andico@gmail.com (moonlite focuser example)
// heavily modified by RBB

// ----------------------------------------------------------------------------------------------------------
// FIRMWARE START
#include <Arduino.h>
#include <myQueue.h>                //  By Steven de Salas
#ifdef TEMPERATUREPROBE
#include <OneWire.h>                // needed for DS18B20 temperature probe, see https://github.com/PaulStoffregen/OneWire
#include <myDallasTemperature.h>    // needed for DS18B20 temperature probe, see https://github.com/milesburton/Arduino-Temperature-Control-Library
#endif
#ifdef OLEDDISPLAY
#include <Wire.h>                   // needed for I2C, installed when installing the Arduino IDE
#include <mySSD1306Ascii.h>         // oled
#include <mySSD1306AsciiWire.h>     // oled
#endif
#include <myEEPROM.h>               // needed for EEPROM
#include <myeepromanything.h>       // needed for EEPROM
#ifdef BLUETOOTH
#include <SoftwareSerial.h>         // needed for bt adapter - this library is already included when you install the Arduino IDE
#endif
#include <myHalfStepper.h>          // Author: Tom Biuso <tjbtech@yahoo.com>

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

#define TEMPREFRESHRATE     5000L       // refresh rate between temperature conversions unless an update is requested via serial command
#define LCDUPDATESTEPCOUNT  15          // the number of steps moved which triggers an lcd update when moving, do not make too small
#define OLEDADDRESS         0x3C
#define OLED_SDA            A4          // connected to SDA pin on OLED
#define OLED_SCL            A5          // connected to SCL pin on OLED, and connect OLED VCC pin to 5V and OLED GND pin to GND
// Connect OLED VCC pin to 5V and OLED GND pin to GND
#define PBswitchesPin   A0
#define bledIN          A1
#define gledOUT         A2
#define Buzzer          A3
#define ch1temp         2
#define EEPROMSIZE      1024      // ATMEGA328P 1024 EEPROM
#define TIMEINTERVAL    10000L
#define MAXSTEPLIMIT    65000
#define STARTMAXPOS     10000
#define STARTPOS        5000
#define MAXINC          1000
#define PULSETIME       5
#define MOTORSPEEDSLOW  4
#define MOTORSPEEDMED   7
#define MOTORSPEEDFAST  50
#define MOTORSPEEDDELAY 2500             // the delay in microseconds between steps
#define SLOW            0                // motorspeeds
#define MED             1
#define FAST            2
#define STEP1           1                 // step modes
#define STEP2           2
#define STEPSPERREVOLUTION  200           // NEMA17 motor Full steps
// you need to change the above line to reflect your stepper motor, examples below
// #define STEPSPERREVOLUTION 2048        // 24BBYJ-48 motor, if half stepping multiply by 2
// #define STEPSPERREVOLUTION 1036        // NEMA17-PG5 motor, if half stepping multiply by 2
// #define STEPSPERREVOLUTION 200         // NEMA17 motor, if half stepping multiply by 2
// #define STEPSPERREVOLUTION 5370        // NEMA17-PG25 motor, if half stepping multiply by 2
#define IN1             3                 // Stepper Motor stuff, control pins for L293D Mini driver board
#define IN2             4
#define IN3             5
#define IN4             6
#define MAXCOMMAND      15
#define TEMP_PRECISION  10
#define TMPREFRESHRATE  5000
#define MTRTHRESHHOLD   200

const String programName = "myFP-L293DM";
const String programVersion = "175";
char ProgramAuthor[] = "(c) R BROWN 2019";
char coilpwrtxt[]  = "Coil power  =";
char revdirtxt[]   = "Reverse Dir =";
char offtxt[]      = "OFF";
char ontxt[]       = "ON ";

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

#ifdef PUSHBUTTONS
int PBVal;                  // Push button value
#endif

#define SerialPortSpeed 9600
Queue<String> queue(10);              // receive serial queue of commands
char line[MAXCOMMAND];
byte eoc;                             // end of command
byte idx;                             // index into command string

#ifdef BLUETOOTH
char btline[MAXCOMMAND];
byte bteoc;
byte btidx;
#define BTPortSpeed 9600
#define btRX    11                    // define Bluetooth Adapter
#define btTX    10
SoftwareSerial btSerial( btTX, btRX);
#endif

byte displayenabled = 1;              // used to enable and disable the display
long olddisplaytimestampNotMoving;
byte updatecount;                     // loop variable used in updating lcd when moving
byte LCD1602Screen;                   // used to decide which LCD screen to display
byte lcdupdatestepcount;              // the number of steps moved which triggers an lcd update when moving
#ifdef OLEDDISPLAY
SSD1306AsciiWire myoled;
#endif

byte tprobe1;                         // indicate if there is a probe attached to that channel
byte requesttempflag;
double ch1tempval;                    // temperature value for probe
double lasttempval;                   // holds previous temperature value - used if ismoving and if temp request < 10s apart
#ifdef TEMPERATUREPROBE
OneWire oneWirech1(ch1temp);          // setup temperature probe 1
DallasTemperature sensor1(&oneWirech1);
DeviceAddress tpAddress;            // used to send precision setting to specific sensor
double starttempval;
long lasttempconversion;            // holds time of last conversion
long temprefreshrate;               // refresh rate between temperature conversions unless an update is requested via serial command
#endif

HalfStepper mystepper(STEPSPERREVOLUTION, IN1, IN3, IN4, IN2);
int motorPins[] = { IN1, IN2, IN3, IN4 };       // used to disable the output so it does not overheat

void software_Reboot()
{
  // jump to the start of the program
  asm volatile ( "jmp 0");
}

#ifdef PUSHBUTTONS
// read the push button switches and return state of switches
// 1 = SW1 ON AND SW2 OFF, 2 = SW2 ON AND SW1 OFF, 3 = SW1 ON and SW2 ON, 0 = OFF
int readpbswitches(int pinNum)
{
  // for 1.2K sw1 (681) 650-720, sw2 (338) 310-380, sw1 and sw2 (509) 460-530
  // for 1K sw1 (630) 600-660, sw2 (335) 300-370, sw1 and sw2 (510) 480-540
  int val = 0;                       // variable to store the read value
  digitalWrite(pinNum, HIGH);
  val = analogRead(pinNum);          // read the input pin
  if ( val >= 650 && val <= 720 )
  {
    return 1;                        // toggle sw1 ON and SW2 OFF
  }
  else if ( val >= 460 && val <= 530 )
  {
    return 3;                    // toggle sw1 and sw2 ON
  }
  else if ( val >= 310 && val <= 380 )
  {
    return 2;                    // toggle sw2 ON and SW1 OFF
  }
  else return 0;                   // switches are OFF
}
#endif

// disable the stepper motor outputs
void clearOutput()
{
  // check to see what is selected, keep or release
  if ( !myfocuser.coilPwr )
  {
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(motorPins[i], 0);
    }
  }
}

// Move stepper anticlockwise
void anticlockwise()
{
#ifdef INOUTLEDS
  (!myfocuser.ReverseDirection) ? digitalWrite( gledOUT, 1) : digitalWrite( bledIN, 1);
#endif
  (!myfocuser.ReverseDirection) ?  mystepper.step(-1) :  mystepper.step(1);
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
  (!myfocuser.ReverseDirection) ?  mystepper.step(1) :  mystepper.step(-1);
#ifdef INOUTLEDS
  (!myfocuser.ReverseDirection) ? digitalWrite( bledIN, 0 ) : digitalWrite( gledOUT, 0);
#endif
}

// set the microstepping mode
void setstepmode(byte stepmode)
{
  if ( stepmode == STEP1 )
  {
    mystepper.SetSteppingMode(SteppingMode::FULL);
    myfocuser.stepmode = STEP1;
  }
  else if ( stepmode == STEP2 )
  {
    mystepper.SetSteppingMode(SteppingMode::HALF);
    myfocuser.stepmode = STEP2;
  }
  else
  {
    mystepper.SetSteppingMode(SteppingMode::FULL);
    myfocuser.stepmode = STEP1;
  }
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

#ifdef DEBUG
  Serial.print("replystr = "); Serial.println(replystr);
  Serial.print("len = "); Serial.println(len);
  Serial.print("mycmd = "); Serial.println(mycmd);
  Serial.print("param = "); Serial.println(param);
#endif

  if ( len > 1)
    cmdval = (int) mycmd[0] + ((int) mycmd[1] * 256);
  else
    cmdval = (int) mycmd[0];

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
      writenow = 1;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
      gotonewposition = 0;
      isMoving = 0;
      break;
    case 18003: // set fullstep mode
      setstepmode(1);
      writenow = 1;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
      break;
    case 18515: // set halfstep mode
      setstepmode(2);
      writenow = 1;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
      break;
    case 21331: // set stepmode
      // myfocuser command
      tmp = (byte) hexstr2long(param) & 0x03;
      setstepmode((byte) tmp );
      writenow = 1;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
      break;
    case 20307: // :SOxxxx# set the coilPwr setting
      myfocuser.coilPwr = (byte) decstr2int(param) & 0x01;
      writenow = 1;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
      break;
    case 21075: // :SRxx# set the Reverse Direction setting
      myfocuser.ReverseDirection = (byte) decstr2int(param) & 0x01;
      writenow = 1;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
      break;
    case 19780: // :DMx# set displaystate C or F
      myfocuser.tempmode = (byte) decstr2int(param) & 0x01;
      writenow = 1;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
      break;
    case 21325: // set motorSpeed - time delay between pulses, acceptable values are 00, 01 and 02 which
      // correspond to a slow, med, high
      // myfocuser command
      tmp = (byte) decstr2int(param) & 0x03;
      motorSpeed = (byte) tmp;
      savedmotorSpeed = motorSpeed;           // remember the speed setting
      switch (motorSpeed)
      {
        case SLOW: mystepper.setSpeed(MOTORSPEEDSLOW); break;
        case MED: mystepper.setSpeed(MOTORSPEEDMED); break;
        case FAST: mystepper.setSpeed(MOTORSPEEDFAST); break;
      }
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
#ifdef OLEDDISPLAY
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
  myoled.print("Motor Speed = ");
  switch (motorSpeed )
  {
    case SLOW: myoled.print("Slow");
      break;
    case MED: myoled.print("Med");
      break;
    case FAST: myoled.print("Fast");
      break;
  }
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
  btSerial.begin(BTPortSpeed);            // start bt adapter
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
  TSWTHRESHOLD = MTRTHRESHHOLD;
  motorspeedchange = 1;
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
  gotonewposition = 0;
  displayenabled = 0;

  displayenabled = 0;
  olddisplaytimestampNotMoving = 0L;
  updatecount = 0;
  lcdupdatestepcount = 15;
#ifdef OLEDDISPLAY
  displayenabled = 1;                          // display is enabled
  olddisplaytimestampNotMoving = previousMillis;
  // Setup the OLED
  Wire.begin();
  myoled.begin(&Adafruit128x64, OLEDADDRESS);
  myoled.set400kHz();
  myoled.setFont(Adafruit5x7);
  myoled.clear();                                 // clrscr OLED
  myoled.Display_Normal();                        // black on white
  myoled.Display_On();                            // display ON
  myoled.Display_Rotate(0);                       // portrait, not rotated
  myoled.Display_Bright();

  // The screen size is 128 x 64, so using characters at 6x8 this gives 21chars across and 8 lines down
  myoled.println(programName);                    // print startup screen
  myoled.println(programVersion);
  myoled.InverseCharOn();
  myoled.println(ProgramAuthor);
  myoled.InverseCharOff();
#endif

#ifdef TEMPERATUREPROBE
  lasttempconversion = 0L;
  temprefreshrate = TMPREFRESHRATE;
#endif

  // setup INx step lines as outputs, set low so motor does not move
  pinMode( IN1, OUTPUT );
  pinMode( IN2, OUTPUT );
  pinMode( IN3, OUTPUT );
  pinMode( IN4, OUTPUT );
  digitalWrite( IN1, 0);
  digitalWrite( IN2, 0);
  digitalWrite( IN3, 0);
  digitalWrite( IN4, 0);

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
    // set the focuser back to the previous settings
    // done after this in one hit
    // mark current eeprom address as invalid and use next one
    // each time focuser starts it will read current storage, set it to invalid, goto next location and
    // write values to there and set it to valid - so it doesnt always try to use same locations over and
    // over and destroy the eeprom
    // using it like an array of [0-nlocations], ie 100 storage locations for 1k EEPROM
    EEPROM_readAnything( currentaddr, myfocuser );
    myfocuser.validdata = 0;
    EEPROM_writeAnything(currentaddr, myfocuser);    // update values in EEPROM
    // goto next free address and write data
    currentaddr += datasize;
    // bound check the eeprom storage and if greater than last index [0-EEPROMSIZE-1] then set to 0
    if ( currentaddr >= (nlocations * datasize) )
      currentaddr = 0;
    myfocuser.validdata = 99;
    EEPROM_writeAnything(currentaddr, myfocuser);    // update values in EEPROM
  }
  else
  {
    ResetFocuserDefaults();
  }

  currentPosition = myfocuser.fposition;  // Set focuser defaults from saved values in EEPROM.
  targetPosition = myfocuser.fposition;
  maxSteps = myfocuser.maxstep;

  if ( myfocuser.coilPwr == 0)
    clearOutput();
  myfocuser.stepmode = myfocuser.stepmode & 0x01;
  motorSpeed = FAST;
  savedmotorSpeed = FAST;
  setstepmode(myfocuser.stepmode);
  mystepper.setSpeed(MOTORSPEEDFAST);
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

#ifdef PUSHBUTTONS
  PBVal = readpbswitches(PBswitchesPin);
  if ( PBVal != 0)
  {
    // if its a genuine button press
    delay(50);
    PBVal = readpbswitches(PBswitchesPin);
    // now check the pbval using a switch for 1 2 and 3
    switch ( PBVal )
    {
      case 1:                        // toggle sw1 is ON and 2 is off
        // move IN
        targetPosition = targetPosition - 1;
        if ( targetPosition < 0 )
          targetPosition = 0;
#ifdef OLEDDISPLAY
        updatepositionoled();
#endif
        gotonewposition = 1;
        break;
      case 2:                        // toggle sw2 is ON and SW1 is OFF
        // move OUT
        targetPosition = targetPosition + 1;
        // this line required because only target commands received are checked
        if ( targetPosition > myfocuser.maxstep )
          targetPosition = myfocuser.maxstep;
#ifdef OLEDDISPLAY
        updatepositionoled();
#endif
        gotonewposition = 1;
        break;
      case 3:                        // toggle sw1 and sw2 are ON
#ifdef BUZZER
        digitalWrite(Buzzer, 1);    // turn on buzzer
#endif
        while ( readpbswitches(PBswitchesPin) == 3 )  // wait for pb to be released
          ;
        currentPosition = 0;
        targetPosition = 0;
        isMoving = 0;
        gotonewposition = 0;
#ifdef BUZZER
        digitalWrite(Buzzer, 0);     // turn off buzzer
#endif
#ifdef OLEDDISPLAY
              updatepositionoled();
#endif
        break;
      default:
        // do nothing
        break;
    } // end of switch
  } // end of pb test
#endif

  // Move the position by a single step if target is different to current position
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
        motorSpeed = SLOW;
        switch (motorSpeed)
        {
          case SLOW: mystepper.setSpeed(MOTORSPEEDSLOW); break;
          case MED: mystepper.setSpeed(MOTORSPEEDMED); break;
          case FAST: mystepper.setSpeed(MOTORSPEEDFAST); break;
        }
      }
    }

    isMoving = 1;
    writenow = 1;             // updating of EEPROM off
    previousMillis = millis();    // keep updating previousMillis whilst focuser is moving

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

#ifdef OLEDDISPLAY    // check if lcd needs updating during move
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
    delayMicroseconds(MOTORSPEEDDELAY);  // required else stepper will not move
  }
  else
  {
    // focuser is NOT moving now, move is completed
    isMoving = 0;
    gotonewposition = 0;

    // reset motorSpeed
    motorSpeed = savedmotorSpeed;
    switch (motorSpeed)
    {
      case SLOW: mystepper.setSpeed(MOTORSPEEDSLOW); break;
      case MED: mystepper.setSpeed(MOTORSPEEDMED); break;
      case FAST: mystepper.setSpeed(MOTORSPEEDFAST); break;
    }

#ifdef OLEDDISPLAY
    // see if the display needs updating
    long currentMillis = millis();
    if ( ((currentMillis - olddisplaytimestampNotMoving) > myfocuser.updatedisplayintervalNotMoving) || (currentMillis < olddisplaytimestampNotMoving) )
    {
      olddisplaytimestampNotMoving = currentMillis;    // update the timestamp
      displaylcd();                           // update ALL the display values - takes about 2s
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
    } // end of if( tprobe1 == 1 )
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
        EEPROM_writeAnything(currentaddr, myfocuser);    // update values in EEPROM
        writenow = 0;
      }
    }
    clearOutput(); // release the stepper coils to save power
  }
}

void clearSerialPort()
{
  while ( Serial.available() )
    Serial.read();
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
      {
        btidx = MAXCOMMAND - 1;
      }
    }
    else
    {
      if (btinChar == '#')
      {
        bteoc = 1;
        btidx = 0;
        queue.push(String(btline));
        bteoc = 0;
        memset( btline, 0, MAXCOMMAND);
      }
    }
  }
}
#endif

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
      {
        idx = MAXCOMMAND - 1;
      }
    }
    else
    {
      if (inChar == '#')
      {
        eoc = 1;
        idx = 0;
        queue.push(String(line));
        eoc = 0;
        memset( line, 0, MAXCOMMAND);
      }
    }
  }
}

