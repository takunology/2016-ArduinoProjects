//繝槭う繧ｳ繝ｳ縺ｫ縺ｪ縺ｫ繧ゅ▽縺�縺ｦ縺�縺ｪ縺�繧�縺､
#include<SPI.h>
#include <Wire.h>
#include<SD.h>
#define VCC  8
#define GND  9
#define SLAVE 10

int kaisu = 0;

float Hanoura; // display value
char Moji[40];

void setup() {
  Serial.begin(9600);        // 繧ｷ繝ｪ繧｢繝ｫ騾壻ｿ｡縺ｮ蛻晄悄蛹�
  SPI.setDataMode(SPI_MODE0);
}

void loop() {

Serial.begin(9800);

if(Serial.available())
  for (int i = 0; i < 38; i++)
  {
    Moji[i] = Serial.read(); //騾√ｉ繧後※縺阪◆譁�蟄怜�励ｒ縺ｲ縺溘☆繧画�ｼ邏�
    //Serial.print(Moji[i]);
    delay(10);
    Serial.write(Moji[i]);
  }

   //Serial.println();

  char Kion[2];
  char Suibun[1];
  char Nikkou[2];
  char Suion [2];

  char Year[2];
  char Month[1];
  char Day[1];
  char Hour[1];
  char Min[1];
  char Sec[1];
  char hikage[4];

  //譎ょ綾縺ｮ譁�蟄怜�怜沂繧∬ｾｼ縺ｿ
  Year[0] = Moji[0];
  Year[1] = Moji[1];
  Year[2] = Moji[2];
  Year[3] = Moji[3];

  Month[0] = Moji[5];
  Month[1] = Moji[6];

  Day[0] = Moji[8];
  Day[1] = Moji[9];

  Hour[0] = Moji[11];
  Hour[1] = Moji[12];

  Min[0] = Moji[14];
  Min[1] = Moji[15];

  Sec[0] = Moji[17];
  Sec[1] = Moji[18];

  //豌玲ｸｩ縺ｮ譁�蟄怜�怜沂繧∬ｾｼ縺ｿ
  Kion[0] = Moji[20];
  Kion[1] = Moji[21];
  Kion[2] = Moji[22];
  Kion[3] = Moji[23];

  //豌ｴ蛻�驥上�ｮ譁�蟄怜�怜沂繧∬ｾｼ縺ｿ
  Suibun[0] = Moji[25];
  Suibun[1] = Moji[26];
  Suibun[2] = Moji[27];

  //豌ｴ貂ｩ縺ｮ譁�蟄怜�怜沂繧∬ｾｼ縺ｿ
  Suion[0] = Moji[29];
  Suion[1] = Moji[30];
  Suion[2] = Moji[31];
  Suion[3] = Moji[32];

  //譌･縺ｮ蠖薙◆繧矩Κ蛻�縺ｮ譁�蟄怜�怜沂繧∬ｾｼ縺ｿ
  Nikkou[0] = Moji[33];
  Nikkou[1] = Moji[34];
  Nikkou[2] = Moji[35];
  Nikkou[3] = Moji[36];

  //譚｡莉ｶ蛻�蟯舌′縺ｧ縺阪ｋ繧医≧縺ｫint蝙九∈螟画鋤
  int Byou = Sec[1];
  int Byou1 = Min[0];
  int Hun = Min[1];

  if(kaisu == 100)
  {
    kaisu = 0;
  }

if (kaisu == 999)
  {
    unsigned int thermocouple; // 14-Bit Thermocouple Temperature Data + 2-Bit
    unsigned int internal; // 12-Bit Internal Temperature Data + 4-Bit

    //辭ｱ髮ｻ蟇ｾ縺ｮ繧ｻ繝�繝医い繝�繝�
#ifdef GND
    pinMode(GND, OUTPUT);
    digitalWrite(GND, LOW);
#endif
#ifdef VCC
    pinMode(VCC, OUTPUT);
    digitalWrite(VCC, HIGH);
#endif
    pinMode(SLAVE, OUTPUT);
    digitalWrite(SLAVE, HIGH);

    digitalWrite(SLAVE, LOW);                             //  Enable the chip
    thermocouple = (unsigned int)SPI.transfer(0x00) << 8;   //  Read high byte thermocouple
    thermocouple |= (unsigned int)SPI.transfer(0x00);       //  Read low byte thermocouple
    internal = (unsigned int)SPI.transfer(0x00) << 8;       //  Read high byte internal
    internal |= (unsigned int)SPI.transfer(0x00);           //  Read low byte internal
    digitalWrite(SLAVE, HIGH);                            //  Disable the chip

    //螻倶ｸ頑ｸｩ蠎ｦ(辭ｱ髮ｻ蟇ｾ)/////////////////////////////////////////////////////
    if ((thermocouple & 0x0001) != 0) {

      if ((internal & 0x0004) != 0) {

      }
      if ((internal & 0x0002) != 0) {

      }
      if ((internal & 0x0001) != 0) {

      }
    } else {
      if ((thermocouple & 0x8000) == 0) { // 0邃�莉･荳�   above 0 Degrees Celsius
        Hanoura = (thermocouple >> 2) * 0.25;
      } else {                          // 0邃�譛ｪ貅�   below zero
        Hanoura = (0x3fff - (thermocouple >> 2) + 1)  * -0.25;
      }
    }
  }
  kaisu++;
  /*
    //蟷ｴ
    Serial.write(Year[0]);
    Serial.write(Year[1]);
    Serial.write(Year[2]);
    Serial.write(Year[3]);
    //譛�
    Serial.write(Month[0]);
    Serial.write(Month[1]);
    //譌･
    Serial.write(Day[0]);
    Serial.write(Day[1]);
    //譎�
    Serial.write(Hour[0]);
    Serial.write(Hour[1]);
    //蛻�縲�
    Serial.write(Min[0]);
    Serial.write(Min[1]);
    //遘�
    Serial.write(Sec[0]);
    Serial.write(Sec[1]);
    //豌玲ｸｩ
    Serial.write(Kion[0]);
    Serial.write(Kion[1]);
    Serial.write(Kion[2]);
    Serial.write(Kion[3]);
    //豌ｴ蛻�驥�
    Serial.write(Suibun[0]);
    Serial.write(Suibun[1]);
    Serial.write(Suibun[2]);
    //豌ｴ貂ｩ
    Serial.write(Suion[0]);
    Serial.write(Suion[1]);
    Serial.write(Suion[2]);
    Serial.write(Suion[3]);
    //譌･蜈画ｸｩ蠎ｦ
    Serial.write(Nikkou[0]);
    Serial.write(Nikkou[1]);
    Serial.write(Nikkou[2]);
    Serial.write(Nikkou[3]);
  */ //譌･髯ｰ貂ｩ蠎ｦ
  dtostrf(Hanoura, 2, 1, hikage);
  Serial.write(hikage);
  //Serial.print("\n");

  delay(1000);

}

