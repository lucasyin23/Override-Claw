#include "main.h"

#include <cmath>
#include <cstdio>
#include <vector>

// NOTE: on a pros::MotorGroup, tare_position(), set_brake_mode() and
// set_current_limit() only touch the FIRST motor unless you use the _all version.
namespace kp::lift {
    // Lift motor group: motors 9, 10 (forward) and motor 2 (reversed), blue 600 RPM gearset
    pros::MotorGroup lift_motors({9, 10, -2}, pros::MotorGearset::blue, pros::MotorUnits::degrees);

    // Limit switch on 3-Wire ADI port 'A'
    pros::adi::DigitalIn limit_switch('A');

    static constexpr int DRIVE_CURRENT_MA = 2500; // V5 default
    static constexpr int HOLD_CURRENT_MA = 2250;  // only while holding

    static constexpr int UP_MV = 12000;
    static constexpr int DOWN_MV = 4000;   // the switch only clicks reliably below full speed
    static constexpr int HOME_MV = 3500;
    static constexpr uint32_t HOME_TIMEOUT_MS = 3500;

    static constexpr int AUTO_RPM = 200;   // go_to and bump_up
    static constexpr double ARRIVED_DEG = 10.0;
    static constexpr double FLOOR_DEG = 10.0;

    // Commanded to move but not moving for this long = hard stop or jam. Stop pushing.
    // Off for now: the thresholds below are untested guesses.
    static constexpr bool STALL_PROTECTION = false;
    static constexpr double STALL_RPM = 20.0;
    static constexpr uint32_t STALL_MS = 300;

    static bool is_holding = false;
    static bool is_coasting = false;
    static bool is_homed = true;
    static bool is_homing_running = false;
    static bool is_bumping = false;

    static uint32_t stall_since = 0;
    static int stall_dir = 0;  // direction that is stalled; ignored until the button is let go
    static int last_dir = 0;

    static double go_to_target = 0.0;
    static int bumps_done = 0;

    static void stop_holding() {
        if (is_holding) {
            lift_motors.set_current_limit_all(DRIVE_CURRENT_MA);
            is_holding = false;
        }
        is_coasting = false;
    }

    static void coast() {
        if (is_coasting) {
            return;
        }
        stop_holding();
        lift_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
        lift_motors.move_voltage(0);
        is_coasting = true;
    }

    // Each motor holds its own position, so they can't fight over one shared target.
    static void hold() {
        if (is_holding) {
            return;
        }
        lift_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
        lift_motors.set_current_limit_all(HOLD_CURRENT_MA);
        lift_motors.brake();
        is_holding = true;
        is_coasting = false;
    }

    // At the switch: cut power and re-zero all three encoders, once per arrival.
    static void settle_at_bottom() {
        coast();
        if (!is_homed) {
            lift_motors.tare_position_all();
            is_homed = true;
        }
    }

    static void rest() {
        if (is_homed) {
            return;
        }
        if (is_bottom()) {
            settle_at_bottom();
        } else if (get_position() <= FLOOR_DEG) {
            coast();
        } else {
            hold();
        }
    }

    void initialize() {
        is_holding = false;
        is_coasting = false;
        is_homing_running = false;
        is_bumping = false;
        stall_since = 0;
        stall_dir = 0;
        last_dir = 0;
        lift_motors.set_current_limit_all(DRIVE_CURRENT_MA);
        coast();
        lift_motors.tare_position_all(); // the lift starts every match down
        is_homed = true;
    }

    bool is_bottom() {
        return limit_switch.get_value() == 1;
    }

    void tare_position() {
        lift_motors.tare_position_all();
    }

    double get_position() {
        return lift_motors.get_position();
    }

