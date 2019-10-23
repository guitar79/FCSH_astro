myFocuserPro ASCOM Driver ChangeLog

// (c) Copyright Robert Brown 2014-2018. All Rights Reserved.
// The schematic, code and ideas for myFocuser are released into the public domain. Users are free to implement
// these but may NOT sell this project or projects based on this project for commercial gain without express 
// written permission granted from the author. Schematics, Code, Firmware, Ideas, Applications, Layout are 
// protected by Copyright Law.
// Permission is NOT granted to any person to redistribute, market, manufacture or sell for commercial gain 
// the myFocuserPro products, ideas, circuits, builds, variations and units described or discussed herein or 
// on this site.
// Permission is granted for personal and Academic/Educational use only.

// 14102018 1.6.0.8
// Adjust throttling

// 15012018 1.6.0.7
// Major rewrite of interface
// Major rewrite of driver comms using mutex semaphore and handling of client requests
// Fix bug in temperature precision list box selection
// Fix bug in setting receive timeout interval
// Set windows locale to US-Eng

// 10072017 RBB 1.6.0.6
// Major rewrite of code for stepsize and other settings on Settings form
// Major rewrite of code for temp offset on main form
// Validate tempoffset and stepsize on application start

// 05042017 RBB 1.6.0.5
// issue of sometimes lcd does not update after a reconnect

// 28092016 RBB 1.6.0.4
// Fix for stepsize values between 0 and 1

// 11092016 RBB 1.6.0.3
// Fix issue with APT
// Fix issue with Moonlite ASCOM driver
// Add listbox to setupdialog form, to specify serialport readtimeout value

// 05082016 RBB 1.6.0.2
// Fix issue with DelayOnConnect not being remembered correctly
// Fix issue with ascom driver timeout
// Add Get Version on connect and write to logfile

// 06062016 RBB 1.6.0.1 
// Rearrange layout on setupdialog form
// Add Send MotorSpeedChange (msc) values on Connect checkbox
// Fix error on not sending disable of motorspeedchange (was always enabled)
// Fix error on not sending stepsize on connect

// 16052016 RBB 1.6.0.0 - USE FIRMWARE V160 or higher
// Support home position switch in hardware
// Set LCD update when moving
// Set temperature resolution (9-12)
// Set Motorspeed change and threshold setting
// Set motorspeedchange value (threshold) when moving - switches to slowspeed when nearing destination
// Set Enable/Disable motorspeedchange when moving
// Set Step size and Step Size Enabled

// 18042016 RBB 1.5.3.9
// Changes to temperature code, tempwaitdelay no longer used

// 12042016 RBB 1.5.3.8
// Added ability to change com port speed

// 04-Feb-2016 RBB 1.5.3.7
// Added Celsius/Fahrenheit checkbox to setup dialogform, checked = celsius, unchecked = fahrenheit, this is sent to
// controller on Connect, status is remembered between sessions

// 31-Jan-2016 RBB 1.5.3.6
// Changes for Win10 compatibility

// 26-Jan-2016 RBB 1.5.3.5
// added capability to set the length of time that each lcd screen is displayed for, new menu setting added

// 04-01-2016 1.5.3.4
// Cleaned up code in SetupDialogForm

// 30-12-2015 1.5.3.3
// All properties now use the ASCOM profile store, and are viewable with the ASCOM Profiler
// Driver must be run at least once for the settings to appear in the profile store
// DriverInfo and DriverVersion now return correct values

// 11-12-2015 1.5.3.2
// Form Location and Size is remembered. Use shift key to override when loading or closing

// 07-12-2015 1.5.3.1
// Cleaned up exception errors and version info

// 27-Nov-2015 1.5.2
// Changed init code for serial driver

// 29-Oct-2015 1.5.1 
// Support for MaxIncrement added

// 23-Oct-2015 1.5.0
// added changes to support RAPS128 board, stepmode to 1/128
// MaxSteps now 65000

// 02-Sep-2015 RBB 1.4.9
// Added DelayTempCalls on SetupDialogForm and validation on CmdOK
// Added validation in SetupDialogForm CmdCancel
// Implemented LastTempReading in GetTemp() - helps reduce serial traffic - WBPFM
// Obeys DelayTempCalls waiting time 

// 30-Aug-2015 RBB 1.4.8
// Recoded GetTemp to implement a delay of 5s between requests, if sooner then return prev value

// 29-Aug-2015 RBB 1.4.7
// Recoded some of the serial routines to provide better exception handling

// 13-Jul-2015 RBB 1.4.6
// GetTemp can only be called if focuser is not moving, if moving returns last temp reading 
// (if not set then returns -20.0)

// 09-Jul-2015 RBB 1.4.5
// added ICO plus link to sourceforge site

// 16-Jun-2015 RBB 1.4.4
// added ability to retain temperature across calls in case of fallback
// added protection around conversions and data parsing
// if calls/conversions fail, ASCOM driver will return previous value existing prior to call
// Added delay (s) on connect to SetupDialog Form
// fixed bug in Update - was not updating MaxSteps and FocuserPos to the controller correctly

// 11-Jun-2015 RBB 1.4.3
// Changes to setup dialog temp offset validation and parsing
 
// 10-Jun-2015 RBB 1.4.2
// Added Checkbox on dialogform to be able to disable updatefposlcd() - turn off lcd update while moving - default is ON
//
// 27-Apr-2015 RBB 1.4.1
// changed :gz temperature cpnversion to be be windows regional setting aware
//
// 16-Mar-2015 RBB 1.4.0
// changed maxSteps to 60000 to match INDI moonlite driver
//
// 19-Feb-2015 RBB 1.3.9
// added update feature to dialog box
// changed maxfocuserlimit to 64000 to match controller firmware settings
//
// 01-Feb-2015 RBB 1.3.8
// Added validation of numeric entry into textboxes for Focuser Position and MaxSteps
//
// 01-Feb-2015 RBB 1.3.7
// changed Temp offset from list box to a text entry box, this means can use values like 1.5
// changed to float, bounds checked at < -3.1 and +3.0
//
// 30-Jan-2015 RBB 1.3.6
// Added temp offsetval to calibrate temp probe value
//
// 28-Jan-2015 RBB 1.3.5
// SetupDialog Form - Added Properties.Settings.Default.ComPortString to remember comport name
// linked to variable ComPortName
//
// 09-Jan-2015 RBB 1.3.4
// Used combobox for comport and saved in application.default.settings.mycomport
//
// 08-Jan-2015 RBB 1.3.3
// Added speed setting as trackbar, linked to Application.Default.Settings
//
// 07-Jan-2015 RBB 1.3.2
// Increased wait time to 2s from 500ms when connecting
//
// 02-Jan-2015 RBB 1.3.1
// Was returning Temperature as Hex value, changed to Decimal value now
//
// 01-Dec-2014 RBB
// Now supports all versions
//
// 24-Nov-2014 RBB
// changed to use stored controller values in EEPROM, minor changes in connected()
// ASCOM does not support get direction or get half steps etc
//
// 05-Nov-2014	RBB	1.0.0	Initial edit, created from ASCOM driver template
