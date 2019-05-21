String s;
char c[4];
void setup() {

  Serial.begin(9600);

}
 
  static void main(String[] args)
  {
    c = new char[]{'A', 'B','C'};
    s = new String(c);
  }


void loop() {
    
  Serial.print(s);
}

