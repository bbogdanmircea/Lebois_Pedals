
//Logitech Driving Force Shifter USB Adapter 
//By Armandoiglesias 2018
//Based on Jason Duncan functionreturnfunction Project
//Video tutorial https://www.youtube.com/watch?v=dLpWEu8kCec
//Use Arduino Leonardo
//Install Joystick Library 
//Attribution-NonCommercial-NoDerivatives 4.0 International

#include <Joystick.h>

//Lebois Clutch defines and variables START
// comment either out to disable || Mettre en commentaire pour désactiver
#define USE_CLUTCH
//#define DEBUG   //allow to tune min and max
#define ClutchPin      A3
int minClutch = 90;
int maxClutch = 490;
int lastClutchState;
int currentClutchState;
int minClutchRead = 1023;
int maxClutchRead = 0;
//Lebois debug variables
bool sendDebug = false;
int unsigned long lastDebug = 0;
int DebugRefreshRate = 250;
//Lebois Clutch defines and variables END

// Create the Joystick
Joystick_ Joystick;

// H-shifter mode analog axis thresholds
#define HS_XAXIS_12        400
#define HS_XAXIS_56        500
#define HS_YAXIS_135       800
#define HS_YAXIS_246       300

// Sequential shifter mode analog axis thresholds
#define SS_UPSHIFT_BEGIN   670
#define SS_UPSHIFT_END     600
#define SS_DOWNSHIFT_BEGIN 430
#define SS_DOWNSHIFT_END   500

// Handbrake mode analog axis limits
#define HB_MAXIMUM         530
#define HB_MINIMUM         400
#define HB_RANGE           (HB_MAXIMUM-HB_MINIMUM)

// Digital inputs definitions
#define DI_REVERSE         1
#define DI_MODE            3
#define DI_RED_CENTERRIGHT 4
#define DI_RED_CENTERLEFT  5
#define DI_RED_RIGHT       6
#define DI_RED_LEFT        7
#define DI_BLACK_TOP       8
#define DI_BLACK_RIGHT     9
#define DI_BLACK_LEFT      10
#define DI_BLACK_BOTTOM    11
#define DI_DPAD_RIGHT      12
#define DI_DPAD_LEFT       13
#define DI_DPAD_BOTTOM     14
#define DI_DPAD_TOP        15

// Shifter state
#define DOWN_SHIFT         -1
#define NO_SHIFT           0
#define UP_SHIFT           1

// Shifter mode
#define SHIFTER_MODE       0
#define HANDBRAKE_MODE     1

// LED blink counter
int led=0;

// Shifter state
int shift=NO_SHIFT;

// Handbrake mode
int mode=SHIFTER_MODE;

int b[16];

int gear=0;                          // Default value is neutral

// Constant that maps the phyical pin to the joystick button.
//const int pinToButtonMap = 9;

void setup() {
   // G29 shifter analog inputs configuration 
  pinMode(A0, INPUT_PULLUP);   // X axis
  pinMode(A2, INPUT_PULLUP);   // Y axis

  pinMode(2, INPUT); 


  for(int i=0; i<16; i++) b[i] = 0;
  b[DI_MODE] =0;

//Lebois Clutch setup START 
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("DEBUG mode activated");
#endif
  Joystick.setXAxisRange(minClutch, maxClutch);
//Lebois Clutch setup END
  
  // Initialize Joystick Library
  Joystick.begin();

//Lebois Clutch setup START
  lastClutchState = analogRead(ClutchPin);
  delay(1000); //safety delay to flash the code
//Lebois Clutch setup END
}

// Last state of the button
int lastButtonState = 0;

void loop() {
//Lebois Clutch read START
#if defined(DEBUG)
  if (millis() - lastDebug > DebugRefreshRate) {
    sendDebug = true;
    lastDebug = millis();
  }
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
////Lebois Clutch read END

  int x=analogRead(0);                 // X axis
  int y=analogRead(2);                 // Y axis

  int _isreverse = digitalRead(2);
  int _gear_ = 0;

if( _isreverse == 1 ){

      _gear_ = 8;
      b[DI_REVERSE]= 1;

}else{ 
  

  if(b[DI_MODE]==0)                    // H-shifter mode?
  {
    if(x<HS_XAXIS_12)                  // Shifter on the left?
    {
      if(y>HS_YAXIS_135) _gear_=1;       // 1st gear
      if(y<HS_YAXIS_246) _gear_=2;       // 2nd gear
    }
    else if(x>HS_XAXIS_56)             // Shifter on the right?
    {
      if(y>HS_YAXIS_135) _gear_=5;       // 5th gear
      if(y<HS_YAXIS_246) _gear_=6;       // 6th gear
     
    }
    else                               // Shifter is in the middle
    {
      if(y>HS_YAXIS_135) _gear_=3;       // 3rd gear
      if(y<HS_YAXIS_246) _gear_=4;       // 4th gear
    }
   
  }

}
  
  
  if(gear!=6) b[DI_REVERSE]=0;         // Reverse gear is allowed only on 6th gear position
  
   if (_gear_ != gear ){
      gear = _gear_;
      desactivar();
      Joystick.setButton(gear-1, HIGH);
   }
   delay(50);

//Lebois sendDebug START
  if (sendDebug) {
    Serial.println("");
  }
  sendDebug = false;
//Lebois sendDebug END   
}

void desactivar(){
  // Depress virtual button for current gear
  for(int i = 0; i <= 10 ; i++ )  Joystick.setButton(i, LOW);
}
