// V1.0 : ajout du mode verbose
// V1.1 : correction
// Disable verbose mode once you have calibrated your pedals to make work at full speed !
// comment either out to disable || Mettre en commentaire pour désactiver

#define USE_ACCEL
#define USE_BRAKE
#define USE_CLUTCH
//#define VERBOSE_MODE   //allow to tune min and max

int AcceleratorPin = A1;
int BrakePin = A2;
int ClutchPin = A3;

int minAccelerator = 50;
int maxAccelerator = 500;
int minBrake = 100;
int maxBrake = 500;
int minClutch = 0;
int maxClutch = 600;

int lastAcceleratorState ;
int lastBrakeState;
int lastClutchState;
int currentAcceleratorState;
int currentBrakeState;
int currentClutchState;

#include "Joystick.h"
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_MULTI_AXIS, 4, 0,
                   true, false, false, false, false, false,
                   false, false, true, true, false);

void setup() {
#ifdef VERBOSE_MODE
  Serial.begin(9600);
  Serial.println("Verbose mode activated");
#endif

  Joystick.setXAxisRange(minClutch, maxClutch);
  Joystick.setAcceleratorRange(minAccelerator, maxAccelerator);
  Joystick.setBrakeRange(minBrake, maxBrake);
  Joystick.begin();
  lastAcceleratorState = analogRead(AcceleratorPin);
  lastBrakeState = analogRead(BrakePin);
  lastClutchState = analogRead(ClutchPin);
  delay(1000); //safety delay to flash the code
}

void loop() {

#ifdef USE_ACCEL

  int currentAcceleratorState = analogRead(AcceleratorPin);

  if (currentAcceleratorState != lastAcceleratorState)
  {
    Joystick.setAccelerator(currentAcceleratorState);
    lastAcceleratorState = currentAcceleratorState;
  }
#ifdef VERBOSE_MODE
  Serial.println("acceleration ");
  Serial.println(currentAcceleratorState);
  delay(500);
#endif
#endif

#ifdef USE_BRAKE
  int currentBrakeState = analogRead(BrakePin);
  if (currentBrakeState != lastBrakeState)
  {
    Joystick.setBrake(currentBrakeState);
    lastBrakeState = currentBrakeState;
}
#ifdef VERBOSE_MODE
    Serial.println("frein ");
    Serial.println(currentBrakeState);
    delay(500);
#endif
#endif

#ifdef USE_CLUTCH
  int currentClutchState = analogRead(ClutchPin);
    if (currentClutchState != lastClutchState)
    {
      Joystick.setXAxis(currentClutchState);
      lastClutchState = currentClutchState;
    }
#ifdef VERBOSE_MODE
    Serial.println("embrayage ");
    Serial.println(currentClutchState);
    delay(500);
#endif
#endif

    delay(10);
  }
