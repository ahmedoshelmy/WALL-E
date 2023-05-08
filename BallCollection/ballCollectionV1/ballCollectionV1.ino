#include "communication.h"

#include <Servo.h>

//===================== START: Variables =========================
// TODO Rest of variables : Aliaa [DONE]
int commands = 0;
Command c;

#define ROTATING_SERVO_PIN 17
#define RIGHT_CAMERA_SERVO_PIN 16
#define LEFT_CAMERA_SERVO_PIN 15

// TODO: Set Ultrasonic pin
#define ULTRA_SONIC_PIN_T 12
#define ULTRA_SONIC_PIN_E 13

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

#define ENR 3
#define ENL 4 // ENLble adjustLeft
#define right_motor_back1 36
#define right_motor_back2 37
#define right_motor_front1 39
#define right_motor_front2 38
#define left_motor_front1 26
#define left_motor_front2 27
#define left_motor_back1 29
#define left_motor_back2 28
#define rollerA 22
#define rollerB 23

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
  Stop_The_Right();
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

void Forward()
{
  Serial.println("Arduino: Move Forward");
  analogWrite(ENL, fullSpeed);
  analogWrite(ENR, fullSpeed);
  Forward_The_Right();
  Forward_The_Left();
}

void Back()
{
  analogWrite(ENR, fullSpeed * fwd_spd_factor);
  analogWrite(ENL, fullSpeed * fwd_spd_factor);
  Reverse_The_Left();
  Reverse_The_Right();
}
void adjustLeft()
{
  analogWrite(ENR, fullSpeed);
  analogWrite(ENL, fullSpeed);
  Forward_The_Right();
  Stop_The_Left();
}

