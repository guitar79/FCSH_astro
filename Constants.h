// -----------------------------------------------------------------------------------

//Constants

#if boardtype==0
  
  // DHT22 pins
  #define DHTPIN 2 // what pin we're connected to
  #define DHTTYPE DHT22 // DHT 22 (AM2302)
  
  // button pins
  #define btn0 7 
  #define btn1 8 
  #define btn2 9 
  #define btn3 10
  //#define ledPin 13  // LED를 d13핀으로 설정한다.
  
  // OLED pins
  #define OLED 4
  
  // Stepper Dirver (DRV8825) pins
  #define DIR 4
  #define STEP 3
  #define MS0 A1
  #define MS1 12
  #define MS2 13

#elif boardtype==1
  // 여기에 추가
#endif
// 버튼 정의
#define KEY_NONE 0
#define KEY_PREV 2
#define KEY_NEXT 3
#define KEY_SELECT 4
#define KEY_BACK 1

// Stepper Driver Models
//#define A4988    1
//#define DRV8825  2

// Stepp motor Models
#define MOTOR_STEPS 200

// Length of each stepper's table
//#define LEN_A4988   5
//#define LEN_DRV8825 6
   
// The various microsteps for different driver models, with the bit modes for each
//unsigned int StepsA4988  [LEN_A4988]  [2] = { {1,0}, {2,1}, {4,2}, {8,3}, {16,7} };
//unsigned int StepsDRV8825[LEN_DRV8825][2] = { {1,0}, {2,1}, {4,2}, {8,3}, {16,4}, {32,5} };


