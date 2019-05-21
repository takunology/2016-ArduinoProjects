#include <string.h>
#include <Wire.h>

// 電源起動時とリセットの時だけのみ処理する関数
void setup()
{
     Serial.begin(9600) ;               // シリアル通信の初期化
     Wire.begin() ;                     // I2Cの初期化、マスターとする
     delay(5000) ;                      // 5秒後に開始
}

// コード送信
void i2csend(char sendtxt[7])
{
     int i ;
     char dt[7] ;
     
     memcpy(dt,sendtxt,7) ;             // 送信データの準備
          
     Wire.beginTransmission(8) ;        // 通信の開始処理
     Wire.write(dt,7) ;                 // 通信データを準備する
     Wire.endTransmission() ;           // データの送信と終了処理
     
     delay(500) ;                       // スレーブがデータを準備するまで待つ
       
     Wire.requestFrom(8,7) ;            // スレーブ(8)にデータ送信要求(7byte受信)をだす
     for (i=0 ; i<7 ; i++) {            // 1byte × 7回データを取り出す
          dt[i] = Wire.read() ;
     }
     Serial.println(dt) ;               // IDEにシリアル通信で表示する
     // delay(100) ;
}

// メインの処理関数
void loop()
{
    i2csend("ASDFGHJ");                 // 送信文字列を関数へ引き渡す(7byte)
    i2csend("1234567");
}
