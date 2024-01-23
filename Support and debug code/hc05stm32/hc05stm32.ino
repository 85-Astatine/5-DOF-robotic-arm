#define LED PC13
char data;
void setup() {
  // put your setup code here, to run once:
Serial1.begin(9600);//rxA9   txA10
pinMode(LED,OUTPUT);
pinMode(PA12,INPUT_PULLUP);

digitalWrite(LED,1);
Serial1.print("Start");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial1.println(digitalRead(PA12));
if(Serial1.available()>0){
  data=Serial1.read();
  if(data=='A'){
    digitalWrite(LED,1);
    Serial1.print("LED ON\n");
  }
  if(data=='B'){
    digitalWrite(LED,0);
    Serial1.print("LED OFF\n");
  }
  Serial1.print(data);
}
}
