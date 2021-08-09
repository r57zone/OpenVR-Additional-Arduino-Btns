float Axis[5];

void setup() {
  //Setup pins
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  
  Serial.begin(115200);
}
void loop() {
  Axis[0] = 0;
  Axis[1] = 0;
  Axis[2] = 0;
  Axis[3] = 0;
  Axis[4] = 0;

  if (digitalRead(3) == LOW) {
    Axis[0] = -1;
    Axis[1] = 1;
  }

  if (digitalRead(4) == LOW) {
     Axis[0] = 1;
     Axis[1] = -1;
  }
    

  if (digitalRead(5) == LOW) {
    Axis[2] = -1;
    Axis[3] = 1;
  }

  if (digitalRead(6) == LOW) {
    Axis[2] = 1;
    Axis[3] = -1;
  }

  if (digitalRead(7) == LOW)
    Axis[4] = 1;

  if (digitalRead(8) == LOW)
    Axis[4] = 2;
     
  Serial.write((byte*)&Axis, sizeof(Axis)); 
}