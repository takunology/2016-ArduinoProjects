#include <common.h>
#include <SFE_CC3000.h>
#include <SFE_CC3000_Callbacks.h>
#include <SFE_CC3000_Client.h>
#include <SFE_CC3000_SPI.h>
#include<SPI.h>
#include <Wire.h>
#define VCC  8
#define GND  9
#define SLAVE 10

//WIFI設定//////
char ap_ssid[] = "Tphone";
char ap_password[] = "takuyan0910";
unsigned int ap_security = WLAN_SEC_UNSEC;
//Sparkfunクラウドの認証
const String publicKey = "7J0QQRV3w3tGZaJAoK0G";
const String privatekey = "mznPPvKWXWiGjNqDVAyG";
const byte NUM_FIELDS = 6;
const String fieldNames[NUM_FIELDS] = {"casetemp","date","shadeplace","sunnyplace","water","watertemp"};
String fieldData[NUM_FIELDS];
//WiFiシールドの設定
#define CC3000_INT      2   // Needs to be an interrupt pin (D2/D3)
#define CC3000_EN       7   // Can be any digital pin
#define CC3000_CS       10  // Preferred is pin 10 on Uno
#define IP_ADDR_LEN     4   // Length of IP address in bytes
SFE_CC3000 wifi = SFE_CC3000(CC3000_INT, CC3000_EN, CC3000_CS);
SFE_CC3000_Client client = SFE_CC3000_Client(wifi);
unsigned int timeout = 30000;   


char Moji[40];

char jikan[1] = {0};//10分毎電磁弁判定用の値
const int chipSelect = 4;
float Hanoura; // display value

String mojiretu; 



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

 setupWiFi();

}

void loop() {

  //受信データの受け取り
  for (int i = 0; i < 38; i++)
  {
    Moji[i] = Serial.read(); //送られてきた文字列をひたすら格納
    //Serial.print(Moji[i]);
   mojiretu = String(Moji[i]);
  }

  //Serial.print(mojiretu);
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
//  fieldData = String();
//  Serial.print(m);

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
      // Serial.print("ERROR: ");
      if ((internal & 0x0004) != 0) {
        // Serial.print("Short to Vcc, ");
      }
      if ((internal & 0x0002) != 0) {
        // Serial.print("Short to GND, ");
      }
      if ((internal & 0x0001) != 0) {
        // Serial.print("Open Circuit, ");
      }
      //Serial.println("\n");
    } else {
      if ((thermocouple & 0x8000) == 0) { // 0℃以上   above 0 Degrees Celsius
        Hanoura = (thermocouple >> 2) * 0.25;
      } else {                          // 0℃未満   below zero
        Hanoura = (0x3fff - (thermocouple >> 2) + 1)  * -0.25;
        //Serial.print("SUION:");
        //Serial.print(Hanoura);

      }
    }
  }

  Serial.print(Year);

  //表示
  //if (Hun == 48 && Byou == 48 && Byou1 == 48)
  {
    int a = Sec[1];
    //Serial.print("ASCII Code: ");
    //Serial.println(a);
    //Serial.println();
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
  */}
  delay(1000);
}

void setupWiFi()
{
  ConnectionInfo connection_info;
  int i;
  
  // Initialize CC3000 (configure SPI communications)
  if ( wifi.init() )
  {
    Serial.println(F("CC3000 Ready!"));
  }
  else
  {
    // Error: 0 - Something went wrong during CC3000 init!
    Serial.println(F("Error: 0"));
  }
  
  // Connect using DHCP
  Serial.print(F("Connecting to: "));
  Serial.println(ap_ssid);
  if(!wifi.connect(ap_ssid, ap_security, ap_password, timeout))
  {
    // Error: 1 - Could not connect to AP
    Serial.println(F("Error: 1"));
  }
  
  // Gather connection details and print IP address
  if ( !wifi.getConnectionInfo(connection_info) ) 
  {
    // Error: 2 - Could not obtain connection details
    Serial.println(F("Error: 2"));
  }
  else
  {
    Serial.print(F("My IP: "));
    for (i = 0; i < IP_ADDR_LEN; i++)
    {
      Serial.print(connection_info.ip_address[i]);
      if ( i < IP_ADDR_LEN - 1 )
      {
        Serial.print(".");
      }
    }
    Serial.println();
  }
}

