#include <Wire.h>

char dt[10] ;

// データを受信すると処理される割込み関数
void receiveEvent(int howMany)
{
     int i ;
  
     if(Wire.available()) {
         for (i=0 ; i<7 ; i++) {
            dt[i] = Wire.read()  ;      // 7byte受信する
         }
     }
     Serial.print(dt) ;                 // IDEにシリアル通信で表示する
}

// データ要求を受信すると処理される割込み関数
void requestEvent() 
{
    int i ;
    Wire.write(dt,7) ;                  // マスターに7byte送信する
}

// 電源起動時とリセットの時だけのみ処理する関数
void setup()
{
     Serial.begin(9600) ;               // シリアル通信の初期化
     Wire.begin(8) ;                    // I2Cの初期化、マイアドレスは8とする
     Wire.onRequest(requestEvent) ;     // 割込み関数の登録
     Wire.onReceive(receiveEvent) ;     // 割込み関数の登録
}

// メインの処理関数
void loop()
{
}
