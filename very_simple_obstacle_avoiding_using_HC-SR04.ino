/*
Standing on the shoulders of giants:
 
Stepper Motor Control - one revolution
 This is the examply by Tom Igoe delivered with th Arduino IDE.

 Adapted by Stefan Thesen for 28BYJ-48 stepper motor
 https://blog.thesen.eu

again adapted by wtlx for flexible movement of the SMARS Robot
 */

#include <Stepper.h>

// Get steps per turn and RPM out of datasheet:
// ============================================
// 5.625/64 deg per step --> 4096 steps for 360 deg
// step relates to half-step sequencing, which means 8 steps
// We use 4 full steps ==> 2048 steps per turn
//
// recommended frequency range 600-1000Hz for 8 half-steps
// 1000Hz/4096 half-steps --> approx 4sec per turn is max speed 
// ==> 15rpm
// Note: actually we will use ~500Hz using the 4 step approach
// but motor will not be able to turn faster.

// Get stepping sequence out of datasheet:
// =======================================
// Stepping sequence as given in datasheet
// this is an 8-step half-step approach
//         Steps
// Wire  1  2  3  4  5  6  7  8
// 1                    x  x  x
// 2              x  x  x 
// 3        x  x  x
// 4     x  x                 x
// 
// We use only even / full steps thus:
//         Steps
// Wire  2  4  6  8
//   1         x  x
//   2      x  x 
//   3   x  x 
//   4   x        x
// 
// Code of Arduino Stepper Lib has implemented:
//         Steps
// Wire  1  2  3  4
//   1         x  x
//   2   x  x      
//   3      x  x 
//   4   x        x
//
// ==> Simple Solution: exchange wire 2&3


const int stepsPerRevolution = 2048;  // here go the 2048 steps
// for your motor

// initialize the stepper library on pins 8 through 11 -> IN1, IN2, IN3, IN4
// as shown above, we need to exchange wire 2&3, which we do in the constructor
Stepper leftStepper(stepsPerRevolution, 8, 10, 9, 11);
Stepper rightStepper(stepsPerRevolution, 4, 6, 5, 7);



int stepsToDo;  //variable to be flexible in the loop (e.g. depending on the distance to obstacles)


 
// define functions for basic movement
// using 'for' to move steppers (pseudo)simultaniously    
    
    void goForward(int x){              
           for(int s=0; s<x; s++){
             leftStepper.step(1);
             rightStepper.step(-1);
             }       
    }
        
    
    void goBack(int x){              
           for(int s=0; s<x; s++){
             leftStepper.step(-1);
             rightStepper.step(1);
             }       
    }


//turning left and right standing at the same place - with both motors
  void turnLeft(int x){              
           for(int s=0; s<x; s++){
             leftStepper.step(-1);
             rightStepper.step(-1);
             }       
    }
    
    void turnRight(int x){              
           for(int s=0; s<x; s++){
             leftStepper.step(1);
             rightStepper.step(1);
             }       
    } 




// HC-SR04 Sensor
int trigger=12; 
int echo=13; 
float duration=0; 
float distance=0; 
float realDistance=0;



int whatDistance(){
    digitalWrite(trigger, LOW); 
    delayMicroseconds(2); 
    digitalWrite(trigger, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigger, LOW); 
    duration = pulseIn(echo, HIGH); 
    distance = (duration/2) * 0.03432;    
    return distance;
    }




void setup() 
{
  // set the speed at 15 rpm:
  rightStepper.setSpeed(15);
  leftStepper.setSpeed(15);
  
  // set pinmodes for HC-04
  pinMode(trigger, OUTPUT); 
  pinMode(echo, INPUT); 
    
Serial.begin(9600); // Starts the serial communication

}

void loop() 
{
 
Serial.print("distance: ");
    Serial.println(distance);
    
realDistance = whatDistance();
Serial.println(realDistance);    
        
        
if (realDistance<=0) {
    realDistance = whatDistance();
    }   
    
while (realDistance>=5) {
    stepsToDo = 2048;  // steps for a complete revolution
    goForward(stepsToDo);
    realDistance = whatDistance();
    }    
      
    
    
stepsToDo = 512;    // in case there is just 5cm or below to the obstacle turn rund 45 degrees to right
turnRight(stepsToDo);  
    
    
realDistance = whatDistance(); //get new distance
 
 while (realDistance<=15) {
    stepsToDo = 512;  // steps for 1/4 revolution
    turnRight(stepsToDo);
    realDistance = whatDistance();
    } 
    
}
