
// TODO: Implement Left, Forward, Right, U_Turn, Stop functions.
// TODO: Change pins in setup to the right ones.

//connect 10 to EnA
//connect 11 to EnB



char path[50];   //Assuming number of operations won't exceed 50
int actualSize;
bool shortestPathCalculated;
char realPath[100];
int currRealIdx;//used in first try when robot is solving the maze

int currIdx;//Used in Second try when robot is walking on correct path

int fullSpeed=254;

float ENA=10;
float ENB=11;

int IR1;
int IR2;
int IR3;
int IR4;
int IR5;


int IR1Add=2;
int IR2Add=3;
int IR3Add=4;
int IR4Add=5;
int IR5Add=6;

int motor_lA = 7;
int motor_lB = 8;
int motor_rA = 9;
int motor_rB = 12;///connect IN4 with 12


bool calculateShortestAgain=false;

void Forward()
{
  digitalWrite(motor_lA, 1);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);
  

  // delay(100);
}

void Left()
{
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);

  // delay(10);
}

void Right()
{
  digitalWrite(motor_lA, 1);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 0);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);

  // delay(10);
}

void U_Turn()
{
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 1);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);

  // delay(900);
}

void Stop()
{
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 0);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);

  delay(1000);
}

void MiniLeft()
{
  while(!(IR1 == HIGH && IR2 == HIGH && IR3 == LOW && IR4 == HIGH && IR5 == HIGH))
  {
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 1);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/4.0);
  analogWrite(ENB,fullSpeed/4.0);

  }
  // delay(10);
}

void MiniRight()
{
  while(!(IR1 == HIGH && IR2 == HIGH && IR3 == LOW && IR4 == HIGH && IR5 == HIGH))
  {
  digitalWrite(motor_lA, 1);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 0);
  digitalWrite(motor_rB, 1);
  analogWrite(ENA,fullSpeed/4.0);
  analogWrite(ENB,fullSpeed/4.0);

  }
  // delay(10);
}


void GOT_SHORTEST_PATH(char MAZE_ARRAY[], int SIZE_OF_ARRAY)
{
    int pIndex = 0;
    for (int i = 0; i < SIZE_OF_ARRAY; i++)
    {
        if (MAZE_ARRAY[i] != 0)
        {
            path[pIndex++] = MAZE_ARRAY[i];
        }
        if (!calculateShortestAgain && MAZE_ARRAY[i] == 'B')
        {
            calculateShortestAgain = true;
        }
    }
    actualSize = pIndex;


    // This part was added due to this example [SLBSLLBLBLBLLRBLLBLLL]
    if (calculateShortestAgain)
    {
        CALCULATE_SHORTEST_PATH(path, actualSize);
    }
}

