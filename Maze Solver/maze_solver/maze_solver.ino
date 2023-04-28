// TODO: Check how to differentiate between lane Intersection and end of maze
// TODO: Implement Left, Forward, Right, U_Turn, Stop functions.
// TODO: Change pins in setup to the right ones.

//connect 10 to EnA
//connect 11 to EnB

double fwd_spd_factor = 1 / 4.0;
double turn_spd_factor = 1 / 2.0;

char path[50];   //Assuming number of operations won't exceed 50
int actualSize;
bool shortestPathCalculated;
char realPath[100];
int currRealIdx;//used in fIRVst try when robot is solving the maze

int currIdx;//Used in Second try when robot is walking on correct path

int fullSpeed=254;

bool flag=1;

int turn_delay = 1000 ; 

int ENA=9;
int ENB=13;

int IR1=2;
int IR2=11;
int IR3=10;
int IR4=6;
int IR5=6;

int IRV1;
int IRV2=-1;
int IRV3=-1;
int IRV4=-1;
int IRV5;

int old2=2;
int old3=3;
int old4=4;


// int IRV1Add=2;
// int IRV2Add=3;
// int IRV3Add=4;
// int IRV4Add=5;
// int IRV5Add=6;

int motor_lA = 7;
int motor_lB = 8;
int motor_rA = 9;
int motor_rB = 12;///connect IN4 with 12

int right_motor_front1 = 2; 
int right_motor_front2 = 3; 
int right_motor_back1  = 4; 
int right_motor_back2  = 5; 
int left_motor_front1  = 15; 
int left_motor_front2  = 16; 
int left_motor_back1   = 17; 
int left_motor_back2   = 18; 



bool calculateShortestAgain=false;

bool CheckEndGame()
{
  Forward();
  delay(100);
  if(IRV2==1 && IRV3==1 && IRV4==1){
    return true;
  } //end of maze
  return false;
}

void Forward(){

  digitalWrite(right_motor_front1 ,HIGH);
  digitalWrite(right_motor_front2 ,LOW);
  digitalWrite(right_motor_back1 ,HIGH);
  digitalWrite(right_motor_back2 ,LOW);
  digitalWrite(left_motor_front1 ,HIGH);
  digitalWrite(left_motor_front2 ,LOW);
  digitalWrite(left_motor_back1 ,HIGH);
  digitalWrite(left_motor_back2 ,LOW);
  analogWrite(ENA, fullSpeed * fwd_spd_factor);
  analogWrite(ENB, fullSpeed * fwd_spd_factor);

}


void Back()
{
  digitalWrite(right_motor_front1 ,LOW);
  digitalWrite(right_motor_front2 ,HIGH);
  digitalWrite(right_motor_back1 ,LOW);
  digitalWrite(right_motor_back2 ,HIGH);
  digitalWrite(left_motor_front1 ,LOW);
  digitalWrite(left_motor_front2 ,HIGH);
  digitalWrite(left_motor_back1 ,LOW);
  digitalWrite(left_motor_back2 ,HIGH);
  analogWrite(ENA, fullSpeed * fwd_spd_factor);
  analogWrite(ENB, fullSpeed * fwd_spd_factor);
  // delay(50);
}

void slightForward()
{
  digitalWrite(motor_lA, 1);
  digitalWrite(motor_lB, 0);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  analogWrite(ENA,fullSpeed/4.0);
  analogWrite(ENB,fullSpeed/4.0);
  // delay(50);
}

void Left()
{
  digitalWrite(right_motor_front1 ,HIGH);
  digitalWrite(right_motor_front2 ,LOW);
  digitalWrite(right_motor_back1 ,HIGH);
  digitalWrite(right_motor_back2 ,LOW);
  digitalWrite(left_motor_front1 ,LOW);
  digitalWrite(left_motor_front2 ,HIGH);
  digitalWrite(left_motor_back1 ,LOW);
  digitalWrite(left_motor_back2 ,HIGH);
  analogWrite(ENA, fullSpeed * turn_spd_factor);
  analogWrite(ENB, fullSpeed * turn_spd_factor);

  IRV3 = digitalRead(IR3);
  IRV2 = digitalRead(IR2);
  IRV4 = digitalRead(IR4);
  
  delay(turn_delay);
  // while(IRV3!=1 || IRV2!=0 || IRV4!=0){   //Center
  // IRV3 = digitalRead(IR3);
  // IRV2 = digitalRead(IR2);
  // IRV4 = digitalRead(IR4);
  //   digitalWrite(right_motor_front1 ,HIGH);
  //   digitalWrite(right_motor_front2 ,LOW);
  //   digitalWrite(right_motor_back1 ,HIGH);
  //   digitalWrite(right_motor_back2 ,LOW);
  //   digitalWrite(left_motor_front1 ,LOW);
  //   digitalWrite(left_motor_front2 ,HIGH);
  //   digitalWrite(left_motor_back1 ,LOW);
  //   digitalWrite(left_motor_back2 ,HIGH);
  // }

  // delay(1000);
}

