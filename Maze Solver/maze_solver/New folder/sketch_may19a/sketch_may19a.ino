#include <Arduino.h>

// TODO: Check how to differentiate between lane Intersection and end of maze
// TODO: Implement adjustLeft, forwardMove, adjustRight, U_Turn, stopRobot functions.
// TODO: Change pins in setup to the adjustRight ones.

// connect 10 to EnA
// connect 11 to EnB

double fwd_spd_factor = 1 / 3.0;
double turn_fwd_spd_factor = 1 / 2.0;
double turn_rev_spd_factor = 1 / 4.0;
double adjust_turn_factor = 0.45;


bool leftFollower = true;

char path[50];  // Assuming number of operations won't exceed 50
int actualSize;
bool shortestPathCalculated;
char realPath[100];
int currRealIdx;  // used in fIRVst try when robot is solving the maze

int currIdx;  // Used in Second try when robot is walking on correct path

int fullSpeed = 255;
int turn_delay = 1000;

int secondRunFlag;

int farLeftIR = 26;
int frontNearLeftIR = 22;
int frontCenterIR = 23;
int frontNearRightIR = 24;
int farRightIR = 25;

int backNearLeftIR = 27;   // To Be Changed
int backCenterIR = 28;     // To Be Changed
int backNearRightIR = 29;  // To Be Changed

int farLeftReading;
int nearLeftReading = -1;
int centerReading = -1;
int nearRightReading = -1;
int farRightReading;

bool isRealFront = true;

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

void stopLeftWheels();
void stopRightWheels();
void calculateShortestPath(char *, int);
void readSensors();
void gotShortestPath(char MAZE_ARRAY[], int SIZE_OF_ARRAY);

void stopRobot() {

  stopLeftWheels();
  stopRightWheels();
}

bool isCenter() {
  return !farLeftReading && !nearLeftReading && centerReading && !nearRightReading && !farRightReading;
}

void calculateShortestPath(char MAZE_ARRAY[], int SIZE_OF_ARRAY) {
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
  gotShortestPath(MAZE_ARRAY, SIZE_OF_ARRAY);
}

void gotShortestPath(char MAZE_ARRAY[], int SIZE_OF_ARRAY) {
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
    calculateShortestPath(path, actualSize);
  }
}

void forwardRightWheels() {
  digitalWrite(right_motor_front1, HIGH);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, HIGH);
  digitalWrite(right_motor_back2, LOW);
}
void forwardLeftWheels() {
  digitalWrite(left_motor_front1, HIGH);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, HIGH);
  digitalWrite(left_motor_back2, LOW);
}

void reverseRightWheels() {
  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, HIGH);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, HIGH);
}
void reverseLeftWheels() {
  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, HIGH);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, HIGH);
}

void stopLeftWheels() {
  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, LOW);
}

void stopRightWheels() {
  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, LOW);
}

// direction: (0) left, (1) right
void adjustRobot(int adjustSpeed, bool adjustDirection) {
  if (adjustDirection) {
    analogWrite(ENL, adjustSpeed);
    if (isRealFront) {
      forwardLeftWheels();
      stopRightWheels();
    } else {
      stopLeftWheels();
      forwardRightWheels();
    }
  } else {
    analogWrite(ENR, adjustSpeed);
    if (isRealFront) {
      forwardRightWheels();
      stopLeftWheels();
    } else {
      stopRightWheels();
      forwardLeftWheels();
    }
  }
}

void turnLeft() {
  Serial.println("Inside turnLeft ..");
  realPath[currRealIdx++] = 'L';

  stopRobot();
  delay(350);

  if (isRealFront) {
    analogWrite(ENR, fullSpeed * turn_fwd_spd_factor);
    analogWrite(ENL, fullSpeed * turn_rev_spd_factor);

    forwardRightWheels();
    reverseLeftWheels();
  } else {
    analogWrite(ENR, fullSpeed * turn_rev_spd_factor);
    analogWrite(ENL, fullSpeed * turn_fwd_spd_factor);

    reverseRightWheels();
    forwardLeftWheels();
  }

  delay(900);

  readSensors();

  if (farRightReading || nearRightReading) {
    Serial.println("tets");
    adjustRobot(fullSpeed * turn_rev_spd_factor, 1);

    while (!isCenterReading()) {
      readSensors();
    }
  }

  while (!isCenterReading()) {
    readSensors();
  }
}

