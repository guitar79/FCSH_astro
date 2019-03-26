/*
 * === USE AT YOUR OWN RISK ===
 * For more information on setting OnStep up see:
 *   http://www.stellarjourney.com/index.php?r=site/equipment_onstep and 
 * join the OnStep Groups.io at
 *   https://groups.io/g/onstep
 * 
 * See the corresponding Pins.xxx.h file for detailed information on this pin map
 * to be sure it matches your wiring
 *
*/

// === CONFIGURATION START

#define FCS_GSnano_OFF
#ifdef  FCS_GSnano_ON
// -------------------------------------------------------------------------------------------------
// Pin map for FCS_GSnano

// DHT22 pins
#define DHTPIN 2 // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302)

// SVO pins (Cover Open/Close)
#define COVER1 0 // 
#define COVER1 1 // 

// BUTTON pins (User interface)
#define UPpin 8
#define DOWNpin 9
#define RIGHTpin 10
#define LEFTpin 7

// OLED pins (User interface)
#define OLED_RESET 4
#define SDA 25
#define SCL 26

// MOTOR DRV pins (DRV8825)
#define DIR 4
#define STEP 5 
#define MS0 8 
#define MS1 7 
#define MS2 6
#define MOTOR_STEPS 200

// OLED pins
//#define OLED 4

// RELAY pins
#define relayPin1 1
#define relayPin2 2
#define relayPin3 3
#define relayPin4 4

#endif