void Forward_The_Right(){
  digitalWrite(right_motor_front1 ,HIGH);
  digitalWrite(right_motor_front2 ,LOW);
  digitalWrite(right_motor_back1 ,HIGH);
  digitalWrite(right_motor_back2 ,LOW);
}
void Forward_The_Left(){
  digitalWrite(right_motor_front1 ,HIGH);
  digitalWrite(right_motor_front2 ,LOW);
  digitalWrite(right_motor_back1 ,HIGH);
  digitalWrite(right_motor_back2 ,LOW);
}



void Right()
{
  digitalWrite(right_motor_front1 ,LOW);
  digitalWrite(right_motor_front2 ,HIGH);
  digitalWrite(right_motor_back1 ,LOW);
  digitalWrite(right_motor_back2 ,HIGH);
  digitalWrite(left_motor_front1 ,HIGH);
  digitalWrite(left_motor_front2 ,LOW);
  digitalWrite(left_motor_back1 ,HIGH);
  digitalWrite(left_motor_back2 ,LOW);
  analogWrite(ENA, fullSpeed * turn_spd_factor);
  analogWrite(ENB, fullSpeed * turn_spd_factor);

IRV3 = digitalRead(IR3);
  IRV2 = digitalRead(IR2);
  IRV4 = digitalRead(IR4);
    delay(turn_delay); 
    
  // while(IRV3!=1 || IRV2!=0 || IRV4!=0){
  //   IRV3 = digitalRead(IR3);
  // IRV2 = digitalRead(IR2);
  // IRV4 = digitalRead(IR4);
  //  digitalWrite(right_motor_front1 ,LOW);
  // digitalWrite(right_motor_front2 ,HIGH);
  // digitalWrite(right_motor_back1 ,LOW);
  // digitalWrite(right_motor_back2 ,HIGH);
  // digitalWrite(left_motor_front1 ,LOW);
  // digitalWrite(left_motor_front2 ,HIGH);
  // digitalWrite(left_motor_back1 ,LOW);
  // digitalWrite(left_motor_back2 ,HIGH);
  // }

  // delay(1000);
}

void U_Turn()
{
  digitalWrite(right_motor_front1 ,HIGH);
  digitalWrite(right_motor_front2 ,LOW);
  digitalWrite(right_motor_back1 ,HIGH);
  digitalWrite(right_motor_back2 ,LOW);
  digitalWrite(left_motor_front1 ,LOW);
  digitalWrite(left_motor_front2 ,HIGH);
  digitalWrite(left_motor_back1 ,LOW);
  digitalWrite(left_motor_back2 ,HIGH);
  analogWrite(ENA,fullSpeed * turn_spd_factor);
  analogWrite(ENB,fullSpeed * turn_spd_factor);
    
    
  while(IRV2!=0 || IRV3!=1 || IRV4!=0){
    IRV2=digitalRead(IR2);
    IRV3=digitalRead(IR3);
    IRV4=digitalRead(IR4);
    digitalWrite(motor_lA, 0);
  digitalWrite(motor_lB, 1);
  digitalWrite(motor_rA, 1);
  digitalWrite(motor_rB, 0);
  }

  // delay(5000);
}