void turnRight() {
  Serial.println("Inside turnRight ..");
  realPath[currRealIdx++] = 'R';

  stopRobot();
  delay(350);

  if (isRealFront) {
    analogWrite(ENR, fullSpeed * turn_rev_spd_factor);
    analogWrite(ENL, fullSpeed * turn_fwd_spd_factor);

    forwardLeftWheels();
    reverseRightWheels();
  } else {
    analogWrite(ENR, fullSpeed * turn_fwd_spd_factor);
    analogWrite(ENL, fullSpeed * turn_rev_spd_factor);

    reverseLeftWheels();
    forwardRightWheels();
  }

  delay(900);
  readSensors();

  if (farLeftReading || nearLeftReading) {
    Serial.println("tets2");
    adjustRobot(fullSpeed * turn_rev_spd_factor, 0);

    while (!isCenterReading()) {
      readSensors();
    }
  }

  while (!isCenterReading()) {
    readSensors();
  }
}

bool isNearLeft() {
  return !farLeftReading && nearLeftReading && !nearRightReading && !farRightReading;
}

bool isNearRight() {
  return !farLeftReading && !nearLeftReading && nearRightReading && !farRightReading;
}

bool isLeftTurn() {
  return farLeftReading && !farRightReading;
}

bool isRightTurn() {
  return farRightReading && !farLeftReading && !centerReading;
}

bool isRightIntersection() {
  return farRightReading && centerReading && !farLeftReading;
}

bool isIntersection() {
  return farLeftReading && farRightReading;
}

bool isDeadEnd() {
  return !farLeftReading && !nearLeftReading && !centerReading && !nearRightReading && !farRightReading;
}

bool isEndGame() {
  return !farLeftReading && nearLeftReading && !centerReading && nearRightReading && !farRightReading;
}

bool isCenterReading() {
  return centerReading;
}

void readSensors() {
  if (isRealFront) {
    nearLeftReading = digitalRead(frontNearLeftIR);
    centerReading = digitalRead(frontCenterIR);
    nearRightReading = digitalRead(frontNearRightIR);
  } else {
    nearLeftReading = digitalRead(backNearLeftIR);
    centerReading = digitalRead(backCenterIR);
    nearRightReading = digitalRead(backNearRightIR);
  }
  farLeftReading = digitalRead(farLeftIR);
  farRightReading = digitalRead(farRightIR);
}

int old1=-1,old2=-1,old3=-1,old4=-1,old5=-1,old6=-1,old7=-1,old8=-1;
int new1=-1,new2=-1,new3=-1,new4=-1,new5=-1,new6=-1,new7=-1,new8=-1;

void printSensors() {
  new1=digitalRead(frontNearLeftIR);
  new2=digitalRead(frontCenterIR);
  new3=digitalRead(frontNearRightIR);
  new4=digitalRead(farLeftIR);
  new5=digitalRead(farRightIR);
  new6=digitalRead(backNearLeftIR);
  new7=digitalRead(backCenterIR);
  new8=digitalRead(backNearRightIR);
  if(new1!=old1 || new2!=old2 || new3!=old3 || 
  new4!=old4 || new5!=old5 || new6!=old6 || new7!=old7 || new8!=old8){
  Serial.print(" ");
  Serial.print(digitalRead(frontNearLeftIR));
  Serial.print(" ");
  Serial.print(digitalRead(frontCenterIR));
  Serial.print(" ");
  Serial.print(digitalRead(frontNearRightIR));
  Serial.print(" ");
  Serial.println("");
  Serial.print( digitalRead(farLeftIR));
  Serial.print(" ");
  Serial.print(" ");
  Serial.print(" ");
  Serial.print( digitalRead(farRightIR));
  Serial.println("");
  Serial.print(" ");

  Serial.print(digitalRead(backNearLeftIR));
  Serial.print(" ");
  Serial.print( digitalRead(backCenterIR));
  Serial.print(" ");
  Serial.print( digitalRead(backNearRightIR));
  Serial.print(" ");
  Serial.println("");

  Serial.println("---------------------------------");
  

  }
  old1=new1;old2=new2;old3=new3;old4=new4;old5=new5;old6=new6;old7=new7;old8=new8;
}

