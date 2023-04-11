// TODO: Check how to differentiate between lane Intersection and end of maze
// TODO: Implement Left, Forward, Right, U_Turn, Stop functions.
// TODO: Change pins in setup to the right ones.

//connect 10 to EnA
//connect 11 to EnB



char path[50];   //Assuming number of operations won't exceed 50
int actualSize;
bool shortestPathCalculated;
char realPath[100];
int currRealIdx;//used in fIRVst try when robot is solving the maze

int currIdx;//Used in Second try when robot is walking on correct path

int fullSpeed=254;

int ENA=10;
int ENB=11;

int IR1=2;
int IR2=3;
int IR3=4;
int IR4=5;
int IR5=6;

int IRV1;
int IRV2;
int IRV3;
int IRV4;
int IRV5;


// int IRV1Add=2;
// int IRV2Add=3;
// int IRV3Add=4;
// int IRV4Add=5;
// int IRV5Add=6;

int motor_lA = 7;
int motor_lB = 8;
int motor_rA = 9;
int motor_rB = 12;///connect IN4 with 12


bool calculateShortestAgain=false;

void Forward()
{
  Serial.println("Forward");
  digitalWrite(motor_lA, 1);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/3.0);
  analogWrite(ENB,fullSpeed/3.0);
  

  // delay(100);
}

void Back()
{
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 1);
  digitalWrite(motor_rA, 0);
  digitalWrite(motor_rB, 1);
  analogWrite(ENA,fullSpeed/3.0);
  analogWrite(ENB,fullSpeed/3.0);
  // delay(50);
}

void slightForward()
{
  digitalWrite(motor_lA, 1);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);
  // delay(50);
}

void Left()
{
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 1);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);

  IRV3 = digitalRead(IR3);
  IRV2 = digitalRead(IR2);
  IRV4 = digitalRead(IR4);
  while(IRV3!=1 && IRV2!=0 && IRV4!=0){
    
  IRV3 = digitalRead(IR3);
  IRV2 = digitalRead(IR2);
  IRV4 = digitalRead(IR4);
    digitalWrite(motor_lA, 0);
    digitalWrite(motor_lB, 1);
    digitalWrite(motor_rA, 1);
    digitalWrite(motor_rB, 0);
    analogWrite(ENA,fullSpeed/2.0);
    analogWrite(ENB,fullSpeed/2.0);
  }

  // delay(1000);
}

void Right()
{
  digitalWrite(motor_lA, 1);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 0);
  digitalWrite(motor_rB, 1);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);

IRV3 = digitalRead(IR3);
  IRV2 = digitalRead(IR2);
  IRV4 = digitalRead(IR4);
  while(IRV3!=1 && IRV2!=0 && IRV4!=0){
    IRV3 = digitalRead(IR3);
  IRV2 = digitalRead(IR2);
  IRV4 = digitalRead(IR4);
    digitalWrite(motor_lA, 1);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 0);
  digitalWrite(motor_rB, 1);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);
  }

  // delay(1000);
}

void U_Turn()
{
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 1);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);

  // delay(5000);
}

void Stop()
{
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 0);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);

  // delay(1000);
}

void MiniLeft()
{
  while(!(IRV1 == 0 && IRV2 == 0 && IRV3 == 1 && IRV4 == 0 && IRV5 == 0))
  {
  digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 1);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);

  }
  // delay(100);
}

void MinIRVight()
{
  while(!(IRV1 == 0 && IRV2 == 0 && IRV3 == 1 && IRV4 == 0 && IRV5 == 0))
  {
  digitalWrite(motor_lA, 1);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 0);
  digitalWrite(motor_rB, 1);
  analogWrite(ENA,fullSpeed/2.0);
  analogWrite(ENB,fullSpeed/2.0);

  }
  // delay(100);
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
  //DECLARING IRV1 IRV2 IRV3 IRV4 AND IRV5 AS INPUTS
  Serial.begin(9600);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);

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


// void setup()
// {
//   Serial.begin(9600);
//   pinMode(2,INPUT);
// }

// void loop()
// {
//   int reading = digitalRead(2);
//   Serial.println(reading);
//   delay(15);
// }

// الاسود وحايد

