#include <Ramp.h>//https://github.com/siteswapjuggler/RAMP
ramp myramp;
/*
   ramp myramp;
   myramp is vairable which will change value and to get value we use myramp.update();
   to make chanhes in value we use myramp.go(end value,duration in millisec);
   https://www.arduino.cc/en/Reference/ServoWriteMicroseconds
   myservo.writeMicroseconds(1500);


*/
/* Sweep
  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
int grippermax=160;
int grippermin=40;
#define gripperpin PA3
#define wristpitchpin PA7
#define wristrollpin PA6
#define elbowpin PA2
#define sholderpin PA0
#define hippin PA1

Servo gripper;  // create servo object to control a servo
Servo wristpitch;
Servo wristroll;
Servo elbow;
Servo sholder;
Servo hip;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  gripper.attach(gripperpin);  // attaches the servo on pin 9 to the servo object
  wristpitch.attach(wristpitchpin);
  wristroll.attach(wristrollpin);
  wristroll.write(90);
  elbow.attach(elbowpin);
  sholder.attach(sholderpin);
  hip.attach(hippin);
  hip.write(90);
 
  
  Serial1.begin(9600);
  Serial1.println("Start");
/*
    gripper.write(100);
  wristpitch.write(90);
  wristroll.write(90);
*/
}

void loop() {
  /*
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 15ms for the servo to reach the position
    }
  */
  /*
Serial1.println(gripper.read());
Serial1.println(wristpitch.read());
Serial1.println(wristroll.read());
Serial1.println("------");
*/
Serial1.println(gripper.read());
gripper.write(90);
wristroll.write(120);
elbow.write(70);
sholder.write(90);
hip.write(60);
delay(200);
  /*
  //delay(1000);
  //myservo.write(180);
  //delay(1000);
*/
}
