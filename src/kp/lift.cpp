#include "main.h"

namespace kp::lift {
    // Lift motors (Blue 600 RPM gearsets)
    pros::MotorGroup lift_motors({18, -20}, pros::MotorGearset::blue, pros::MotorUnits::degrees);

    // Limit switch on 3-Wire Port H
    pros::adi::DigitalIn limit_switch('H');

    static double hold_target = 0.0;
    static bool is_holding = false;

    void initialize() {
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        lift_motors.tare_position();
        hold_target = 0.0;
        is_holding = true;
    }

    bool is_bottom() {
        // limit_switch.get_value() returns 1 (true) when pressed
        return limit_switch.get_value() == 1;
    }

    void set_power(int input) {
        is_holding = false;

        // If the switch is pressed, prevent the driver from forcing the lift further down
        if (is_bottom() && input < 0) {
            lift_motors.move_voltage(0);
            return;
        }

        int voltage = (input * 12000) / 127;
        lift_motors.move_voltage(voltage);
    }

    void brake() {
        // CASE 1: At the bottom limit switch
        if (is_bottom()) {
            // Cut all motor voltage completely so it runs cold
            lift_motors.move_voltage(0);
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

            // Re-calibrate encoder zero while resting at the bottom
            lift_motors.tare_position();
            hold_target = 0.0;
            is_holding = false;
            return;
        }

        // CASE 2: Lift is suspended off the floor
        if (!is_holding) {
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            hold_target = lift_motors.get_position();
            is_holding = true;
        }

        // Actively fight gravity to maintain mid/high height
        lift_motors.move_absolute(hold_target, 200);
    }
}