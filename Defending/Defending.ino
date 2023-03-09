

float ENA = 10;
float ENB = 11;
int motor_lA = 7;
int motor_lB = 8;
int motor_rA = 9;
int motor_rB = 12;  ///connect IN4 with 12
int fullSpeed = 254;

void Forward() {
  digitalWrite(motor_lA, 1);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA, fullSpeed / 2.0);
  analogWrite(ENB, fullSpeed / 2.0);
}

void Backward() {
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 1);
  digitalWrite(motor_rA, 0);
  digitalWrite(motor_rB, 1);
  analogWrite(ENA, fullSpeed / 2.0);
  analogWrite(ENB, fullSpeed / 2.0);
}
void Stop() {
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 0);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
void Move(int pState) {
  if (pState == 1)
    Forward();
  else if (pState == -1)
    Backward();
  else Stop();
}
void setup() {
  //DECLARING MOTORS AS OUTPUTS
  pinMode(motor_lA, OUTPUT);
  pinMode(motor_lB, OUTPUT);
  pinMode(motor_rA, OUTPUT);
  pinMode(motor_rB, OUTPUT);
  Serial.begin(9600);
}
int state = 0;
//0C -1L 1R

void loop() {
 Move(state); 
  String input;
  input = Serial.readStringUntil('\n');

  teststr.trim();
  if (input == "L")
    state = -1;
  else if (input == "R")
    state = 1;
  else if (input == "C")
    state = 0;
  Serial.println(state);
  Serial.println(input);
  delay(50);
}
