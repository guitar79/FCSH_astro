/*
   === USE AT YOUR OWN RISK ===
   For more information on setting OnStep up see:
     http://www.stellarjourney.com/index.php?r=site/equipment_onstep and
   join the OnStep Groups.io at
     https://groups.io/g/onstep

   See the corresponding Pins.xxx.h file for detailed information on this pin map
   to be sure it matches your wiring

*/

// === CONFIGURATION START

#define FCS_Kevin_touch_ON
#ifdef  FCS_Kevin_touch_ON

// DEFINABLE FEATURES
// Caution: Do not enable a feature if you have not added the associated hardware circuits to support that feature
// With this version you can enable or disable specific add-ons and build the controller with the options you want
// using a single firmware file




// do not change
//#define DEBUG 


// FIRMWARE START
#include <Arduino.h>
#include <myQueue.h>                    // By Steven de Salas
#include <myEEPROM.h>                   // needed for EEPROM
#include <myeepromanything.h>           // needed for EEPROM





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
    byte stepsizeenabled;                 // if 1, controller returns step size
    byte lcdupdateonmove;                 // update position on lcd when moving
} GSfocus;

#define TEMPREFRESHRATE     1000L       // refresh rate between temperature conversions unless an update is requested via serial command
#define LCDUPDATESTEPCOUNT  15          // the number of steps moved which triggers an lcd update when moving, do not make too small

#define bledIN              A1
#define gledOUT             A2
#define ch1temp             2



#define myDir               4
#define myStep              3
#define myEnable            A6
#define myM2                13         // microstepping lines
#define myM1                12         // M0/M1/M2 sets stepping mode 000=F, 100=1/2, 010=1/4, 110=1/8, 001=1/16, 101=1/16, 110=1/16 etc
#define myM0                A1         // steps per revolution = 200, 400, 800, 1600, 6400
#define BTTX                6

#define EEPROMSIZE        1024      // ATMEGA328P 1024 EEPROM
#define TIMEINTERVAL      10000L
#define MAXSTEPLIMIT      999999
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

Queue<String> queue(10);            // receive serial queue of commands
char line[MAXCOMMAND];
byte eoc;                           // end of command
byte idx;                           // index into command string

byte displayenabled = 1;            // used to enable and disable the display
long olddisplaytimestampNotMoving;  // timestamp of last display update
byte lcdupdatestepcount;            // the number of steps moved which triggers an lcd update when moving
int updatecount;                    // loop variable used in updating lcd when moving






byte tprobe1;                       // indicate if there is a probe attached to that channel
double ch1tempval;                  // temperature value for probe
double lasttempval;                 // holds previous temperature value - used if ismoving and if temp request < 10s apart




#endif
