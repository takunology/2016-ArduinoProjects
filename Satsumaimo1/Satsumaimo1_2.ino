//マイコンにGROVEシールドがのっているやつ
#include<SPI.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include <TimeLib.h>

int B = 3975;//温度センサー用定数
char S[1];//弁の状態
float O;//非接触温度センサユニット用
float suion; //熱電対(水温用)
int Denji = 0; 

#define VCCC 7
#define GNDD 6
#define VCC  8
#define GND  9

#define DENJI1 2 //イモの弁
#define DENJI2 3 //排水の弁
#define Test 4

#define SLAVE 10

//初期化/////////////////////////////////////////////////
void setup() {


  Serial.begin(9600);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);

  //電磁弁のピン状態
  pinMode(DENJI1, OUTPUT);
  digitalWrite(DENJI1, LOW); //出力0V

  //電磁弁2個目のピン状態
  pinMode(DENJI2, OUTPUT);
  digitalWrite(DENJI2, LOW);

  pinMode(VCCC, OUTPUT);
  digitalWrite(VCCC, HIGH);

  pinMode(GNDD, OUTPUT);
  digitalWrite(GNDD, LOW);

  pinMode(Test, OUTPUT);
  digitalWrite(Test, HIGH);

}

//時計用関数処理///////////////////////////////////////////
void print2digits(int number) {
  if (number >= 0 && number < 10) {
//    Serial.write('0');
  }
//  Serial.print(number);
}

