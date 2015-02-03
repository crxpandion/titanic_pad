#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();
/*
 * Ardrumo sketch
 *
 * Use with the Ardrumo software here:
 * http://code.google.com/p/ardrumo/
 * This is designed to let an Arduino act as a drum machine
 * in GarageBand (sorry, Mac OS X only).
 */


#define LEDPIN     13     // status LED pin
#define PIEZOTHRESHOLD 5  // analog threshold for piezo sensing
#define PADNUM 1          // number of pads

int val;

const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;


void setup() {
  pinMode(LEDPIN, OUTPUT);
  MIDI.begin();
  Serial.begin(115200);   // set serial output rate
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void colorLed(int red, int green, int blue) {
   analogWrite(redPin, red);
   analogWrite(greenPin, green);
   analogWrite(bluePin, blue);
}

int randomPWMInt() {
  return random(0, 255);
}

long debounceArray;
long debounceDelay = 200;

boolean debounce(long debounceTime, int index) {
   if ((millis() - debounceTime) > debounceDelay) {
     debounceTime = millis();
     return false;
   }
   return true;
}

void loop() {

  // Loop through each piezo and send data
  // on the serial output if the force exceeds
  // the piezo threshold
//  for(int i = 0; i < PADNUM; i++) {
//    debounceArray[i] = 0;
//  }
  
  for(int i = 0; i < PADNUM; i++) {
    val = analogRead(i);
    if( val >= PIEZOTHRESHOLD ) {
      if ((millis() - debounceArray) > debounceDelay) {
        debounceArray = millis();
        digitalWrite(LEDPIN,HIGH);  // indicate we're sending MIDI data
        MIDI.sendNoteOn(41, 127, 1);
        delay(100);
        MIDI.sendNoteOff(41, 127, 1);
        digitalWrite(LEDPIN,LOW);
        colorLed(randomPWMInt(),randomPWMInt(), randomPWMInt() ); 
      }
    }
  }
}
