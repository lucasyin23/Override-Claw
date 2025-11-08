#include "main.h"
#include "kp/intake.hpp"

#define CONTROLLER_MOVEMENT
std::string alliance = "N/A";

void initialize() {
	pros::lcd::initialize();
  kp::chassis::initialize();
  kp::controller::initialize();
  kp::intake::initialize();
  
  
  kp::automation::set("TEST", automation::test);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
  kp::automation::run();
}

void opcontrol() {
  lemlib::Chassis *chassis = kp::chassis::get_chassis();
  pros::Controller master(pros::E_CONTROLLER_MASTER);
  // pre-ready loop
  while (kp::controller::is_ready() == false) {
    if (kp::controller::lock() == false) {
      kp::controller::delay();
      continue;
    }

    LCD_MAIN_CLEAR
    LCD_1("# %s # %s", alliance, kp::automation::title())
    LCD_2("X:BLUE / Y:RED ")
    LCD_3("A:READY / B:RESET")
    LCD_4("R1:ATM NEXT / R2:ATM PREPARE / DN:ATM TESTER")

    kp::chassis::refresh_xy();

    if (CTRL_PRESS_ONCE(DOWN)) {
      kp::automation::launch_selection();
    }

     if (CTRL_PRESS_ONCE(X)) {
      alliance = "BLUE";
      kp::automation::clear();
      kp::automation::set("BRL", automation::regional_left);
      CTRL_VIBRATE(".")
    }

    if (CTRL_PRESS_ONCE(Y)) {
      alliance = "RED";
      kp::automation::clear();
      kp::automation::set("RRL", automation::regional_left);
      CTRL_VIBRATE(".")
    }

    if (CTRL_PRESS_ONCE(R1)) {
      kp::automation::next();
    }

    if (CTRL_PRESS_ONCE(R2)) {
        if (kp::chassis::is_pose_origin()) {
        kp::automation::run();
      }
    }

    if (CTRL_PRESS_ONCE(B)) {
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

    // --- Intake Toggle Setup ---
  enum IntakeMode { STOPPED, PICKUP, OUTTAKE, FEED_TOP, SCORE };
  IntakeMode currentMode = STOPPED;
  while (kp::controller::is_ready() == true) {

    // Intake toggle logic
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
      if (currentMode == PICKUP) {
        kp::intake::stop();
        currentMode = STOPPED;
      } else {
        kp::intake::pickup();
        currentMode = PICKUP;
      }
    }

    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
      if (currentMode == OUTTAKE) {
        kp::intake::stop();
        currentMode = STOPPED;
      } else {
        kp::intake::outtake();
        currentMode = OUTTAKE;
      }
    }

    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
      if (currentMode == FEED_TOP) {
        kp::intake::stop();
        currentMode = STOPPED;
      } else {
        kp::intake::feed_top();
        currentMode = FEED_TOP;
      }
    }

    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
      if (currentMode == SCORE) {
        kp::intake::stop();
        currentMode = STOPPED;
      } else {
        kp::intake::score();
        currentMode = SCORE;
      }
    }

    // --- Drive controls ---
    #ifdef CONTROLLER_MOVEMENT
    kp::chassis::track_dual_joystick(1.6, 1);
    #endif

    kp::controller::delay();
  }
}