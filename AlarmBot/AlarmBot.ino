int buzzer = 5;
int LED = 13;
#include <CuteBuzzerSounds.h>

byte alarmOn = false;
char c=' ';
boolean NL = true;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);
//Create second to achieve multiple comms
//SoftwareSerial mySerial2(4, 5);


void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(LED, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  cute.init(buzzer);
  cute.play(S_CONNECTION);
}

void loop() {
  // put your main code here, to run repeatedly:

  char transmission = ' ';

//Ideally (if there was 2 HM-10's you would read from mySerial2 (slave)
//and output in mySerial (master)
  
  if (mySerial.available()) {
    transmission = mySerial.read();
    delay(100);
    Serial.print("Received Transmission: ");
    Serial.println(transmission);
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


  if (alarmOn) {
    alarm();
    Serial.println("Alarm active");
  } if (!alarmOn) {
    alarmOff();
  }

  if (transmission == 'I') {
    if (!alarmOn) {
      alarmOn = true;
    } else if (alarmOn) {
      alarmOn = false;
      Serial.println("Alarm Disabled");
      alarmOff();
    }
  }

 

}

void alarm() {
  Serial.println("Alarm Triggered");
  tone(buzzer, 1000); // Send 1KHz sound signal...
  digitalWrite(LED, HIGH);
  delay(500);        // ...for 0.5 sec
  tone(buzzer, 750); // Send 0.5KHz sound signal..
  digitalWrite(LED, LOW);
  delay(500);        // ...for 0.5 sec

}
void alarmOff() {

  noTone(buzzer);

}
