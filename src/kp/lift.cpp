#include "main.h"
#include <vector>

namespace kp::lift {
    // Lift motor group: motors 9, 10 (forward) and motor 2 (reversed), green 200 RPM gearset
    pros::MotorGroup lift_motors({9, 10, -2}, pros::MotorGearset::green, pros::MotorUnits::degrees);

    // Limit switch on 3-Wire ADI port 'A'
    pros::adi::DigitalIn limit_switch('A');

    static double hold_targets[3] = {0.0, 0.0, 0.0};
    static bool is_holding = false;
    static bool is_homed = true;
    static bool is_homing_running = false;
    static bool is_bumping = false;

    void initialize() {
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        lift_motors.set_current_limit(2000);
        lift_motors.move_voltage(0);
        lift_motors.tare_position();
        for(int i = 0; i < 3; i++) hold_targets[i] = 0.0;
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
        for(int i = 0; i < 3; i++) hold_targets[i] = 0.0;
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

        // Reset brake mode back to coast when driving so motors don't fight
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

        // Hard stop: prevent driving down if already pressing bottom limit switch
        if (is_bottom() && input < 0) {
            lift_motors.move_voltage(0);
            return;
        }

        // Scale input: green gearset has high torque, so full 12000 mV is safe and smooth
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

        // Active individual motor position hold when idling mid-air
        if (!is_holding) {
            std::vector<double> current_pos = lift_motors.get_positions();
            for (size_t i = 0; i < current_pos.size() && i < 3; i++) {
                hold_targets[i] = current_pos[i];
            }
            is_holding = true;
        }

        // Apply hold with green gearset max velocity (200 RPM)
        lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        for (size_t i = 0; i < lift_motors.size(); i++) {
            lift_motors[i].move_absolute(hold_targets[i], 200);
        }
    }

    // Bump the lift up by 120 degrees when claw toggles
    void bump_up() {
        if (!is_homing_running && !is_bumping) {
            is_bumping = true;
            is_holding = false;
            is_homed = false;
            
            lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            lift_motors.move_relative(120.0, 200);
            
            // Background task to hold manual overrides while the bump finishes
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

        for(int i = 0; i < 3; i++) hold_targets[i] = 0.0;
        is_holding = false;
        is_homed = true;
        is_homing_running = false;
    }

    void home() {
        if (!is_homing_running && !is_bumping) {
            is_homing_running = true;
            is_holding = false;
            is_homed = false;
            for(int i = 0; i < 3; i++) hold_targets[i] = 0.0;
            lift_motors.move_voltage(0);

            pros::Task background_home(home_worker);
        }
    }

    bool is_homing() {
        return is_homing_running;
    }
}