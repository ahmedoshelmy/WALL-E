
// TODO: Implement Left, Forward, Right, U_Turn, Stop functions.
// TODO: Change pins in setup to the right ones.


char path[50];   //Assuming number of operations won't exceed 50
int actualSize;
bool shortestPathCalculated;
char realPath[100];
int currRealIdx;//used in first try when robot is solving the maze

int currIdx;//Used in Second try when robot is walking on correct path

void Forward()
{

}

void Left()
{

}

void Right()
{

}

void U_Turn()
{

}

void Stop()
{

}

void MiniLeft()
{

}

void MiniRight()
{

}


void GOT_SHORTEST_PATH(char MAZE_ARRAY[], int SIZE_OF_ARRAY)
{
  int pIndex=0;
  for(int i=0;i<SIZE_OF_ARRAY;i++)
  {
    if(MAZE_ARRAY[i]!=0)
    {
      path[pIndex++]=MAZE_ARRAY[i];
    }

  }
  actualSize=pIndex;
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
  
  for(int i = 0; i <= SIZE_OF_ARRAY-2; i++)
    {
      ACTION = MAZE_ARRAY[i];
           
      if(ACTION == 'B')
        {
          if(MAZE_ARRAY[i-1]== 'L' && MAZE_ARRAY[i+1] == 'R')
            {
              MAZE_ARRAY[i] = 'B';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
              // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1); 
            }

           if(MAZE_ARRAY[i-1]== 'L' && MAZE_ARRAY[i+1] == 'S')
            {
              MAZE_ARRAY[i] = 'R';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
              // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
            }

            if(MAZE_ARRAY[i-1]== 'R' && MAZE_ARRAY[i+1] == 'L')
            {
              MAZE_ARRAY[i] = 'B';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
              // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);         
            }

            if(MAZE_ARRAY[i-1]== 'S' && MAZE_ARRAY[i+1] == 'L')
            {
              MAZE_ARRAY[i] = 'R';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
              // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);             
            }

            if(MAZE_ARRAY[i-1]== 'S' && MAZE_ARRAY[i+1] == 'S')
            {
              MAZE_ARRAY[i] = 'B';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
              // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);             
            }

            if(MAZE_ARRAY[i-1]== 'L' && MAZE_ARRAY[i+1] == 'L')
            {
              MAZE_ARRAY[i] = 'S';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
              // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
            }
            
          i = -1;
        }
       
       delay(100);   
    }
    shortestPathCalculated = true;
    GOT_SHORTEST_PATH(MAZE_ARRAY,SIZE_OF_ARRAY)
}

void setup 
{
  //DECLARING IR1 IR2 IR3 IR4 AND IR5 AS INPUTS
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);

  //DECLARING MOTORS AS OUTPUTS
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);


  shortestPathCalculated=false;
  actualSize=0;
  currRealIdx=0;
  currIdx=0;


}


void loop 
{
  // Leftmost -> IR1 - IR2 - IR3 - IR4 - IR5 <- Rightmost
  IR1 = digitalRead(2) //First Left
  IR2 = digitalRead(3) //Second Left
  IR3 = digitalRead(4) //Center
  IR4 = digitalRead(5) //Second Right
  IR5 = digitalRead(6) //First Right

  if(shortestPathCalculated){
    if (IR1 == LOW && IR2 == LOW && IR3 == HIGH && IR4 == LOW && IR5 == LOW)//Straight path
    {
      Forward();
    }
    else if(IR1 == LOW && IR2 == HIGH && IR3 == HIGH && IR4 == LOW && IR5 == LOW)
    {
      MiniLeft();
    }
    else if(IR1 == LOW && IR2 == LOW && IR3 == HIGH && IR4 == HIGH && IR5 == LOW)
    {
      MiniRight();
    }
    else if(IR1 == HIGH && IR2 == HIGH && IR3 == HIGH && IR4 == HIGH && IR5 == HIGH)
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
    if (IR1 == LOW && IR2 == LOW && IR3 == HIGH && IR4 == LOW && IR5 == LOW)//Straight path
    {
      Forward();
    }

    if (IR1 == HIGH && IR2 == HIGH && IR3 == LOW && IR4 == LOW && IR5 == LOW)//Left turn
    {
      Left();
      realPath[currRealIdx]='L';

    }

    if (IR1 == LOW && IR2 == LOW && IR3 == LOW && IR4 == HIGH && IR5 == HIGH)//Right Turn
    {
      Right();
      realPath[currRealIdx]='R';
      

    }

    if (IR1 == HIGH && IR2 == HIGH && IR3 == LOW && IR4 == HIGH && IR5 == HIGH)//T Intersection
    {
      Left(); // As left is possible
      realPath[currRealIdx]='L';

    }

    if (IR1 == HIGH && IR2 == HIGH && IR3 == HIGH && IR4 == LOW && IR5 == LOW)//Left T Intersection
    {
      Left();// As Left is possible
      realPath[currRealIdx]='L';
    }

    if (IR1 == LOW && IR2 == LOW && IR3 == HIGH && IR4 == HIGH && IR5 == HIGH)//Right T Tntersection
    {
      Forward();//As Straight path is possible
      realPath[currRealIdx]='S';

    }

    if (IR1 == LOW && IR2 ==LOW && IR3 == LOW && IR4 == LOW && IR5 == LOW)//Dead End
    {
      U_Turn(); //As no other direction is possible
      realPath[currRealIdx]='B';

    }

    if (IR1 == HIGH && IR2 ==HIGH && IR3 == HIGH && IR4 == HIGH && IR5 == HIGH)//4 Lane intersection //Check this
    {
      Left(); //As no other direction is possible
    } 

    if (IR1 == LOW && IR2 == HIGH && IR3 == HIGH && IR4 == LOW && IR5 == LOW)
    {
      MiniLeft();
    }

    if (IR1 == LOW && IR2 == LOW && IR3 == HIGH && IR4 == HIGH && IR5 == LOW)
    {
      MiniRight();
    }

    //--------------
    //      |<-
    //      |
    //      |

    if (IR1 == HIGH && IR2 ==HIGH && IR3 == HIGH && IR4 == HIGH && IR5 == HIGH)//End of Maze
    {
      Stop(); //As no other direction is possible
    } 
  }

}