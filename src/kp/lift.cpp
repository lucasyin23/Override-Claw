#include "main.h"

namespace kp::lift {
    // Lift motor group: dual blue 600 RPM motors (port 18 forward, port 20 reversed)
    pros::MotorGroup lift_motors({18, -20}, pros::MotorGearset::blue, pros::MotorUnits::degrees);

    // Limit switch on 3-Wire ADI port 'H'
    pros::adi::DigitalIn limit_switch('H');

    static double hold_target = 0.0;
    static bool is_holding = false;
    static bool is_homed = true;
    static bool is_homing_running = false;

    void initialize() {
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        lift_motors.move_voltage(0);
        lift_motors.tare_position();
        hold_target = 0.0;
        is_holding = false;
        is_homed = true;
        is_homing_running = false;
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
        if (input != 0) {
            is_homed = false; // Driver input clears homed state lock
        }

        is_holding = false;

        // Hard stop: prevent driving down if already pressing bottom limit switch
        if (is_bottom() && input < 0) {
            lift_motors.move_voltage(0);
            return;
        }

        // Scale joystick value (-127 to 127) to mV (-12000 to 12000)
        int voltage = (input * 12000) / 127;
        lift_motors.move_voltage(voltage);
    }

    void brake() {
        // Coast and zero voltage if homing in progress, homed, at limit switch, or at floor
        if (is_homing_running || is_homed || is_bottom() || lift_motors.get_position() <= 10.0) {
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

    static void home_worker() {
        uint32_t start_time = pros::millis();
        const uint32_t timeout_ms = 2500;

        // If already resting on bottom limit switch, zero and exit immediately
        if (is_bottom()) {
            lift_motors.move_voltage(0);
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            tare_position();
            is_holding = false;
            is_homed = true;
            is_homing_running = false;
            return;
        }

        // Drive lift down until limit switch triggers or timeout expires
        while (!is_bottom() && (pros::millis() - start_time < timeout_ms)) {
            lift_motors.move_voltage(-7000);
            pros::delay(10);
        }

        // Settle motors and stop
        lift_motors.move_voltage(0);
        pros::delay(80);

        // Coast and tare encoder
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        tare_position();

        hold_target = 0.0;
        is_holding = false;
        is_homed = true;
        is_homing_running = false;
    }

    void home() {
        if (!is_homing_running) {
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