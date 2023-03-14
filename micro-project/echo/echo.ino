//simple echo program

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (Serial.available()){
    Serial.print((char) Serial.read());
  } 
}
