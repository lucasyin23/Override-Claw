#pragma once

// The auton path creator: drive the route by hand and press A at each point.
// Open it with Y on the setup screen (before pressing A to go ready). It runs until
// you save or discard; driving and every mechanism stay live on their normal buttons.
//
//   sticks, L1 L2, R1 R2, B, RIGHT   exactly as in driver control
//   A           POINT: record where the robot is, plus any lift, claw or toggler change
//   X           UNDO the last point
//   Y           SAVE and exit: /usd/path_play.txt, a numbered copy, and a C++ printout
//   hold DOWN + Y   DISCARD and exit
//
// Controller screen: "REC pt3 i7" = 3 points, 7 steps; the middle line says what the
// last press did; the bottom line is the pose.
//
// The path starts from wherever odometry says the robot is when the recorder opens,
// and the replay sets the pose back to that START. Replay from the same physical spot.
namespace kp::path_recorder {
    void run();         // blocks until save or discard
    void dump_saved();  // print the saved path as C++ to `pros terminal`
}
