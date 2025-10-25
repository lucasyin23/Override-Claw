#include "main.h"

#define CONTROLLER_MOVEMENT
std::string alliance = "N/A";

void initialize() {
	pros::lcd::initialize();
  kp::chassis::initialize();
  kp::controller::initialize();
  
  kp::automation::set("TEST", automation::test);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
  kp::automation::run();
}

void opcontrol() {
  lemlib::Chassis *chassis = kp::chassis::get_chassis();

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

  while (kp::controller::is_ready() == true) {

    #ifdef CONTROLLER_MOVEMENT
    kp::chassis::track_dual_joystick(1.6, 1);
    #endif
    kp::controller::delay();
  }
}