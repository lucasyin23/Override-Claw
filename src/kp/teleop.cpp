#include "main.h"

// Uncomment to print lift position, current and temperature to `pros terminal` twice a second.
// #define LIFT_TELEMETRY

namespace kp::teleop {
    namespace {
        pros::Controller master(pros::E_CONTROLLER_MASTER);
    }

    void tick() {
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
            kp::claw::toggle();
        }

        // Only bump when the toggle just closed the claw on something
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
            kp::claw::toggle();
            if (kp::claw::is_extended()) {
                kp::lift::bump_up();
            }
        }

        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            kp::toggle::toggle();
        }

        // Lift Controls
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            kp::lift::set_power(127);
        }
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            kp::lift::set_power(-127);
        }
        else if (!kp::lift::is_homing()) {
            kp::lift::brake();
        }

        // Homing Reset
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            kp::lift::home(); // Runs in background task, does not freeze controls
        }

        #ifdef CONTROLLER_MOVEMENT
        kp::chassis::track_dual_joystick(1, 1);
        #endif

        #ifdef LIFT_TELEMETRY
        static uint32_t last_log = 0;
        if (pros::millis() - last_log >= 500) {
            last_log = pros::millis();
            kp::lift::log_status();
        }
        #endif
    }
}
