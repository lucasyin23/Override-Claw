#include "main.h"
#include "pros/misc.h"

std::string alliance = "N/A";

void initialize() {
    pros::lcd::initialize();
    kp::chassis::initialize();
    // First entry = the default auton. With nothing recorded it does nothing.
    kp::automation::set("recorded", kp::pathrec::play_recorded);
    kp::automation::set("drive", automation::drive);
    kp::controller::initialize();
    kp::lift::initialize();
    kp::claw::initialize();
    kp::toggle::initialize();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
    kp::automation::run();
}

void opcontrol() {
    // The team presses A before plugging in. This covers forgetting: on field control
    // the setup screen would leave the lift and claw dead.
    if (pros::competition::is_connected()) {
        kp::controller::ready();
    }

    // Pre-ready loop
    while (kp::controller::is_ready() == false) {
        if (kp::controller::lock() == false) {
            kp::controller::delay();
            continue;
        }

        LCD_MAIN_CLEAR
        LCD_1("# %s # %s", alliance, kp::automation::title())
        LCD_2("A:READY / X:RESET / Y:RECORD / B:PLAY / L1:PRINT")
        LCD_3("R1:ATM NEXT / R2:ATM RUN / DN:ATM TESTER")
        LCD_4("LF:PID ANGLE / RT:PID LINEAR / UP:VISION")

        kp::chassis::refresh_xy();

        if (CTRL_PRESS_ONCE(UP)) {
            kp::vision::launch_vision_tuner();
        }

        if (CTRL_PRESS_ONCE(LEFT)) {
            kp::chassis::launch_angular_adjustment();
        }

        if (CTRL_PRESS_ONCE(RIGHT)) {
            kp::chassis::launch_linear_adjustment();
        }

        if (CTRL_PRESS_ONCE(DOWN)) {
            kp::automation::launch_selection();
        }

        if (CTRL_PRESS_ONCE(R1)) {
            kp::automation::next();
        }

        if (CTRL_PRESS_ONCE(R2)) {
            if (kp::chassis::is_pose_origin()) {
                kp::automation::run();
            }
        }

        if (CTRL_PRESS_ONCE(X)) {
            kp::chassis::reset();
            CTRL_VIBRATE(".")
        }

        if (CTRL_PRESS_ONCE(Y)) {
            kp::path_recorder::run();
        }

        if (CTRL_PRESS_ONCE(B)) {
            kp::pathrec::play_recorded();
        }

        if (CTRL_PRESS_ONCE(L1)) {
            kp::path_recorder::dump_saved();
        }

        if (CTRL_PRESS_ONCE(A)) {
            kp::controller::ready();
            kp::controller::short_delay();
            CTRL_VIBRATE("-")
        }

        #ifdef CONTROLLER_MOVEMENT
        kp::chassis::track_dual_joystick(1.6, 1);
        #endif

        kp::controller::release();
        kp::controller::short_delay();
    }

    LCD_MAIN_CLEAR
    LCD_1("# %s # %s", alliance, kp::automation::title())

    while (kp::controller::is_ready() == true) {
        kp::teleop::tick();
        kp::controller::delay();
    }
}
