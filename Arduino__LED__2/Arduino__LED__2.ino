void setup()
{
  pinMode(1, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  for (int i = 1; i <= 3; i++)
  {
    digitalWrite(1, HIGH);
    delay(70);
    digitalWrite(1, LOW);
    digitalWrite(4, HIGH);
    delay(70);
    digitalWrite(4, LOW);
    digitalWrite(7, HIGH);
    delay(70);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    delay(70);
    digitalWrite(8, LOW);
    digitalWrite(12, HIGH);
    delay(70);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
    delay(70);
    digitalWrite(13, LOW);

    digitalWrite(13, HIGH);
    delay(70);
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    delay(70);
    digitalWrite(12, LOW);
    digitalWrite(8, HIGH);
    delay(70);
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
    delay(70);
    digitalWrite(7, LOW);
    digitalWrite(4, HIGH);
    delay(70);
    digitalWrite(4, LOW);
    digitalWrite(1, HIGH);
    delay(70);
    digitalWrite(1, LOW);
  }
  delay(1000);
  for (int i = 1; i <= 5; i++ )
  {
    digitalWrite(1, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    delay(100);

    digitalWrite(1, LOW);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    delay(100);
  }
  delay(1500);
}

