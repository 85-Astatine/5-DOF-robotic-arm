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
#define gripperopensw PB4
#define gripperclosesw PA15



double xx,yy;
double prevxx,prevyy;
int xxmax=300,yymax=300;
int xxposvar,xxnegvar,yyposvar,yynegvar;
long xxpostime,xxnegtime,yypostime,yynegtime;
double l1=125,l2=125;//l1>l2 always
double totalangle;
int maxcordbuffer=5;
int sholderdev=45;//35
int elbowdev=10;
double defaultarmpos[6] = {90, 90, 90, 90, 100, 90};
double currentarmpos[6] = {90, 90, 90, 90, 100, 90};
long asignvaluetime, displayvaluetime;
Servo gripper;  // create servo object to control a servo
Servo wristM_PItch;
Servo wristroll;
Servo elbow;
Servo sholder;
Servo hip;
int d=1;//////////////////////////////////////////////////////////////////////
#define rodbuttontopmain PB8
#define rodbuttonbottom PB9
#define rodledrightfront PB0
#define rodledleftback PB1

int wristupvar,wristdownvar;
long wristuptime,wristdowntime;
int wristmin=0,wristmax=160;
#include<Wire.h>
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_STM32.h>
#define OLED_RESET     -1
Adafruit_SSD1306 display(OLED_RESET);//sda b7   sck b6
#define displaybutton PA12
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
  delay(100);
  pinMode(POT, INPUT);
  pinMode(gripperopensw, INPUT_PULLUP);
  pinMode(gripperclosesw, INPUT_PULLUP);
  pinMode(displaybutton, INPUT_PULLUP);
pinMode(rodbuttontopmain,INPUT_PULLUP);
pinMode(rodbuttonbottom,INPUT_PULLUP);
pinMode(rodledrightfront,OUTPUT);
pinMode(rodledleftback,OUTPUT);
digitalWrite(rodledrightfront,1);
digitalWrite(rodledleftback,1);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial1.begin(9600);
  Serial1.println("Starting");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  delay(50);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 5);
  display.print("Starting");
  display.display();
  
  

////////////calculations

xx=l1*cos(defaultarmpos[4]*M_PI/180)+l2*cos((defaultarmpos[4]-defaultarmpos[3])*M_PI/180);
yy=l1*sin(defaultarmpos[4]*M_PI/180)+l2*sin((defaultarmpos[4]-defaultarmpos[3])*M_PI/180);
Serial1.print(acos(0.5)/M_PI*180);
Serial1.print(xx);
Serial1.print("\t");
Serial1.println(yy);




////////////////////////////
  delay(3000);
  digitalWrite(rodledrightfront,0);
digitalWrite(rodledleftback,0);
  display.clearDisplay();
  display.display();
}

