#include <Ramp.h>
rampDouble myramp;
#include <Servo.h>
#include<math.h>
//#define gripperpin PA3
#define wristpitchpin PA7
#define wristrollpin PA6
#define elbowpin PA2
#define sholderpin PA0
#define hippin PA1
#define POT PA4
int potval;
int grippermax = 0, grippermin = 150;
int gripperincrementpersec = 100;
int gripperopenvar, gripperclosevar;
long gripperopentime, gripperclosetime;
long loopdev;


double xx = 150, yy = 1;
double prevxx, prevyy;
int xxmax = 300, yymax = 300;
int xxposvar, xxnegvar, yyposvar, yynegvar;
long xxpostime, xxnegtime, yypostime, yynegtime;
double l1 = 125, l2 = 125; //l1>l2 always
double totalangle;
int maxcordbuffer = 5;
int sholderdev = 45; //35
int elbowdev = 10;
double defaultarmpos[6] = {30, 165, 90, 89, 45, 90};
double currentarmpos[6] = {30, 165, 90, 90, 100, 90};
long asignvaluetime, displayvaluetime;
Servo gripper;  // create servo object to control a servo
Servo wristM_PItch;
Servo wristroll;
Servo elbow;
Servo sholder;
Servo hip;
#define rodbuttontopmain PB8
#define rodbuttonbottom PB9

int wristupvar, wristdownvar;
long wristuptime, wristdowntime;
int wristmin = 0, wristmax = 160;

///////////circle 170,1
int pos1[6];
int pos2[6];
int pos3[6];
int pos4[6];
int pos5[6];
int pos6[6];
int pos7[6];
int pos8[6];

int xmin = 100;
int xmax = 240;
int xmid;
int start = 0;
int hipmax=65;
int hipmin=115;
int hipmid=90;
int phasesp=500;
int stoptime=0;
void setup() {
  // put your setup code here, to run once:
  //gripper.attach(gripperpin);  // attaches the servo on M_PIn 9 to the servo object
  wristM_PItch.attach(wristpitchpin);
  wristroll.attach(wristrollpin);
  wristroll.write(90);
  elbow.attach(elbowpin);
  sholder.attach(sholderpin);
  hip.attach(hippin);
  defaultpos();
  Serial1.begin(9600);
  Serial1.println("start");
  pinMode(rodbuttontopmain, INPUT_PULLUP);
  pinMode(rodbuttonbottom, INPUT_PULLUP);
  xmid=(xmin+xmax)/2;
  delay(1000);


  ////////////calculations
  /*
    xx=l1*cos(defaultarmpos[4]*M_PI/180)+l2*cos((defaultarmpos[4]-defaultarmpos[3])*M_PI/180);
    yy=l1*sin(defaultarmpos[4]*M_PI/180)+l2*sin((defaultarmpos[4]-defaultarmpos[3])*M_PI/180);
    Serial1.print(acos(0.5)/M_PI*180);
    Serial1.print(xx);
    Serial1.print("\t");
    Serial1.println(yy);
  */
  inversekinematics();

}

void loop() {
  if (digitalRead(rodbuttontopmain) == 0) {
    start = 0;
  }

  if (digitalRead(rodbuttonbottom) == 0) {
    start = 1;
  }
  Serial1.println(start);
  if (start == 1) {
    //currentarmpos();
    myramp.go(100+loopdev, phasesp);
    delay(stoptime);
    while (xx >= xmin && xx < xmid) {
      xx = map(myramp.update()-loopdev, 0, 100, xmin, xmid);
      inversekinematics();
      currentarmpos[5]=map(myramp.update()-loopdev, 0, 100, hipmid, hipmax);
      currentpos();
      Serial1.println(xx);
    }
    myramp.go(100*2+loopdev, phasesp);
    delay(stoptime);
    while (xx >= xmid && xx < xmax) {
      xx = map(myramp.update()-loopdev, 100, 200, xmid, xmax);
      inversekinematics();
      currentarmpos[5]=map(myramp.update()-loopdev, 100, 200, hipmax, hipmid);
      currentpos();
      Serial1.println(xx);
    }
    myramp.go(100*3+loopdev, phasesp);
    delay(stoptime);
    while (xx <= xmax && xx > xmid) {
      xx = map(myramp.update()-loopdev, 200, 300, xmax, xmid);
      inversekinematics();
      currentarmpos[5]= map(myramp.update()-loopdev, 200, 300, hipmid, hipmin);
      currentpos();
      Serial1.println(xx);
    }
    myramp.go(100*4+loopdev, phasesp);
    delay(stoptime);
    while (xx <= xmid && xx > xmin) {
      xx = map(myramp.update()-loopdev, 300, 400, xmid, xmin);
      inversekinematics();
      currentarmpos[5]= map(myramp.update()-loopdev, 300, 400, hipmin, hipmid);
      currentpos();
      Serial1.println(xx);
    }
    loopdev+=400;
  }


}
void defaultpos() {
  gripper.write(defaultarmpos[0]);
  wristM_PItch.write(defaultarmpos[1]);
  wristroll.write(defaultarmpos[2]);
  elbow.write(defaultarmpos[3] + elbowdev);
  sholder.write(defaultarmpos[4] + sholderdev);
  hip.write(defaultarmpos[5]);
}
void currentpos() {
  gripper.write(currentarmpos[0]);
  wristM_PItch.write(currentarmpos[1]);
  wristroll.write(currentarmpos[2]);
  elbow.write(currentarmpos[3] + elbowdev);
  sholder.write(currentarmpos[4] + sholderdev);
  hip.write(currentarmpos[5]);
}
void inversekinematics() {
  totalangle = acos((xx / sqrt(sq(xx) + sq(yy)))); //(y/x))
  Serial.println(totalangle * (180 / (PI)));
  currentarmpos[3] = ((sq(l1) + sq(l2) - sq(xx) - sq(yy))) / (2 * l1 * l2);
  //if(currentarmpos[3]<0){currentarmpos[3]=-currentarmpos[3];}
  currentarmpos[3] = 180 - (acos(currentarmpos[3]) * (180 / (PI)));
  currentarmpos[4] = (sq(xx) + sq(yy) + sq(l1) - sq(l2)) / (2 * sqrt(sq(xx) + sq(yy)) * l1);
  currentarmpos[4] = (acos(currentarmpos[4]) * (180 / (PI))) + totalangle * (180 / (PI));
}
