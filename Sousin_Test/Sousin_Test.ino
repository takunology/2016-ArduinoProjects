#include<Wire.h>
int a = 0;
int b = 10;
int c = 20;
float d = 12.3;
String u = "23.1";

void setup() {
  Serial.begin(9600);
}

void loop() {
  byte * bx = (byte *) &d;
  Serial.write(a++); // int型データの送信
  Serial.write(b++);
  Serial.write(c++);
  Serial.write(bx[4]);
  Serial.write(bx[3]);
  Serial.write(bx[2]); 
  Serial.println(u);
  delay(100);
 
  
}
