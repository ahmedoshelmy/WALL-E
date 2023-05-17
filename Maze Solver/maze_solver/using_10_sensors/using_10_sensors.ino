#include <Arduino.h>

// TODO: Check how to differentiate between lane Intersection and end of maze
// TODO: Implement adjustLeft, Forward, adjustRight, U_Turn, Stop functions.
// TODO: Change pins in setup to the adjustRight ones.

// connect 10 to EnA
// connect 11 to EnB

double fwd_spd_factor = 1 / 3.0;
double turn_spd_factor = 1 / 1.0;
double turn_rev_spd_factor = 1 / 2.0;
double adjust_turn_factor = 0.45;

bool isFront = 1;

bool leftFollower = true;

char path[50];  // Assuming number of operations won't exceed 50
int actualSize;
bool shortestPathCalculated;
char realPath[100];
int currRealIdx;  // used in fIRVst try when robot is solving the maze

int currIdx;  // Used in Second try when robot is walking on correct path

int fullSpeed = 255;
int turn_delay = 1000;

int flag;

/* front pins */
int IR1 = A4;
int IR2 = A2;
int IR3 = A1;
int IR4 = A0;
int IR5 = A3;

/* rear pins */
int IR6 /* TODO: pins */;
int IR7 /* TODO: pins */;
int IR8 /* TODO: pins */;
int IR9 /* TODO: pins */;
int IR10 /* TODO: pins */;

int frontFarLeftReading;
int frontNearLeftReading = -1;
int frontCenterReading = -1;
int frontNearRightReading = -1;
int frontFarRightReading;

int rearFarLeftReading;
int rearNearLeftReading = -1;
int rearCenterReading = -1;
int rearNearRightReading = -1;
int rearFarRightReading;

////////////////////////
int prefarLeftReading;
int prenearLeftReading = -1;
int precenterReading = -1;
int prenearRightReading = -1;
int prefarRightReading;

// int old2 = 2;
// int old3 = 3;
// int old4 = 4;

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
bool isTurningAround = false;

void Stop_The_Left();
void Stop_The_Right();
void CALCULATE_SHORTEST_PATH(char *, int);
void readSensors();
void GOT_SHORTEST_PATH(char MAZE_ARRAY[], int SIZE_OF_ARRAY);

void Stop() {

  Stop_The_Left();
  Stop_The_Right();
}

bool isCenter() {
  return (isFront && !frontFarLeftReading && !frontNearLeftReading && frontCenterReading && !frontNearRightReading && !frontFarRightReading)
          || (!isFront && !rearFarLeftReading && !rearNearLeftReading && rearCenterReading && !rearNearRightReading && !rearFarRightReading);
}

