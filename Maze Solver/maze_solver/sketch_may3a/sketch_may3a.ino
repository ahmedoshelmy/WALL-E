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

int game_mode = 0;

#include <Wire.h>
#include <Servo.h> 
Servo s;
// #include <LiquidCrystal_I2C.h>

// LiquidCrystal_I2C lcd(0x27,20,4); 

int ENR = 3 ; 
int ENL = 4 ; // Enable Left 
int right_motor_front1 = 36; 
int right_motor_front2 = 37; 
int right_motor_back1  = 38; 
int right_motor_back2  = 39; 
int left_motor_front1  = 26; 
int left_motor_front2  = 27; 
int left_motor_back1   = 28; 
int left_motor_back2   = 29; 



void pinModeMotors(){
  pinMode(right_motor_front1 ,OUTPUT);
  pinMode(right_motor_front2 ,OUTPUT);
  pinMode(right_motor_back1 ,OUTPUT);
  pinMode(right_motor_back2 ,OUTPUT);
  pinMode(left_motor_front1 ,OUTPUT);
  pinMode(left_motor_front2 ,OUTPUT);
  pinMode(left_motor_back1 ,OUTPUT);
  pinMode(left_motor_back2 ,OUTPUT);
}

void lCDINIT(){
  //   lcd.init();
  // lcd.backlight();
  // lcd.clear();
  // lcd.setCursor(0,0);
  // lcd.print("Mirco-Project V1.0");
  // for (int i = 2;i > 0;i--){
  //   lcd.setCursor(0,1);
  //   lcd.print("Starting in : ");
  //   lcd.print(i);
  //   delay(1000);
  // }
  // lcd.setCursor(0, 1);
  // lcd.print("                      ");
}

int commands = 0;
Command c;

void CheckGameLCDSerial(){
//  if (c.read()){
//     if (c == "set_game"){
//       game_mode = c["g"].get<int>();
//     }
//     commands++;
//     Serial.print("Command received , updating ...\n");
//   }

  // if (commands == 0){
  //   lcd.setCursor(0, 1);
  //   lcd.print("Waiting for cmd ..");
  // }else{
  //   lcd.setCursor(0, 1);
  //   lcd.print("Received: ");
  //   lcd.print(commands);
  // }

  

  // lcd.setCursor(0, 2);
  // lcd.print("Game mode = ");
  // lcd.print(game_mode);

  // lcd.setCursor(2, 3);
  
  // switch (game_mode) {
  // case 0:
  //   lcd.print("Ball Collect     ");
  //   break;
  // case 1:
  //   lcd.print("Darts            ");
  //   break;
  // case 2:
  //   lcd.print("Defending        ");
  //   break;
  // case 3:
  //   lcd.print("Maze             ");
  //   break;
  // case 4:
  //   lcd.print("Pew Pew          ");
  //   break;
  // default:
  //   lcd.print("ERROR            ");
  // }

}


void Stop(){

  digitalWrite(right_motor_front1 ,LOW);
  digitalWrite(right_motor_front2 ,LOW);
  digitalWrite(right_motor_back1 ,LOW);
  digitalWrite(right_motor_back2 ,LOW);
  digitalWrite(left_motor_front1 ,LOW);
  digitalWrite(left_motor_front2 ,LOW);
  digitalWrite(left_motor_back1 ,LOW);
  digitalWrite(left_motor_back2 ,LOW);
  analogWrite(ENR ,100 ) ; 
  analogWrite(ENL ,100 ) ; 

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

}




void setup()
{
  Serial.begin(31250);
  pinModeMotors();
}

void loop ()
{

  Forward();
}