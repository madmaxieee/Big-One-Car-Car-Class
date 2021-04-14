#include <SoftwareSerial.h>

SoftwareSerial BT(11, 10);
char text;

void setup()
{
    Serial.begin(9600);
    BT.begin(9600);
}
void loop()
{
    if(Serial.available()){
        text = Serial.read();

    }

    if (BT.available())
    {
        Serial.print(char(BT.read()));
    }
}
