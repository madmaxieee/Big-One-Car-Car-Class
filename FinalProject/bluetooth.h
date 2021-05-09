/***************************************************************************/
// File        [bluetooth.h]
// Author     [Erik Kuo]
// Synopsis   [Code for bluetooth communication]
// Functions  [ask_BT, send_msg, send_byte]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

/*if you have no idea how to start*/
/*check out what you have learned from week 2*/

#include <SoftwareSerial.h>

SoftwareSerial BT(8, 9);

enum BT_CMD
{
    NOTHING,
    LEFT,
    RIGHT,
    BACK,
    STOP,
    FORWARD,
    START,
    DAOCHE
};

char getChar(){
    while (!BT.available())
    {
    }
    char c;
    return BT.read();
}

BT_CMD ask_BT()
{
    BT_CMD message = NOTHING;
    char cmd;
    while (!BT.available())
    {
    }
    cmd = BT.read();
    // delay(1);
    #ifdef DEBUG
    Serial.print("cmd : ");
    Serial.println(cmd);
    #endif
//    BT.write(cmd);
    switch (cmd)
    {
    case 'r':
        return RIGHT;
    case 'l':
        return LEFT;
    case 'b':
        return BACK;
    //case 's':
    //return STOP;
    case 's':
        //Serial.println(cmd);
        return START;
    case 'f':
        //Serial.println(cmd);
        return FORWARD;
    case 'd':
        return DAOCHE;
    }
    return message;
} // ask_BT

void send_msg(char c)
{
    BT.write(c);
} // send_msg

// send UID back through SoftwareSerial object: BT
void send_byte(byte *id, byte &idSize)
{
    if (!id)
        return;
    //indicates the start of communication
    send_msg('i');
    for (byte i = 0; i < idSize; i++)
    { // Send UID consequently.
        BT.write(id[i]);
    }
#ifdef DEBUG
    Serial.print("Sent id: ");
    for (byte i = 0; i < idSize; i++)
    { // Show UID consequently.
        Serial.print(id[i], HEX);
    }
    Serial.println();
#endif
} // send_byte
