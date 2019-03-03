// -------------------------------------------------------------------------------------------------
// Pin map for FCS_Teensy3.2

#if defined(__MK20DX256__)

// DHT22 pins
#define DHTPIN 2 // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302)

// SVO pins (Cover Open/Close)
#define COVER1 0 // 
#define COVER1 1 // 

// BUTTON pins (User interface)
#define uiKeyPrev 5
#define uiKeyNext 6
#define uiKeySelect 8
#define uiKeyBack 7

// OLED pins (User interface)
#define SDA 25
#define SCL 26

// MOTOR DRV pins (DRV8825)
#define Axis1DirPin  4    // Pin 21 (Dir)
#define Axis1StepPin  4    // Pin 20 (Step)
#define Axis1_FAULT   14  // Pin 18 (Fault)
//#define Axis1_Aux    
#define Axis1_M2      17    // Pin 17 (Microstep Mode 2 or SPI CS)
#define Axis1_M1      16    // Pin 16 (Microstep Mode 1 or SPI SCK)
#define Axis1_M0      15    // Pin 15 (Microstep Mode 0 or SPI MOSI)
#define Axis1_EN      14    // Pin 14 (Enable)

// OLED pins
//#define OLED 4

#else
#error "Wrong processor for this configuration!"






#endif
