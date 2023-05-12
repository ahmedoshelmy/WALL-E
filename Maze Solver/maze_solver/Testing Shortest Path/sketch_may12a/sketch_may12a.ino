char path[50];  // Assuming number of operations won't exceed 50
int actualSize;
bool shortestPathCalculated;
char realPath[100]={'S','L','L','S','B','L','L','B','S','B','L','L','L','B','S','L','L','L','R','R','B','L','L','L','B','S','S','S','B','L','L','L','B','L','R','S','B','L','L','B','S','R'};
int currRealIdx;  // used in fIRVst try when robot is solving the maze

bool calculateShortestAgain = false;


int currIdx;  // Used in Second try when robot is walking on correct path

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

void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
CALCULATE_SHORTEST_PATH(realPath,42);

for(currIdx;currIdx<actualSize;currIdx++)
{
  Serial.println(path[currIdx]);
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
