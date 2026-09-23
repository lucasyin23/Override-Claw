#include "main.h"

namespace kp::armtake {
    // Port 9 Blue cartridge (adjust to -9 if intake spins the wrong way)
    pros::Motor armtake_motor(9, pros::MotorGearset::blue, pros::MotorUnits::degrees);

    // 0: Off, 1: Intaking, -1: Outtaking
    static int current_state = 0;

    void initialize() {
        armtake_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        current_state = 0;
    }

    void set_power(int input) {
        int voltage = (input * 12000) / 127;
        armtake_motor.move_voltage(voltage);
    }

    void intake() {
        armtake_motor.move_voltage(12000); // 100% max speed
        current_state = 1;
    }

    void outtake() {
        armtake_motor.move_voltage(-12000); // 100% max speed
        current_state = -1;
    }

    void stop() {
        armtake_motor.move_voltage(0);
        current_state = 0;
    }

    int get_state() {
        return current_state;
    }

    void toggle_intake() {
        if (current_state == 1) {
            stop();
        } else {
            intake();
        }
    }

    void toggle_outtake() {
        if (current_state == -1) {
            stop();
        } else {
            outtake();
        }
    }
}