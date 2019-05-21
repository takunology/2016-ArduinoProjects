#include<SPI.h>
#include <Wire.h>
#include<SD.h>
#define VCC  8
#define GND  9
#define SLAVE 10

char Moji[40];

char jikan[1] = {0};//10分毎電磁弁判定用の値
const int chipSelect = 8;
float Hanoura; // display value

//const int chipSelect = 4; //SD用チップ番号選ぶ

void setup() {
  Serial.begin(9600) ;        // シリアル通信の初期化

  //熱電対のセットアップ
#ifdef GND
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
#endif
#ifdef VCC
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);
#endif
  digitalWrite(SLAVE, HIGH);

  SPI.setDataMode(SPI_MODE0);
  pinMode(10, OUTPUT);

}


void loop() {
  for (int i = 0; i < 38; i++)
  {
    Moji[i] = Serial.read(); //送られてきた文字列をひたすら格納
  }
  
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

  //時刻の文字列埋め込み
  Year[0] = Moji[6];
  Year[1] = Moji[7];
  Year[2] = Moji[8];
  Year[3] = Moji[9];

  Month[0] = Moji[11];
  Month[1] = Moji[12];

  Day[0] = Moji[14];
  Day[1] = Moji[15];

  Hour[0] = Moji[17];
  Hour[1] = Moji[18];

  Min[0] = Moji[20];
  Min[1] = Moji[21];

  Sec[0] = Moji[23];
  Sec[1] = Moji[24];

  //気温の文字列埋め込み
  Kion[0] = Moji[25];
  Kion[1] = Moji[26];
  Kion[2] = Moji[27];
  Kion[3] = Moji[28];

  //水分量の文字列埋め込み
  Suibun[0] = Moji[30];
  Suibun[1] = Moji[31];
  Suibun[2] = Moji[32];

  //水温の文字列埋め込み
  Suion[0] = Moji[34];
  Suion[1] = Moji[35];
  Suion[2] = Moji[36];
  Suion[3] = Moji[37];

  //日の当たる部分の文字列埋め込み
  Nikkou[0] = Moji[1];
  Nikkou[1] = Moji[2];
  Nikkou[2] = Moji[3];
  Nikkou[3] = Moji[4];

  //条件分岐ができるようにint型へ変換
  int Byou = Sec[1];
  int Byou1 = Min[0];
  int Hun = Min[1];

  if (Hun == 48 && Byou == 48 && Byou1 == 48)
  {
    unsigned int thermocouple; // 14-Bit Thermocouple Temperature Data + 2-Bit
    unsigned int internal; // 12-Bit Internal Temperature Data + 4-Bit

    pinMode(10, OUTPUT);
    digitalWrite(SLAVE, LOW);                             //  Enable the chip
    thermocouple = (unsigned int)SPI.transfer(0x00) << 8;   //  Read high byte thermocouple
    thermocouple |= (unsigned int)SPI.transfer(0x00);       //  Read low byte thermocouple
    internal = (unsigned int)SPI.transfer(0x00) << 8;       //  Read high byte internal
    internal |= (unsigned int)SPI.transfer(0x00);           //  Read low byte internal
    digitalWrite(SLAVE, HIGH);                            //  Disable the chip

    //屋上温度(熱電対)/////////////////////////////////////////////////////
    if ((thermocouple & 0x0001) != 0) {

      if ((internal & 0x0004) != 0) {

      }
      if ((internal & 0x0002) != 0) {

      }
      if ((internal & 0x0001) != 0) {

      }
    } else {
      if ((thermocouple & 0x8000) == 0) { // 0℃以上   above 0 Degrees Celsius
        Hanoura = (thermocouple >> 2) * 0.25;
      } else {                          // 0℃未満   below zero
        Hanoura = (0x3fff - (thermocouple >> 2) + 1)  * -0.25;
      }
    }
  }

  //表示
  //if (Hun == 48 && Byou == 48 && Byou1 == 48)
  {
    int a = Sec[1];
    /*Serial.print(Year[0]);
      Serial.print(Year[1]);
      Serial.print(Year[2]);
      Serial.print(Year[3]);
      Serial.print("/");
      Serial.print(Month[0]);
      Serial.print(Month[1]);
      Serial.print("/");
      Serial.print(" ");
      Serial.print(Day[0]);
      Serial.print(Day[1]);
      Serial.print(" ");
      Serial.print(Hour[0]);
      Serial.print(Hour[1]);
      Serial.print(":");
      Serial.print(Min[0]);
      Serial.print(Min[1]);
      Serial.print(":");
      Serial.print(Sec[0]);
      Serial.print(Sec[1]);
      Serial.print(" ");
      Serial.print(" KION:");
      Serial.print(Kion[0]);
      Serial.print(Kion[1]);
      Serial.print(Kion[2]);
      Serial.print(Kion[3]);
      Serial.print(" ");
      Serial.print(" SUIBUN:");
      Serial.print(Suibun[0]);
      Serial.print(Suibun[1]);
      Serial.print(Suibun[2]);
      Serial.print(" ");
      Serial.print(" SUION:");
      Serial.print(Suion[0]);
      Serial.print(Suion[1]);
      Serial.print(Suion[2]);
      Serial.print(Suion[3]);
      Serial.print(" ");
      Serial.print(" NIKKOU:");
      Serial.print(Nikkou[0]);
      Serial.print(Nikkou[1]);
      Serial.print(Nikkou[2]);
      Serial.print(Nikkou[3]);
      Serial.print(" HIKAGE");
      Serial.print(Hanoura, 1);
      Serial.print("\n");
    */
  }

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if(dataFile)
  {
    dataFile.write(Year[0]);
      dataFile.write(Year[1]);
      dataFile.write(Year[2]);
      dataFile.write(Year[3]);
      dataFile.write("/");
      dataFile.write(Month[0]);
      dataFile.write(Month[1]);
      dataFile.write("/");
      Serial.write(" ");
      Serial.write(Day[0]);
      Serial.write(Day[1]);
      Serial.write(" ");
      Serial.write(Hour[0]);
      Serial.write(Hour[1]);
      Serial.write(":");
      Serial.write(Min[0]);
      Serial.write(Min[1]);
      Serial.write(":");
      Serial.write(Sec[0]);
      Serial.write(Sec[1]);
      Serial.write(" ");
      Serial.write(" KION:");
      Serial.write(Kion[0]);
      Serial.write(Kion[1]);
      Serial.write(Kion[2]);
      Serial.write(Kion[3]);
      Serial.write(" ");
      Serial.write(" SUIBUN:");
      Serial.write(Suibun[0]);
      Serial.write(Suibun[1]);
      Serial.write(Suibun[2]);
      Serial.write(" ");
      Serial.write(" SUION:");
      Serial.print(Suion[0]);
      Serial.print(Suion[1]);
      Serial.print(Suion[2]);
      Serial.print(Suion[3]);
      Serial.print(" ");
      Serial.print(" NIKKOU:");
      Serial.print(Nikkou[0]);
      Serial.print(Nikkou[1]);
      Serial.print(Nikkou[2]);
      Serial.print(Nikkou[3]);
      Serial.print(" HIKAGE");
      Serial.print(Hanoura, 1);
      Serial.print("\n");
      dataFile.close();
  }
  else
  {
    Serial.print("Error");
  }
  delay(1000);
}