void CALCULATE_SHORTEST_PATH(char MAZE_ARRAY[], int SIZE_OF_ARRAY) {
  /*ONCE THE ROBOT COMPLETES THE MAZE THE FINAL SHORTEST PATH CALCULATED
    IS STORED IN THE ROBOT MEMORY.THIS SHORTEST PATH IS USED TO COMPLETE
    THE SAME MAZE IN SHORTEST AMOUNT OF TIME.(L :LEFT, R:RIGHT, B:BACK,S:STRAIGHT)
    BEHIGH1 ARE THE FEW SUBSTITUTIONS TO CONVERT FULL MAZE PATH TO ITS
    SHORTEST PATH:
    LBL = S
    LBR = B
    LBS = R
    RBL = B
    SBL = R
    SBS = B
    LBL = S */
  char ACTION;
  for (int i = 0; i <= SIZE_OF_ARRAY - 2; i++) {
    ACTION = MAZE_ARRAY[i];
    if (ACTION == 'B') {
      if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'R') {
        MAZE_ARRAY[i] = 'B';
        MAZE_ARRAY[i - 1] = 0;
        MAZE_ARRAY[i + 1] = 0;
        // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
        continue;
      }
      if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'S') {
        MAZE_ARRAY[i] = 'R';
        MAZE_ARRAY[i - 1] = 0;
        MAZE_ARRAY[i + 1] = 0;
        // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
        continue;
      }
      if (MAZE_ARRAY[i - 1] == 'R' && MAZE_ARRAY[i + 1] == 'L') {
        MAZE_ARRAY[i] = 'B';
        MAZE_ARRAY[i - 1] = 0;
        MAZE_ARRAY[i + 1] = 0;
        // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
        continue;
      }
      if (MAZE_ARRAY[i - 1] == 'S' && MAZE_ARRAY[i + 1] == 'L') {
        MAZE_ARRAY[i] = 'R';
        MAZE_ARRAY[i - 1] = 0;
        MAZE_ARRAY[i + 1] = 0;
        // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
        continue;
      }
      if (MAZE_ARRAY[i - 1] == 'S' && MAZE_ARRAY[i + 1] == 'S') {
        MAZE_ARRAY[i] = 'B';
        MAZE_ARRAY[i - 1] = 0;
        MAZE_ARRAY[i + 1] = 0;
        // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
        continue;
      }
      if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'L') {
        MAZE_ARRAY[i] = 'S';
        MAZE_ARRAY[i - 1] = 0;
        MAZE_ARRAY[i + 1] = 0;
        // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
        continue;
      }
      // i = -1;
    }
    // delay(100);
  }
  shortestPathCalculated = true;
  calculateShortestAgain = false;
  GOT_SHORTEST_PATH(MAZE_ARRAY, SIZE_OF_ARRAY);
}

void GOT_SHORTEST_PATH(char MAZE_ARRAY[], int SIZE_OF_ARRAY) {
  int pIndex = 0;
  for (int i = 0; i < SIZE_OF_ARRAY; i++) {
    if (MAZE_ARRAY[i] != 0) {
      path[pIndex++] = MAZE_ARRAY[i];
    }
    if (!calculateShortestAgain && MAZE_ARRAY[i] == 'B') {
      calculateShortestAgain = true;
    }
  }
  actualSize = pIndex;
  // This part was added due to this example [SLBSLLBLBLBLLRBLLBLLL]
  if (calculateShortestAgain) {
    CALCULATE_SHORTEST_PATH(path, actualSize);
  }
}

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
  //Serial.println("Adjusting right");
  if (isFront) {
    analogWrite(ENR, fullSpeed);
    analogWrite(ENL, fullSpeed);
    Forward_The_Left();
    Reverse_The_Right();
  } else {
    analogWrite(ENL, fullSpeed);
    analogWrite(ENR, fullSpeed);
    Reverse_The_Left();
    Forward_The_Right();
  }
  while (!isCenter()) {
    readSensors();
  }
}

void adjustBackRight() {
  analogWrite(ENR, fullSpeed);
  analogWrite(ENL, fullSpeed);
  Reverse_The_Left();
  Forward_The_Right();
}

void smothStop(int f) {
  while (f != 0) {

    f = f / 2;

    analogWrite(ENR, f);
    analogWrite(ENL, f);

    Forward();
    delay(100);
  }
}

void turnLeft() {
  Serial.println("Inside turnLeft ..");
  realPath[currRealIdx++] = 'L';
  // Stop();
  // delay(500);

  if (isFront) {
    analogWrite(ENR, fullSpeed * 0.7);
    analogWrite(ENL, fullSpeed * 0.75);

    Forward_The_Right();
    Reverse_The_Left();    
  } else {
    analogWrite(ENL, fullSpeed * 0.7);
    analogWrite(ENR, fullSpeed * 0.75);

    Reverse_The_Right();
    Forward_The_Left(); 
  }

  delay(500);

  readSensors();

  while(!isCenter()) {
    readSensors();
  }

  // analogWrite(ENR, fullSpeed * 1/2.0);
  // analogWrite(ENL, fullSpeed * 1/4.0);

  // Forward_The_Right();
  // Reverse_The_Left();

  // delay(3500);

  // delay(500)
  //  while (!isCenter()) {
  //   readSensors();
  // }
}

