myFocuserPro Arduino Firmware ChangeLog
// (c) Copyright Robert Brown 2014-2019. All Rights Reserved.

// YOU MAY NOT SELL CONTROLLERS OR PCB'S BASED ON THIS PROJECT
// for commercial gain without express written permission granted from the author.
// Schematics, Code, Firmware, Ideas, Applications, Layout are protected by International Copyright Law.

// Permission is NOT granted to any person to redistribute, market, manufacture or sell for commercial gain the myFocuser
// products, ideas, PCB's, circuits, builds, variations and units described or discussed herein or on this site.
// Permission is granted for personal and Academic/Educational use only.

// THIS MEANS THAT YOU CANNOT RE-DESIGN AND MAKE AVAILABLE KITS OR PCB BASED ON THIS PROJECT AND
// OFFER THOSE KITS FOR SALE TO OTHERS. THAT IS A BREACH OF COPYRIGHT.

Do not forget to copy the library folders as instructed

Configurations are specified at the start of each firmware file
Some changes may be specific to a specific driver board
General changes across all firmware versions are listed here

Do not change any of files or replace the library files with other versions

==============================================================================================================
The firmware zip file contains
 - firmware for all driver boards
 - test programs
 - links to videos
 - library files (each in its own folder under "myFocuserPro Required Libraries" - read how to use these with Arduino IDE
==============================================================================================================
Professional PCB: MYFOCUSERPRO2 PCB CAN BE ORDERED ONLINE
PCB				Link					Firmware to use
DRV8825HW203-M-MT-F-BT		https://aisler.net/p/DWEURMAC		Focuserv1xx_DRV8825_HW203 or Focuserv172_RAPS128_HW203
DRV8825HW203_FIRBT 		https://aisler.net/p/UGHLNFJC		Focuserv1xx_DRV8825_HW203_IR	for infra red controller
DRV8825HW203_FRE 		https://aisler.net/p/KMHRZAON		Focuserv1xx_DRV8825_HW203_RE	for rotary encoder
A4998-M-MT-F-BT			https://aisler.net/p/RLQDYNPU		Focuserv1xx_A4998_HW203
DRV8825-M-MT-F-BT		https://aisler.net/p/QVXMBSWW		Focuserv1xx_DRV8825
EASYDRIVER-HW203-M-MT-F-BT	https://aisler.net/p/NCMLHIHM 		Focuserv1xx_DRV8825_HW203
EASYDRIVER-HW203_FRE 		https://aisler.net/p/ISHXZBYS		Focuserv1xx_DRV8825_HW203_RE
L298N-M-MT-F-BT			https://aisler.net/p/QWESFIOS		Focuserv1xx_L298N	
ULN2003-M-MT-F	 		https://aisler.net/p/DEYGMRQC		Focuserv1xx_ULN2003
==============================================================================================================

// 1.75 16122018
// rewrite for oled
// introduce half stepping to ULN2003
// major changes to commands and processing
// speed improvements
// code space reductions
// major changes to focuser struct
// rewrite temperature code
// Compatible with myFocuser ASCOM driver
// Compatible with myFocuser Application
// Compatible with Moonlite ASCOM driver and application
// You need to first clear the EEPROM before reprogramming the controller
// Tests/ClearEEPROM/ClearEEPROM.ino

// 1.74 22092018
// Fix for display temp C/F
// Fix for invalid position
// Fix for not handling single char commands

// 1.73 21082018
// Changes to pushbutton code
// Changes to coil power enabling/disabling

// v1.72
// Change firmware to single file
// Changes to processCommand()
// Changes to clockwise() and anticlockwise()
// Changes to serial events code incl bluetooth
// Implement queue for received serial commands
// Add defines for BUZZER and INOUT LEDS
// Wrap include of OneWire into #define as only use for I2C
// Consolidate check code routines
// Consolidate serial response routines
// myXXXX libraries must be placed in myDocuments/Arduino/libraries
// Complier Define Options now used to generate different firmware versions
// Enable defines for BLUETOOTH, TEMPERATUREPROBE, PUSHBUTTONS, LCDDISPLAY, INOUTLEDS, BUZZER
// Reduce memory footprint

// v1.71 18112017
// Fix major bug in nearinghomepos

// v1.70 31102017
// Fix errors in timer overflows

// v1.69 29072017
// Changes for compatibility with IDE v1.6.8
// Changes to Dallas Temperature Code and EEPROM and HalfStepper files and AFMotor (where included)
// Minor fixes

// v1.68 ALL VERSIONS 03112016
// Fix errors related to motorspeed setting and motor delays

// v1.67 ALL VERSIONS 28092016
// Fix for small values of stepsize between 0-1

// v1.66 ALL VERSION 11092016
// Fix to work with APT 3.13
// Fix incorrect logic in handling of GT temperature command for moonlite

// v1.65_xx FULL VERSIONS ONLY 15082016
// Fix to PBCode (again)
// Change to GF Get Firmware code

// v1.64_xx FULL VERSIONS ONLY 11082016
// Fix to PBCode as v163 fix upset motor speed settings

// v1.63_xxxx All VERSIONS 04082016
// Small number of fixes related to LED's and PB code

// v1.62_xxxx ALL VERSIONS 26062016
// Fix error when disable/enable display

// v1.61_xxxx ALL VERSIONS 07062016
// Fix error when requesting MaxIncrement

// v1.60_xxxx ALL VERSIONS 18052016
// Tweaks to improve response
// Remove updatelcd when moving
// Add lcdupdateonmove, set and get
// Add LCD update when moving
// Remove tempwaitdelay - wait delay when requesting temperature
// Focuser remembers temperature mode (C/F) setting
// Focuser now remembers temperature resolution setting correctly (was resetting to 0.5)
// Add get and set temperature resolution (9-12)
// Add get/set Motorspeed change and threshold setting
// Removed numwrites
// Removed :XY to free up space
// Support reset of focuser defaults from Windows App
// Save current focuser settings to EEPROM support for Windows App/
// Get motorspeed
// Set motorspeedchange value (threshold) when moving - switches to slowspeed when nearing destination
// Return motorspeedchange value for which stepper slows down at end of its move
// Enable/Disable motorspeedchange when moving
// Get status of motorspeedchange enabled/disabled
// Add Set/Get Step size
// Add Stepsize enabled/disabled
// Fix for setting LCD page display time
// 
// For use with Windows App v1750 or greater
// For use with ASCOM driver v1600 or greater


// v1.58_xxxxxxT (All versions with temperature probe _MT _F 29042016
// Further refinement of temperature code

// v1.57 18042016 (all versions)
// Major changes to temperature code

// v1.56_xxx 12042016 (all versions) - works with WinApp 1729 and ASCOM 1538 (or greater)
// Changes to support higher serial port baud rates

// v1.54_xxx 24022016 (Full versions only)
// Fixed bug in temperature display on LCD (was not showing Fahrenheit values)

// v1.53_XXX 26012016
// Added extra LCD page to display temperature and maxsteps
// Delay between LCD pages is user configurable and remembered between sessions
// This new feature only works with the latest application software and ASCOM drivers
// fixed bug in setting temperrorwaitdelay "SE" - was being set to 20,000 instead of 2,000
// Added GX and SX to handle updatedisplayintervalNotMoving

// v1.52_XXX 14122015
// updating of LCD etc now via timers to ensure regular update and response to commands (Full versions)
// minor bug fixes and removal of unneccessary code (All versions)
// motorspeed values verified/changed in some versions

// v1.51_XXX 15112015
// Major bug fixes

// v1.50_XXX 13112015
// Restored dummy send on startup to communicate with moonlite focuser

// v1.49_XXX 09112015
// changes to DS18B20 precision and gettemp routines, fixes to IN/OUT/Buzzer LEDS
// added commands for display in celsius or fahrenheit (:SC# and :SF#)
// changed lcd display to handle C/F as well as increased step mode 64/128

// v1.48_XXX 24102015
// MaxSteps limit now 65000
// get MaxIncrement returns MaxIncrement

// v1.47_XXX 03092015
// Initiate a temperature conversion on connect()

// v1.46_XXX 31082015
// Made the wait between tempcalls a saved variable in EEPROM. now it can be reconfigured via Serial interface
// Added serial commands :SExx# and :GE# to send and get the tempwaitdelay time in milliseconds
//
// v1.45_XXX 30082015
// Fixed issue in :GZ (line of code was left out by mistake)
// Changed time between temp calls to 5s from 10s
//
// v1.44_XXX 14072015
// Major changes in GZ/GT calls, return last temp if focuser is moving, also if new temp call is
// received less than 10s from the last one - prevents system interruo\pting moves and causing focuser
// halt as temp conversion takes 750-1000ms
// Issue with FocusMax (now fixed), reported by Tomas Wikander (thanks)

// v1.43_XXX 20062015
// Push buttons got broken from v1.34, now fixed, reported by Michael Glibstrup (thanks)

// v1.42_XXX 10062015
// Added protocol commands to disable/enable updating of lcd when motor is moving
// :SU0#     Set updatefposlcd() off, lcd does not update when motor moving, enables faster motor speed
// :SU1#     Set updatefposlcd() on, lcd updates when motor moving

// v1.41_XXX 28052015
// Minor fixes
//
// v1.40_XXX 25052015
// On a :FQ (Halt), will write position to EEPROM after delay (in case user turns off focuser later on without any further moves)
// On powerup, both LED IN/OUT now light up with Buzzer On to indicate boot up cycle
//
// v139_XXX 08052015
// Added serial commands :DS0# :DS1# and :DG# to enable and disable display
//
// v1.38_XXX 27032015
// added lcd display update of current position and target position whilst moving updatefposlcd()
// added constant value LCDUPDATERATE for handling update (flicker) of LCD when not moving
// changed command :XY# to dump additional parameters
// changed motorSpeed values to get better differences between slow, med and fast
// changes to processcmd() SN, SP, PH, FQ, FG
//
// v1.35_XXX 22032015
// Halfsteps, Reverse Direction,  and CoilPwr now saved in EEPROM to allow for manual operation
// Changes required to startup and also processcmd (to ensure eeprom write)
// Changes to displaylcd() re stepmode
//
// v1.34_XXX 16032015
// Test changes for INDI moonlite focuser
// Added gotonewposition variable, used with SN followed by FG (Moonlite)
// Problem is that SN is causing focuser to immediately move, its not waiting for FG
// now FG sets this flag to true, and when move is complete it is reset to false
// the moving condition in the main loop now also uses this flag as a condition to move
// Added GC for moonlite
// Changed maxFocuserLimit to 60000 for be compatible with moonlite
//
// v1.33_XXX 20022015
// changed Maxsteps and FocuserPosition to type long - On ,net and c# integers are 32 bit so support
// values greater than 32767. Changes maxFocuserLimit to 64000
//
// v1.32_XXX 05022015
// add stepdelay variable for moonlite protocol support
//
// v1.31_XXX 03022015
// changes to processcmd(), refined :POXX# to do temperature offset calibration for Moonlite protocol
// moved :GB to top of command processing to respond quicker
// added float tempoffsetval
//
// v1.30_XXX 30012014
// Changes for Coil Power, using /ENABLE to turn off power after a move
// Requires /ENABLE of DRV8825 or EASYDRIVER to be wired to D8
// Changed temperature precision of probe
//
// v1.29_XXX 03012014
// code change to ensure currentaddr does not end up with invalid value if not initialized
//
// v1.28_XXX 30122014
// Minor changes to motorSpeed and stepontime values so that 1/4 and 1/8 stepping works better
// added protocol to set motorSpeed via serial command
// fixed bug in ReverseDirection
//
// v1.27_XXX 18122014
// Minor bug fix to analogWrite() statements
//
// v1.26_XXX 13122014
// In GF, Get Firmware version, return program name and version number (ASCOM uses GV)
//
// v1.25_XXX 10122014
// Modified driver code to write values to EEPROM on SP and SM commands
// At present code only updates 30s after a move. Using the Set() commands for
// position and maxStep does not initiate a move so these would not be saved
// unless there was a move() command that followed before shutdown/power-off
// Doubtful whether user would ever actually alter these parameters like this
// scenario describes, however included just in case
//
// v1.24_XXX 01122014
// Copied from DRV8825 Minimal
// supports push buttons, LCD and temperature probe
// To work with ONE common Windows APP v1.25 and above
//
// based on some portions of original code by Dave Wells, and orly.andico@gmail.com (moonlite focuser example)
// heavily modified by RBB to work with myFocuser