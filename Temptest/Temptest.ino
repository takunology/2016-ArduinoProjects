#include<math.h>
#include<Wire.h>
#include<LiquidCrystal.h>
float ain;
int suibun;
int B = 3975;
float T = 0, R = 0;

LiquidCrystal lcd(16, 17, 5, 4, 3, 1);

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

void setup() {
  
Serial.begin(9600);
lcd.begin(16, 2);
lcd.begin(16, 2);
lcd.print("Hello");
}

void loop() {
  // put your main code here, to run repeatedly:
ain = analogRead(0);
suibun = analogRead(1);

//temp = ((5*ain)/1024)*100;

R = (1023 - ain) * 10000 / ain;
T = 1 / (log(R / 10000) / B + 1 / 298.15) - 273.15;
Serial.print(suibun);

 String i;
if(suibun <  570.0 || suibun == 0.0) //570未満なら水あげる
{
  i = "A";
  Serial.print(i);
}
else
{
  i = "S";
  Serial.print(i);
}

Serial.print(" ");
Serial.print(T);
Serial.print("\n");

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Hello");

delay(1000);
}
