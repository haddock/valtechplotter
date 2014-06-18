#include <Servo.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMSbot(0x61); // Rightmost jumper closed
Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepper1 = AFMStop.getStepper(32, 1);
Adafruit_StepperMotor *myStepper2 = AFMStop.getStepper(32, 2);

void forwardstep1() {  
  myStepper1->onestep(FORWARD, SINGLE);
}
void backwardstep1() {  
  myStepper1->onestep(BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  myStepper2->onestep(FORWARD, DOUBLE);
}
void backwardstep2() {  
  myStepper2->onestep(BACKWARD, DOUBLE);
}


// Now we'll wrap the 3 steppers in an AccelStepper object
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

 
Servo myservo;  // create servo object to control a servo 
 
int angle;
int servoVal;   
int change;

void setup()
{
  Serial.begin(9600);
  servoVal = 0;
  angle = 0;
  change = 1;
  myservo.attach(10);
  delay(10);
  AFMSbot.begin(); // Start the bottom shield
  AFMStop.begin(); // Start the top shield
   
  stepper1.setMaxSpeed(200.0);
  stepper1.setAcceleration(100.0);
    
  stepper2.setMaxSpeed(200.0);
  stepper2.setAcceleration(100.0);
}

void loop()
{
    angle += change;
    if(angle > 254) {
      change = -1;
    }
    if(angle < 2) {
      change = 1;
    }
    servoVal = map(angle, 0, 1023, 0, 255);    
    myservo.write(servoVal);           
    Serial.print("servo at");
    Serial.println(servoVal);
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
	stepper1.moveTo(-stepper1.currentPosition());

    if (stepper2.distanceToGo() == 0)
	stepper2.moveTo(-stepper2.currentPosition());

    stepper1.run();
    stepper2.run();
    
    
}

void moveLeft(int steps) { 
  stepper2.moveTo(stepper2.currentPosition() + steps);
  stepper1.moveTo(stepper1.currentPosition() - steps);
}

void moveRight(int steps) {
  stepper2.moveTo(stepper2.currentPosition() + steps);
  stepper1.moveTo(stepper1.currentPosition() - steps);
}

void moveUp(int steps) {
  stepper2.moveTo(stepper2.currentPosition() - steps);
  stepper1.moveTo(stepper1.currentPosition() - steps);
}

void moveDown(int steps) {
  stepper2.moveTo(stepper2.currentPosition() + steps);
  stepper1.moveTo(stepper1.currentPosition() + steps);
}
