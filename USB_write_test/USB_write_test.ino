#include<SPI.h>
#include<SD.h>

const int chipSelect = 4;
File file;
int a = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial){}
  if(!SD.begin(chipSelect)){
    return;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  a++;

    file = SD.open("test.txt", FILE_WRITE);
  if(file)
  {
    file.println(a++);
    file.close();
  }
}



