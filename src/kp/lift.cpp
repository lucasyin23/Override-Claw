#include "main.h"

namespace kp::lift {
    // Lift motor group: motors 9, 10 (forward) and motor 2 (reversed), blue 600 RPM gearset
    pros::MotorGroup lift_motors({9, 10, -2}, pros::MotorGearset::blue, pros::MotorUnits::degrees);

    // Limit switch on 3-Wire ADI port 'A'
    pros::adi::DigitalIn limit_switch('A');

    static double hold_target = 0.0;
    static bool is_holding = false;
    static bool is_homed = true;
    static bool is_homing_running = false;
    static bool is_bumping = false;

    void initialize() {
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        lift_motors.set_current_limit(2250);
        lift_motors.move_voltage(0);
        lift_motors.tare_position();
        hold_target = 0.0;
        is_holding = false;
        is_homed = true;
        is_homing_running = false;
        is_bumping = false;
    }

    bool is_bottom() {
        return limit_switch.get_value() == 1;
    }

    void tare_position() {
        lift_motors.tare_position();
        hold_target = 0.0;
    }

    double get_position() {
        return lift_motors.get_position();
    }

    void set_power(int input) {
        if (is_homing_running || is_bumping) {
            return;
        }

        if (input != 0) {
            is_homed = false; // Driver input clears homed state lock
        }

        is_holding = false;

        // Hard stop: prevent driving down if already pressing bottom limit switch
        if (is_bottom() && input < 0) {
            lift_motors.move_voltage(0);
            return;
        }

        // Scale button value: up capped at 10500 mV for faster movement, down matches homing speed (-3500 mV)
        int voltage = 0;
        if (input > 0) {
            voltage = (input * 12000) / 127;
        } else if (input < 0) {
            voltage = (input * 4000) / 127;
        }

        lift_motors.move_voltage(voltage);
    }

    void brake() {
        if (is_homing_running || is_bumping) {
            return;
        }

        // Coast and zero voltage if homing in progress, homed, at limit switch, or at floor
        if (is_homed || is_bottom() || lift_motors.get_position() <= 10.0) {
            lift_motors.move_voltage(0);
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            is_holding = false;
            return;
        }

        // Active position hold when idling mid-air
        if (!is_holding) {
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            hold_target = lift_motors.get_position();
            is_holding = true;
        }

        lift_motors.move_absolute(hold_target, 200);
    }

    // Bump the lift up by 120 degrees when claw toggles
    void bump_up() {
        if (!is_homing_running && !is_bumping) {
            is_bumping = true;
            is_holding = false;
            is_homed = false;
            
            lift_motors.move_relative(120.0, 200);
            
            // Background task to hold manual overrides for 400ms while the bump finishes
            pros::Task([] {
                pros::delay(400);
                is_bumping = false;
                return 0;
            });
        }
    }

    static void home_worker() {
        uint32_t start_time = pros::millis();
        const uint32_t timeout_ms = 3500;

        if (is_bottom()) {
            lift_motors.move_voltage(0);
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            tare_position();
            is_holding = false;
            is_homed = true;
            is_homing_running = false;
            return;
        }

        while (!is_bottom() && (pros::millis() - start_time < timeout_ms)) {
            lift_motors.move_voltage(-3500);
            pros::delay(10);
        }

        lift_motors.move_voltage(0);
        pros::delay(80);

        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        tare_position();

        hold_target = 0.0;
        is_holding = false;
        is_homed = true;
        is_homing_running = false;
    }

    void home() {
        if (!is_homing_running && !is_bumping) {
            is_homing_running = true;
            is_holding = false;
            is_homed = false;
            hold_target = 0.0;
            lift_motors.move_voltage(0);

            pros::Task background_home(home_worker);
        }
    }

    bool is_homing() {
        return is_homing_running;
    }
}