void turnRight() {
  Serial.println("Inside turnRight ..");
  realPath[currRealIdx++] = 'R';

  if (isFront) {
    analogWrite(ENR, fullSpeed * 0.7);
    analogWrite(ENL, fullSpeed * 0.75);

    Forward_The_Left();
    Reverse_The_Right();    
  } else {
    analogWrite(ENL, fullSpeed * 0.7);
    analogWrite(ENR, fullSpeed * 0.75);

    Reverse_The_Left();
    Forward_The_Right();    
  }

  delay(500);
  readSensors();

  while (!isCenter()) {
    readSensors();
  }
}

bool isNearLeft() {
  return (isFront && !frontFarLeftReading && frontNearLeftReading && frontCenterReading && !frontNearRightReading && !frontFarRightReading)
          || (!isFront && !rearFarLeftReading && rearNearLeftReading && rearCenterReading && !rearNearRightReading && !rearFarRightReading);
}

bool isNearRight() {
  return (isFront && !frontFarLeftReading && !frontNearLeftReading && frontCenterReading && frontNearRightReading && !frontFarRightReading)
          || (!isFront && !rearFarLeftReading && !rearNearLeftReading && rearCenterReading && rearNearRightReading && !rearFarRightReading);
}

bool isLeftTurn() {

  return (isFront && frontFarLeftReading && frontNearLeftReading && frontCenterReading && !frontNearRightReading && !frontFarRightReading)
          || (!isFront && rearFarLeftReading && rearNearLeftReading && rearCenterReading && !rearNearRightReading && !rearFarRightReading);
}

bool isRightTurn() {

  return (isFront && !frontFarLeftReading && !frontNearLeftReading && frontCenterReading && frontNearRightReading && frontFarRightReading)
          || (!isFront && !rearFarLeftReading && !rearNearLeftReading && rearCenterReading && rearNearRightReading && rearFarRightReading);
}

bool isIntersection() {
  return (isFront && frontFarLeftReading && frontNearLeftReading && frontCenterReading && frontNearRightReading && frontFarRightReading)
          || (!isFront && rearFarLeftReading && rearNearLeftReading && rearCenterReading && rearNearRightReading && rearFarRightReading);
}

bool isDeadEnd() {
  return (isFront && !frontFarLeftReading && !frontNearLeftReading && !frontCenterReading && !frontNearRightReading && !frontFarRightReading)
          || (!isFront && !rearFarLeftReading && !rearNearLeftReading && !rearCenterReading && !rearNearRightReading && !rearFarRightReading);
}

bool isEndGame() {
  return (isFront && !frontFarLeftReading && frontNearLeftReading && !frontCenterReading && frontNearRightReading && !frontFarRightReading)
          || (!isFront && !rearFarLeftReading && rearNearLeftReading && !rearCenterReading && rearNearRightReading && !rearFarRightReading);
}

bool isCenterReading() {
  return (isFront && frontCenterReading) || (!isFront && rearCenterReading);
}

void readSensors() {
  frontFarLeftReading = digitalRead(IR1);
  frontNearLeftReading = digitalRead(IR2);
  frontCenterReading = digitalRead(IR3);
  frontNearRightReading = digitalRead(IR4);
  frontFarRightReading = digitalRead(IR5);

  rearFarLeftReading = digitalRead(IR6);
  rearNearLeftReading = digitalRead(IR7);
  rearCenterReading = digitalRead(IR8);
  rearNearRightReading = digitalRead(IR9);
  rearFarRightReading = digitalRead(IR10);
}

void Forward() {

  analogWrite(ENL, fullSpeed * 0.4);
  analogWrite(ENR, fullSpeed * 0.4);
  if (isFront) {
    Forward_The_Right();
    Forward_The_Left();
  } else {
    Reverse_The_Right();
    Reverse_The_Left();
  }
}

