int relayPin1 = 9;
int relayPin2 = 10;
int relayPin3 = 11;
int relayPin4 = 12;
int statusPin1 = 0;
int statusPin2 = 0;
int statusPin3 = 0;
int statusPin4 = 0;
int delayTime = 100;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);

}
void loop() 
{
while(!Serial.available()); //시리얼로 데이터 들어올 때까지 대기
char com = Serial.read(); //들어온 데이터를 변수 com에 저장
switch(com) {
   case 'q':
   digitalWrite(relayPin1, 1) ;  statusPin1 = 1;
   delay(100);
   break;
   case 'a':
   digitalWrite(relayPin1, 0) ; statusPin1 = 0;
  delay(delayTime);
   break;
   case 'w':
   digitalWrite(relayPin2, 1) ; statusPin2 = 1;
  delay(delayTime);
   break;
   case 's':
   digitalWrite(relayPin2, 0) ; statusPin2 = 0;
  delay(delayTime);
   break;
   case 'e':
   digitalWrite(relayPin3, 1) ; statusPin3 = 1;
  delay(delayTime);
   break;
   case 'd':
   digitalWrite(relayPin3, 0) ; statusPin3 = 0;
  delay(delayTime);
   break;
   case 'r':
   digitalWrite(relayPin4, 1) ; statusPin4 = 1;
  delay(delayTime);
   break;
   case 'f':
   digitalWrite(relayPin4, 0) ; statusPin4 = 0;
  delay(delayTime);
   break;
}
Serial.print("(");
Serial.print(statusPin1);
Serial.print(",");
Serial.print(statusPin2);
Serial.print(",");
Serial.print(statusPin3);
Serial.print(",");
Serial.print(statusPin4);
Serial.println(")");
Serial.print(com);
}