//ループ開始////////////////////////////////////////////////
void loop() {

  float kion = analogRead(0);
  float suibun = analogRead(1);
  float okujyou = analogRead(2);

if(750 <= suibun)
{
  Denji = 1; //1 = false
}

  //現在時間の取得
  tmElements_t tm;
  if (RTC.read(tm)) {
  }
  
  //気温センサー
  float R = (1023 - kion) * 10000 / kion;
  float T = 1 / (log(R / 10000) / B + 1 / 298.15) - 273.15;

  //非接触温度ユニット
  float V = okujyou * 5 / 1024;
  float O = -3.4248 * V * V + 49.898 * V - 64.227;

  //熱電対(水温)
  if (tm.Second % 2 == 1)
  {
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

    unsigned int thermocouple; // 14-Bit Thermocouple Temperature Data + 2-Bit
    unsigned int internal; // 12-Bit Internal Temperature Data + 4-Bit

    digitalWrite(SLAVE, LOW);                             //  Enable the chip
    thermocouple = (unsigned int)SPI.transfer(0x00) << 8;   //  Read high byte thermocouple
    thermocouple |= (unsigned int)SPI.transfer(0x00);       //  Read low byte thermocouple
    internal = (unsigned int)SPI.transfer(0x00) << 8;       //  Read high byte internal
    internal |= (unsigned int)SPI.transfer(0x00);           //  Read low byte internal
    digitalWrite(SLAVE, HIGH);                            //  Disable the chip

    //屋上温度(熱電対)/////////////////////////////////////////////////////
    if ((thermocouple & 0x0001) != 0) {
      //printf("ERROR: ");
      if ((internal & 0x0004) != 0) {
       // printf("Short to Vcc, ");
      }
      if ((internal & 0x0002) != 0) {
      //  printf("Short to GND, ");
      }
      if ((internal & 0x0001) != 0) {
      //  printf("Open Circuit, ");
      }
     // printf("\n");
    } else {
      if ((thermocouple & 0x8000) == 0) { // 0℃以上   above 0 Degrees Celsius
        suion = (thermocouple >> 2) * 0.25;
      } else {                          // 0℃未満   below zero
        suion = (0x3fff - (thermocouple >> 2) + 1)  * -0.25;
      }
    }
  }

  //電磁弁の処理
  if (tm.Hour >= 4 && tm.Hour <= 6 || tm.Hour >= 18 && tm.Hour <= 20)
  {
    if (Denji == 0)//false
    {
      if (suion < 31.0) //水温が31度未満ならば
      {
        //電磁弁のピン状態
        pinMode(DENJI1, OUTPUT);
        digitalWrite(DENJI1, HIGH); //イモの弁を開ける
        pinMode(DENJI2, OUTPUT);
        digitalWrite(DENJI2, LOW); //排水の弁を閉じる
        S[1] = 'O';
      }
      else //水温が25度を超えていたら
      {
        //電磁弁のピン状態
        pinMode(DENJI1, OUTPUT);
        digitalWrite(DENJI1, LOW); //イモの弁を閉じる
        pinMode(DENJI2, OUTPUT);
        digitalWrite(DENJI2, HIGH); //排水の弁を開ける
      }
    }
    else //水分量が600を超えていたら
    {
      //電磁弁のピン状態
      pinMode(DENJI1, OUTPUT);
      digitalWrite(DENJI1, LOW); //イモ側の弁を閉じる
      pinMode(DENJI2, OUTPUT);
      digitalWrite(DENJI2, LOW); //排水の弁を閉じる
      S[1] = 'X';
    }
  }
  else //時間外ならば
  {
    //電磁弁のピン状態
    pinMode(DENJI1, OUTPUT);
    digitalWrite(DENJI1, LOW); //イモ側の弁を閉じる
    pinMode(DENJI2, OUTPUT);
    digitalWrite(DENJI2, LOW); //排水の弁を閉じる
    S[1] = 'X';
  }

  
if(tm.Hour == 11)
{
  Denji = 0;
}

  //表示
  /*
    Serial.print("KION:"); //気温
    Serial.print(T);
    Serial.print("   ");
    Serial.print("TIHYOU:"); //地表温度(日光部分)非接触
    Serial.print(O);
    Serial.print("   ");
    Serial.print("SUION:"); //水温(熱電対①)
    Serial.print(suion);
    Serial.print("    ");
    Serial.print("SUIBUN:"); //水分
    Serial.print(suibun);
    Serial.print("   ");
    Serial.print("Hyouka: "); //電磁弁動作判定
    Serial.print(S);
    Serial.print("\n");
  */

  //シリアル通信用の文字列配列//////////////////////////////////
  char Temp[5];
  char Suibun[5];
  char Suion[5];
  char Nikkou[5];
  char Handan[1];

  char YEAR[5];
  char MON[5];
  char DAY[5];
  char HOUR[5];
  char Min[5];
  char Sec[5];

  //文字列の参照と有効数字範囲の指定、配列の指定//////////////////
  dtostrf(T , 2, 1, Temp);
  dtostrf(suibun, 3, 0, Suibun);
  dtostrf(suion, 2, 1, Suion);
  dtostrf(O , 2, 1, Nikkou);

  dtostrf((tmYearToCalendar(tm.Year)), 4, 0, YEAR);
  dtostrf(tm.Month, 2, 0, MON);
  dtostrf(tm.Day, 2, 0, DAY);
  dtostrf(tm.Hour, 2, 0, HOUR);
  dtostrf(tm.Minute, 2, 0, Min);
  dtostrf(tm.Second, 2, 0, Sec);

  //シリアル送信
  //if (tm.Minute % 5 == 0)
  {
    Serial.write(YEAR);
    //Serial.print(" ");
    Serial.write(MON);
    //Serial.print(" ");
    Serial.write(DAY);
    //Serial.print(" ");
    Serial.write(HOUR);
    //Serial.print(" ");
    Serial.write(Min);
    //Serial.print(" ");
    Serial.write(Sec);
    //Serial.print(" ");
    Serial.write(Temp);
    //Serial.print(" ");
    Serial.write(Suibun);
    //Serial.print(" ");
    Serial.write(Suion);
    //Serial.print(" ");
    Serial.write(Nikkou);
    Serial.write(S[1]);
    //Serial.println();
  }

  delay(1000);

}