void Back() {
  analogWrite(ENR, fullSpeed * 0.4);
  analogWrite(ENL, fullSpeed * 0.4);
  if (isFront) {
    Reverse_The_Left();
    Reverse_The_Right();
  } else {
    Forward_The_Left();
    Forward_The_Right();
  }
}
void adjustLeft() {
  //Serial.println("Adjusting left");
  if (isFront) {
    analogWrite(ENR, fullSpeed);
    analogWrite(ENL, fullSpeed);
    Forward_The_Right();
    Reverse_The_Left();
  } else {
    analogWrite(ENL, fullSpeed);
    analogWrite(ENR, fullSpeed);
    Reverse_The_Right();
    Forward_The_Left();
  }
  while (!isCenter()) {
    readSensors();
  }
}

void adjustBackLeft() {
  analogWrite(ENR, fullSpeed);
  analogWrite(ENL, fullSpeed);
  Reverse_The_Right();
  // Stop_The_Left();
  Forward_The_Left();
}

void U_Turn() {
  Serial.println("Inside U_Turn ..");

  realPath[currRealIdx++] = 'B';

  if (isFront) {
    analogWrite(ENR, fullSpeed * 0.7);
    analogWrite(ENL, fullSpeed * 0.75);

    Reverse_The_Right();
    Forward_The_Left();    
  } else {
    analogWrite(ENL, fullSpeed * 0.7);
    analogWrite(ENR, fullSpeed * 0.75);

    Forward_The_Right();
    Reverse_The_Left();    
  }

  while (!isCenter()) {
    readSensors();
  }
  isFront = !isFront;
}
void setup() {
  // DECLARING farLeftReading nearLeftReading centerReading nearRightReading AND farRightReading AS INPUTS
  Serial.begin(9600);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);

  pinMode(IR6, INPUT);
  pinMode(IR7, INPUT);
  pinMode(IR8, INPUT);
  pinMode(IR9, INPUT);
  pinMode(IR10, INPUT);

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
  realPath[0] = 'S';
  currRealIdx = 1;
  currIdx = 0;

  flag = 0;

  Stop();
  delay(1000);

  readSensors();

  // precenterReading = centerReading;
  // prefarLeftReading = farLeftReading;
  // prenearLeftReading = nearLeftReading;
  // prefarRightReading = farRightReading;
  // prenearRightReading = nearRightReading;
}

void lsrb() {

  readSensors();
  // if (precenterReading != centerReading || prefarLeftReading != farLeftReading || prenearLeftReading != nearLeftReading || prefarRightReading != farRightReading || prenearRightReading != nearRightReading) {
  //   Serial.print(farLeftReading);
  //   Serial.print(" ");
  //   Serial.print(nearLeftReading);
  //   Serial.print(" ");
  //   Serial.print(centerReading);
  //   Serial.print(" ");
  //   Serial.print(nearRightReading);
  //   Serial.print(" ");
  //   Serial.print(farRightReading);
  //   Serial.println();

  //   precenterReading = centerReading;
  //   prefarLeftReading = farLeftReading;
  //   prenearLeftReading = nearLeftReading;
  //   prefarRightReading = farRightReading;
  //   prenearRightReading = nearRightReading;
  // }

  if (isLeftTurn()) {
    turnLeft();
  } else if (isIntersection()) {  // Intersection
    Serial.println("Found Intersection...");
    turnLeft();
  } else if (isNearLeft()) {  // adjustLeft
    adjustLeft();
  } else if (isNearRight()) {  // adjustRight
    adjustRight();
  } else if (isCenter()) {  // Straight
    Forward();
  } else if (isRightTurn()) {
    Forward();
    delay(220);
    Stop();

    readSensors();
    if (isNearLeft()) {  // adjustLeft
      // realPath[currRealIdx++] = 'S';

      adjustLeft();
    } 
    if (isNearRight()) {  // adjustRight
      // realPath[currRealIdx++] = 'S';

      adjustRight();
    }
    if (isCenter()) {  // Straight
      realPath[currRealIdx++] = 'S';
      Forward();
    } else {
      Back();
      delay(220);
      turnRight();
    }
  } else if (isDeadEnd()) {  // Dead End
    Back(); 
    delay(250);
    U_Turn();
  } else if (isEndGame()) {
    Serial.println("END GAME...");
    CALCULATE_SHORTEST_PATH(realPath, currRealIdx);
    flag = 1;

    Stop();
  }
}

