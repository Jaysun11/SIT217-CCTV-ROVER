//STARTUP SONG
#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_G5  784
#define REST      0

// change this to make the song slower or faster
int tempo = 200;

// change this to whichever pin you want to use
int buzzer = 5;

int melody[] = {

  
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8, //1
  REST,6, NOTE_G5,4, REST,4, NOTE_G4,8

  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

//--------------------------

//Initialize Motor Code
// Motor One
int ENA = 6;
int IN1 = 7;
int IN2 = 8;
// Motor Two
int ENB = 11;
int IN3 = 9;
int IN4 = 10;

const int LED = 12;

byte alarmOn = false;
byte torchOn = false;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3);

void setup() {
  // put your setup code here, to run once:

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(buzzer,OUTPUT);


  mySerial.begin(9600);
  Serial.begin(9600);

  digitalWrite(ENA, 5);
  digitalWrite(ENB, 5);



//SONG

// iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }


//END SONG
 
}

void loop() {
  char transmission;

  if(mySerial.available()){
    transmission = mySerial.read();
    delay(100);;
    Serial.println(transmission);
  } 

  if (alarmOn){
    alarm();
  } if (!alarmOn){
    alarmOff();
  }
  
  if (torchOn){
    torch();
  } else if (torchOff){
    torchOff();
  }

  switch (transmission) {
    
  case 'A':
    goStraight();
  
  break;
    
  case 'C':
    reverse();
    
  break;

  case 'D':
   turnLeft();
   
  break;
   
  case 'B':
   turnRight();
   
  break;

  case 'G':
  
   if (!torchOn){
    torchOn = true;
  } else {
    torchOn = false;
  }
   
  break;

  case 'a':
  //held straight
  break;

  case 'c':
  //held back
  break;

  case 'd':
  //held left
  break;

  case 'b':
  //held right
  break;

  case 'F':
  if (!alarmOn){
    alarmOn = true;
  } else if (alarmOn){
    alarmOn = false;
    alarmOff();
  }
  
   
  default:
  
  stopMoving();
  
  break;
}
 

}

void turnRight() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  

}

void turnLeft() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
}

void stopMoving() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}

void goStraight() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

}

void reverse() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}

void alarm(){

  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(500);        // ...for 0.5 sec
  tone(buzzer, 750); // Send 0.5KHz sound signal...
  delay(500);        // ...for 0.5 sec

}
void alarmOff(){

  noTone(buzzer);
 
}

void torch(){
  digitalWrite(LED, HIGH);
}
void torchOff(){
  digitalWrite(LED, LOW);
}
