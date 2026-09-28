#include "main.h"

namespace kp::arm {
    pros::Motor arm_motor(10); 

    static double hold_target = 0.0;
    static bool is_holding = false;
    static bool is_moving_down = false;
    static bool is_homed = false; // Flag to keep motor off after homing

    // stall timer for hardstop release
    static uint32_t stall_timer = 0;
    static bool stall_timer_active = false;

    void initialize() {
        arm_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        arm_motor.move_voltage(0);
        arm_motor.tare_position();
        hold_target = 0.0;
        is_holding = false;
        is_homed = true;
        stall_timer_active = false;
    }

    void tare_position() {
        arm_motor.tare_position();
        hold_target = 0.0;
    }

    bool is_stalled() {
        return (arm_motor.get_torque() > 0.65) || 
               (std::abs(arm_motor.get_actual_velocity()) < 2.0 && arm_motor.get_current_draw() > 700);
    }

    void set_power(int input) {
        if (input != 0) {
            is_homed = false; 
        }

        is_holding = false;
        is_moving_down = (input < 0);

        if (!is_moving_down) {
            stall_timer_active = false;
        }

        int voltage = (input * 12000) / 127;
        arm_motor.move_voltage(voltage);
    }

    void brake() {
        // if arm is homed/idle at the bottom, enforce complete power cut
        if (is_homed || arm_motor.get_position() <= 5) {
            arm_motor.move_voltage(0);
            arm_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            is_holding = false;
            is_moving_down = false;
            stall_timer_active = false;
            return;
        }

        // active mid-air hold
        if (!is_holding) {
            arm_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            hold_target = arm_motor.get_position();
            is_holding = true;
        }
        is_moving_down = false;
        stall_timer_active = false;

        arm_motor.move_absolute(hold_target, 110);
    }

    void move_to_target(double target_degrees, int max_speed) {
        is_homed = false; // Clear flag for macro
        is_holding = true;
        is_moving_down = false;
        stall_timer_active = false;
        hold_target = target_degrees;

        arm_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        arm_motor.move_absolute(target_degrees, max_speed);
    }

    void update() {
        if (is_moving_down && is_stalled()) {
            if (!stall_timer_active) {
                stall_timer = pros::millis();
                stall_timer_active = true;
            } else if (pros::millis() - stall_timer >= 500) {
                arm_motor.move_voltage(0);
                arm_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
                arm_motor.tare_position();
                hold_target = 0.0;
                is_holding = false;
                is_homed = true;
                is_moving_down = false;
                stall_timer_active = false;
            }
        } else if (!is_moving_down) {
            stall_timer_active = false;
        }
    }

    void home_to_hardstop(int timeout_ms) {
        uint32_t start_time = pros::millis();
        is_holding = false;
        is_homed = false;
        stall_timer_active = false;

        // 1. initial downward drive
        arm_motor.move_voltage(-6000);
        pros::delay(150); // let motor spin up to avoid false-positive stall check

        // 2. drive down until stall is detected or timeout expires
        while (pros::millis() - start_time < (uint32_t)timeout_ms) {
            if (is_stalled()) {
                break;
            }
            arm_motor.move_voltage(-6000);
            pros::delay(10);
        }

        // 3. brief pause to ensure the arm has fully bottomed out
        pros::delay(100);

        // 4. cut power and tare encoder at the mechanical bottom
        arm_motor.move_voltage(0);
        arm_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        arm_motor.tare_position();

        hold_target = 0.0;
        is_holding = false;
        is_moving_down = false;
        stall_timer_active = false;
        is_homed = true; 
    }
}