void setup() {
 pinMode(10, OUTPUT);  digitalWrite(10, LOW);
}

void loop() {

  
  for(int i=0;i<50;i++){
  analogWrite(10,i);
  delay(1000);
  }

  
}
