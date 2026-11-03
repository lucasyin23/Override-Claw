#pragma once

#include <cstdint>
#include <vector>

// Recorded autonomous paths: the step list, SD save/load, replay and a C++ printout.
// Ported from the 37S path recorder, without the command framework: play() simply
// runs the steps in order, each drive blocking until it finishes.
//
// Pressing POINT captures the pose and the mechanisms, in the order a replay needs:
//   LIFT deg         (if the lift moved)    set before the drive, travels during it
//   MOVE/POSE/TURN   (if the robot moved)   the drive itself
//   WAITLIFT ms      (if the lift moved and the claw changed, or nothing drove)
//   TOGGLE 1|0       (if the toggler changed)
//   CLAW 1|0         (if the claw changed)  1 = close, 0 = open
//   BUMP             (if R1 bumped the lift) after the claw has closed
//
// The drive kind comes from how the robot moved since the last point:
//   TURN  turned in place                            -> turnToHeading
//   MOVE  drove, ended facing along the travel line  -> moveToPoint
//   POSE  drove AND ended facing somewhere else      -> moveToPose
// Backing up is recorded too: MOVE and POSE carry a forwards flag.
namespace kp::pathrec {
    constexpr float POS_EPS = 2.0f;   // in: less travel than this = "didn't move"
    constexpr float THETA_EPS = 8.0f; // deg: less rotation than this = "didn't turn"
    constexpr float LIFT_EPS = 15.0f; // encoder deg: less = "lift didn't move"

    // Replay speed caps (0-127). Slower is more repeatable and gives the lift time.
    constexpr int DRIVE_MAX_SPEED = 80;
    constexpr int TURN_MAX_SPEED = 70;

    // Timeouts (ms). Generous on purpose: a move that arrives finishes early.
    constexpr int MS_PER_INCH = 50;
    constexpr int MS_PER_DEG = 16;
    constexpr int DRIVE_MS_MIN = 1000;
    constexpr int DRIVE_MS_MAX = 6000;
    constexpr int TURN_MS_MIN = 600;
    constexpr int TURN_MS_MAX = 3500;
    constexpr int WAITLIFT_MS = 1500;
    constexpr int CLAW_SETTLE_MS = 200; // piston closing before anything else moves

    constexpr const char* PLAY_PATH = "/usd/path_play.txt"; // what replay loads
    constexpr const char* INDEX_PATH = "/usd/path_idx.txt";
    constexpr const char* ARCHIVE_FMT = "/usd/path_%02d.txt";

    enum class Kind : uint8_t {
        START,    // x, y, theta        -> setPose
        MOVE,     // x, y, t, a = fwd   -> moveToPoint
        POSE,     // x, y, theta, t, a  -> moveToPose
        TURN,     // theta, t           -> turnToHeading
        LIFT,     // deg                -> lift::go_to (travels meanwhile)
        WAITLIFT, // t                  -> lift::wait_until_there
        CLAW,     // a = 1 close, 0 open
        BUMP,     //                    -> lift::bump_up
        TOGGLE,   // a = 1 on, 0 off
        WAIT,     // t                  -> delay
    };

    struct Item {
        Kind kind = Kind::WAIT;
        float x = 0, y = 0, theta = 0; // in, in, deg (LemLib frame: 0 = +Y, CW +)
        float deg = 0;                 // LIFT: encoder degrees
        int t = 0;                     // timeout or duration, ms
        int a = 0;                     // MOVE/POSE: 1 forwards; CLAW/TOGGLE: 1 on
    };

    using Routine = std::vector<Item>;

    bool is_drive(Kind k);
    const char* tag_of(Kind k);

    // The drive step for going from prev to (x, y, theta), timeout filled in.
    // kind == WAIT means the robot didn't really move.
    Item classify_node(const Item& prev, float x, float y, float theta);

    bool save(const Routine& r, const char* path, const char* name);
    int save_autonumbered(const Routine& r, const char* name); // archive number, -1 on failure
    void remember(const Routine& r); // keeps it in memory for a robot with no SD card
    bool load(Routine& out, const char* path);
    bool load_default(Routine& out); // PLAY_PATH, newest archive, then memory

    void play(const Routine& r);
    void dump_cpp(const Routine& r, const char* fn_name = "recorded_path");

    // Auton entry: load the saved path and run it. Nothing saved = does nothing.
    void play_recorded();
}
