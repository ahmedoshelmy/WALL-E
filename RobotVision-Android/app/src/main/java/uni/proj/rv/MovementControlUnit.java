package uni.proj.rv;

import uni.proj.ec.Command;
// ================ RULES =========
// Right = decreasing
// Left = increasing
public class MovementControlUnit {
    static final int ERR_RNG = 2;
    enum State {
        Forward,    //move forward straight
        Centering,  //! centering it self to take the ball
        Turning,    //rotating with specific angle
        Stopped
    }

    enum  DIRECTION {
        FRONT,
        LEFT,
        RIGHT,
        Stop
    }

    RobotGame robotGame;
    State curr_state;
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
    public void forwardMotion() throws Exception {
        // if it's it first time set the relative angle
        if(curr_state != State.Forward) {
            first_degree = curr_degree;
            curr_state = State.Forward;
        }
        if(change != curr_degree - first_degree) {
            robotGame.print((curr_degree - first_degree) + "\n");
            change = curr_degree - first_degree;
        }
        if(change > ERR_RNG) { //increasing => left => right to tune
            robotGame.print("I'm Android till you to move right\n");

            robotGame.sendCommand(Command.fromString("move{d=2}"));
        }
        else if(change < ERR_RNG ) { //decreasing => right => left to tune
            robotGame.print("I'm Android till you to move left\n");
            robotGame.sendCommand(Command.fromString("move{d=1}"));
        } else {
            robotGame.print("I'm Android till you to move forward\n");

            robotGame.sendCommand(Command.fromString("move{d=0}"));

        }
    }

    public void turningMotion() throws Exception {
        // if it's it first time set the relative angle

        if(curr_state != State.Turning) {
            first_degree = curr_degree;

            curr_state = State.Turning;
        }

        if(change != curr_degree - first_degree) {
            robotGame.print((curr_degree - first_degree) + "\n");
            change = curr_degree - first_degree;
        }

        if(change > degree_turn + ERR_RNG) { //increasing => left => right to tune
            robotGame.sendCommand(Command.fromString("move{d="+DIRECTION.RIGHT.ordinal()+"}"));
        }
        else if(change  < degree_turn   - ERR_RNG) { //decreasing => right => left to tune
            robotGame.sendCommand(Command.fromString("move{d="+DIRECTION.LEFT.ordinal()+"}"));
        } else {
            robotGame.sendCommand(Command.fromString("move{d="+DIRECTION.Stop.ordinal()+"}"));
        }
    }
    public void stoppingMotion() {
        // no need to do anything "just a function in case"
        first_degree = curr_degree;
        curr_state = State.Stopped;

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
