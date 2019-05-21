void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  float a = 0, b = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
float a = analogRead(11);
float b = analogRead(5);

 Serial.print(a);
 Serial.print(" ");
 Serial.print(b);
 Serial.print("\n");
 delay(500);
}
