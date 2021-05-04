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
  Serial.begin(19200);
  // reset pin is set to 20, to avoid overwriting
  // we need to give this parameter however the pin is useless(for what we know)
  mfrc522 = MFRC522(SS_PIN, 20);
  SPI.begin();
  mfrc522.PCD_Init();
  BT.begin(19200);
  Serial.println("Done setup");
}

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

// parameter: 0 or 1 or 2, reprenting left, right, back respectively
// return: parameter
bool drive(BT_CMD direction)
{
  static bool stop = false;
//        stop = false;
//      motorWrite(-255, -255);
//      delay(50);
//      motorWrite(-180, 255);
//      delay(350);
//      motorWrite(200, 200);
  if(direction==LEFT)
  {
    stop = false;
      motorWrite(-255, -255);
      delay(50);
      motorWrite(-180, 255);
      delay(370);
      motorWrite(150, 150);
      //Serial.println("LEFT");
  }
  else if(direction==RIGHT)
  {
      stop = false;
      motorWrite(-255, -255);
      delay(50);
      motorWrite(255, -180);
      delay(450);
      motorWrite(200, 200);
      //Serial.println("RIGHT");
  }
  else if(direction==BACK)
  {
      stop = false;
      motorWrite(-255, -255);
      delay(50);
      motorWrite(-255, 255);
      delay(600);
      motorWrite(200, 200);
      //Serial.println("BACK");
  }
  else if(direction==STOP)
  {
      if (!stop)
      {
        motorWrite(-255, -255);
        delay(20);
      }
      motorWrite(0, 0);
      stop = true;
      //Serial.println("STOP");
  }
  else if(direction==FORWARD)
  {
      stop = false;
      motorWrite(150, 150);
      //Serial.println("FORWARD");
      return false;
  }
  else if(direction==START)
  {
      motorWrite(200, 200);
      //Serial.println("START");
  }
  else if(direction==DAOCHE)
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

void tracking()
{
  static int error;

  int current_error = R1 * 0.90 + R2 * 0.45 \
                      - L2 * 0.45 - L1 * 0.90;
  int d_error = current_error - error;
  // Serial.println(d_error);
  int left = 100 + 0.4 * error + 0.3 * d_error;
  int right = 100 - 0.5 * error - 0.3 * d_error;
  if (M + R2 + L2 >= 900)
    left = right = 100;
  error = current_error;
  //Serial.println(error);
  motorWrite(left, right);
}

// if out of node return 0
// if enter node return 1
// if in node return 2
int checkNode()
{
  static const int threshold = 1000;
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
  if (temp != sum >= threshold)
    return 1;
  if (pFlag)
    return 2;
}

void loop()
{
  static int time = 0;
  UID = rfid(idSize);

  //update sensor values
  // R1 = analogRead(IR0) * 0.7;
  // R2 = analogRead(IR1) * 0.5;
  // M = analogRead(IR2);
  // L2 = analogRead(IR3) * 0.6;
  // L1 = analogRead(IR4) * 0.8;
  R1 = analogRead(IR0);
  R2 = analogRead(IR1) * 0.7;
  M  = analogRead(IR2);
  L2 = analogRead(IR3);
  L1 = analogRead(IR4) * 0.72;

  BT_CMD msg;
  static bool start_flag = true;
  if (!start_flag)
  {
    msg = ask_BT();
    if (msg == START)
    {
      drive(msg);
      start_flag = true;
      msg = ask_BT();
    }
  }
  else
  {
    if (checkNode() == 1 || UID != 0)
    {
      drive(RIGHT);
      // send_msg('p');
      // drive(msg);
      // msg = ask_BT();
    }
    else if(checkNode() == 0)
    {
      tracking();
    }

    // UID is the return value of rfid()
    // 0 if nothing detected (won't send anything)
    send_byte(UID, idSize);
  }
}
