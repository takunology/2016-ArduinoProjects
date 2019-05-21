#include <DS1307RTC.h>
#include <Time.h>
#include <TimeLib.h>
#include<SPI.h>

void setup() {
  Serial.begin(9600);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
}

void loop() {
  tmElements_t tm;
  if (RTC.read(tm)) {
    if (tm.Minute % 10 == 0 && tm.Second == 0)
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

  delay(1000);

}

  void print2digits(int number) {
    if (number >= 0 && number < 10) {
      Serial.write('0');
    }
    Serial.print(number);
  }
