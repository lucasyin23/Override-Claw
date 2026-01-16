#include "main.h"
#include "kp/intake.hpp"
#include "pros/misc.h"

#define CONTROLLER_MOVEMENT
std::string alliance = "N/A";

void initialize() {
	pros::lcd::initialize();
  kp::chassis::initialize();
  kp::controller::initialize();
  kp::intake::initialize();
  
  kp::automation::set("SAWP", automation::Solo_AWP);
  kp::automation::set("R7", automation::right_seven);
  kp::automation::set("L7", automation::left_seven);
  kp::automation::set("SAWP", automation::Solo_AWP);
  kp::automation::set("L43", automation::left_fourthree);
  kp::automation::set("SKILLS", automation::skills);
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

    if (CTRL_PRESS_ONCE(B)) {
      kp::matchload::toggle();
    }
    
    if (CTRL_PRESS_ONCE(Y)) {
      kp::trapdoor::togglet();
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
    if (CTRL_PRESS_ONCE(B)) {
      kp::matchload::toggle();
    }

    if (CTRL_PRESS_ONCE(Y)) {
      kp::wing::toggle();
    }

    if (CTRL_PRESS_ONCE(DOWN)) {
      kp::trapdoor::togglet();
    }

    // intake controls 
    static bool feedEnabled = false;
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
      feedEnabled = !feedEnabled;  // toggle middle high goal score + pickup & trapdoor
    if (feedEnabled) {
      kp::intake::feed_top();     // turn intake feed ON
      kp::trapdoor::togglet();   //  trapdoor toggles WITH feed
    } else {
      kp::intake::stop();        // turn intake feed OFF
      kp::trapdoor::togglet();  //  trapdoor toggles back
    }
    }
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) { // pickup only
      feedEnabled = false;
      kp::intake::pickup();
    }
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) { // outtake only
      feedEnabled = false;
      kp::intake::outtake();
    }
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { // pickup + score
      feedEnabled = false;
      kp::intake::score();
    }
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) { // slow outtake
      feedEnabled = false;
      kp::intake::slow();
    }
    else if (!feedEnabled) { // stop intake
      kp::intake::stop();
    }
    

    // Drive controls 
    #ifdef CONTROLLER_MOVEMENT
    kp::chassis::track_dual_joystick(1.6, 1);
    #endif

    kp::controller::delay();
  }
}