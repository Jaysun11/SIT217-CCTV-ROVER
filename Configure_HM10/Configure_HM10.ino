
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

char c=' ';
boolean NL = true;
 
void setup() 
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    mySerial.begin(9600);  
    Serial.println("BTserial started at 9600");
}
 
void loop()
{
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    if (mySerial.available())
    {
        c = mySerial.read();
        Serial.write(c);
    }
 
 
    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        c = Serial.read();
 
        // do not send line end characters to the HM-10
        if (c!=10 & c!=13 ) 
        {  
             mySerial.write(c);
        }
 
        // Echo the user input to the main window. 
        // If there is a new line print the ">" character.
        Serial.write(c);
    }
}
