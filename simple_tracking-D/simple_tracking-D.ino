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

int R1 = 0;
int R2 = 0;
int M = 0;
int L2 = 0;
int L1 = 0;

void motorWrite(float Vl, float Vr)
{
  if (Vl > 255)
    Vl = 255;
  if (Vr > 255)
    Vr = 255;
  if (Vl < -255)
    Vl = -255;
  if (Vr < -255)
    Vr = -255;
  analogWrite(ENA, Vl > 0 ? Vl : -Vl);
  analogWrite(ENB, Vr > 0 ? Vr : -Vr);
  digitalWrite(IN1, Vl > 0 ? 1 : 0);
  digitalWrite(IN3, Vr > 0 ? 1 : 0);
  digitalWrite(IN2, Vl > 0 ? 0 : 1);
  digitalWrite(IN4, Vr > 0 ? 0 : 1);
}

void tracking()
{
  static int error;
  int R1 = analogRead(IR0);
  int R2 = analogRead(IR1)*0.7;
  int M = analogRead(IR2);
  int L2 = analogRead(IR3);
  int L1 = analogRead(IR4)*0.72;
//  Serial.println(R1);
//  Serial.println(R2);
//  Serial.println(M);
//  Serial.println(L2);
//  Serial.println(L1);
//  Serial.println();
//  delay(300);
  int current_error = R1 * 0.90 + R2 * 0.45 - L2 * 0.45 - L1 * 0.90;
  int d_error = current_error - error;
  // Serial.println(d_error);
  int left = 100 + 0.4 * error + 0.3 * d_error;
  int right = 100 - 0.5 * error - 0.3 * d_error;
  error = current_error;
  //Serial.println(error);
  motorWrite(left, right);
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
  R1 = analogRead(IR0) * 0.9;
  R2 = analogRead(IR1) * 0.5;
  M = analogRead(IR2);
  L2 = analogRead(IR3) * 0.6;
  L1 = analogRead(IR4) * 0.7;

  // Serial.println(R1);
  // Serial.println(R2);
  // Serial.println(M);
  // Serial.println(L2);
  // Serial.println(L1);
  // Serial.println();
  // delay(1000);

  tracking();
}
