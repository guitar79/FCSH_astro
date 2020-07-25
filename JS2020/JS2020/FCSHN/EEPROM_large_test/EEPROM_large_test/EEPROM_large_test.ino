#include <EEPROM.h> //부호고려

int value = 0;    //EEPROM value

void setup(){
  delay(1000);
  Serial.begin(115200);
  eepRead();
  Serial.println(value);
  delay(3000);
}

void loop(){
  eepWrite(value);
}

void eepRead()
{
 int N = EEPROM.read(0); // 0은 자릿수 저장
 int dir = EEPROM.read(1); // 1은 부호저장 (0이상이면 1, 아니면 0)
 int v = 0;

 if(N==0) v = EEPROM.read(2);
 else v = EEPROM.read(2)*256+EEPROM.read(3);
 if(!dir) v *= (-1); //dir이 0이면 음수이므로 -1을 곱한다
 
 value = v;
 
  Serial.print("ROM value : ");
  Serial.println(v);
}

void eepWrite(int value_)
{
  int v = value_;
    if(v>=0) EEPROM.write(1,1);
    else EEPROM.write(1,0);

    if(v>255) 
    {
      EEPROM.write(0,1);
      EEPROM.write(2,v/256);
      EEPROM.write(3,v%256);
    }
    else 
    {
      EEPROM.write(0,0);
      EEPROM.write(2,v);
      EEPROM.write(3,0);
    }
  
}

void serialCommand(String commandString)
{
  value = commandString.toInt();
  eepWrite(value);
  eepRead();
  Serial.print(EEPROM.read(0));
  Serial.print(" ");
  Serial.print(EEPROM.read(1));
  Serial.print(" ");
  Serial.print(EEPROM.read(2));
  Serial.print(" ");
  Serial.println(EEPROM.read(3));
  delay(3000);
}

String inputString;
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      serialCommand(inputString);
      inputString = "";
    }
  }
}