void rslb() {

  readSensors();
  // if (precenterReading != centerReading || prefarLeftReading != farLeftReading || prenearLeftReading != nearLeftReading || prefarRightReading != farRightReading || prenearRightReading != nearRightReading) {
  //   Serial.print(farLeftReading);
  //   Serial.print(" ");
  //   Serial.print(nearLeftReading);
  //   Serial.print(" ");
  //   Serial.print(centerReading);
  //   Serial.print(" ");
  //   Serial.print(nearRightReading);
  //   Serial.print(" ");
  //   Serial.print(farRightReading);
  //   Serial.println();

  //   precenterReading = centerReading;
  //   prefarLeftReading = farLeftReading;
  //   prenearLeftReading = nearLeftReading;
  //   prefarRightReading = farRightReading;
  //   prenearRightReading = nearRightReading;
  // }

  if (isRightTurn()) {
    turnRight();
  } else if (isIntersection()) {  // Intersection
    Serial.println("Found Intersection...");
    turnRight();
  } else if (isNearLeft()) {  // adjustLeft
    adjustLeft();
  } else if (isNearRight()) {  // adjustRight
    adjustRight();
  } else if (isCenter()) {  // Straight
    Forward();
  } else if (isLeftTurn()) {
    Forward();
    delay(200);
    readSensors();
    if (isNearLeft()) {  // adjustLeft
      // realPath[currRealIdx++] = 'S';

      adjustLeft();
    } 
    if (isNearRight()) {  // adjustRight
      // realPath[currRealIdx++] = 'S';

      adjustRight();
    }
    if (isCenter()) {  // Straight
      realPath[currRealIdx++] = 'S';
      Forward();
    } else {
      Back();
      delay(200);
      turnLeft();
    }
  } else if (isDeadEnd()) {  // Dead End
    Back();
    delay(250);
    U_Turn();
  } else if (isEndGame()) {
    Serial.println("END GAME...");
    CALCULATE_SHORTEST_PATH(realPath, currRealIdx);
    flag = 1;

    Stop();
  }
}

// الاسود وحايد

void loop() {
  // if (!flag)

  if (leftFollower) {
    lsrb();
  } else {
    rslb();
  }

  // else
  // {
  //   // Go on right path

  //   readSensors();
  //   if (isLeftTurn() || isRightTurn() || isIntersection())
  //   {
  //     switch (path[currIdx++])
  //     {
  //     case 'R':
  //     {
  //       turnRight();
  //       break;
  //     }
  //     case 'L':
  //     {
  //       turnLeft();
  //       break;
  //     }
  //     case 'S':
  //     {
  //       Forward();
  //       break;
  //     }

  //     default:
  //       break;
  //     }
  //   }
  //   else if (isNearLeft())
  //   { // adjustLeft
  //     adjustLeft();
  //   }
  //   else if (isNearRight())
  //   { // adjustRight
  //     adjustRight();
  //   }
  //   else
  //   {
  //     Forward();
  //   }

  //   if (currIdx == actualSize)
  //   {
  //     while (!isEndGame())
  //     {
  //       Forward();
  //     }
  //     Stop();
  //     flag = 0;
  //     currIdx = 0;
  //     currRealIdx = 0;
  //   }
  // }
}
