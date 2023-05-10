#include "communication.h"

void setup() {
  // put your setup code here, to run once:
    Serial.begin(31250);

    print("$start{m=2,d=90}"); //forward


}
int commands = 0;
Command c;
void loop() {
  // put your main code here, to run repeatedly:
  if(c.read()) {
    if(c == "move") {
      int d = c["d"].get<int>();
      switch (d) {
        case 0:
          print("Arduino: Move Forward %i \n", d);
          break;
        case 1:
          print("Arduino: Move Left %i \n", d);
          break;
        case 2:
          print("Arduino: Move Right %i \n", d);
          break;
      }
    }
  }
    delay(1000);
    // print("$start{m=1}"); //centering
    // delay(1000);

    // print("$start{m=2,d=10}");//turning
    // delay(1000);

    // print("$start{m=2,d=20}");//turning
    // delay(1000);

    // print("$start{m=2,d=-90}");//turning
    // delay(1000);


    // print("$start{m=3}"); //stoping

  // if (c.read())
  //   {
  //     ReadFromDetection = true;
  //     if (c == "set_game")
  //     {
  //         game_mode = c["g"].get<int>();
  //     }

  //     if (c == "set_act")
  //       action_bc = c["a"].get<int>();
  //     commands++;
  //     print("game_mode = %d, commands = %d, act = %d \n", game_mode, commands, action_bc);
  //           switch (action_bc) {
  //               case 4:
  //                   print("Arduino: Avoid Forward\n");
  //                   break;
  //               case 1:
  //                   print("Arduino: Move Forward\n");
  //                   break;
  //               case 2:
  //                   print("Arduino: Move Right\n");
  //                   break;
  //               case 3:
  //                   print("Arduino: Move Left\n");
  //                   break;
  //               default:
  //                   print("Arduino: nothing\n");
  //                   break;
  //           }
  //   }

}
