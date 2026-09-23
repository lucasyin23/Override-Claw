#include "main.h"

namespace kp::arm {
    // Port 10 arm motor
    pros::Motor arm_motor(10, pros::MotorGearset::green, pros::MotorUnits::degrees);

    static double hold_target = 0.0;
    static bool is_holding = false;

    void initialize() {
        arm_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        arm_motor.tare_position();
        hold_target = 0.0;
        is_holding = true;
    }

    void set_power(int input) {
        is_holding = false;
        int voltage = (input * 12000) / 127;
        arm_motor.move_voltage(voltage);
    }

    void brake() {
        if (!is_holding) {
            hold_target = arm_motor.get_position();
            is_holding = true;
        }
        arm_motor.move_absolute(hold_target, 60);
    }
}