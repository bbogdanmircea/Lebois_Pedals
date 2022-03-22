// V1.0 : ajout du mode verbose
// V1.1 : correction
// V1.2 : display min and max read when verbose_mode is active.
// V1.3 : handbrake analog and digital mode added
// V1.4 : code fix + fast debug : no need to disable DEBUG to operate at full speed.

// comment either out to disable || Mettre en commentaire pour désactiver

#define USE_ACCEL
#define USE_BRAKE
#define USE_CLUTCH
//#define USE_ANALOG_HANDBRAKE
//#define USE_DIGITAL_HANDRAKE
//#define DEBUG   //allow to tune min and max

#define AcceleratorPin A1
#define BrakePin       A2
#define ClutchPin      A3
#define HandbrakePin   A0

int minAccelerator = 210;
int maxAccelerator = 610;
int minBrake = 125;
int maxBrake = 500;
int minClutch = 90;
int maxClutch = 490;
int minHandbrake = 100;
int maxHandbrake = 300;
int digitalHandbrakeSwitch = 200;

int lastAcceleratorState ;
int lastBrakeState;
int lastClutchState;
int lastHandbrakeState;
int currentAcceleratorState;
int currentBrakeState;
int currentClutchState;
int currentHandbrakeState;

int minAcceleratorRead = 1023;
int maxAcceleratorRead = 0;
int minBrakeRead = 1023 ;
int maxBrakeRead = 0;
int minClutchRead = 1023;
int maxClutchRead = 0;
int minHandbrakeRead = 1023;
int maxHandbrakeRead = 0;

bool sendDebug = false;
int unsigned long lastDebug = 0;
int DebugRefreshRate = 250;

#include "Joystick.h"
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_MULTI_AXIS, 4, 0,
                   true, true, false, true, false, false,
                   false, false, true, true, false);

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("DEBUG mode activated");
#endif

  Joystick.setRxAxisRange(minAccelerator, maxAccelerator);
  Joystick.setBrakeRange(minBrake, maxBrake);
  Joystick.setXAxisRange(minClutch, maxClutch);
  Joystick.setYAxisRange(minHandbrake, maxHandbrake);
  Joystick.begin();
  lastAcceleratorState = analogRead(AcceleratorPin);
  lastBrakeState = analogRead(BrakePin);
  lastClutchState = analogRead(ClutchPin);
  lastHandbrakeState = analogRead(HandbrakePin);
  delay(1000); //safety delay to flash the code
}

void loop() {
#if defined(DEBUG)
  if (millis() - lastDebug > DebugRefreshRate) {
    sendDebug = true;
    lastDebug = millis();
  }
#endif

#ifdef USE_ACCEL
  currentAcceleratorState = analogRead(AcceleratorPin);
  if (currentAcceleratorState != lastAcceleratorState)
  {
    Joystick.setRxAxis(currentAcceleratorState);
    lastAcceleratorState = currentAcceleratorState;
  }
#ifdef DEBUG
  minAcceleratorRead = min(minAcceleratorRead, currentAcceleratorState);
  maxAcceleratorRead = max(maxAcceleratorRead, currentAcceleratorState);
  if (sendDebug) {
    Serial.print("Acceleration : ");
    Serial.print(currentAcceleratorState);
    Serial.print(". Min is : ");
    Serial.print(minAcceleratorRead);
    Serial.print(". Max is : ");
    Serial.println(maxAcceleratorRead);
  }
#endif
#endif

#ifdef USE_BRAKE
  currentBrakeState = analogRead(BrakePin);
  if (currentBrakeState != lastBrakeState)
  {
    Joystick.setBrake(currentBrakeState);
    lastBrakeState = currentBrakeState;
  }
#ifdef DEBUG
  minBrakeRead = min(minBrakeRead, currentBrakeState);
  maxBrakeRead = max(maxBrakeRead, currentBrakeState);
  if (sendDebug) {
    Serial.print("       Brake : ");
    Serial.print(currentBrakeState);
    Serial.print(". Min is : ");
    Serial.print(minBrakeRead);
    Serial.print(". Max is : ");
    Serial.println(maxBrakeRead);
  }
#endif
#endif

#ifdef USE_CLUTCH
  currentClutchState = analogRead(ClutchPin);
  if (currentClutchState != lastClutchState)
  {
    Joystick.setXAxis(currentClutchState);
    lastClutchState = currentClutchState;
  }
#ifdef DEBUG
  minClutchRead = min(minClutchRead, currentClutchState);
  maxClutchRead = max(maxClutchRead, currentClutchState);
  if (sendDebug) {
    Serial.print("      Clutch : ");
    Serial.print(currentClutchState);
    Serial.print(". Min is : ");
    Serial.print(minClutchRead);
    Serial.print(". Max is : ");
    Serial.println(maxClutchRead);
  }
#endif
#endif

#ifdef USE_ANALOG_HANDBRAKE
  currentHandbrakeState = analogRead(HandbrakePin);
  if (currentHandbrakeState != lastHandbrakeState) {
    Joystick.setYAxis(currentHandbrakeState);
    lastHandbrakeState = currentHandbrakeState;
  }

#ifdef DEBUG
  minHandbrakeRead = min(minHandbrakeRead, currentHandbrakeState);
  maxHandbrakeRead = max(maxHandbrakeRead, currentHandbrakeState);
  if (sendDebug) {
    Serial.print("   Handbrake : ");
    Serial.print(currentHandbrakeState);
    Serial.print(". Min is : ");
    Serial.print(minHandbrakeRead);
    Serial.print(". Max is : ");
    Serial.println(maxHandbrakeRead);
  }
#endif
#endif

  if (sendDebug) {
    Serial.println("");
  }

#ifdef USE_DIGITAL_HANDBRAKE
  if(analogRead(HandbrakePin)>digitalHandbrakeSwitch){currentHandbrakeState = 1;}
  else{currentHandbrakeState = 0;}

  if (currentHandbrakeState != lastHandbrakeState)
  {
  Joystick.setButton(0, currentHandbrakeState);
    lastHandbrakeState = currentHandbrakeState;
  }
  #ifdef DEBUG
  minHandbrakeRead = min(minHandbrakeRead, currentHandbrakeState);
  maxHandbrakeRead = max(maxHandbrakeRead, currentHandbrakeState);
  Serial.print("Handbrake : ");
  Serial.print(currentHandbrakeState);
  Serial.print(". Min is : ");
  Serial.print(minHandbrakeRead);
  Serial.print(". Max is : ");
  Serial.println(maxHandbrakeRead);
  #endif
#endif

  sendDebug = false;
}