void CALCULATE_SHORTEST_PATH(char MAZE_ARRAY[], int SIZE_OF_ARRAY)
{
    /*ONCE THE ROBOT COMPLETES THE MAZE THE FINAL SHORTEST PATH CALCULATED
    IS STORED IN THE ROBOT MEMORY.THIS SHORTEST PATH IS USED TO COMPLETE
    THE SAME MAZE IN SHORTEST AMOUNT OF TIME.(L :LEFT, R:RIGHT, B:BACK,S:STRAIGHT)
    BELOW ARE THE FEW SUBSTITUTIONS TO CONVERT FULL MAZE PATH TO ITS
    SHORTEST PATH:
    LBL = S
    LBR = B
    LBS = R
    RBL = B
    SBL = R
    SBS = B
    LBL = S */

    char ACTION;

    for (int i = 0; i <= SIZE_OF_ARRAY - 2; i++)
    {
        ACTION = MAZE_ARRAY[i];

        if (ACTION == 'B')
        {
            if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'R')
            {
                MAZE_ARRAY[i] = 'B';
                MAZE_ARRAY[i - 1] = 0;
                MAZE_ARRAY[i + 1] = 0;
                // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
                continue;
            }

            if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'S')
            {
                MAZE_ARRAY[i] = 'R';
                MAZE_ARRAY[i - 1] = 0;
                MAZE_ARRAY[i + 1] = 0;
                // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
                continue;
            }

            if (MAZE_ARRAY[i - 1] == 'R' && MAZE_ARRAY[i + 1] == 'L')
            {
                MAZE_ARRAY[i] = 'B';
                MAZE_ARRAY[i - 1] = 0;
                MAZE_ARRAY[i + 1] = 0;
                // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
                continue;
            }

            if (MAZE_ARRAY[i - 1] == 'S' && MAZE_ARRAY[i + 1] == 'L')
            {
                MAZE_ARRAY[i] = 'R';
                MAZE_ARRAY[i - 1] = 0;
                MAZE_ARRAY[i + 1] = 0;
                // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
                continue;
            }

            if (MAZE_ARRAY[i - 1] == 'S' && MAZE_ARRAY[i + 1] == 'S')
            {
                MAZE_ARRAY[i] = 'B';
                MAZE_ARRAY[i - 1] = 0;
                MAZE_ARRAY[i + 1] = 0;
                // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
                continue;
            }

            if (MAZE_ARRAY[i - 1] == 'L' && MAZE_ARRAY[i + 1] == 'L')
            {
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


void setup ()
{
  //DECLARING IR1 IR2 IR3 IR4 AND IR5 AS INPUTS
  
  pinMode(IR1Add, INPUT);
  pinMode(IR2Add, INPUT);
  pinMode(IR3Add, INPUT);
  pinMode(IR4Add, INPUT);
  pinMode(IR5Add, INPUT);

  //DECLARING MOTORS AS OUTPUTS
  pinMode(motor_lA, OUTPUT);
  pinMode(motor_lB, OUTPUT);
  pinMode(motor_rA, OUTPUT);
  pinMode(motor_rB, OUTPUT);


  shortestPathCalculated=false;
  actualSize=0;
  currRealIdx=0;
  currIdx=0;


}


void loop ()
{
  // Leftmost -> IR1 - IR2 - IR3 - IR4 - IR5 <- Rightmost
  IR1 = digitalRead(2);//First Left
  IR2 = digitalRead(3);//Second Left
  IR3 = digitalRead(4);//Center
  IR4 = digitalRead(5);//Second Right
  IR5 = digitalRead(6);//First Right

  if(shortestPathCalculated){
    if (IR1 == HIGH && IR2 == HIGH && IR3 == LOW && IR4 == HIGH && IR5 == HIGH)//Straight path
    {
      Forward();
    }
    else if(IR1 == HIGH && IR2 == LOW && IR3 == LOW && IR4 == HIGH && IR5 == HIGH)
    {
      MiniLeft();
    }
    else if(IR1 == HIGH && IR2 == HIGH && IR3 == LOW && IR4 == LOW && IR5 == HIGH)
    {
      MiniRight();
    }
    else if(IR1 == LOW && IR2 == LOW && IR3 == LOW && IR4 == LOW && IR5 == LOW)
    {
      Stop();
    }
    else{
      switch(path[currIdx]){
        case 'S':
          Forward();
          break;
        case 'L':
          Left();
          break;
        case 'R':
          Right();
          break;
        case 'B': //We'll check if we can remove it.
          U_Turn();
          break;
      }
      currIdx++;
    }
  }
  else{
    if (IR1 == HIGH && IR2 == HIGH && IR3 == LOW && IR4 == HIGH && IR5 == HIGH)//Straight path
    {
      Forward();
    }

    if (IR1 == LOW && IR2 == LOW && IR3 == HIGH && IR4 == HIGH && IR5 == HIGH)//Left turn
    {
      Left();
      realPath[currRealIdx]='L';

    }

    if (IR1 == HIGH && IR2 == HIGH && IR3 == HIGH && IR4 == LOW && IR5 == LOW)//Right Turn
    {
      Right();
      realPath[currRealIdx]='R';
      

    }

    if (IR1 == LOW && IR2 == LOW && IR3 == HIGH && IR4 == LOW && IR5 == LOW)//T Intersection
    {
      Left(); // As left is possible
      realPath[currRealIdx]='L';

    }

    if (IR1 == LOW && IR2 == LOW && IR3 == LOW && IR4 == HIGH && IR5 == HIGH)//Left T Intersection
    {
      Left();// As Left is possible
      realPath[currRealIdx]='L';
    }

    if (IR1 == HIGH && IR2 == HIGH && IR3 == LOW && IR4 == LOW && IR5 == LOW)//Right T Tntersection
    {
      Forward();//As Straight path is possible
      realPath[currRealIdx]='S';

    }

    if (IR1 == HIGH && IR2 ==HIGH && IR3 == HIGH && IR4 == HIGH && IR5 == HIGH)//Dead End
    {
      // U_Turn(); //As no other direction is possible
      Stop();
      realPath[currRealIdx]='B';

    }

    if (IR1 == LOW && IR2 ==LOW && IR3 == LOW && IR4 == LOW && IR5 == LOW)//4 Lane intersection //Check this
    {
      Left(); //As no other direction is possible
    } 

    if (IR1 == HIGH && IR2 == LOW && IR3 == LOW && IR4 == HIGH && IR5 == HIGH)
    {
      MiniLeft();
    }

    if (IR1 == HIGH && IR2 == HIGH && IR3 == LOW && IR4 == LOW && IR5 == HIGH)
    {
      MiniRight();
    }

    //--------------
    //      |<-
    //      |
    //      |

    if (IR1 == LOW && IR2 ==LOW && IR3 == LOW && IR4 == LOW && IR5 == LOW)//End of Maze
    {
      Stop(); //As no other direction is possible
      CALCULATE_SHORTEST_PATH(realPath, 100);
    } 
  }

}