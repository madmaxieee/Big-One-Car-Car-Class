#define IR0 A0
#define IR1 A4
#define IR2 A1
#define IR3 A3
#define IR4 A2

#define ENA 5
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 7
#define ENB 6

// 腳位設定 (資料型別設定)

void motorWrite(float Vl, float Vr){
    if(Vl>255)
      Vl=255;
    if(Vr>255)
      Vr=255;
    if(Vl<-255)
      Vl=-255;
    if(Vr<-255)
      Vr=-255;
    analogWrite(ENA, Vl > 0 ? Vl : -Vl);
    analogWrite(ENB, Vr > 0 ? Vr : -Vr);
    digitalWrite(IN1, Vl > 0 ? 1 : 0);
    digitalWrite(IN3, Vr > 0 ? 1 : 0);
    digitalWrite(IN2, Vl > 0 ? 0 : 1);
    digitalWrite(IN4, Vr > 0 ? 0 : 1);
}

void tracking(){
    int R1=analogRead(IR0);
    int R2=analogRead(IR1);
    int M=analogRead(IR2);
    int L2=analogRead(IR3);
    int L1=analogRead(IR4);
    int error=R1*0.7+R2*0.5-L2*0.6-L1*0.8;
    
    int left=140+0.3*error;
    int right=140-0.5*error;
    motorWrite(left, right);
    Serial.println(error);
    Serial.println(left);
    Serial.println(right);
    Serial.println();
    //delay(1000);
    
}
void setup()
{
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);

    pinMode(IR0, INPUT);
    pinMode(IR1, INPUT);
    pinMode(IR2, INPUT);
    pinMode(IR3, INPUT);
    pinMode(IR4, INPUT);

    Serial.begin(9600);
}

void loop()
{
     tracking();
    // 10.7 volt
//    Serial.println(analogRead(IR0));
//    Serial.println(analogRead(IR1));
//    Serial.println(analogRead(IR2));
//    Serial.println(analogRead(IR3));
//    Serial.println(analogRead(IR4));
//    Serial.println();
//    delay(1000);
    // analogWrite(ENA, 255);
    // analogWrite(ENB, 255);
    // motorWrite(-255, 255);
    // delay(1000);
    // motorWrite(255, -255);
    // delay(1000);
    // delay(1000); //轉五秒
    // digitalWrite(IN1, LOW);
    // digitalWrite(IN3, LOW);
    // digitalWrite(IN2, HIGH);
    // digitalWrite(IN4, HIGH);
    // delay(1000);
    // digitalWrite(IN1, LOW);
    // digitalWrite(IN3, HIGH);
    // digitalWrite(IN2, HIGH);
    // digitalWrite(IN4, LOW);
    // delay(1000);
    // digitalWrite(IN1, HIGH);
    // digitalWrite(IN3, LOW);
    // digitalWrite(IN2, LOW);
    // digitalWrite(IN4, HIGH);
    // delay(1000);
    // digitalWrite(IN1, LOW);
    // digitalWrite(IN3, LOW);
    // digitalWrite(IN2, LOW);
    // digitalWrite(IN4, LOW);
    // delay(1000);
}