void Stop()
{
  digitalWrite(right_motor_front1 ,LOW);
  digitalWrite(right_motor_front2 ,LOW);
  digitalWrite(right_motor_back1 ,LOW);
  digitalWrite(right_motor_back2 ,LOW);
  digitalWrite(left_motor_front1 ,LOW);
  digitalWrite(left_motor_front2 ,LOW);
  digitalWrite(left_motor_back1 ,LOW);
  digitalWrite(left_motor_back2 ,LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

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
  analogWrite(ENA,fullSpeed/4.0);
  analogWrite(ENB,fullSpeed/4.0);

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
  analogWrite(ENA,fullSpeed/4.0);
  analogWrite(ENB,fullSpeed/4.0);

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
  pinMode(right_motor_front1, OUTPUT);
  pinMode(right_motor_front2, OUTPUT);
  pinMode(right_motor_back1, OUTPUT);
  pinMode(right_motor_back2, OUTPUT);
   pinMode(left_motor_front1, OUTPUT);
  pinMode(left_motor_front2, OUTPUT);
  pinMode(left_motor_back1, OUTPUT);
  pinMode(left_motor_back2, OUTPUT);



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
  // Forward();

  // Serial.print(digitalRead(IR1));
  // if(IRV2!=old2 || IRV3!=old3 || IRV4!=old4){
  // Serial.print(IRV2);
  // Serial.print(IRV3);
  // Serial.print(IRV4);
  // Serial.println();
  // old2=IRV2;old3=IRV3;old4=IRV4;
  // }
  // // Serial.print(digitalRead(IR5));

  // IRV1=digitalRead(IR1);
  IRV2=digitalRead(IR2);
  IRV3=digitalRead(IR3);
  IRV4=digitalRead(IR4);
  // IRV5=digitalRead(IR5);
  // Forward();
  
  // Serial.println("---------------------");
  delay(15);



if(flag){
  Serial.println('B');

  if(IRV2==0 && IRV3==1 && IRV4==0){        //Straight
    Forward();
  }
  else if(IRV2==1 && IRV3==1 && IRV4==0){   //Left
    // Serial.println('L');
    Left();
    delay(100);
    // realPath[currRealIdx++]='L';
  }
  else if(IRV2==0 && IRV3==1 && IRV4==1){   //Right
    //Check for a straight-right intersection
    // Serial.println('R');
    // while(IRV2==0 && IRV3==1 && IRV4==1){
    //   IRV2=digitalRead(IR2);
    //   IRV3=digitalRead(IR3);
    //   IRV4=digitalRead(IR4);
    //   Forward();
    //   // realPath[currRealIdx++]='S';
    //   // Serial.println('S');
    // }
    // if(IRV2==0 && IRV3==0 && IRV4==0){    //No intersection, just right
    //   while(IRV2==0 && IRV3==0 && IRV4==0){
    //     IRV2=digitalRead(IR2);
    //   IRV3=digitalRead(IR3);
    //   IRV4=digitalRead(IR4);
    //     Back();
    //   }
      Left();
      delay(100);
      // realPath[currRealIdx++]='R';
    }
    else if(IRV2==0 && IRV3==1 && IRV4==0){   //Intersection, we go forward
      Forward();
      // realPath[currRealIdx++]='S';
      // Serial.println('S');
    }
  }
  else if(IRV2==1 && IRV3==0 && IRV4==0){   //Left awy
    Left();
    delay(100);
  }
  else if(IRV2==0 && IRV3==0 && IRV4==1){   //Right awy
    
    Right();
    delay(100);
  }
  else if(IRV2==1 && IRV3==1 && IRV4==1){   //Intersection
    // Check if end of maze
    // bool endGame = CheckEndGame();
    // if(endGame){ //end of maze
    //   Stop();
    //   flag=0;
    //   CALCULATE_SHORTEST_PATH(realPath, currRealIdx);
    // }
    // else {
    //   while(IRV2==0 && IRV3==0 && IRV4==0){
    //     IRV2=digitalRead(IR2);
    //   IRV3=digitalRead(IR3);
    //   IRV4=digitalRead(IR4);
    //     Back();
    //   }
    // }
    //T intersection or cross, turn left
    // Serial.println('L');
    Left();
    delay(100);
    // realPath[currRealIdx++]='L';
  }
  else if(IRV2==0 && IRV3==0 && IRV4==0){   //Dead End
    Serial.println('B');
    U_Turn();
    delay(100);
    // realPath[currRealIdx++]='B';
  }
  // delay(5000);
}

// if(!flag){
//   if(IRV2==1 && IRV3==0 && IRV4==0){
//     Left();
//   }
//   else if(IRV2==0 && IRV3==0 && IRV4==1){
//     Right();
//   }
//   else if(currIdx==actualSize){
//     Stop();
//   }
//   else if(!(IRV2==0 && IRV3==1 && IRV4==0)){  // Not Straight
//     switch(path[currIdx++]){
//       case 'L':
//         Left();
//       case 'R':
//         Right();
//       case 'S':
//         Forward();
//       case 'B':
//         U_Turn();
//     }
//   }
// }



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

// }

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