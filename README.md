# WALL-E — Multi-Game Robot

An Arduino-controlled robot built for a university Microprocessors course, designed to play 5 different physical games using a shared chassis, motor system, and Android companion app for control and computer vision.

## The 5 games

- **Ball Collection** — drives around and collects balls using a roller mechanism (dedicated DC motor).
- **Soccer** — a soccer-playing mode using the roller and a dedicated soccer DC motor.
- **Defending** — a goalkeeping/defending mode (`Defending/Defending.ino`).
- **Moving Gun** — a turret-style gun with independent left/right servo control.
- **Maze Solver** — autonomous maze navigation using IR sensors (`Maze Solver/maze_solver`).

An additional **Object Detection** module and a **RobotVision-Android** app add camera-based object detection, and a laser sensor is wired in for range/obstacle sensing.

## Hardware & control

- Chassis driven by independent left/right DC motor H-bridges, plus dedicated motors for the roller and soccer mechanisms (see `Pin Diagram.md` for the full pin mapping).
- 6 servo motors: soccer, left/right mobile arms, left/right gun, and a rotating roof/turret.
- IR sensors for maze-following and a laser module for detection.
- All motor/servo/sensor control logic lives in Arduino sketches (`Arduino/`, plus a per-game `.ino` in each game's folder).

## Android control app

`EmbeddedCommunication/` is a native Android app (Java) that talks to the Arduino over serial communication, sending game-mode and movement commands from a phone. `RobotVision-Android/` handles the camera-based object detection side.

## Tech

Arduino/C++ (motor & sensor control), Android/Java (companion app, serial communication, vision).
