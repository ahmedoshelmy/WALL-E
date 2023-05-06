#include "communication.h"

#include <Servo.h>

//===================== START: Variables =========================
// TODO Rest of variables : Aliaa [DONE]
int commands = 0;
Command c;

#define ROTATING_SERVO_PIN 17
#define RIGHT_CAMERA_SERVO_PIN 16
#define LEFT_CAMERA_SERVO_PIN 15

// TODO : Set Ultrasonic pin
#define ULTRA_SONIC_PIN 5

int game_mode = 0; // for switch between game modes

Servo rotating_servo; // for the up servo
Servo camera_right;   // for camera
Servo camera_left;    // for camera

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

// ====== Ball Collection Variables
//* action_bc:
//* 0 => mean nothing
//* 1 => mean take forward
//* 2 => mean take right
//* 3 => mean take left
//* 4 => avoid forward
int action_bc = 0; // for taken action for Ball Collecting

int round_no = 0; // round number taken by robot

int last_turn_dir = 1; //* right  = 1 | left = -1
bool is_bet_balls = 0; //* is balls in front of WallE
// TODO: refactor better names
// TODO: set values
//* taken from pic
#define d1 65
#define d4 65
#define dd = 30
int d2 = 65;
int d3 = 65;

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

void turnLeft()
{
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
  analogWrite(ENR, fullSpeed * turn_spd_factor);
  analogWrite(ENL, fullSpeed);

  Stop();
  delay(500);

  Forward_The_Left();
  Reverse_The_Right();
  delay(400);
}

void Forward()
{
  analogWrite(ENL, fullSpeed /* * fwd_spd_factor */);
  analogWrite(ENR, fullSpeed /* * fwd_spd_factor */);
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

  Stop_The_Left();
  Stop_The_Right();
}
// I Don't What it do But I found it im defending code
void RotateCamera()
{
  for (int i = -45; i <= 45; i++)
  {
    camera_right.write(90 + i);
    camera_left.write(90 - i);
    delay(15);
  }
}

void setup()
{
  Serial.begin(31250);
  // TODO : Found the right values
  camera_right.attach(15);
  camera_left.attach(16);
  rotating_servo.attach(ROTATING_SERVO_PIN);
  //* for Ball Collecting I think
  rotating_servo.write(180);

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
  // pinMode(3, INPUT);

  Serial.println("start Ball Collecting");
}

void loop()
{
  if (c.read())
  {
    // if (c == "set_game")
    // {
    //     game_mode = c["g"].get<int>();
    // }

    if (c == "set_act")
      action_bc = c["a"].get<int>();
    commands++;
    print("game_mode = %d, commands = %d, act = %d \n", game_mode, commands, action_bc);
  }

  // TODO : Menna
  // delay(50);
}

void finish_round()
{
  // TODO : Sara
}

void first_round()
{
  // TODO : Sara
}
