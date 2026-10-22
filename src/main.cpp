#include "main.h"
#include "pros/misc.h"

#define CONTROLLER_MOVEMENT
std::string alliance = "N/A";

void initialize() {
    pros::lcd::initialize();
    kp::chassis::initialize();
    kp::controller::initialize();
    kp::intake::initialize();
    kp::arm::initialize();
    kp::lift::initialize();
    kp::armtake::initialize();

    kp::automation::set("drive", automation::drive);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
    kp::automation::run();
} 

void opcontrol() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);

    // Pre-ready loop
    while (kp::controller::is_ready() == false) {
        if (kp::controller::lock() == false) {
            kp::controller::delay();
            continue;
        }

        LCD_MAIN_CLEAR
        LCD_1("# %s # %s", alliance, kp::automation::title())
        LCD_2("A:READY / B:RESET")
        LCD_3("R1:ATM NEXT / R2:ATM PREPARE / DN:ATM TESTER")
        LCD_4("LF:PID ANGLE / RT:PID LINEAR")

        kp::chassis::refresh_xy();

        if (CTRL_PRESS_ONCE(UP)) {
            kp::vision::launch_vision_tuner();
        }

        if (CTRL_PRESS_ONCE(LEFT)) {
            kp::chassis::launch_angular_adjustment();
        }

        if (CTRL_PRESS_ONCE(RIGHT)) {
            kp::chassis::launch_linear_adjustment();
        }

        if (CTRL_PRESS_ONCE(DOWN)) {
            kp::automation::launch_selection();
        }

        if (CTRL_PRESS_ONCE(R1)) {
            kp::automation::next();
        }

        if (CTRL_PRESS_ONCE(R2)) {
            if (kp::chassis::is_pose_origin()) {
                kp::automation::run();
            }
        }

        if (CTRL_PRESS_ONCE(X)) {
            kp::chassis::reset();
            CTRL_VIBRATE(".")
        }

        if (CTRL_PRESS_ONCE(A)) {
            kp::controller::ready();
            kp::controller::short_delay();
            CTRL_VIBRATE("-")
        }

        #ifdef CONTROLLER_MOVEMENT
        kp::chassis::track_dual_joystick(1.6, 1);
        #endif

        kp::controller::release();
        kp::controller::short_delay();
    }

    LCD_MAIN_CLEAR
    LCD_1("# %s # %s", alliance, kp::automation::title())

    while (kp::controller::is_ready() == true) {
        // Lift Controls
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            kp::macro::stop();
            kp::lift::set_power(127);
        } 
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            kp::macro::stop();
            kp::lift::set_power(-127);
        } 
        else if (!kp::macro::is_active() && !kp::lift::is_homing()) {
            kp::lift::brake();
        }

        // Arm & Macro Controls
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
            kp::intake::stop_intake_only();
            kp::macro::score();
        }
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            kp::intake::stop_intake_only();
            kp::macro::stop();
            kp::armtake::stop();
            kp::arm::set_power(55);
        } 
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            kp::intake::stop_intake_only();
            kp::macro::stop();
            kp::arm::set_power(-55);
        }
        else if (!kp::macro::is_active()) {
            kp::arm::brake();
        }

        // Armtake Solo Toggles
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            kp::armtake::toggle_outtake();
        }
        else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
            kp::armtake::toggle_intake();
        }

        // Combined Intake + Armtake Toggles
        else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            kp::intake::tintake();
        }
        else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            kp::intake::touttake();
        }

        // Homing Reset
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
            kp::macro::stop();
            kp::armtake::stop();
            kp::intake::stop();
            kp::lift::home(); // Runs in background task, does not freeze controls
        }

        // Safety & Drive Controls
        kp::arm::update();

        #ifdef CONTROLLER_MOVEMENT
        kp::chassis::track_dual_joystick(1, 1);
        #endif
        
        kp::controller::delay();
    }
}