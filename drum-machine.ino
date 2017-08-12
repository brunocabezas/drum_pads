#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

//Pins initialization
const int LED = 9; //digital
const int BUTTON_1 = 2; //digital
const int BUTTON_2 = 3; //digital
const int PAD_1 = A0;
const int PAD_2 = A1;
const int PAD_3 = A2;
const int PAD_4 = A3;

const char ledDefaultState = 'LOW';

//The MIDI note value to be played (instrument)
byte NOTE_1 = 27;
byte NOTE_2 = 27;
byte NOTE_3 = 27;
byte NOTE_4 = 27;

byte resetMIDI = 4;
int instrument = 27;
int ledState = 0; // not used
int touchSensorLimit = 1;
int button1State = 0;
int button2State = 0;
int lastButton1State = 0;
int lastButton2State = 0;

void setup() {
  Serial.begin(57600);

  //Setup soft serial for MIDI control
  mySerial.begin(31250);

  //Setting inputs and outputs
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(pinLed, OUTPUT);

  //Reset the VS1053
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);
  //0xB0 is channel message, set channel volume to near max (127)
  talkMIDI(0xB0, 0x07, 120);
}

void loop() {
  //Serial.println("Basic Instruments");
  int pad1 = analogRead(PAD_1),
      pad2 = analogRead(PAD_2),
      pad3 = analogRead(PAD_3),
      pad4 = analogRead(PAD_4);

  talkMIDI(0xB0, 0, 0x00); //Default bank GM1
  button1State = digitalRead(BUTTON_1);
  button2State = digitalRead(BUTTON_2);
  // talkMIDI(0xB0, 0, 0x78); //Bank : drums
  talkMIDI(0xC0, 5, 0); // Set instrument number 

  // compare the buttonState to its previous state
  if (button1State != lastButton1State) {
    // if the state has changed, increment the counter
    if (button1State == HIGH) {
      NOTE_1 = NOTE_1 + 1;
    }
    delay(50);
    //  } else if (button2State != lastButton2State){
    //    if (button2State == HIGH) {
    //      NOTE_2 = NOTE_2 + 1;
    //    }
    //    delay(50);
  } else {

    if (pad1 > touchSensorLimit){
      // Play Note on channel 1 (0x90), some note value (note), middle velocity (0x45):) {
      digitalWrite(pinLed, HIGH);
      noteOn(0, 39, 60);
      delay(50);
      
      //Turn off the note with a given off/release velocity
      noteOff(0, 39, 60);
      delay(50);
      
      digitalWrite(pinLed, LOW);
      String text = "pad1 - inst: ";
      String toPrint = text + NOTE_1;
      Serial.println(toPrint);
    }

    if (pad2 > touchSensorLimit) {
      digitalWrite(pinLed, HIGH);
      // Play static note for now
      int thisNote = NOTE_2;
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, thisNote, 60);
      delay(50);
      //Turn off the note with a given off/release velocity
      noteOff(0, thisNote, 60);
      digitalWrite(pinLed, LOW);
      delay(50);
      String text = "pad2 - inst: ";
      String toPrint = text + thisNote;
      Serial.println(toPrint);
    }

    if (pad3 > touchSensorLimit) {
      digitalWrite(pinLed, HIGH);
      // Play static note for now
      int thisNote = NOTE_2;
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, thisNote, 60);
      delay(50);
      //Turn off the note with a given off/release velocity
      noteOff(0, thisNote, 60);
      digitalWrite(pinLed, LOW);
      delay(50);
      String text = "pad3 - inst: ";
      String toPrint = text + thisNote;
      Serial.println(toPrint);
    }

    if (pad4 > touchSensorLimit) {
      digitalWrite(pinLed, HIGH);
      // Play static note for now
      int thisNote = NOTE_2;
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      noteOn(0, thisNote, 60);
      delay(50);
      //Turn off the note with a given off/release velocity
      noteOff(0, thisNote, 60);
      digitalWrite(pinLed, LOW);
      delay(50);
      String text = "pad4 - inst: ";
      String toPrint = text + thisNote;
      Serial.println(toPrint);
    }

    // turning off led if sensors value are <= than the limit
    if (pad1 <= touchSensorLimit && pad2 <= touchSensorLimit && pad3 <= touchSensorLimit && pad4 <= touchSensorLimit) {
      digitalWrite(pinLed, 0);
    }
  }
  lastButton1State = button1State;
  lastButton2State = button2State;
}

//void playNote(note) {
//  digitalWrite(pinLed, HIGH);
//  //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
//  noteOn(0, note, 60);
//  delay(50);
//  //Turn off the note with a given off/release velocity
//  noteOff(0, note, 60);
//  digitalWrite(pinLed, LOW);
//  delay(50);
//  String text = "pad4 - inst: ";
//  String toPrint = text + thisNote;
//  Serial.println(toPrint);
//}

// MIDI INTERFACE
// ----------------------------
//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  //digitalWrite(pinLed, HIGH);
  mySerial.write(cmd);
  mySerial.write(data1);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if ( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2);

  //digitalWrite(pinLed, LOW);
}
