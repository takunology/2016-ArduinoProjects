#include <SD.h>
#include<SPI.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include <TimeLib.h>
const int chipSelect = 5;

int B = 3975;
String S;
float O;
#define VCC  8
#define GND  9

#define DENJI1 2
#define DENJI2 3
#define Test 4

#define SLAVE 10

void setup() {

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

  pinMode(Test, OUTPUT);
  digitalWrite(Test, HIGH);
}

void loop() {
  unsigned int thermocouple; // 14-Bit Thermocouple Temperature Data + 2-Bit
  unsigned int internal; // 12-Bit Internal Temperature Data + 4-Bit
  float disp; // display value

  delay(500);
  digitalWrite(SLAVE, LOW);                             //  Enable the chip
  thermocouple = (unsigned int)SPI.transfer(0x00) << 8;   //  Read high byte thermocouple
  thermocouple |= (unsigned int)SPI.transfer(0x00);       //  Read low byte thermocouple
  internal = (unsigned int)SPI.transfer(0x00) << 8;       //  Read high byte internal
  internal |= (unsigned int)SPI.transfer(0x00);           //  Read low byte internal
  digitalWrite(SLAVE, HIGH);                            //  Disable the chip

  float kion = analogRead(0);
  float okujyou = analogRead(1);
  float suibun = analogRead(2);

  //現在時間/////////////////////////////////////////////////////////
  tmElements_t tm;
  if (RTC.read(tm)) {
    if (tm.Second == 0)
    {
      Serial.print(tmYearToCalendar(tm.Year));
      Serial.write('/');
      Serial.print(tm.Month);
      Serial.write('/');
      Serial.print(tm.Day);
      Serial.print(" ");
      print2digits(tm.Hour);
      Serial.write(':');
      print2digits(tm.Minute);
      Serial.write(':');
      print2digits(tm.Second);
      Serial.print("  ");
    }
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }

  }

  //気温(温度センサー)///////////////////////////////////////////////
  float R = (1023 - kion) * 10000 / kion;
  float T = 1 / (log(R / 10000) / B + 1 / 298.15) - 273.15;

  //水温(熱電対)/////////////////////////////////////////////////////
  if ((thermocouple & 0x0001) != 0) {
    Serial.print("ERROR: ");
    if ((internal & 0x0004) != 0) {
      Serial.print("Short to Vcc, ");
    }
    if ((internal & 0x0002) != 0) {
      Serial.print("Short to GND, ");
    }
    if ((internal & 0x0001) != 0) {
      Serial.print("Open Circuit, ");
    }
    Serial.println("\n");
  } else {
    if ((thermocouple & 0x8000) == 0) { // 0℃以上   above 0 Degrees Celsius
      disp = (thermocouple >> 2) * 0.25;
    } else {                          // 0℃未満   below zero
      disp = (0x3fff - (thermocouple >> 2) + 1)  * -0.25;
      Serial.print("SUION:");
      Serial.print(disp);
    }

    //地表温度(非接触温度ユニット)/////////////////////////////////////
    float V = okujyou * 5 / 1024;
    float O = -3.4248 * V * V + 49.898 * V - 64.227;

    //水分量///////////////////////////////////////////////////////////
    //時間の処理が必要
    if (tm.Second >= 0 && tm.Second <= 30)
    {

          //電磁弁のピン状態
          pinMode(DENJI1, OUTPUT);
          digitalWrite(DENJI1, HIGH); //イモの弁を開ける
          pinMode(DENJI2, OUTPUT);
          digitalWrite(DENJI2, LOW); //排水の弁を閉じる
          S = "O";
    }
    if(tm.Second >= 31 && tm.Second <= 40)
    {
          //電磁弁のピン状態
          pinMode(DENJI1, OUTPUT);
          digitalWrite(DENJI1, LOW); //イモの弁を閉じる
          pinMode(DENJI2, OUTPUT);
          digitalWrite(DENJI2, HIGH); //排水の弁を開ける
    }
    if(tm.Second >= 41 && tm.Second <= 59)
    {
          pinMode(DENJI1, OUTPUT);
          digitalWrite(DENJI1, LOW); //イモの弁を閉じる
          pinMode(DENJI2, OUTPUT);
          digitalWrite(DENJI2, LOW); //排水の弁を開ける
    }

  }

  //表示/////////////////////////////////////////////////////////////

  if (tm.Second == 0)
  {
    Serial.print("KION:"); //気温
    Serial.print(T);
    Serial.print("   ");
    Serial.print("TIHYOU:"); //地表温度(日光部分)非接触
    Serial.print(O);
    Serial.print("   ");
    Serial.print("SUION:"); //水温(熱電対①)
    Serial.print(disp);
    Serial.print("   ");
    Serial.print("SUIBUN:"); //水分
    Serial.print(suibun);
    Serial.print("   ");
    Serial.print("Hyouka: "); //電磁弁動作判定
    Serial.print(S);
    Serial.print("\n");

    ////文字列送信処理/////////////////
    Wire.begin(8);
    Wire.write("Hello");
  }
  int i = 1;
     File dataFile = SD.open("data.txt", FILE_WRITE);
    dataFile.println( T + O + disp + suibun + S + i);
    dataFile.close();
  delay(1000);

}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

