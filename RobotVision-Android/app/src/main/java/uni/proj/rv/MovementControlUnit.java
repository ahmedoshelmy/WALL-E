package uni.proj.rv;

import uni.proj.ec.Command;
// ================ RULES =========
// Right = decreasing
// Left = increasing
public class MovementControlUnit {
    static final int ERR_RNG = 3;
    public  enum State {
        Forward,    //move forward straight
        Centering,  //! centering it self to take the ball
        Turning,    //rotating with specific angle
        Stopped
    }

    enum  DIRECTION {
        FRONT,
        LEFT,
        RIGHT,
        Stop,
        ADJ_LEFT,
        ADJ_RIGHT
    }

    RobotGame robotGame;
    public State curr_state;
    int first_degree;   // first degree at start of any state
    int curr_degree;    // current degree in that time

    int degree_turn;    // what i want the robot to turn "turn angle"

    public MovementControlUnit(RobotGame robotGame) {
        curr_state = State.Stopped;
        first_degree = -1;
        degree_turn = 0;
        this.robotGame = robotGame;
    }

    // called by sensor when it feel change
    // handel the state machine
    public void handelDegreeChange(int degree) throws Exception {

        curr_degree = degree;
        // if it's the first time
        if(first_degree == -1) {
            first_degree = curr_degree;
            return;
        }

        switch (curr_state) {
            case Forward:
                forwardMotion();
                break;
            case Turning:
                turningMotion();
                break;
            case Centering:
                centeringMotion();
                break;
            case Stopped:
                stoppingMotion();
                break;
        }
    }
    int change = 0;
    DIRECTION last_dir_cmd = DIRECTION.Stop;
    public void forwardMotion() throws Exception {
        // if it's it first time set the relative angle

        if(curr_state != State.Turning) {
            first_degree = curr_degree;

            curr_state = State.Turning;
        }
        // ==== Printing purpose =======
        if(curr_degree - first_degree != change) {

            robotGame.print((curr_degree-first_degree) + "\n");
        }
        change = curr_degree - first_degree;

        if(change > ERR_RNG ) { //increasing => left => right to tune
            if( last_dir_cmd != DIRECTION.RIGHT) {
                robotGame.print((curr_degree - first_degree) + "\n");
                robotGame.print("phone: move right\n");
                robotGame.sendCommand(Command.fromString("move{d=2}"));
            }
            last_dir_cmd = DIRECTION.RIGHT;
        }
        else if(change  < -1* ERR_RNG ) { //decreasing => right => left to tune
            if( last_dir_cmd != DIRECTION.LEFT) {
                robotGame.print((curr_degree - first_degree) + "\n");

                robotGame.print("phone: move left\n");
                robotGame.sendCommand(Command.fromString("move{d=1}"));
            }
            last_dir_cmd = DIRECTION.LEFT;
        }
        else {
            if( last_dir_cmd != DIRECTION.FRONT) {
                robotGame.print((curr_degree - first_degree) + "\n");

                robotGame.print("phone:  forward\n");
                robotGame.sendCommand(Command.fromString("move{d=0}"));
            }
            stoppingMotion();
        }
    }

    public void turningMotion() throws Exception {
        // if it's it first time set the relative angle

        if(curr_state != State.Turning) {
            first_degree = curr_degree;

            curr_state = State.Turning;
        }
        // ==== Printing purpose =======
        if(curr_degree - first_degree != change) {

            robotGame.print((curr_degree-first_degree) + "\n");
        }
        change = curr_degree - first_degree;

        if(change > degree_turn + ERR_RNG ) { //increasing => left => right to tune
            if( last_dir_cmd != DIRECTION.RIGHT) {
                robotGame.print((curr_degree - first_degree) + "\n");
                robotGame.print("phone: move right\n");
                robotGame.sendCommand(Command.fromString("move{d=2}"));
            }
            last_dir_cmd = DIRECTION.RIGHT;
        }
        else if(change  < degree_turn   - ERR_RNG ) { //decreasing => right => left to tune
            if( last_dir_cmd != DIRECTION.LEFT) {
                robotGame.print((curr_degree - first_degree) + "\n");

                robotGame.print("phone: move left\n");
                robotGame.sendCommand(Command.fromString("move{d=1}"));
            }
            last_dir_cmd = DIRECTION.LEFT;
        }
        else {
            if( last_dir_cmd != DIRECTION.Stop) {
                robotGame.print((curr_degree - first_degree) + "\n");

                robotGame.print("phone:  stop\n");
                robotGame.sendCommand(Command.fromString("move{d=3}"));
            }
            stoppingMotion();
        }
    }
    public void stoppingMotion() {
        // no need to do anything "just a function in case"
        first_degree = curr_degree;
        curr_state = State.Stopped;
        last_dir_cmd = DIRECTION.Stop;
    }
    public void centeringMotion() {
        // till I know what should I do
        if(curr_state != State.Centering) {
            first_degree = curr_degree;
            curr_state = State.Centering;

        }
    }


    public void onCommand(Command command) throws Exception {
        if(command.cmd.equals("start")) {
            // i will assume that the first argument will always be the new state
            // if it turning => second argument  will be the degree
            if ((Integer)command.arguments[0].value == State.Forward.ordinal()) {
                robotGame.print("<<  Starting Forward \n");
                forwardMotion();
            }
            else if ((Integer)command.arguments[0].value == State.Turning.ordinal()) {
                degree_turn = (Integer)command.arguments[1].value;
                robotGame.print("<<  Turning with  " +degree_turn+" \n");
                turningMotion();
            }
            else if ((Integer)command.arguments[0].value == State.Centering.ordinal()) {
                robotGame.print("<<  Centering \n");
            }
            else {
                robotGame.print("<< Stopping" + command.arguments[0].value  + "\n");
            }
        }
        //command.cmd

        //command.arguments[0].name;
    }
}
