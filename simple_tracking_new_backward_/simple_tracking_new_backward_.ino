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
int aVl[50] = {};
double sVl = 0;
int aVr[50] = {};
double sVr = 0;

void motorWrite(float Vl, float Vr)
{
    static int ticks = 0;
    
    if (Vl > 255)
        Vl = 255;
    if (Vr > 255)
        Vr = 255;
    if (Vl < -255)
        Vl = -255;
    if (Vr < -255)
        Vr = -255;
    sVl -= aVl[ticks];
    sVr -= aVr[ticks];
    aVl[ticks] = double(Vl) / 50;
    aVr[ticks] = double(Vr) / 50;
    sVl += aVl[ticks];
    sVr += aVr[ticks];
    ticks++;
    ticks %= 50;
    analogWrite(ENA, Vl > 0 ? Vl : -Vl);
    analogWrite(ENB, Vr > 0 ? Vr : -Vr);
    digitalWrite(IN1, Vl > 0 ? 1 : 0);
    digitalWrite(IN3, Vr > 0 ? 1 : 0);
    digitalWrite(IN2, Vl > 0 ? 0 : 1);
    digitalWrite(IN4, Vr > 0 ? 0 : 1);
}

void updateIR()
{
    R1 = analogRead(IR0);
    R2 = analogRead(IR1) * 0.7;
    M = analogRead(IR2);
    L2 = analogRead(IR3);
    L1 = analogRead(IR4) * 0.72;
}

void tracking()
{
    static int error;
    updateIR();
    int current_error = R1 * 0.90 + R2 * 0.45 - L2 * 0.45 - L1 * 0.90;
    int d_error = (current_error - error) * (current_error > 0 ? -1 : 1);
    // Serial.println(d_error);
    int left = 0.4 * current_error + 10 * d_error;
    int right = -0.5 * current_error - 10 * d_error;
    if (M + R2 + L2 >= 900){
        left = 0;
        right = 0;
    }
    error = current_error;

    //Serial.println(error);
    if (R1 < 200 && R2 < 200 && M < 200 && L1 < 200 && L2 < 200 && abs(error) < 100)
    {
        motorWrite(-100, -100);
        return;
    }
    if (error > 0)
        motorWrite(140 + left, 140 + right);
    else
        motorWrite(140 + left * 0.8, 140 + right * 0.8);
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
}
