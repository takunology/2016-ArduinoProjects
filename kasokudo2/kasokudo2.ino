#include <Wire.h>

void setup() {
  int id;
  Serial.begin(9600);
  //Serial.println("setup");
}

 void loop(){
  int val[3];
  char buff[15];
  adxl_read(val);

  sprintf(buff,"acc:%d",val[2]);
  lcd_setCursor(0,0);
  lcd_print(buff);
  
  sprintf(buff,"%d,%d",val[0],val[1]);
  lcd_setCursor(0,1);
  lcd_print(buff);
  
  sprintf(buff,"%d,%d,%d",val[0],val[1],val[2]);
  Serial.println(buff);
  delay(500);   
 }
