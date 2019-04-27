const int buttonPin0 = 10; // 스위치를 D2 핀으로 설정한다.
const int ledPin0 = 13; // LED를 D11핀으로 설정한다.
int button0State = 0; // 스위치 값을 받아 변수에 저장한다.

void setup() {
pinMode(ledPin0, OUTPUT); // LED(D11)를 OUTPUT으로 설정한다.
pinMode(buttonPin0, INPUT_PULLUP); // 스위치(D2)를 INPUT으로 설정한다.
}

void loop() {
  button0State = digitalRead(buttonPin0); // 스위치의 값을 읽어서 buttonState 변수에 저장한다.
if (button0State == LOW) { // 만약, 스위치의 값이 1(5V)라면 !
  digitalWrite(ledPin0, HIGH); // LED는 1(5V)를 출력한다.
}
else { // 만약, 스위치 값이 1이 아니라면 !!
digitalWrite(ledPin0, LOW); // LED는 0(0V)를 출력한다.
}
}

