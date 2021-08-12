int buzzer = 5;

byte alarmOn = false;

bool testingInterDeviceConnection = false;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);


void setup() {
  pinMode(buzzer, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  String transmission;

  if (mySerial.available()) {
    
    if (!testingInterDeviceConnection){
      transmission = mySerial.read();
      delay(100);
      Serial.println(transmission);
    } else {
      transmission = Serial.Read();
      Serial.println(transmission);
    }

  }

  if (alarmOn) {
    alarm();
  } if (!alarmOn) {
    alarmOff();
  }

  switch (transmission) {

    case "Intruder":

      if (!alarmOn) {
        alarmOn = true;
      } else if (alarmOn) {
        alarmOn = false;
        alarmOff();
      }

      break;

    default:

      break;

  }

}

void alarm() {
  Serial.println("Alarm Triggered");
  tone(buzzer, 1000); // Send 1KHz sound signal...
  digitalWrite(13, HIGH);
  delay(500);        // ...for 0.5 sec
  tone(buzzer, 750); // Send 0.5KHz sound signal..
  digitalWrite(13, LOW);.
  delay(500);        // ...for 0.5 sec

}
void alarmOff() {

  noTone(buzzer);

}
