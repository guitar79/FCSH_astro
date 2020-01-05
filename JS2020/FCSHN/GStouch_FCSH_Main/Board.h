
#ifndef Board_h
#define Board_h

//#define Teensy
#ifndef Teensy
#define Arduino_GStouch 1
#endif

#ifdef Teensy
#endif

#ifdef Arduino_GStouch
  
#define motorInterfaceType 1
#define DIR 4
#define STEP 3
#define MS0 15
#define MS1 12
#define MS2 13
#define MOTOR_STEPS 200
#define DHT22_PIN 2
#define ServoPin 3
#define PWM 20

//need to set
/*
#define RelayPin1 1
#define RelayPin2 2
#define RelayPin3 3
#define RelayPin4 4
*/
#endif

#endif
