#include "main.h"

namespace kp::lift {
    // Ports 18 and 19 with Blue 600 RPM gearsets (Port -19 is reversed)
    pros::MotorGroup lift_motors({15, -16}, pros::MotorGearset::blue, pros::MotorUnits::degrees);

    void initialize() {
        // HOLD brake mode locks the motors in place against gravity
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    }

    void set_power(int input) {
        // Direct voltage scaling (-12000 to +12000 mV) for max speed
        int voltage = (input * 12000) / 127;
        lift_motors.move_voltage(voltage);
    }

    void brake() {
        lift_motors.brake();
    }
}