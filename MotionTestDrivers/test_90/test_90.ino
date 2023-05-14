#include "communication.h"
int commands = 0;
Command c;

#define ROTATING_SERVO_PIN 17
#define RIGHT_CAMERA_SERVO_PIN 16
#define LEFT_CAMERA_SERVO_PIN 15

// TODO: Set Ultrasonic pin
#define ULTRA_SONIC_PIN_T 9
#define ULTRA_SONIC_PIN_E 8
#define MAX_US_RD

int game_mode = 0; // for switch between game modes

// Servo rotating_servo; // for the up servo
// Servo camera_right;   // for camera
// Servo camera_left;    // for camera

//? I think we should use it
#define fwd_spd_factor 1 / 4.0
#define turn_spd_factor 1 / 2.0

int fullSpeed = 255;

int old2 = 2;
int old3 = 3;
int old4 = 4;
int ENR = 3;
int ENL = 4; // Enable Left
int right_motor_front1 = 36;
int right_motor_front2 = 37;
int right_motor_back1 = 39;
int right_motor_back2 = 38;
int left_motor_front1 = 26;
int left_motor_front2 = 27;
int left_motor_back1 = 29;
int left_motor_back2 = 28;
int rollerA = 22 ; 
int rollerB = 23 ; 
bool ReadFromDetection = false;

// ====== Ball Collection Variables
//* action_bc:
//* 0 => mean nothing
//* 1 => mean take forward
//* 2 => mean right
//* 3 => mean left
//* 4 => avoid forward
int action_bc = 0; // for taken action for Ball Collecting

int round_no = 0; // round number taken by robot
const int all_rounds = 5;
const int GoalDist = 40;
int last_turn_dir = 1;       //* right  = 1 | left = -1
int last_round_turn_dir = 1; //* right  = 1 | left = -1
bool is_bet_balls = 0;       //* is balls in front of WallE
// TODO: refa ctor better names
// TODO: set values
//* taken from pic
#define d1 30
#define d4 65
#define dd 30
#define offset_d 20
bool FirstTime = true;
int d2 = 67 ;//250;
int d3 = 220;
// ====== Defending Variables
int direction = 0; // for detect the direction of Defend

//===================== END: Variables =====================
//===================== Start: Methods =====================

void Forward(float l_speed, float r_speed)
{
    
    digitalWrite(right_motor_front1, HIGH);
    digitalWrite(right_motor_front2, LOW);
    digitalWrite(right_motor_back1, HIGH);
    digitalWrite(right_motor_back2, LOW);
    digitalWrite(left_motor_front1, HIGH);
    digitalWrite(left_motor_front2, LOW);
    digitalWrite(left_motor_back1, HIGH);
    digitalWrite(left_motor_back2, LOW);

    analogWrite(ENL, l_speed);
    analogWrite(ENR, r_speed);
    // for(int i = 15 ; i <= 255;i+=20){
    //   analogWrite(ENL, i);
    //   analogWrite(ENR, i);
    //   delay(100) ; 
    // }
}
void Move_Forward_Left() {
  Forward(0, 255);
}
void Move_Forward_Right() {
  Forward(255,0);
}
void Forward_The_Right()
{
  digitalWrite(right_motor_front1, HIGH);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, HIGH);
  digitalWrite(right_motor_back2, LOW);
}
void Forward_The_Left()
{
  digitalWrite(left_motor_front1, HIGH);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, HIGH);
  digitalWrite(left_motor_back2, LOW);
}

void Reverse_The_Right()
{
  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, HIGH);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, HIGH);
}
void Reverse_The_Left()
{
  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, HIGH);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, HIGH);
}

void Stop_The_Left()
{
  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, LOW);
}

void Stop_The_Right()
{
  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, LOW);
}

void adjustRight()
{
  analogWrite(ENR, fullSpeed);
  analogWrite(ENL, fullSpeed);
  Forward_The_Left();
  Reverse_The_Right();
  // while (!isCenter()) {
  //   readSensors();
  // }
}
void onRoller(){
   digitalWrite(rollerA, HIGH);
   digitalWrite(rollerB, LOW);
}

void offRoller(){
   digitalWrite(rollerB, LOW);
   digitalWrite(rollerA, LOW);
}
void turnLeft()
{
  Serial.println("Arduino: Turn Left");

  analogWrite(ENR, fullSpeed);
  analogWrite(ENL, fullSpeed * turn_spd_factor);
  Stop();
  delay(500);
  Forward_The_Right();
  Reverse_The_Left();
  delay(400);
}