void loop() {
if (millis() - asignvaluetime > 50) {
    getaccvalues();
    currentpos();
    asignvaluetime = millis();
  }

  
  //put your main code here, to run repeatedly:
  hipposition();
  //gripperposition();
  if(digitalRead(gripperopensw)==0||digitalRead(gripperclosesw)==0){
    Serial1.println("rotate");
    wristroll.write(180);
    delay(200);
    wristroll.write(90);
    delay(1000);
    wristroll.write(0);
    delay(200);
  }

if(digitalRead(rodbuttonbottom)==0||digitalRead(rodbuttontopmain)==0){
  wristpitchposition();
  if(digitalRead(rodbuttontopmain)==0&&AcX<-3500){
    if (xxposvar == 0) {
      xxpostime = millis();
      xxposvar = 1;
    }
    if (millis() - xxpostime > 1000/map(AcX,-3500,-16000,1,150)) {
      xx=xx+d;
      //Serial1.println(xx);
      xxpostime = millis();
    }
}
else{
  xxposvar = 1;
}
if(digitalRead(rodbuttontopmain)==0&&AcX>3500){
  
    if (xxnegvar == 0) {
      xxnegtime = millis();
      xxnegvar = 1;
    }
    if (millis() - xxnegtime > 1000/map(AcX,3500,16000,1,150)) {
      xx=xx-d;
      //Serial1.println(xx);
      xxnegtime = millis();
    }
}
else{
  xxnegvar = 1;
}

/////////////////////

  if(digitalRead(rodbuttontopmain)==0&&AcY<-3500){
    if (yyposvar == 0) {
      yypostime = millis();
      yyposvar = 1;
    }
    if (millis() - yypostime > 1000/map(AcY,-3500,-16000,1,150)) {
      yy=yy-d;
      //Serial1.println(yy);
      yypostime = millis();
    }
}
else{
  yyposvar = 1;
}
if(digitalRead(rodbuttontopmain)==0&&AcY>3500){
  
    if (yynegvar == 0) {
      yynegtime = millis();
      yynegvar = 1;
    }
    if (millis() - yynegtime > 1000/map(AcY,3500,16000,1,150)) {
      yy=yy+d;
      //Serial1.println(yy);
      yynegtime = millis();
    }
}
else{
  yynegvar = 1;
}

if(xx>xxmax){
  xx=prevxx;
}
if(xx<60){
  xx=prevxx;
}

if(yy>yymax){
  yy=prevyy;
}

if(yy<0){/////28 for 20 degree rotation
  yy=prevyy;
}


if(sq(xx)+sq(yy)-sq(l1+l2)<-maxcordbuffer&&sq(xx)+sq(yy)-sq(l1-l2)>maxcordbuffer){
  ///////////////calculations
totalangle=acos((xx/sqrt(sq(xx) + sq(yy))));//(y/x))
Serial.println(totalangle*(180/(PI)));
currentarmpos[3]=((sq(l1)+sq(l2)-sq(xx)-sq(yy)))/(2*l1*l2);
//if(currentarmpos[3]<0){currentarmpos[3]=-currentarmpos[3];}
currentarmpos[3]=180-(acos(currentarmpos[3])*(180/(PI)));
currentarmpos[4]=(sq(xx)+sq(yy)+sq(l1)-sq(l2))/(2*sqrt(sq(xx) + sq(yy))*l1);
currentarmpos[4]=(acos(currentarmpos[4])*(180/(PI)))+totalangle*(180/(PI));

Serial1.print(xx);
Serial1.print(": ");
Serial1.print(yy);
Serial1.print(": ");
Serial1.print(currentarmpos[3]+elbowdev);
Serial1.print("\t");
Serial1.println(currentarmpos[4]+sholderdev);

//currentarmpos[3]=defaultarmpos[3];
//currentarmpos[4]=defaultarmpos[4];

}

else{
  Serial1.println("Values exceded");
  xx=prevxx;
  yy=prevyy;
}

prevxx=xx;
prevyy=yy;


}
  
    displaycurrentarm();//only happens if button pressed
}
void defaultpos() {
  gripper.write(defaultarmpos[0]);
  wristM_PItch.write(defaultarmpos[1]);
  wristroll.write(defaultarmpos[2]);
  elbow.write(defaultarmpos[3]+elbowdev);
  sholder.write(defaultarmpos[4]+sholderdev);
  hip.write(defaultarmpos[5]);
}
void currentpos() {
  gripper.write(currentarmpos[0]);
  wristM_PItch.write(currentarmpos[1]);
  wristroll.write(currentarmpos[2]);
  elbow.write(currentarmpos[3]+elbowdev);
  sholder.write(currentarmpos[4]+sholderdev);
  hip.write(currentarmpos[5]);
}
void getaccvalues() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  /*
  if(AcX<3500&&AcX>-3500&&AcY<3500&&AcY>-3500){
    analogWrite(rodledrightfront,0);
  analogWrite(rodledleftback,0);
  }
else if(AcX<-3500){
  analogWrite(rodledleftback,map(AcX,-3500,-16000,0,255));
  analogWrite(rodledrightfront,0);
}
else if(AcY<-3500){
  analogWrite(rodledleftback,map(AcY,-3500,-16000,0,255));
  analogWrite(rodledrightfront,0);
}
else if(AcX>3500){
  analogWrite(rodledrightfront,map(AcX,3500,16000,0,255));
  analogWrite(rodledleftback,0);
}
else if(AcY>3500){
  analogWrite(rodledrightfront,map(AcY,3500,16000,0,255));
  analogWrite(rodledleftback,0);
}
else{
  analogWrite(rodledrightfront,0);
  analogWrite(rodledleftback,0);
}
*/
/*
Serial1.print(" | X = "); Serial1.print(AcX);
  Serial1.print(" | Y = "); Serial1.print(AcY);
  Serial1.print(" | Z = "); Serial1.println(AcZ);
  */
  /*
  
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("X: ");
    display.println(AcX);

    display.setTextSize(2);
    display.setCursor(0,20);
    display.print("Y: ");
    display.println(AcY);

    display.setTextSize(2);
    display.setCursor(0,40);
    display.print("Z: ");
    display.println(AcZ);
    display.display();
  */
}
void displaycurrentarm() {
  if (digitalRead(displaybutton) == 0) {
  display.clearDisplay();
  display.setTextSize(0);
  display.setCursor(0, 9);
  display.print("G:");
  display.println(currentarmpos[0]);

  display.setTextSize(0);
  display.setCursor(0, 18);
  display.print("WP:");
  display.println(currentarmpos[1]);

  display.setTextSize(0);
  display.setCursor(0, 27);
  display.print("WR:");
  display.println(currentarmpos[2]);

  display.setTextSize(0);
  display.setCursor(0, 36);
  display.print("E:");
  display.println(currentarmpos[3]);

  display.setTextSize(0);
  display.setCursor(0, 45);
  display.print("S:");
  display.println(currentarmpos[4]);

  display.setTextSize(0);
  display.setCursor(0, 54);
  display.print("H:");
  display.println(currentarmpos[5]);
  display.display();
}
}
void hipposition() {
  for (int x = 0; x < 20; x++) {
    potval += analogRead(POT) / 20;
  }
  potval = potval / 20;
  currentarmpos[5] = map(potval, 0, 205, 0, 180);
  //Serial1.println(map(potval,0,205,0,180));
  //hip.write(map(potval,0,205,0,180));
}
void gripperposition() {
if (digitalRead(gripperopensw) == 0) {
    if (gripperopenvar == 0) {
      gripperopentime = millis();
      gripperopenvar = 1;
    }
    if (millis() - gripperopentime > 1000 / gripperincrementpersec) {
      currentarmpos[0]++;
      Serial1.println(currentarmpos[0]);
    }

    if (currentarmpos[0] > grippermin) {
      currentarmpos[0] = grippermin;
    }
  }
  else {
    gripperopenvar = 0;
  }
  if (digitalRead(gripperclosesw) == 0) {
    if (gripperclosevar == 0) {
      gripperclosetime = millis();
      gripperclosevar = 1;
    }
    if (millis() - gripperclosetime > 1000 / gripperincrementpersec) {
      currentarmpos[0]--;
      Serial1.println(currentarmpos[0]);
    }

    if (currentarmpos[0] < grippermax) {
      currentarmpos[0] = grippermax;
    }
  }
  else {
    gripperclosevar = 0;
  }
}
void wristpitchposition(){
  if(digitalRead(rodbuttonbottom)==0&&AcX<-3500){
    if (wristupvar == 0) {
      wristuptime = millis();
      wristupvar = 1;
    }
    if (millis() - wristuptime > 1000/map(AcX,-3500,-16000,1,100)) {
      currentarmpos[1]--;
      Serial1.println(currentarmpos[1]);
      wristuptime = millis();
    }

    if (currentarmpos[1] < wristmin) {
      currentarmpos[1] = wristmin;
    }
}
else{
  wristupvar = 1;
}
if(digitalRead(rodbuttonbottom)==0&&AcX>3500){
    if (wristdownvar == 0) {
      wristdowntime = millis();
      wristdownvar = 1;
    }
    if (millis() - wristdowntime > 1000/map(AcX,3500,16000,1,100)) {
      currentarmpos[1]++;
      wristdowntime = millis();
      Serial1.println(currentarmpos[1]);
    }

    if (currentarmpos[1] > wristmax) {
      currentarmpos[1] = wristmax;
    }
}
else{
  wristdownvar = 1;
}
}
