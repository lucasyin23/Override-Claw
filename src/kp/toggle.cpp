#include "main.h"

namespace kp::toggle {
    pros::Motor toggle_motor(-1, pros::MotorGearset::blue);
    static bool is_spinning = false;

    static constexpr int SPIN_POWER = 70; // out of 127

    void initialize() {
        toggle_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        set(false);
    }

    void set(bool on) {
        is_spinning = on;
        toggle_motor.move(on ? SPIN_POWER : 0);
    }

    void toggle() {
        set(!is_spinning);
    }

    bool is_on() {
        return is_spinning;
    }
}