void loop()
{

  // Serial.print(digitalRead(IR1));
  Serial.print(digitalRead(IR2));
  Serial.print(digitalRead(IR3));
  Serial.print(digitalRead(IR4));
  // Serial.print(digitalRead(IR5));
  Serial.println();

  // IRV1=digitalRead(IR1);
  IRV2=digitalRead(IR2);
  IRV3=digitalRead(IR3);
  IRV4=digitalRead(IR4);
  // IRV5=digitalRead(IR5);
  
  // Serial.println("---------------------");
  delay(15);



  if(IRV2==0 && IRV3==1 && IRV4==0){
    Forward();
  }
  else if(IRV2==1 && IRV3==1 && IRV4==0){
    Left();
  }
  else if(IRV2==0 && IRV3==1 && IRV4==1){
    Right();
  }
  else if(IRV2==1 && IRV3==0 && IRV4==0){
    Left();
  }
  else if(IRV2==0 && IRV3==0 && IRV4==1){
    Right();
  }
  else if(IRV2==1 && IRV3==1 && IRV4==1){
    while(IRV2==1 && IRV3==1 && IRV4==1){
      Forward();
    }
    if(IRV2==0 && IRV3==0 && IRV4==0){
      while(IRV2==0 && IRV3==0 && IRV4==0){
        Back();
      }
      Left();
    }
  }




  // if(false){
  //   Stop();
  // }
  // else {

  // if (IRV1 == 0 && IRV2 == 0 && IRV3 == 1 && IRV4 == 0 && IRV5 == 0)//Straight path
  //   {
  //     Serial.println("IRV3 1");
  //     Forward();
  //   }

  //   if (IRV1 == 1 && IRV2 == 1 && IRV3 == 0 && IRV4 == 0 && IRV5 == 0)//Left turn
  //   {
  //     Left();
  //     realPath[currRealIdx++]='L';

  //   }

  //   if (IRV1 == 0 && IRV2 == 0 && IRV3 == 0 && IRV4 == 1 && IRV5 == 1)//Right Turn
  //   {
  //     Right();
  //     realPath[currRealIdx++]='R';
      

  //   }

  //   if (IRV1 == 1 && IRV2 == 1 && IRV3 == 0 && IRV4 == 1 && IRV5 == 1)//T Intersection
  //   {
  //     Left(); // As left is possible
  //     realPath[currRealIdx++]='L';

  //   }

  //   if (IRV1 == 1 && IRV2 == 1 && IRV3 == 1 && IRV4 == 0 && IRV5 == 0)//Left T Intersection
  //   {
  //     Left();// As Left is possible
  //     realPath[currRealIdx++]='L';
  //   }

  //   if (IRV1 == 0 && IRV2 == 0 && IRV3 == 1 && IRV4 == 1 && IRV5 == 1)//Right T Tntersection
  //   {
  //     Forward();//As Straight path is possible
  //     realPath[currRealIdx++]='S';

  //   }

  //   if (IRV1 == 0 && IRV2 ==0 && IRV3 == 0 && IRV4 == 0 && IRV5 == 0)//Dead End
  //   {
  //     // U_Turn(); //As no other dIRVection is possible
  //     // Stop();
  //     U_Turn();
  //     // realPath[currRealIdx++]='B';
  //     // Stop();

  //   }


  //   if (IRV1 == 0 && IRV2 == 1 && IRV3 == 1 && IRV4 == 0 && IRV5 == 0)
  //   {
  //     MiniLeft();
  //   }

  //   if (IRV1 == 0 && IRV2 == 0 && IRV3 == 1 && IRV4 == 1 && IRV5 == 0)
  //   {
  //     MinIRVight();
  //   }

  //   //here we want to check if it's + of end of game
  //   if (IRV1 == 1 && IRV2 ==1 && IRV3 == 1 && IRV4 == 1 && IRV5 == 1)//4 Lane intersection //Check this////////////////////////////////////////////////////////
  //   {
  //     bool endgame=0;
  //     slightForward();
  //     if(IRV1 == 1 && IRV2 ==1 && IRV3 == 1 && IRV4 == 1 && IRV5 == 1){
  //       endgame=1;
  //     }
  //     if(endgame){
  //       Stop();
  //     }
  //     else {     
  //       slightBack();
  //       Left(); //As no other dIRVection is possible
  //     }
  //     //if left go left and add L to realPath, if end stop and calc shortest
  //   } 
  
  // }

}

// void loop ()
// {
//   // Left();
//   // Leftmost -> IRV1 - IRV2 - IRV3 - IRV4 - IRV5 <- Rightmost
//   // IRV1 = digitalRead(2);//FIRVst Left
//   // IRV2 = digitalRead(3);//Second Left
//   // IRV3 = digitalRead(4);//Center
//   // IRV4 = digitalRead(5);//Second Right
//   // IRV5 = digitalRead(6);//FIRVst Right
//   // Serial.println("---------------------");
//   // Serial.println(digitalRead(IRV1));
//   // Serial.println(digitalRead(IRV2));
//   // Serial.println(digitalRead(IRV3));
//   // Serial.println(digitalRead(IRV4));
//   // Serial.println(digitalRead(IRV5));


