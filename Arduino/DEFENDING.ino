//===================== START: COMMUNICATION.H ===================== 
//TODO: fix errors related to spamming
char str_buff[200]; //a buffer for printing
void print(const char* format , ...){
  va_list args;
  va_start(args, format);
  vsprintf(str_buff, format, args);
  va_end(args);
  Serial.print(str_buff);
}

bool str_equ(const char* c0 , const char* c1){
  for (int i = 0;;i++){
    if (c0[i] != c1[i])
      return false;
    if (c0[i] == '\0')
      return true;
  }
}

struct CommandArgument{
  char* name;
  void* data;

  template<typename T>
  bool operator==(const T& v){
    return (((T)data) == v);
  }


  bool name_equal(const char* c){
    return str_equ(c , name);
  }

  template<typename T>
  T get(){
    return ((T)data);
  }

  template<typename T>
  T* getPtr(){
    return ((T*)data);
  }
};

class Command{
  char* name = nullptr;
  CommandArgument* args = nullptr;
  int args_size = 0;

  void readUntil(char* buff , int len){
    int i = 0;
    while (len){
      if (Serial.available()){
        buff[i] = Serial.read();
        i++;
        len--;
      }
    }
  }

public:
  bool read(){
    
    if (Serial.available()){
      int name_size = Serial.read();
      name = (char*) malloc(name_size+1);
      name[name_size] = '\0';
      readUntil(name , name_size);
      while (Serial.available() == 0){}
      args_size = Serial.read();
      int x = args_size;
      args = (CommandArgument*) malloc(args_size * sizeof(CommandArgument));
      while (x--){
        //read an argument
        while (Serial.available() == 0){}
        int ns = Serial.read();
        args[args_size - x - 1].name = (char*) malloc(ns+1);
        args[args_size - x - 1].name[ns] = '\0';
        readUntil(args[args_size - x - 1].name , ns);
        while (Serial.available() == 0){}
        int vs = Serial.read();
        args[args_size - x - 1].data = malloc(vs);
        readUntil((char*) args[args_size - x - 1].data , vs);
      }
      return true;
    }
    return false;
  }

  CommandArgument operator[](const char* r){
    for (int i = 0;i < args_size;i++){
      if (str_equ(r , args[i].name)){
        return args[i];
      }
    }

    return args[0]; //default
  }

  CommandArgument operator[](int i){
    return args[i];
  }

  bool operator==(const Command c){
    return str_equ(name , c.name);
  }

  bool operator==(const char* c){
    return str_equ(name , c);
  }

  //returns the command string
  char* cmd(){
    return name;
  }

  //returns the size of the args array
  int size(){
    return args_size;
  }

  ~Command(){
    if (name == nullptr) return;
    free(name);
    for (int i = 0;i < args_size;i++){
      free(args[i].name);
      free(args[i].data);
    }
    if (args != nullptr)
      free(args);

    name = nullptr;
    args = nullptr;
  }
};
//===================== END: COMMUNICATION.H ===================== 

//===================== START: Variables ===================== 

#include <Wire.h>
#include<Servo.h> 



int commands = 0;
Command c;

#define ROTATING_SERVO_PIN 17
#define RIGHT_CAMERA_SERVO_PIN 16
#define LEFT_CAMERA_SERVO_PIN 15

int game_mode = 0; // for switch between game modes
int direction = 0; // for detect the direction of Defend 

Servo rotating_servo; // for the up servo 
Servo camera_right;   // for camera
Servo camera_left;    // for camera

double fwd_spd_factor = 1 / 4.0;
double turn_spd_factor = 1 / 2.0;

int fullSpeed = 255;

int old2 = 2;
int old3 = 3;
int old4 = 4;

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

//===================== END: Variables =====================

// Functions 
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

void Stop() {

  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, LOW);
  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, LOW);
}

void Forward() {

  analogWrite(ENL, fullSpeed * fwd_spd_factor);
  analogWrite(ENR, fullSpeed * fwd_spd_factor);
  Forward_The_Right();
  Forward_The_Left();
}

void Back() {
  analogWrite(ENR, fullSpeed * fwd_spd_factor);
  analogWrite(ENL, fullSpeed * fwd_spd_factor);
  Reverse_The_Left();
  Reverse_The_Right();
}

void RotateCamera(){
  for(int i=-45; i<=45; i++){
    camera_right.write(90+i);
    camera_left.write(90-i);
    delay(15);
  }  
}

void onSetDefending(){
  RotateCamera();
  rotating_servo.write(90);
}
 
// END FUNCTIONS
void setup(){
  Serial.begin(31250);


  camera_right.attach(15);
  camera_left.attach(16);
  rotating_servo.attach(ROTATING_SERVO_PIN);
  rotating_servo.write(0);

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
}

void loop() {
  if (c.read()){
    if (c == "set_game"){
      game_mode = c["g"].get<int>();

      if(game_mode == 2)
        onSetDefending();
    }

    if(c == "set_dirction")
      {
        direction = c["d"].get<int>();

        if(direction == 1)
          Forward(); 
        else if(direction == 2)
          Back();
        else
          Stop();                       
      }

    commands++;
    Serial.print("Command received , updating ...\n");
  }
}