void turnRight()
{
  Serial.println("Arduino: Turn Right");

  analogWrite(ENR, fullSpeed * turn_spd_factor);
  analogWrite(ENL, fullSpeed);

  Stop();
  delay(500);

  Forward_The_Left();
  Reverse_The_Right();
  //delay(400);
}


void Back(float speed)
{
    digitalWrite(right_motor_front1, LOW);
    digitalWrite(right_motor_front2, HIGH);
    digitalWrite(right_motor_back1, LOW);
    digitalWrite(right_motor_back2, HIGH);
    digitalWrite(left_motor_front1, LOW);
    digitalWrite(left_motor_front2, HIGH);
    digitalWrite(left_motor_back1, LOW);
    digitalWrite(left_motor_back2, HIGH);
    analogWrite(ENL, speed);
    analogWrite(ENR, speed);
}

void Left(float speed)
{
    print("Arduino : move left");
    digitalWrite(right_motor_front1, HIGH);
    digitalWrite(right_motor_front2, LOW);
    digitalWrite(right_motor_back1, HIGH);
    digitalWrite(right_motor_back2, LOW);
    
    digitalWrite(left_motor_front1, LOW);
    digitalWrite(left_motor_front2, HIGH);
    digitalWrite(left_motor_back1, LOW);
    digitalWrite(left_motor_back2, HIGH);
    analogWrite(ENL, speed);
    analogWrite(ENR, speed);
}

void Forward_The_Right(float speed)
{
    digitalWrite(right_motor_front1, HIGH);
    digitalWrite(right_motor_front2, LOW);
    digitalWrite(right_motor_back1, HIGH);
    digitalWrite(right_motor_back2, LOW);
    analogWrite(ENL, speed);
    analogWrite(ENR, speed);
}
void Forward_The_Left(float speed)
{
    digitalWrite(right_motor_front1, HIGH);
    digitalWrite(right_motor_front2, LOW);
    digitalWrite(right_motor_back1, HIGH);
    digitalWrite(right_motor_back2, LOW);
    analogWrite(ENL, speed);
    analogWrite(ENR, speed);
}

void Right(float speed)
{
    print("Arduino : move right");
    digitalWrite(right_motor_front1, LOW);
    digitalWrite(right_motor_front2, HIGH);
    digitalWrite(right_motor_back1, LOW);
    digitalWrite(right_motor_back2, HIGH);

    digitalWrite(left_motor_front1, HIGH);
    digitalWrite(left_motor_front2, LOW);
    digitalWrite(left_motor_back1, HIGH);
    digitalWrite(left_motor_back2, LOW);

    analogWrite(ENL, speed);
    analogWrite(ENR, speed);
}

void U_Turn(float speed)
{
    digitalWrite(right_motor_front1, HIGH);
    digitalWrite(right_motor_front2, LOW);
    digitalWrite(right_motor_back1, HIGH);
    digitalWrite(right_motor_back2, LOW);
    digitalWrite(left_motor_front1, LOW);
    digitalWrite(left_motor_front2, HIGH);
    digitalWrite(left_motor_back1, LOW);
    digitalWrite(left_motor_back2, HIGH);
    analogWrite(ENL, speed);
    analogWrite(ENR, speed);

    // delay(5000);
}

void Stop()
{
    digitalWrite(right_motor_front1, LOW);
    digitalWrite(right_motor_front2, LOW);
    digitalWrite(right_motor_back1, LOW);
    digitalWrite(right_motor_back2, LOW);
    digitalWrite(left_motor_front1, LOW);
    digitalWrite(left_motor_front2, LOW);
    digitalWrite(left_motor_back1, LOW);
    digitalWrite(left_motor_back2, LOW);
    analogWrite(ENL, 0);
    analogWrite(ENR, 0);
}

// I Don't What it do But I found it in defending code
void RotateCamera()
{
  for (int i = -45; i <= 45; i++)
  {
    // camera_right.write(90 + i);
    // camera_left.write(90 - i);
    // delay(15);
  }
}
// int ReadUS() {

//   do {

