#include "bluetooth.h"
#include "RFID.h"
#include "globals.h"

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

// for RFID
#define SS_PIN 10

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
    // reset pin is set to 20, to avoid overwriting
    // we need to give this parameter however the pin is useless(for what we know)
    mfrc522 = MFRC522(SS_PIN, 20);
    SPI.begin();
    mfrc522.PCD_Init();
    BT.begin(9600);
    Serial.println("Done setup");
}

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
    Vl = round(Vl * 0.9);
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

// parameter: 0 or 1 or 2, reprenting left, right, back respectively
// return: parameter
bool drive(BT_CMD direction)
{
    static bool stop = false;

    if (direction == LEFT)
    {
        stop = false;
        delay(200);
        drive(STOP);
        delay(500);
        motorWrite(-180, 255);
        delay(250);
        // motorWrite(150, 150);
        // BT.write('L');
    }
    else if (direction == RIGHT)
    {
        stop = false;
        delay(200);
        drive(STOP);
        delay(500);
        motorWrite(255, -180);
        delay(450);
        // motorWrite(150, 150);
        // Serial.println("RIGHT");
        // BT.write('R');
    }
    else if (direction == BACK)
    {
        stop = false;
        delay(100);
        drive(STOP);
        delay(300);
        motorWrite(-200, 200);
        delay(600);
        //Serial.println("BACK");
        // BT.write('B');
    }
    else if (direction == STOP)
    {
        if (!stop)
        {
            motorWrite(-255, -255);
            delay(50);
        }
        motorWrite(0, 0);
        stop = true;
        //Serial.println("STOP");
    }
    else if (direction == FORWARD)
    {
        stop = false;
        motorWrite(150, 150);
        delay(200);
        //Serial.println("FORWARD");
        // BT.write('F');
        return false;
    }
    else if (direction == START)
    {
        motorWrite(200, 200);
        //Serial.println("START");
    }
    else if (direction == DAOCHE)
    {
        stop = false;
        //turn left
        motorWrite(-255, -255);
        delay(50);
        motorWrite(-180, 255);
        delay(350);
        //backup
        motorWrite(-150, -150);
        //wait for rfid
        while ((UID = rfid(idSize)) == 0)
        {
        }
        motorWrite(200, 200);
        //Serial.println("DAOCHE");
    }
    return true;
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
    int d_error = current_error - error;
    // Serial.println(d_error);
    int left = 0.4 * error + 0 * d_error;
    int right = -0.5 * error - 0 * d_error;
    if (M + R2 + L2 >= 900)
        left = right = 0;
    error = current_error;

    //Serial.println(error);
    if (R1 < 200 && R2 < 200 && M < 200 && L1 < 200 && L2 < 200 && abs(error) < 100)
    {
        motorWrite(-100, -100);
        return;
    }
    if (error > 0)
        motorWrite(100 + left, 100 + right);
    else
        motorWrite(100 + left * 0.8, 100 + right * 0.8);
}

// if out of node return 0
// if enter node return 1
// if in node return 2
int checkNode()
{
    static int last_node_time = 0;
    static const int threshold = 1500;
    // static double arr[10] {};
    static double sum = 0;
    // static int ticks = 0;
    static bool pFlag = false;
    // sum -= arr[ticks % 10];
    // arr[ticks % 10] = double(R2 + R1 + M + L1 + L2) / 10;
    // sum += arr[ticks % 10];
    sum = R2 + R1 + M + L1 + L2;
    // ticks++;
    bool temp = pFlag;
    pFlag = sum >= threshold;

    if (!pFlag)
        return 0;
    if (!temp && pFlag && sVl + sVr > 30)
    {
        //last_node_time = millis();
        //Serial.println(sVl + sVr);
        return 1;
    }
    if (pFlag)
        return 2;
}

void loop()
{
    R1 = analogRead(IR0);
    R2 = analogRead(IR1) * 0.7;
    M = analogRead(IR2);
    L2 = analogRead(IR3);
    L1 = analogRead(IR4) * 0.72;
    if (checkNode() == 0)
    {
        tracking();
    }
    else if (checkNode() == 2)
    {
        motorWrite(100, 100);
    }
    return;
    static int i = 0;
    // static BT_CMD dir[] = {FORWARD, FORWARD, FORWARD, DAOCHE,
    //                        FORWARD, FORWARD, RIGHT,   LEFT,
    //                        RIGHT,   BACK,    FORWARD};
    static BT_CMD dir[] = {LEFT, FORWARD, RIGHT, RIGHT, BACK, LEFT, RIGHT};
    UID = rfid(idSize);

    static bool start_flag = false;
    BT_CMD msg;
    if (!start_flag)
    {
        msg = ask_BT();
        if (msg == START)
        {
            drive(msg);
            start_flag = true;
        }
    }
    else
    {
        updateIR();
        if (checkNode() == 1)
        {
            // drive(STOP);
            // delay(1000);
            drive(dir[i]);
            i++;
        }
        else if (checkNode() == 0)
        {
            tracking();
        }
        else if (checkNode() == 2)
        {
            motorWrite(100, 100);
        }
        //   motorWrite(200,200);

        // UID is the return value of rfid()
        // 0 if nothing detected (won't send anything)
        send_byte(UID, idSize);
    }
}
