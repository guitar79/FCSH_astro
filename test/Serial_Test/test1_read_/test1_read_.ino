//2019.05.02

  #define c 3
  char A;
  char lastA;

void setup() {
  Serial.begin(9600);
  Serial.println("testing...");
}

void loop() {
  
  A = Serial.read();

  switch(c) {
    
  case 1:
    if(lastA != A) Serial.println(A);
    break;
    
  case 2:
    if(lastA != A) Serial.print(A);
    break;
    
  case 3:
    if (Serial.available() > 0) {
                // 읽어온 것을 변수에 저장
                A = Serial.read();

                // 읽어온 것을 다시 시리얼을 통해 송신
                Serial.println(A);
                
        }
        //https://m.blog.naver.com/PostView.nhn?blogId=jamduino&logNo=220826763412&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F
  }
  
  lastA = A;
  
}

//RESULT1
//INPUT : ABC
//OUTPUT : A()B()C () = ENTER

//RESULT2
//INPUT : ABC
//OUTPUT : A B C

//RESULT
//Serial.available()은 수신이 있으면 버퍼에 저장되어있는 값을 반환한다.
//즉, 이 값이 0이 아니라면 수신된(받은) 값이 존재한다는 뜻이다.