    void set_power(int input) {
        if (is_homing_running || is_bumping) {
            return;
        }

        if (input == 0) {
            brake();
            return;
        }

        // Hard stop: prevent driving down if already pressing bottom limit switch
        if (is_bottom() && input < 0) {
            stall_since = 0;
            settle_at_bottom();
            return;
        }

        const int dir = input > 0 ? 1 : -1;
        if (dir != last_dir) {
            stall_since = 0;
            last_dir = dir;
        }

        // Still pushing into the stop we already hit: keep resting until the button changes.
        if (stall_dir == dir) {
            rest();
            return;
        }
        stall_dir = 0;

        is_homed = false; // Driver input clears homed state lock
        stop_holding();

        if (STALL_PROTECTION && std::fabs(lift_motors.get_actual_velocity()) < STALL_RPM) {
            const uint32_t now = pros::millis();
            if (stall_since == 0) {
                stall_since = now;
            } else if (now - stall_since >= STALL_MS) {
                stall_since = 0;
                stall_dir = dir;
                printf("[lift] stalled going %s at %.0f deg\n", dir > 0 ? "up" : "down", get_position());
                rest();
                return;
            }
        } else {
            stall_since = 0;
        }

        lift_motors.move_voltage(dir > 0 ? (input * UP_MV) / 127 : (input * DOWN_MV) / 127);
    }

    void brake() {
        if (is_homing_running || is_bumping) {
            return;
        }
        stall_since = 0;
        stall_dir = 0;
        last_dir = 0;
        rest();
    }

    // Bump the lift up when the claw grabs
    void bump_up() {
        if (!is_homing_running && !is_bumping) {
            is_bumping = true;
            is_homed = false;
            stop_holding();
            bumps_done++;

            lift_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
            lift_motors.move_relative(BUMP_DEG, AUTO_RPM);

            // Background task to hold manual overrides for 400ms while the bump finishes
            pros::Task([] {
                pros::delay(400);
                is_bumping = false;
            });
        }
    }

    int bump_count() {
        return bumps_done;
    }

    bool bump_active() {
        return is_bumping;
    }

    // Each motor moves relative to its own encoder, so small differences can't make them fight.
    void go_to(double degrees) {
        if (is_homing_running) {
            return;
        }
        is_homed = false;
        stall_dir = 0;
        stop_holding();
        go_to_target = degrees;
        lift_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
        lift_motors.move_relative(degrees - get_position(), AUTO_RPM);
    }

    void wait_until_there(int timeout_ms) {
        const uint32_t start = pros::millis();
        while (std::fabs(get_position() - go_to_target) > ARRIVED_DEG &&
               pros::millis() - start < (uint32_t)timeout_ms) {
            pros::delay(10);
        }
    }

    static void home_worker() {
        const uint32_t start_time = pros::millis();

        if (is_bottom()) {
            settle_at_bottom();
            is_homing_running = false;
            return;
        }

        while (!is_bottom() && (pros::millis() - start_time < HOME_TIMEOUT_MS)) {
            lift_motors.move_voltage(-HOME_MV);
            pros::delay(10);
        }

        const bool reached = is_bottom();
        coast();
        pros::delay(80);

        // Only a real switch click is a zero. On a timeout the old zero stays, the lift
        // keeps working, and driving it down onto the switch re-zeroes it.
        if (reached) {
            lift_motors.tare_position_all();
            is_homed = true;
        }
        is_homing_running = false;

        if (!reached) {
            printf("[lift] homing timed out without hitting the switch\n");
            kp::controller::get_master().print(1, 0, "HOME FAILED");
            pros::delay(60);
            CTRL_VIBRATE("---")
        }
    }

    void home() {
        if (!is_homing_running && !is_bumping) {
            is_homing_running = true;
            is_homed = false;
            stall_since = 0;
            stall_dir = 0;
            stop_holding();
            lift_motors.move_voltage(0);

            pros::Task background_home(home_worker);
        }
    }

    bool is_homing() {
        return is_homing_running;
    }

    void log_status() {
        const std::vector<double> pos = lift_motors.get_position_all();
        const std::vector<std::int32_t> ma = lift_motors.get_current_draw_all();
        const std::vector<double> temp = lift_motors.get_temperature_all();
        if (pos.size() < 3 || ma.size() < 3 || temp.size() < 3) {
            return;
        }
        printf("[lift] deg %.0f %.0f %.0f | mA %d %d %d | C %.0f %.0f %.0f | hold %d\n",
               pos[0], pos[1], pos[2], (int)ma[0], (int)ma[1], (int)ma[2],
               temp[0], temp[1], temp[2], is_holding ? 1 : 0);
    }
}