void forwardMove() {
  if (isRealFront) {
    analogWrite(ENL, fullSpeed * fwd_spd_factor);
    analogWrite(ENR, fullSpeed * fwd_spd_factor);
    forwardRightWheels();
    forwardLeftWheels();
  } else {
    analogWrite(ENR, fullSpeed * fwd_spd_factor);
    analogWrite(ENL, fullSpeed * fwd_spd_factor);
    reverseLeftWheels();
    reverseRightWheels();
  }
}

void Back() {
  if (isRealFront) {
    analogWrite(ENR, fullSpeed * fwd_spd_factor);
    analogWrite(ENL, fullSpeed * fwd_spd_factor);
    reverseLeftWheels();
    reverseRightWheels();
  } else {
    analogWrite(ENL, fullSpeed * fwd_spd_factor);
    analogWrite(ENR, fullSpeed * fwd_spd_factor);
    forwardRightWheels();
    forwardLeftWheels();
  }
}

void setup() {
  // DECLARING farLeftReading nearLeftReading centerReading nearRightReading AND farRightReading AS INPUTS
  Serial.begin(9600);

  // Serial.println("bmb");

  pinMode(farLeftIR, INPUT);
  pinMode(frontNearLeftIR, INPUT);
  pinMode(frontCenterIR, INPUT);
  pinMode(frontNearRightIR, INPUT);
  pinMode(farRightIR, INPUT);

  pinMode(backCenterIR, INPUT);
  pinMode(backNearLeftIR, INPUT);
  pinMode(29, INPUT);

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

  secondRunFlag = 0;

  stopRobot();
  delay(1000);

  // readSensors();
}

void lsrb() {
  readSensors();
  printSensors();

  // if (isLeftTurn()) {
  //   turnLeft();
  // } else if (isCenter()) {  // Straight
  //   forwardMove();
  // } else if (isIntersection()) {  // Intersection
  //   Serial.println("Found Intersection...");
  //   turnLeft();
  // } else if (isRightIntersection()) {
  //   realPath[currRealIdx++] = 'S';
  //   forwardMove();
  // } else if (isRightTurn()) {
  //   turnRight();
  // } else if (isNearLeft()) {  // adjustLeft
  //   adjustRobot(fullSpeed * adjust_turn_factor, 0);
  // } else if (isNearRight()) {  // adjustRight
  //   adjustRobot(fullSpeed * adjust_turn_factor, 1);
  // } else if (isDeadEnd()) {  // Dead End
  //   isRealFront = !isRealFront;
  //   realPath[currRealIdx++] = 'B';
  // } else if (isEndGame()) {
  //   Serial.println("END GAME...");
  //   calculateShortestPath(realPath, currRealIdx);

  //   while (!isEndGame()) {
  //     stopRobot();
  //     readSensors();
  //   }
  //   secondRunFlag = 1;
  //   isRealFront = true;
  // }
}

void useCalculatedPath() {
  readSensors();
  if (isLeftTurn() || isRightTurn() || isIntersection() || isRightIntersection()) {
    switch (path[currIdx++]) {
      case 'R':
        turnRight();
        break;
      case 'L':
        turnLeft();
        break;
      case 'S':
        forwardMove();
        break;
      case 'B':
        isRealFront = !isRealFront;
        break;
    }
  } else if (isNearLeft()) {  // adjustLeft
    adjustRobot(fullSpeed * adjust_turn_factor, 0);
  } else if (isNearRight()) {  // adjustRight
    adjustRobot(fullSpeed * adjust_turn_factor, 1);
  } else {
    forwardMove();
  }

  if (currIdx == actualSize) {
    while (!isEndGame()) {
      forwardMove();
    }
    secondRunFlag = 0;
    currIdx = 0;
    currRealIdx = 0;

    while (true) {
      stopRobot();
    }
  }
}

void loop() {
  if (!secondRunFlag) {
    lsrb();
  } else {
    useCalculatedPath();
  }
  // Serial.println(digitalRead(29));
  // Serial.println("digitalRead(29)");

}
