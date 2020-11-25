// SvenstueBot 1.0

// The following #includes (Servo and NewPing)
// have to be downloaded and installed.
// Search and install them through the arduino ide, 
// using the Library Manager (cmd/ctrl + Shift +  i).

// Library for controlling servo motors
#include <Servo.h> // servo motors must use PWM capable pins.

#include <NewPing.h> // Library for controlling distance sensors.

//-- Constants --//

// Motor Pins:
const int pinLB=7; // define pin6 as left back connect with IN1
const int pinLF=9; // define pin9 as left forward connect with IN2
const int pinRB=10; // define pin10 as right back connect with IN3
const int pinRF=11; // define pin11 as right back connect with IN4
const int motorAspeed=6; // pwm speed pin for the right motor
const int motorBspeed=3; // pwm speed pin for the left motor

// Time
const int detectionFreq=50; // Detection Frequency in milliseconds
const int backTime=500; // Time it takes to reverse x cm in milliseconds.
const int turnTime=800; // Time it takes to turn x degrees in milliseconds


// pwm speed numbers
const int highSpeed=150;
const int mediumSpeed=100;
const int lowSpeed=80;
const int superSlow=100;
const int stopSpeed=0;

// Delay time?
const int delay_time = 250; // wait for servo to turn!!!!!

// Ultra sonic sensor constants
#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//-- Objects --//

// Servo motor that turns the ultra sonic sensor
Servo senseServo;

// Ultra sonic sensor setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

//-- Variables --//

// Ultra Sonic variables
int frontDist = 0; // forward distance
int rightDist = 0; // right distance
int leftDist = 0; // left distance

// Time variabels
unsigned long previousDetect = 0; 
unsigned long backStart = 0; 
unsigned long turnStart = 0; 

// Flags
bool isBacking=false;
bool isTurning=false;

// Analog Stick ints
int  xSpeed = 0;
int  ySpeed = 0;
int  xSpeedNeg = 0;
int  ySpeedNeg = 0;
int  xSpeedInv = 0;
int analogMin = 485;
int analogMax = 515;

//--Radio vars
int leftSpeed = 0;
int rightSpeed = 0;
int roboDirection = 0; //0 stopped, 1 Forward, 2 Left, 3 Right, 4 Reverse
int robotStateLocal = 0; //robotState, 0 for auto, 1 for taurus, 2 for manual override
  // Analog-stick values
uint32_t xVal = 0;
uint32_t yVal = 0;

//--Local includes

#include "radio.h" //Include rf-recieve and answer script.

#include "functions.h" // All the main logic is here!!!

// Setup runs one time, at startup.
void setup() {
  //playMelody(melody);  
  Serial.begin(115200);
  
  // if()statement below halts program if radio is disconnected
  // or broken.
  // Comment out if rf unit is not in use.
  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)){
    //Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }
  // Set up motor pins
  pinMode(pinLB,OUTPUT);
  pinMode(pinLF,OUTPUT);  
  pinMode(pinRB,OUTPUT);
  pinMode(pinRF,OUTPUT);

  //Tell the servo object what pin to use(set up servo pin...)
  senseServo.attach(5);
}

//-- The main loop. --//
void loop() {
  //autonomusMode
  if(robotStateLocal == 0){
    if(isBacking){
      if (millis() - backStart >= backTime){ isBacking=false; }
    }
    else if(isTurning){
      if (millis() - turnStart >= turnTime){ isTurning=false; }
    }
    else {
      if (millis() - previousDetect >= detectionFreq) {
        previousDetect = millis();
        detection(); // check if about to hit into something, and set the direction accordingly.
      }
    }
  }
  //taurusMode
  if(robotStateLocal == 1){
    //taurusMode();
  }
  //Manual Overide
  if(robotStateLocal == 2){
    analogAnalyzer();
  }

}
