// -----------------------------------------------------------------------------------
// Constants

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

