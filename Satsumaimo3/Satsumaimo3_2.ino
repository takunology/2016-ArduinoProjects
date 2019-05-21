//マイコンに赤いシールドが乗っているやつ
#include <Wire.h>
#include <SPI.h>
#include<SD.h>
char Moji[40];
const int chipSelect = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600) ;
  pinMode(10, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  File file = SD.open("datalog.csv", FILE_WRITE);

  file.print("Date,");
  file.print("Time");
  file.print("TEMP,");
  file.print("SUIBUN,");
  file.print("SUION,");
  file.print("NIKKOU,");
  file.print("DENJIBEN,");
  file.print("HIKAGE");
}

void loop() {
  int i = 0;
  while (Serial.available())
  {
    Moji[i] = Serial.read(); //送られてきた文字列をひたすら格納
    Serial.print(Moji[i]); //delay(100);
    i++;
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
  char hikage[4];
  char handan[1];

  //時刻の文字列埋め込み
  Year[0] = Moji[0];
  Year[1] = Moji[1];
  Year[2] = Moji[2];
  Year[3] = Moji[3];

  Month[0] = Moji[4];
  Month[1] = Moji[5];

  Day[0] = Moji[6];
  Day[1] = Moji[7];

  Hour[0] = Moji[8];
  Hour[1] = Moji[9];

  Min[0] = Moji[10];
  Min[1] = Moji[11];

  Sec[0] = Moji[12];
  Sec[1] = Moji[13];

  //気温の文字列埋め込み
  Kion[0] = Moji[14];
  Kion[1] = Moji[15];
  Kion[2] = Moji[16];
  Kion[3] = Moji[17];

  //水分量の文字列埋め込み
  Suibun[0] = Moji[18];
  Suibun[1] = Moji[19];
  Suibun[2] = Moji[20];

  //水温の文字列埋め込み
  Suion[0] = Moji[21];
  Suion[1] = Moji[22];
  Suion[2] = Moji[23];
  Suion[3] = Moji[24];

  //日の当たる部分の文字列埋め込み
  Nikkou[0] = Moji[25];
  Nikkou[1] = Moji[26];
  Nikkou[2] = Moji[27];
  Nikkou[3] = Moji[28];

  //電磁弁の判断
  handan[0] = Moji[29];

  //葉っぱの裏の文字列埋め込み
  hikage[0] = Moji[30];
  hikage[1] = Moji[31];
  hikage[2] = Moji[32];
  hikage[3] = Moji[33];

  int Byou = Sec[0];
  int Byou1 = Sec[1];


  if (Byou1 % 2 == 0)
  {
    //データ保存
    File file = SD.open("datalog.csv", FILE_WRITE);

    file.print(Year[0]);
    file.print(Year[1]);
    file.print(Year[2]);
    file.print(Year[3]);
    file.print("/");
    file.print(Month[0]);
    file.print(Month[1]);
    file.print("/");
    file.print(Day[0]);
    file.print(Day[1]);
    file.print(",");

    file.print(Hour[0]);
    file.print(Hour[1]);
    file.print(":");
    file.print(Min[0]);
    file.print(Min[1]);
    file.print(":");
    file.print(Sec[0]);
    file.print(Sec[1]);
    file.print(",");

    file.print(Kion[0]);
    file.print(Kion[1]);
    file.print(Kion[2]);
    file.print(Kion[3]);
    file.print(",");

    file.print(Suibun[0]);
    file.print(Suibun[1]);
    file.print(Suibun[2]);
    file.print(",");

    file.print(Suion[0]);
    file.print(Suion[1]);
    file.print(Suion[2]);
    file.print(Suion[3]);
    file.print(",");

    file.print(Nikkou[0]);
    file.print(Nikkou[1]);
    file.print(Nikkou[2]);
    file.print(Nikkou[3]);
    file.print(",");

    file.print(handan[0]);
    file.print(",");

    file.print(hikage[0]);
    file.print(hikage[1]);
    file.print(hikage[2]);
    file.print(hikage[3]);
    file.print(",");

    file.println();
    file.close();
  }
  else
  {
    delay(1000);
  }

  delay(60000);
}
