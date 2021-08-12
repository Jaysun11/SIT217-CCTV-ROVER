int buzzer = 5;

byte alarmOn = false;

bool testingInterDeviceConnection = true;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);


void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  char transmission;
  if (testingInterDeviceConnection) {
    if (Serial.available() > 0) {
      transmission = Serial.read();
      delay(100);
      Serial.println("Received Transmission");
    }
  } else {
    if (mySerial.available()) {
      transmission = mySerial.read();
      delay(100);
      Serial.println("Received Transmission");
    }
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
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);        // ...for 0.5 sec
  tone(buzzer, 750); // Send 0.5KHz sound signal..
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);        // ...for 0.5 sec

}
void alarmOff() {

  noTone(buzzer);

}