void U_Turn()
{

  analogWrite(ENR, fullSpeed);
  analogWrite(ENL, fullSpeed /* * turn_spd_factor */);

  Forward_The_Right();
  Reverse_The_Left();

  delay(500);
  // adjustRight();
}
void Stop()
{

  Serial.println("Arduino: Stop");
  Stop_The_Left();
  Stop_The_Right();
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

int ReadUS()
{
  // to read ultrasonic sensor value
  // Clears the trigPin
  digitalWrite(ULTRA_SONIC_PIN_T, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(ULTRA_SONIC_PIN_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_SONIC_PIN_T, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  float duration = pulseIn(ULTRA_SONIC_PIN_E, HIGH);
  // Calculating the distance
  float distance = duration * 0.034 / 2;

  // Prints the distance on the Serial Monitor
  Serial.print("dist : ");
  Serial.println(distance);
  delay(50);

  return distance;
}



void first_round()
{
      FirstTime = false;
  // TODO : Sara
  Forward();
  while (ReadUS() > d1)
    ;

  Stop();
  delay(10);
  Serial.println("Turn right");
  turnRight();
  // Serial.println("Enter First");
  bool is_inc = 1;
  int d_read = ReadUS();
  do {
    Serial.print(is_inc);Serial.print("\t");
    int d_n = ReadUS();
    if(d_read > d_n) { is_inc = 0;
    }
    d_read= d_n;
  } while(is_inc || d_read < d2);
  // while()
  // if(d2 > d1) {
  //   Serial.println("Enter Second");
  //   d_read = ReadUS();
  //   while (!(d_read > d2 && d_read < d2 + 15))
  //     d_read = ReadUS();;  
  // }

  Forward();
}
void setup()
{
  Serial.begin(31250);
  // TODO : Found the right values
  // camera_right.attach(15);
  // camera_left.attach(16);
  // rotating_servo.attach(ROTATING_SERVO_PIN);
  //* for Ball Collecting I think
  // rotating_servo.write(180);
  // camera_left.write(180);
  // camera_right.write(180);

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
  pinMode(rollerA, OUTPUT);
  pinMode(rollerB, OUTPUT);

  pinMode(ULTRA_SONIC_PIN_T, OUTPUT); // Sets the trigPin as an Output
  pinMode(ULTRA_SONIC_PIN_E, INPUT);  // Sets the echoPin as an Input
  // pinMode(3, INPUT);

  Serial.println("start Ball Collecting");
  // Forward();
  //onRoller();
  //first_round();
  //turnRight();
  
  
}
bool ReadFromDetection = false;
void loop()
{
  
  // Serial.println("Arduino: Turn Right");
  
  // analogWrite(ENR, fullSpeed );
  // analogWrite(ENL, fullSpeed);
  // Forward_The_Left();
  // Reverse_The_Right();
  // delay(2000);
  

  // Stop();
  // delay(500);

  
  //Serial.println(ReadUS());
  //delay(50);
  
  //finish_round();
  //while (1)
   // ;
  // turnRight();
  // delay(1000);
  // turnLeft();
  //  Serial.println("woo ");
  // delay(1000);
  // Stop();
  // // ;  ReadUS();

    if (c.read())
    {
      ReadFromDetection = true;
      if (c == "set_game")
      {
          game_mode = c["g"].get<int>();
      }

      if (c == "set_act")
        action_bc = c["a"].get<int>();
      commands++;
      print("game_mode = %d, commands = %d, act = %d \n", game_mode, commands, action_bc);
            switch (action_bc) {
                case 4:
                    print("Arduino: Avoid Forward\n");
                    break;
                case 1:
                    print("Arduino: Move Forward\n");
                    break;
                case 2:
                    print("Arduino: Move Right\n");
                    break;
                case 3:
                    print("Arduino: Move Left\n");
                    break;
                default:
                    print("Arduino: nothing\n");
                    break;
            }
    }
    // // // Menna :TODO: put the ultrasonic code and ultraReading will be the distance
    // // // the robot start and will walk until the wall then turn

    // if (FirstTime)
    //   first_round();

    // if (ReadFromDetection)
    // {
    //   ReadFromDetection = false;
    //   // Assuming that it will move in slow motion till the action is not right
    //   if (action_bc == 2 && last_turn_dir != 1)
    //   {
    //     is_bet_balls = !is_bet_balls;
    //     last_turn_dir = 1;
    //     turnRight();
    //   }
    //   if (action_bc == 3 && last_turn_dir != -1)
    //   { // if it faces a ball that it has to avoid

    //     is_bet_balls = !is_bet_balls;
    //     last_turn_dir = -1;
    //     turnLeft();
    //   }
    //   if (action_bc == 1)
    //   { // Move forward
    //     last_turn_dir = 0;
    //     Forward();
    //   }
    //   if(action_bc == 4 ) //avoid forward
    //    {if (last_turn_dir==1) // if the last dir is right turn left
    //         turnLeft();
    //         else if (last_turn_dir==-1) //if the last dir is left turn right
    //         turnRight();

    //   }
    // }
    // // // Menna :TODO: put the ultrasonic code and ultraReading will be the distance
    // if (ReadUS() <= d1)
    // {
    //   finish_round();
    // }
    // if (round_no == all_rounds)
    // {
    //     Forward();
    //   while (ReadUS() > GoalDist);
    //   turnLeft();
    //   while(ReadUS() > 10)
    //   Stop();
    //   // Menna:TODO: the robot must round like in the first round but in the opposite direction
    //   // We have to call firstround here
    // Serial.println(" == Finish Finish All == ");
    // while(1);
    // }

    // // TODO : Menna
    // delay(50);
}

void finish_round()
{
  round_no++;
  if (is_bet_balls)
    d2 -= offset_d;
  is_bet_balls = 0;
  last_turn_dir = 0;
  // TODO : Sara
  // first turn
  if (last_round_turn_dir == 1)
    turnRight();
  else
    turnLeft();
  while (!(ReadUS() > d2 && ReadUS() < d2 + 5))
      ;
    delay(50);
  if(d2 > d1) {
    while (!(ReadUS() > d2 && ReadUS() < d2 + 5))
      ;  
  }
  // move forward
  Forward();
  while (ReadUS() > d3) //d3
    ;

  // Second turn
  if (last_round_turn_dir == 1)
    turnRight();
  else
    turnLeft();

  while (!(ReadUS() > d4 && ReadUS() < d4 + 5))
      ;
    delay(50);
  if(d4 > d3) {
    while (!(ReadUS() > d4 && ReadUS() < d4 + 5))
      ;  
  }
  last_round_turn_dir *= -1;
  d2 = d3;
  d3 -= dd;
  Stop();
}


