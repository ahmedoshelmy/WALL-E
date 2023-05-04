// TODO: Check how to differentiate between lane Intersection and end of maze
// TODO: Implement adjustLeft, Forward, adjustRight, U_Turn, Stop functions.
// TODO: Change pins in setup to the adjustRight ones.

// connect 10 to EnA
// connect 11 to EnB

double fwd_spd_factor = 1 / 4.0;
double turn_spd_factor = 1 / 2.0;

char path[50];  // Assuming number of operations won't exceed 50
int actualSize;
bool shortestPathCalculated;
char realPath[100];
int currRealIdx;  // used in fIRVst try when robot is solving the maze

int currIdx;  // Used in Second try when robot is walking on correct path

int fullSpeed = 255;
int turn_delay = 1000;

int IR1 = A4;
int IR2 = A2;
int IR3 = A1;
int IR4 = A0;
int IR5 = A3;

int farLeftReading;
int nearLeftReading = -1;
int centerReading = -1;
int nearRightReading = -1;
int farRightReading;

int old2 = 2;
int old3 = 3;
int old4 = 4;

// int IRV1Add=2;
// int IRV2Add=3;
// int IRV3Add=4;
// int IRV4Add=5;
// int IRV5Add=6;

int motor_lA = 7;
int motor_lB = 8;
int motor_rA = 9;
int motor_rB = 12;  /// connect IN4 with 12

int ENR = 3;
int ENL = 4;  // ENLble adjustLeft
int right_motor_back1 = 36;
int right_motor_back2 = 37;
int right_motor_front1 = 39;
int right_motor_front2 = 38;
int left_motor_front1 = 26;
int left_motor_front2 = 27;
int left_motor_back1 = 29;
int left_motor_back2 = 28;

bool calculateShortestAgain = false;

void Forward_The_Right() {
  digitalWrite(right_motor_front1, HIGH);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, HIGH);
  digitalWrite(right_motor_back2, LOW);
}
void Forward_The_Left() {
  digitalWrite(left_motor_front1, HIGH);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, HIGH);
  digitalWrite(left_motor_back2, LOW);
}

void Reverse_The_Right() {
  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, HIGH);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, HIGH);
}
void Reverse_The_Left() {
  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, HIGH);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, HIGH);
}

void Stop_The_Left() {
  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, LOW);
}

void Stop_The_Right() {
  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, LOW);
}

void adjustRight() {
  analogWrite(ENR, fullSpeed);
  analogWrite(ENL, fullSpeed);
  Forward_The_Left();
  Stop_The_Right();
  // while (!isCenter()) {
  //   readSensors();
  // }
}

void turnLeft() {
  analogWrite(ENR, fullSpeed);
  analogWrite(ENL, fullSpeed * turn_spd_factor);
  Forward_The_Right();
  Reverse_The_Left();
  Serial.println("hererererere");
  while (!farRightReading && !nearRightReading) {
    readSensors();
  }
}

void turnRight() {
  analogWrite(ENR, fullSpeed * turn_spd_factor);
  analogWrite(ENL, fullSpeed);
  Forward_The_Left();
  Reverse_The_Right();
  // while (!isCenter()) {
  //   readSensors();
  // }
}

bool isCenter() {
  return !farLeftReading && !nearLeftReading && centerReading && !nearRightReading && !farRightReading;
}

bool isNearLeft() {
  return !farLeftReading && nearLeftReading && centerReading && !nearRightReading && !farRightReading;
}

bool isNearRight() {
  return !farLeftReading && !nearLeftReading && centerReading && nearRightReading && !farRightReading;
}

bool isLeftTurn() {

  path[currRealIdx++] = 'L';
  return farLeftReading;
}

bool isRightTurn() {
  path[currRealIdx++] = 'R';

  return farRightReading;
}

bool isIntersection() {
  return farLeftReading && nearLeftReading && centerReading && nearRightReading && farRightReading;
}

bool isDeadEnd() {
  return !farLeftReading && !nearLeftReading && !centerReading && !nearRightReading && !farRightReading;
}

bool isEndGame() {
  return !farLeftReading && nearLeftReading && !centerReading && nearRightReading && !farRightReading;
}

void readSensors() {
  nearLeftReading = digitalRead(IR2);
  centerReading = digitalRead(IR3);
  nearRightReading = digitalRead(IR4);
  farLeftReading = digitalRead(IR1);
  farRightReading = digitalRead(IR5);
}

void Stop() {

  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, LOW);
  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, LOW);
}

void Forward() {

  analogWrite(ENL, fullSpeed * fwd_spd_factor);
  analogWrite(ENR, fullSpeed * fwd_spd_factor);
  Forward_The_Right();
  Forward_The_Left();
}

void Back() {
  analogWrite(ENR, fullSpeed * fwd_spd_factor);
  analogWrite(ENL, fullSpeed * fwd_spd_factor);
  Reverse_The_Left();
  Reverse_The_Right();
}
void adjustLeft() {
  analogWrite(ENR, fullSpeed);
  analogWrite(ENL, fullSpeed);
  Forward_The_Right();
  Stop_The_Left();
  // while (!isCenter()) {
  //   readSensors();
  // }
}

void U_Turn() {

  analogWrite(ENR, fullSpeed * turn_spd_factor);
  analogWrite(ENL, fullSpeed * turn_spd_factor);
  adjustRight();
  while (!isCenter()) {
    readSensors();
  }
}
void setup() {
  // DECLARING farLeftReading nearLeftReading centerReading nearRightReading AND farRightReading AS INPUTS
  Serial.begin(9600);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);

  // DECLARING MOTORS AS OUTPUTS
  pinMode(right_motor_front1, OUTPUT);
  pinMode(right_motor_front2, OUTPUT);
  pinMode(right_motor_back1, OUTPUT);
  pinMode(right_motor_back2, OUTPUT);
  pinMode(left_motor_front1, OUTPUT);
  pinMode(left_motor_front2, OUTPUT);
  pinMode(left_motor_back1, OUTPUT);
  pinMode(left_motor_back2, OUTPUT);
  pinMode(ENR, OUTPUT);
  pinMode(ENL, OUTPUT);
  pinMode(3, INPUT);

  shortestPathCalculated = false;
  actualSize = 0;
  currRealIdx = 0;
  currIdx = 0;
}

// الاسود وحايد

void loop() {
  readSensors();
  Serial.print(farLeftReading);
  Serial.print(" ");
  Serial.print(nearLeftReading);
  Serial.print(" ");
  Serial.print(centerReading);
  Serial.print(" ");
  Serial.print(nearRightReading);
  Serial.print(" ");
  Serial.print(farRightReading);
  Serial.println();



  if(farLeftReading){
    turnLeft();
  }
  else if (isCenter()) {  //Straight
    Forward();
  } else if (isRightTurn()) {
    Forward();
    readSensors();
    if (isCenter()) {  //Straight
      path[currRealIdx++] = 'S';
      Forward();

    } else if (isNearLeft()) {  //adjustLeft
      path[currRealIdx++] = 'S';

      adjustLeft();
    } else if (isNearRight()) {  //adjustRight
      path[currRealIdx++] = 'S';

      adjustRight();
    } else {
      turnRight();
    }
  } else if (isNearLeft()) {  //adjustLeft
    adjustLeft();
  } else if (isNearRight()) {  //adjustRight
    adjustRight();
  }
else if (isIntersection()) {  //Intersection
  turnLeft();
}
else if (isDeadEnd()) {  //Dead End
  U_Turn();
}
else if (isEndGame()) {
  Stop();
}
}
