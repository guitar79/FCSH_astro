#include <EEPROM.h> //부호고려
void eepRead()
{
 int N = EEPROM.read(0); // 0은 자릿수 저장
 int dir = EEPROM.read(1); // 1은 부호저장 (0이상이면 1, 아니면 0)
 int v = 0;

 if(N==0) v = EEPROM.read(2);
 else v = EEPROM.read(2)*256+EEPROM.read(3);
 if(!dir) v *= (-1); //dir이 0이면 음수이므로 -1을 곱한다
 
 EEPcurrentPosition = v;
 //Serial.println(EEPcurrentPosition);
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
