myFocuserProL Windows Application ChangeLog

// (c) Copyright Robert Brown 2014-2018. All Rights Reserved.
// The schematic, code and ideas for myFocuser are released into the public domain. Users are free to implement
// these but may NOT sell this project or projects based on this project for commercial gain without express 
// written permission granted from the author. Schematics, Code, Firmware, Ideas, Applications, Layout are 
// protected by Copyright Law.
// Permission is NOT granted to any person to redistribute, market, manufacture or sell for commercial gain 
// the myFocuserPro products, ideas, circuits, builds, variations and units described or discussed herein or 
// on this site.
// Permission is granted for personal and Academic/Educational use only.

// 1.0.3.3
// Fix bug in goto preset

// 1.0.3.2
// Fix issue with changing temperature display between C/F on the controller

// 1.0.3.1
// Send status of "update position when moving" to controller when connecting

// 1.0.3.0
// fix logging of error messages upon exception [sometimes ignored]

// 1.0.2.9 22092018
// Fix bug in display C/F
// Rewrite serial IO

// 1.0.2.8 05022018
// Use radio buttons for motor speed on gui rather than menu

// 1.0.2.7 03122017
// Rewrite code app for locale eng-us

// 1.0.2.6 10072017
// Major rewrite of code for stepsize and other settings on Settings form
// Major rewrite of code for temp offset on main form
// Validate tempoffset and stepsize on application start or form load

// 1.0.2.1 05042017
// Add option on extra settings to reset controller on connect
// Default is true. This overcomes bug where reconnecting can sometimes cause lcd to stop displaying focuser data
// Fix for log file not working correctly in sub-folders
// Change reset logfile path so that application is forced to restart

// 1.0.2.0 13032017
// Fix to status message bar 
// Fix to Extra Settings page (checks)

// 1.0.1.9 01012017
// Change to getfirmware, now on menu instead of button, also shown on GUI screeen

// 1.0.1.8 04112016
// Application correctly sets MotorSpeed and MotorSpeedChange Enable/Disable state in Controller when connecting
// Added checkbox on Extra Settings form to update Focuser Position in Application when moving (every 1s)
// MotorSpeed remembered by application and now correctly handled during changes and updates
// GUI layout change to move focuser position higher

// 1.0.1.7 18102016
// Remember form location

// 1.0.1.6 08102016
// Fix: Clearing the focuser position text box and clicking Set Position causes a crash
// Fix: Clearing the focuser position text box and clicking GOTO Position causes a crash
// Fix: Clearing MaxSteps text box and clicking Set MaxSteps causes a crash
// Redesign GUI and remove TxRx boxes and replace with StatusMsgBar

// 1.0.1.3 26/09/2016
// Fix stepsize so that values between 0 and 1 are accepted
// Add saving and restoring of LCDEnabled status so state is remembered
// Add Get Display Status to menu

// 1.0.1.2 12/09/2016
// Added Spanish
// Fixed bug in not remembering chinese and spanish and russian

// 1.0.1.1 10/09/2016
// Added Russian

// 1.0.0.9 09/09/2016
// Added Chinese simplified
// Add check on settings-language-onload to display option selected

// 1.0.0.8 09/09/2016
// Added Italian

// 1.0.0.7 08/09/2016
// Translated some of the text box messages and messagebox text

// 1.0.0.6 07/09/2016
// Added German

// 1.0.0.5 06/09/2016
// Addd French

// 1.0.0.2 05/09/2016
// Added language translation
// Translation provided by https://www.freetranslation.com
// So please, do not blame me! Send me a correction and I will update it
// GUID c2bac59e-10ee-48d9-937e-a8a61e6c2bed
// Changed program to myFocuserProL
// copied from myFocuserPro 1.07.06.05 02092016