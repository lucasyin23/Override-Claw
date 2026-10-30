#include "main.h"

namespace kp::toggle {
    pros::Motor toggle_motor(-1, pros::MotorGearset::blue);
    static bool is_spinning = false;

    void initialize() {
        toggle_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        toggle_motor.move(0);
        is_spinning = false;
    }

    void toggle() {
        is_spinning = !is_spinning;
        if (is_spinning) {
            toggle_motor.move(60); // Full forward speed
        } else {
            toggle_motor.move(0);
        }
    }
}