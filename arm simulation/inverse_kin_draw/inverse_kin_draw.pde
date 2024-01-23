float ang1=90;;
float ang2=100;
float l1=125;
float l2=125;
void setup() {
  scale(1, -1);
  translate(300, -height);
  size(800, 800);
}
void draw() {
  clear();
  scale(1, -1);
  translate(300, -height);
  stroke(128);
  strokeWeight(10);
  line(0, 0, l1*cos(ang1*PI/180), l1*sin(ang1*PI/180));
  stroke(255);
  strokeWeight(5);
  line(l1*cos(ang1*PI/180), l1*sin(ang1*PI/180), l1*cos(ang1*PI/180)+l2*cos((ang1-ang2)*PI/180), l1*sin(ang1*PI/180)+l2*sin((ang1-ang2)*PI/180));
  print(l1*cos(ang1*PI/180)+l2*cos((ang1-ang2)*PI/180));
  print("  ");
  print(l1*sin(ang1*PI/180)+l2*sin((ang1-ang2)*PI/180));
  print("\n ");
  delay(500);
}