//   Serial.println(digitalRead(IRV1),digitalRead(IRV2),digitalRead(IRV3),digitalRead(IRV4),digitalRead(IRV5));

//   // Serial.println("---------------------");
//   delay(15);


//   if(false){
//     if (IRV1 == 0 && IRV2 == 0 && IRV3 == 1 && IRV4 == 0 && IRV5 == 0)//Straight path
//     {
//       Forward();
//     }
//     else if(IRV1 == 0 && IRV2 == 1 && IRV3 == 1 && IRV4 == 0 && IRV5 == 0)
//     {
//       MiniLeft();
//     }
//     else if(IRV1 == 0 && IRV2 == 0 && IRV3 == 1 && IRV4 == 1 && IRV5 == 0)
//     {
//       MinIRVight();
//     }
//     else if(IRV1 == 1 && IRV2 == 1 && IRV3 == 1 && IRV4 == 1 && IRV5 == 1)
//     {
//       Stop();
//     }
//     else{
//       switch(path[currIdx]){
//         case 'S':
//           Forward();
//           break;
//         case 'L':
//           Left();
//           break;
//         case 'R':
//           Right();
//           break;
//         case 'B': //We'll check if we can remove it.
//           U_Turn();
//           break;
//       }
//       currIdx++;
//     }
//   }
//   else{
//     if (IRV1 == 0 && IRV2 == 0 && IRV3 == 1 && IRV4 == 0 && IRV5 == 0)//Straight path
//     {
//       Serial.println("IRV3 1");
//       Forward();
//     }

//     if (IRV1 == 1 && IRV2 == 1 && IRV3 == 0 && IRV4 == 0 && IRV5 == 0)//Left turn
//     {
//       Left();
//       realPath[currRealIdx++]='L';

//     }

//     if (IRV1 == 0 && IRV2 == 0 && IRV3 == 0 && IRV4 == 1 && IRV5 == 1)//Right Turn
//     {
//       Right();
//       realPath[currRealIdx++]='R';
      

//     }

//     if (IRV1 == 1 && IRV2 == 1 && IRV3 == 0 && IRV4 == 1 && IRV5 == 1)//T Intersection
//     {
//       Left(); // As left is possible
//       realPath[currRealIdx++]='L';

//     }

//     if (IRV1 == 1 && IRV2 == 1 && IRV3 == 1 && IRV4 == 0 && IRV5 == 0)//Left T Intersection
//     {
//       Left();// As Left is possible
//       realPath[currRealIdx++]='L';
//     }

//     if (IRV1 == 0 && IRV2 == 0 && IRV3 == 1 && IRV4 == 1 && IRV5 == 1)//Right T Tntersection
//     {
//       Forward();//As Straight path is possible
//       realPath[currRealIdx++]='S';

//     }

//     if (IRV1 == 0 && IRV2 ==0 && IRV3 == 0 && IRV4 == 0 && IRV5 == 0)//Dead End
//     {
//       // U_Turn(); //As no other dIRVection is possible
//       // Stop();
//       U_Turn();
//       realPath[currRealIdx++]='B';

//     }


//     if (IRV1 == 0 && IRV2 == 1 && IRV3 == 1 && IRV4 == 0 && IRV5 == 0)
//     {
//       MiniLeft();
//     }

//     if (IRV1 == 0 && IRV2 == 0 && IRV3 == 1 && IRV4 == 1 && IRV5 == 0)
//     {
//       MinIRVight();
//     }

//     //here we want to check if it's + of end of game
//     if (IRV1 == 1 && IRV2 ==1 && IRV3 == 1 && IRV4 == 1 && IRV5 == 1)//4 Lane intersection //Check this////////////////////////////////////////////////////////
//     {
//       bool endgame=0;
//       slightForward();
//       if(IRV1 == 1 && IRV2 ==1 && IRV3 == 1 && IRV4 == 1 && IRV5 == 1){
//         endgame=1;
//       }
//       if(endgame){
//         Stop();
//       }
//       else {     
//         slightBack();
//         Left(); //As no other dIRVection is possible
//       }
//       //if left go left and add L to realPath, if end stop and calc shortest
//     } 
  

//     // if (IRV1 == 1 && IRV2 ==1 && IRV3 == 1 && IRV4 == 1 && IRV5 == 1)//End of Maze // can be eliminated and added with case of +
//     // {
//     //   Stop(); //As no other dIRVection is possible

//     //   // CALCULATE_SHORTEST_PATH(realPath, currRealIdx);
//   } 
  
    
// }