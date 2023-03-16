
//Made by Abdo Mohammed
//this is an example to show how the communication is done between a phone and Arduino
//you can skip the first 120 lines and go directly to "loop()" to see how it works xD
//2023-03-14

#ifndef __LIB_COMMUNICATION__
#define __LIB_COMMUNICATION__

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
    return (*((T*)data) == v);
  }


  bool name_equal(const char* c){
    return str_equ(c , name);
  }

  template<typename T>
  T get(){
    return *((T*)data);
  }

  template<typename T>
  T* getPtr(){
    return ((T*)data);
  }
};

class Command{
  char* name;
  CommandArgument* args;
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
      name = new char[name_size+1];
      name[name_size] = '\0';
      readUntil(name , name_size);
      while (Serial.available() == 0){}
      args_size = Serial.read();
      int x = args_size;
      args = new CommandArgument[args_size];
      while (x--){
        //read an argument
        while (Serial.available() == 0){}
        int ns = Serial.read();
        args[args_size - x - 1].name = new char[ns+1];
        args[args_size - x - 1].name[ns] = '\0';
        readUntil(args[args_size - x - 1].name , ns);
        while (Serial.available() == 0){}
        int vs = Serial.read();
        args[args_size - x - 1].data = (void*) new char[vs];
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

  char* cmd(){
    return name;
  }

  int size(){
    return args_size;
  }
};

#endif


void setup() {
  Serial.begin(9600);
}

void loop() {
  Command c;
  if (c.read()){
    Serial.println(c.cmd());
    for (int i = 0;i < c.size();i++){
      Serial.print(c[i].name);Serial.print(" = ");Serial.print(c[i].get<int>());Serial.println("");
    }

    Serial.println(c["rng"].get<float>());
    Serial.println(c["str"].getPtr<char>());
  }

}
