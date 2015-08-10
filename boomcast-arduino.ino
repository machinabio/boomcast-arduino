#include "LPD8806.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma

#define BUSSPEED 38400
#define MAXPAYLOADLENGTH 6
#define NLEDS 20
#define VALIDCOMMANDCHARACTERS "abcdefghijklmnopqrstuvwxz" // each letter corresponds the address of an LEDs. Z is reserverd for commands
#define VALIDPAYLOADCHARACTERS "1234567890" // only digits are acceptable characters for the payload

#ifdef __AVR_ATtiny85__
  #include <avr/power.h>
#endif

// Example to control LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/

// Number of RGB LEDs in strand:

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 11; // A3
int clockPin = 10; // A2

boolean demo = false;

boolean valid = false;
char _command;
boolean commandExpected;
int nBytesExpecting;
char payloadBuffer[MAXPAYLOADLENGTH];
int index;

LPD8806 strip = LPD8806(NLEDS, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);

void setup() {

  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  Serial.begin(BUSSPEED);
  clearCommand();
  Serial.flush();
//  Serial.println("Finished startup");
}


void loop() {
  while (Serial.available() > 0) {
    push(Serial.read());
  }
  if ( _command == 'z' && valid ) { // 
    changeProgram();
  } else if ( valid ) { // update LED
    updateLED();  
  } else if (demo) {
      testPattern();
  }
//  Serial.println("Loopinp");
//  delay(1000);
  
}

void push(char serialData) {
//  Serial.print("pushing a character :");
  Serial.println(serialData); 
/* Were we expecting anything in particular? */
  if (commandExpected) receiveCommand(serialData);	
  else if (nBytesExpecting > 0) receivePayload(serialData);
}

void receivePayload(char serialData)
{
//    Serial.println("Receive payload");

  String validCharacters = String(VALIDPAYLOADCHARACTERS);

  if ( validCharacters.indexOf(serialData) == -1) {
    clearCommand();
    return;
  }

  payloadBuffer[index] = serialData;
  ++index;
  if (--nBytesExpecting == 0) valid=true; // if that was the last byte, set valid  
}

void receiveCommand(char serialData)
{
//  Serial.println("Receivecommand");

  String validCharacters = String(VALIDCOMMANDCHARACTERS);

  if ( validCharacters.indexOf(serialData) == -1) {
    clearCommand();
    return;
  }
  _command = serialData;
  commandExpected = false;
  if (serialData == 'z') {
    nBytesExpecting = 1;
  } else {
    nBytesExpecting = MAXPAYLOADLENGTH;
  }
}

/**
* Resets the command parser
*
*/
void clearCommand()
{
//  Serial.println("Clear command");
  valid=false;
  nBytesExpecting=0;
  index=0;
  commandExpected = true;
}






