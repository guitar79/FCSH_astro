//1초단위 (0~100)

int LED = 13;
int value = 0;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);

  pinMode(20, OUTPUT);
  pinMode(22, OUTPUT);
  digitalWrite(20,HIGH);
  digitalWrite(22,HIGH);
  
}

// the loop function runs over and over again forever
void loop() {
  PWM();
}

void PWM()
{
  digitalWrite(LED,HIGH);
  delay(5*value);
  digitalWrite(LED,LOW);
  delay(500-5*value);
  
  Serial.print("value : ");
  Serial.println(value);
}

void serialCommand(String commandString)
{
  if(value<=100&&value>=0) value = commandString.toInt();
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
