#include "main.h"

namespace kp::lift {
    // Lift motors (Ports 18, -20)
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
        return limit_switch.get_value() == 1;
    }

    void set_power(int input) {
        is_holding = false;
        if (is_bottom() && input < 0) {
            lift_motors.move_voltage(0);
            return;
        }
        int voltage = (input * 12000) / 127;
        lift_motors.move_voltage(voltage);
    }

    void brake() {
        if (is_bottom()) {
            lift_motors.move_voltage(0);
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            lift_motors.tare_position();
            hold_target = 0.0;
            is_holding = false;
            return;
        }

        if (!is_holding) {
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            hold_target = lift_motors.get_position();
            is_holding = true;
        }
        lift_motors.move_absolute(hold_target, 200);
    }

    void home(int timeout_ms) {
        uint32_t start_time = pros::millis();

        // ---------------------------------------------------------
        // STEP 1: Cut armtake power so rollers don't drag on tiles
        // ---------------------------------------------------------
        kp::armtake::stop();

        // ---------------------------------------------------------
        // STEP 2: Bring the arm down to its mechanical rest/stop
        // ---------------------------------------------------------
        kp::arm::set_power(-70);
        pros::delay(350);
        kp::arm::brake();

        // ---------------------------------------------------------
        // STEP 3: Bring the lift down until Port H limit switch triggers
        // ---------------------------------------------------------
        while (!is_bottom() && (pros::millis() - start_time < timeout_ms)) {
            lift_motors.move_voltage(-7000); // ~60% power down
            pros::delay(10);
        }

        // ---------------------------------------------------------
        // STEP 4: Cut lift power, tare encoders, and coast
        // ---------------------------------------------------------
        lift_motors.move_voltage(0);
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        lift_motors.tare_position();
        hold_target = 0.0;
        is_holding = false;
    }
}