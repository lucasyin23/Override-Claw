#include "main.h"

namespace kp::intake {
    // 11W blue motor on Port 12 (600 RPM)
    pros::Motor intake_motor(12, pros::MotorGearset::blue);

    const int INTAKE_SPEED = 127;

    // 0: stopped, 1: intaking, -1: outtaking
    static int current_state = 0;

    void initialize() {
        intake_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        intake_motor.move(0);
        current_state = 0;
    }

    // Stops both lower intake and armtake
    void stop() {
        current_state = 0;
        intake_motor.move(0);
        kp::armtake::stop();
    }

    // Stops ONLY Motor 12 (used by R1/R2 macros)
    void stop_intake_only() {
        current_state = 0;
        intake_motor.move(0);
    }

    void tintake() {
        if (current_state == 1) {
            stop(); // Pressing again shuts both off
        } else {
            current_state = 1;
            intake_motor.move(INTAKE_SPEED); // 100% max forward speed
            kp::armtake::intake();
        }
    }

    void touttake() {
        if (current_state == -1) {
            stop(); // Pressing again shuts both off
        } else {
            current_state = -1;
            intake_motor.move(-INTAKE_SPEED); // 100% max reverse speed
            kp::armtake::outtake();
        }
    }

    bool is_running() {
        return current_state != 0;
    }
}