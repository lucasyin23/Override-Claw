#include "main.h"

namespace kp::lift {
    pros::MotorGroup lift_motors({18, -20}, pros::MotorGearset::blue, pros::MotorUnits::degrees);
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

    void set_power(int input) {
        if (input != 0) {
            is_homed = false;
        }

        is_holding = false;

        // prevent powering downward if limit switch is already clicked
        if (is_bottom() && input < 0) {
            lift_motors.move_voltage(0);
            return;
        }

        int voltage = (input * 12000) / 127;
        lift_motors.move_voltage(voltage);
    }

    void brake() {
        // if homing is running, bottomed out, or idle at 0, CUT POWER AND COAST
        if (is_homing_running || is_homed || is_bottom() || lift_motors.get_position() <= 10) {
            lift_motors.move_voltage(0);
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            is_holding = false;
            return;
        }

        // active mid-air hold
        if (!is_holding) {
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            hold_target = lift_motors.get_position(); // Grab wherever driver released
            is_holding = true;
        }
        lift_motors.move_absolute(hold_target, 200);
    }

    void bump_up(double degrees, int speed) {
        is_homed = false;
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        hold_target = lift_motors.get_position() + degrees;
        is_holding = true;
        lift_motors.move_absolute(hold_target, speed);
    }

    static void home_worker() {
        uint32_t start_time = pros::millis();
        const uint32_t timeout_ms = 2500;

        // concurrently run arm homing
        pros::Task arm_task([]() {
            kp::arm::home_to_hardstop(2500);
        });

        // If the lift is ALREADY at the bottom limit switch, doesnt move
        if (is_bottom()) {
            lift_motors.move_voltage(0);
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            lift_motors.tare_position();
            kp::arm::tare_position();
            hold_target = 0.0;
            is_holding = false;
            is_homed = true;
            is_homing_running = false;
            return;
        }

        while (!is_bottom() && (pros::millis() - start_time < timeout_ms)) {
            lift_motors.move_voltage(-7000);
            pros::delay(10);
        }

        lift_motors.move_voltage(0);
        pros::delay(80);

        lift_motors.move_voltage(0);
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        lift_motors.tare_position();
        kp::arm::tare_position();

        hold_target = 0.0;
        is_holding = false;
        is_homed = true;

        while (arm_task.get_state() != pros::E_TASK_STATE_DELETED &&
               (pros::millis() - start_time < (timeout_ms + 1500))) {
            pros::delay(10);
        }

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