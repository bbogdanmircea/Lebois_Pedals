// H Shifter
// This programs reads and send 7 switches corresponding to 6 + 1 speeds from a H Shifter
//
// NOTE: This file is for use with Arduino Leonardo and  Arduino Micro only.
//       Arduino Micro only.
//
// To get this program to work you need MHeironimus GPLv3 joystick libary from
// https://github.com/MHeironimus/ArduinoJoystickLibrary version-1.0
//
//
//
//
// This project is licensed under the terms of the GNU V.3 license.
//
//--------------------------------------------------------------------

#include <Joystick.h>

#define DEBUG 1
#define PIN_GEARS 7

Joystick_ H_Shifter(
  JOYSTICK_DEFAULT_REPORT_ID, //hidReportId
  JOYSTICK_TYPE_JOYSTICK, //joystickType
  PIN_GEARS, //buttonCount 
  0, //hatSwitchCount
  false, //includeXAxis -> Gas
  false, //includeYAxis -> Brake
  false, //includeZAxis -> Clutch
  false, //includeRxAxis 
  false, //includeRyAxis 
  false, //includeRzAxis
  false, //includeRudder 
  false, //includeThrottle 
  false, //includeAccelerator 
  false, //includeBrake 
  false); //includeSteering

// Constant that maps the physical pin to the joystick button.
const int pinGears[PIN_GEARS] = {2, 15, 3, 14, 4, 16, 10}; // pin corresponding to Gears 1, 2, 3, 4, 5, 6, R

void setup() {
  #ifdef DEBUG                          // If the #define DEBUG line at the top is not commented out, then this runs
  Serial.begin(9600);                   // This starts up the Arduino Serial communications, which we use to send info messages to the computer
  Serial.println("H shifter debug started");
  #endif
  
  // Initialize Pins as pullups
  for (int i = 0; i < PIN_GEARS; i++) {
    pinMode(pinGears[i], INPUT_PULLUP);
  }

  // Initialize Joystick Library
  H_Shifter.begin();
}

// Last state of the button
int lastGearState[PIN_GEARS] = {0,0,0,0,0,0,0};

void loop() {
  int currentGearState[PIN_GEARS] = {0,0,0,0,0,0,0};

  // Read pin values
  for (int i = 0; i < PIN_GEARS; i++) {
    currentGearState[i] = !digitalRead(pinGears[i]); // State is True when Pin is Low
    #ifdef DEBUG
      Serial.print("Gear ");
      Serial.print( i );
      Serial.print( " = " );
      Serial.println( currentGearState[i] );
    #endif
    if (currentGearState[i] != lastGearState[i])
    {    
    H_Shifter.setButton(i, currentGearState[i]);
    lastGearState[i] = currentGearState[i];
    }
  }
  delay(50);
}
