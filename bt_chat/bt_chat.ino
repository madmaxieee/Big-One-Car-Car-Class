#include <SoftwareSerial.h>

SoftwareSerial BT(8, 9);
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
        BT.write(text);
    }

    if (BT.available())
    {
        Serial.print(char(BT.read()));
    }
}
