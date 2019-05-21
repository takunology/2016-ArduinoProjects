#include <SPI.h>
 
#define SPI_CS_ON   digitalWrite(SPI_cs, LOW)      // /cs = L
#define SPI_CS_OFF  digitalWrite(SPI_cs, HIGH)     // /cs = H
 
const int SPI_cs = 9;    // 熱電対センサモジュール CSポート番号
 
byte spiDat[4];
char strBuf[16];
 
// -------------------------
// 初期化
// -------------------------
void setup()
{
  Serial.begin(19200);              // シリアルポート初期化
 
  pinMode(SPI_cs, OUTPUT);          // センサモジュール cs
  SPI_CS_OFF;
 
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);    // SPIクロック分周比
  SPI.setBitOrder(MSBFIRST);              // 送信ビット順番
  SPI.setDataMode(SPI_MODE1);             // SPIデータモード切替
}
 
// -------------------------
// メインループ
// -------------------------
void loop() {
  word val;
  byte i;
 
  SPI_CS_ON;
  for(i = 0; i < 2; i++) {          // 2バイトだけ受信
    spiDat[i] = SPI.transfer(0);    // ダミーデータ送信(1バイト受信)
  }
  SPI_CS_OFF;
 
  val = ((word)spiDat[0] << 8) | (word)spiDat[1];
 
  getTemp(val, strBuf);
  Serial.println(strBuf);
 
  delay(1000);      // 1秒
}
 
//
// SPIで読み出したデータを温度値の文字列にシリアル送信する
//
void getTemp(word spival16, char *str) {
  word val, vali;
  byte sign, vals;
 
  val = spival16 >> 2;
 
  if(val & 0x2000) {  // 符号ビットをチェック
    // 負数(2の補数)
    val = ~val + 1;   // 正数に戻す
    val &= 0x3FFF;    // 数値抽出
    sign = true;      // 負数マーク
  } else {
    // 正数
    sign = false;
  }
 
  // 整数部分と小数部分に分ける
  vali = val >> 2;          // 整数部(上位11ビット)
  vals = (byte)val & 0x03;  // 小数部(下位2ビット)
  vals = vals * 25;         // 25=0.25*100 小数部を整数化
 
  // 表示用文字列作成
  if(sign) {
    // 負数
    sprintf(str, "-%d.%-2d", vali, vals);
  } else {
    // 整数
    sprintf(str, "+%d.%-2d", vali, vals);
  }
}

