//郢晄ｧｭ縺�郢ｧ�ｽｳ郢晢ｽｳ邵ｺ�ｽｫ邵ｺ�ｽｪ邵ｺ�ｽｫ郢ｧ繧�笆ｽ邵ｺ�ｿｽ邵ｺ�ｽｦ邵ｺ�ｿｽ邵ｺ�ｽｪ邵ｺ�ｿｽ郢ｧ�ｿｽ邵ｺ�ｽ､
#include<SPI.h>
#include <Wire.h>
#include<SD.h>

#define VCCC 7
#define VCC  8
#define GND  9
#define SLAVE 10

float Hanoura; // display value
char Moji[20];


void setup() {
  Serial.begin(9600);        // 郢ｧ�ｽｷ郢晢ｽｪ郢ｧ�ｽ｢郢晢ｽｫ鬨ｾ螢ｻ�ｽｿ�ｽ｡邵ｺ�ｽｮ陋ｻ譎�謔�陋ｹ�ｿｽ
 // SPI.setDataMode(SPI_MODE0);
 SPI.begin();
 SPI.setBitOrder(MSBFIRST);
}

void loop() {
int i = 0;

int byou = Moji[13];

  while(Serial.available())
{
    Moji[i] = Serial.read(); //鬨ｾ竏夲ｽ臥ｹｧ蠕娯�ｻ邵ｺ髦ｪ笳�隴�ｿｽ陝�諤懶ｿｽ蜉ｱ�ｽ堤ｸｺ�ｽｲ邵ｺ貅倪�郢ｧ逕ｻ�ｿｽ�ｽｼ驍擾ｿｽ
    Serial.write(Moji[i]);
    i++;
}

if (byou % 2 == 1)
  {
    unsigned int thermocouple; // 14-Bit Thermocouple Temperature Data + 2-Bit
    unsigned int internal; // 12-Bit Internal Temperature Data + 4-Bit

    //霎ｭ�ｽｱ鬮ｮ�ｽｻ陝�ｽｾ邵ｺ�ｽｮ郢ｧ�ｽｻ郢晢ｿｽ郢晏現縺�郢晢ｿｽ郢晢ｿｽ
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

    //陞ｻ蛟ｶ�ｽｸ鬆托ｽｸ�ｽｩ陟趣ｽｦ(霎ｭ�ｽｱ鬮ｮ�ｽｻ陝�ｽｾ)/////////////////////////////////////////////////////
    if ((thermocouple & 0x0001) != 0) {

      if ((internal & 0x0004) != 0) {

      }
      if ((internal & 0x0002) != 0) {

      }
      if ((internal & 0x0001) != 0) {

      }
    } else {
      if ((thermocouple & 0x8000) == 0) { // 0驍�ｿｽ闔会ｽ･闕ｳ�ｿｽ   above 0 Degrees Celsius
        Hanoura = (thermocouple >> 2) * 0.25;
      } else {                          // 0驍�ｿｽ隴幢ｽｪ雋��ｿｽ   below zero
        Hanoura = (0x3fff - (thermocouple >> 2) + 1)  * -0.25;
      }
    }
  }

  char hikage[3];

  dtostrf(Hanoura, 2, 1, hikage);
  Serial.write(hikage);
  //Serial.print("\n");

  delay(1000);

}


