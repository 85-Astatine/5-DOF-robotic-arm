// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;



#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_STM32.h>
#define OLED_RESET     -1
Adafruit_SSD1306 display(OLED_RESET);//sda b7   sck b6
void setup(){
  delay(100);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial1.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
 delay(2);
 display.clearDisplay();
 display.setTextColor(WHITE);
 display.setTextSize(2);
 display.setCursor(0,5);
 display.print("MPU Angles");
 display.display();
 delay(3000);
}
void loop(){
  long start=micros();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
//Serial1.println(micros()-start); 
  //Serial1.print(" | Tmp = "); Serial1.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial1.print(" | X = "); Serial1.print(AcX);
  Serial1.print(" | Y = "); Serial1.print(AcY);
  Serial1.print(" | Z = "); Serial1.println(AcZ);
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

}
