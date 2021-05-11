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

void motorWrite(float Vl, float Vr);
void tracking();
void trackFor(int time = 1000);
void updateIR();
bool drive(BT_CMD direction);
int checkNode();

BT_CMD *dir;

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

    char c = getChar();

    // wait for start
    while (c != 's')
    {
        c = getChar();
    }

    //get length
    int digit = 0;
    int len = 1;
    c = getChar();

    while (48 <= int(c) && int(c) <= 57)
    {
        len += round((int(c) - 48) * pow(10, digit));
        digit++;
        c = getChar();
    }

    dir = new BT_CMD[len];

    // parse first direction
    switch (c)
    {
    case 'r':
        dir[0] = RIGHT;
        break;
    case 'l':
        dir[0] = LEFT;
        break;
    case 'b':
        dir[0] = BACK;
        break;
    case 's':
        dir[0] = START;
        break;
    case 'f':
        dir[0] = FORWARD;
        break;
    case 'd':
        dir[0] = DAOCHE;
        break;
    }

    // get other directions
    for (int i = 1; i < len - 1; i++)
    {
        dir[i] = ask_BT();
    }
    dir[len - 1] = BACK;
}

void loop()
{
    static int i = 0;
    // static BT_CMD dir[] = {RIGHT, RIGHT, BACK, RIGHT, RIGHT};
    // static BT_CMD dir[] = {FORWARD, FORWARD, FORWARD, DAOCHE,
    //                        FORWARD, FORWARD, RIGHT,   LEFT,
    //                        RIGHT,   BACK,    FORWARD};
    BT_CMD msg;
    updateIR();

    if (RFID_flag)
    {
        UID = rfid(idSize);
        if (UID != 0)
        {
            send_byte(UID, idSize);
            RFID_flag = false;
        }
    }

    if (checkNode() == 1)
    {
        drive(dir[i]);
        i++;
    }
    else if (checkNode() == 0)
    {
        tracking();
    }
    else if (checkNode() == 2)
    {
        motorWrite(140, 140);
    }
}

void motorWrite(float Vl, float Vr)
{
    static int ticks = 0;
    Vl *= 1.5;
    Vr *= 1.5;
    if (Vl > 255)
        Vl = 255;
    if (Vr > 255)
        Vr = 255;
    if (Vl < -255)
        Vl = -255;
    if (Vr < -255)
        Vr = -255;
    Vl = round(0.8 * Vl);
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

void tracking()
{
    static int error;
    // BT.write('T');
    updateIR();
    int current_error = R1 * 0.90 + R2 * 0.45 - L2 * 0.45 - L1 * 0.90;
    int d_error = (current_error - error) * (current_error > 0 ? -1 : 1);
    // Serial.println(d_error);
    int left = 0.4 * current_error + 10 * d_error;
    int right = -0.5 * current_error - 10 * d_error;
    if (M + R2 + L2 >= 900)
    {
        left = 0;
        right = 0;
    }
    error = current_error;

    //Serial.println(error);
    if (R1 < 50 && R2 < 50 && M < 50 && L1 < 50 && L2 < 50)
    {
        left = -140;
        right = -140;
        return;
    }

    if (error > 0)
    {
        left = 140 + left;
        right = 140 + right;
    }
    else
    {
        left = 140 + left * 0.8;
        right = 140 + right * 0.8;
    }
    motorWrite(left, right);
}

void trackFor(int time = 1000)
{
    clock = millis();
    while (millis() - clock < time)
    {
        tracking();
    }
}

// parameter: 0 or 1 or 2, reprenting left, right, back respectively
// return: parameter
bool drive(BT_CMD direction)
{
    static bool stop = false;

    if (direction == LEFT)
    {
        stop = false;
        delay(400);
        drive(STOP);
        delay(200);
        motorWrite(-180, 255);
        delay(300);
        // motorWrite(150, 150);
        BT.write('L');
    }
    else if (direction == RIGHT)
    {
        stop = false;
        delay(400);
        drive(STOP);
        delay(200);
        motorWrite(255, -180);
        delay(400);
        // motorWrite(150, 150);
        // Serial.println("RIGHT");
        BT.write('R');
    }
    else if (direction == BACK)
    {
        stop = false;
        delay(100);
        drive(STOP);
        delay(200);
        motorWrite(-200, 200);
        delay(400);
        //Serial.println("BACK");
        RFID_flag = true;
        BT.write('B');
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
        BT.write('F');
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
        delay(500);
        drive(STOP);
        delay(500);
        motorWrite(-180, 255);
        delay(350);
        //track for 0.8 s
        clock = millis();
        while (millis() - clock < 800)
        {
            tracking();
        }
        //backup
        motorWrite(-140, -140);
        //wait for rfid
        while ((UID = rfid(idSize)) == 0)
        {
        }
        send_byte(UID, idSize);
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

// if out of node return 0
// if enter node return 1
// if in node return 2
int checkNode()
{
    static int last_node_time = 0;
    static const int threshold = 2000;
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