//   } while();
// }
int ReadUS()
{
  // to read ultrasonic sensor value
  // Clears the trigPin
  float distance = 0;
  
  do {
    digitalWrite(ULTRA_SONIC_PIN_T, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(ULTRA_SONIC_PIN_T, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRA_SONIC_PIN_T, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    float duration = pulseIn(ULTRA_SONIC_PIN_E, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;

    // Prints the distance on the Serial Monitor
    
    delay(50);
  } while(distance >= 600);

  Serial.print("dist : ");
    Serial.println(distance);
  return distance;
}



void first_round()
{
      FirstTime = false;
  // TODO : Sara
  Forward(fullSpeed,fullSpeed);
  while (ReadUS() > d1)
    ;
  Stop();
  delay(10);
  Serial.println("Turn right");
  Right(fullSpeed);
  turn_90_deg();

  Forward(fullSpeed, fullSpeed);
}
void turn_90_deg() {
  print("$start_turn{}");
  while(1) {
    if (c.read())
    {
      ReadFromDetection = true;
      if (c == "set_game")
      {
          game_mode = c["g"].get<int>();
      }
      else if (c == "set_act") {
        action_bc = c["a"].get<int>();
      }
      commands++;
      print("game_mode = %d, commands = %d, act = %d \n", game_mode, commands, action_bc);
    }
    if(action_bc == 5)  {
      action_bc = 0;
      break;
    }
  }
  print("Stop Turning \n");
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(31250);

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
  pinMode(rollerA, OUTPUT);
  pinMode(rollerB, OUTPUT);

  pinMode(ULTRA_SONIC_PIN_T, OUTPUT); // Sets the trigPin as an Output
  pinMode(ULTRA_SONIC_PIN_E, INPUT);  // Sets the echoPin as an Input
  print("========== Start ball collecting ==========\n");
  //print("$start_ball{}");
  // pinMode(3, INPUT);


  print("$start{m=2,d=90}"); //forward



}

void loop() {
  // Serial.print(ReadUS());
  // // nothing, TK_Forward, TK_right, TK_left, AVD_forward
  // // 0         1           2         3        4
  // // put your main code here, to run repeatedly:
  // int last_dir = -1;
  // int us_read = -1000;
  // if(c.read()) {
  //   if(c == "set_d") {
  //     int d = c["d"].get<int>();
  //     switch (d) {
  //       case 1:
  //         // if(us_read - ReadUS() > 40 || ReadUS() - us_read  > 40) {
  //           print("Arduino : take forward \n");
  //           // print("$start{m=0}"); //forward
  //           Forward(255,255);
  //           onRoller();
  //           // us_read = ReadUS();
  //         // }
  //         break;
  //       case 4:
  //         // if(us_read - ReadUS() > 40 || ReadUS() - us_read  > 40) {
  //           print("Avd Forward \n");
  //           offRoller();
  //           // TODO: add direction;
  //           Move_Forward_Left();
  //           onRoller();
  //           //us_read = ReadUS();
  //         // }
  //         break;
        
  //       case 2:
  //         print("Arduino : take Right\n");
  //         // print("$start{m=2,deg=-45}"); //right
  //         Move_Forward_Right();
  //         onRoller();

          
  //         break;
  //       case 3:
  //         print("Arduino : take left \n");
  //         // print("$start{m=2,deg=45}"); //left
  //         Move_Forward_Left();
  //         onRoller();


  //         // Left(100);
  //         break;

  //       default: 
  //         Forward(255,255);
  //         break;
  //     }
  //       // case 0:
  //       //   Forward(255, 255);
  //       //   print("Arduino: Move Forward %i \n", d);
  //       //   break;
  //       // case 1:
  //       //   Left(255);
  //       //   print("Arduino: Move Left %i \n", d);
  //       //   break;
  //       // case 2:
  //       //   Right(255);
  //       //   print("Arduino: Move Right %i \n", d);
  //       //   break;
      
  //   }
  // }

   if(c == "move") {
      int d = c["d"].get<int>();
      switch (d) {
        case 0:
          Forward(255, 255);
          print("Arduino: Move Forward %i \n", d);
          break;
        case 1:
          Left(255);
          print("Arduino: Move Left %i \n", d);
          break;
        case 2:
          Right(255);
          print("Arduino: Move Right %i \n", d);
          break;
        case 3:
          Stop();
          print("Arduino: Stop \n");
          break;
      }
    }
  }
  //delay(1000);
  //Right(255);
  // delay(3000);

  // delay(500);
  // Forward(0,255); //right
  // delay(500);
  // Forward(255, 255);
}
