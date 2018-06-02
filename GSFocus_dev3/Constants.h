// -----------------------------------------------------------------------------------
// Constants
#define boardtype 1

//Constants
#define DHTPIN 2 // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302)

// Stepper Driver Models
#define A4988   1
#define DRV8825 2

// Length of each stepper's table
#define LEN_A4988   5
#define LEN_DRV8825 6
   
// The various microsteps for different driver models, with the bit modes for each
unsigned int StepsA4988  [LEN_A4988]  [2] = { {1,0}, {2,1}, {4,2}, {8,3}, {16,7} };
unsigned int StepsDRV8825[LEN_DRV8825][2] = { {1,0}, {2,1}, {4,2}, {8,3}, {16,4}, {32,5} };

// pin
#if boardtype==0
#define pinM0 11
#define pinM1 12
#define pinM2 13

#elif boardtype==1
#define pinM0 A1
#define pinM1 12
#define pinM2 13
#define pinMCUR 11
#define pinVSEN 6
#define pinCSEN_0 2
#define pinCSEN_1 3
#endif

#define pinFAULT A0 //Analog

#define pinL 0
#define pinU 1
#define pinD 2
#define pinR 3
#define pinOLED 4

