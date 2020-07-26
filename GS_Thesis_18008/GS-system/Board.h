
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
#define DIR 0
#define STEP 13
#define MS0 15
#define MS1 16
#define MS2 17
#define MOTOR_STEPS 200
#define DHT22_PIN 2
#define UPpin 12
#define DOWNpin 9
#define RIGHTpin 10
#define LEFTpin 11
#define ServoPin1 3
#define ServoPin2 5
#define LED 13
#define PWMPin1 20
#define PWMPin2 22
#define MAX_POSITION 65535

//need to set

#define RelayPin1 7
#define RelayPin2 21
#define RelayPin3 8
#define RelayPin4 23

#endif

#endif
