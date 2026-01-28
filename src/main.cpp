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

    if (CTRL_PRESS_ONCE(Y)) {
      kp::matchload::toggle();
    }
    
    if (CTRL_PRESS_ONCE(B)) {
      kp::hood::togglet();
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
    if (CTRL_PRESS_ONCE(Y)) {
      kp::matchload::toggle();
    }

    if (CTRL_PRESS_ONCE(B)) {
      kp::wing::toggle();
    }

    if (CTRL_PRESS_ONCE(UP)) {
      kp::hood::togglet();
    }

    // intake controls 
    static int intake_mode = 0; 
    static bool hood_is_up = false;

    // L1: Pickup
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
        if (intake_mode == 1) intake_mode = 0; 
        else intake_mode = 1;                  
    }
    // L2: Outtake
    else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
        if (intake_mode == 2) intake_mode = 0;
        else intake_mode = 2;
    }
    // R1:Scorer
    else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
        if (intake_mode == 3) intake_mode = 0;
        else intake_mode = 3;
    }
    // R2: Middle Goal 
    else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
        if (intake_mode == 4) intake_mode = 0;
        else intake_mode = 4;
    }
    // LEFT: SLow Outtake 
    else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        if (intake_mode == 5) intake_mode = 0;
        else intake_mode = 5;
        if (hood_is_up == true) { kp::hood::togglet(); hood_is_up = false; }
    }
    // DOWN: Slow Middle Goal
    else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        if (intake_mode == 6) intake_mode = 0;
        else intake_mode = 6;
        if (hood_is_up == true) { kp::hood::togglet(); hood_is_up = false; }
    }
    // A: Stop
     else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
        if (intake_mode == 0) intake_mode = 0;
        else intake_mode = 0;
        if (hood_is_up == true) { kp::hood::togglet(); hood_is_up = false; }
    }


    if (intake_mode == 1) { 
        // MODE 1: PICKUP 
        kp::intake::pickup();
        if (hood_is_up == true) { kp::hood::togglet(); hood_is_up = false; }
    }
    else if (intake_mode == 2) { 
        // MODE 2: UTAKE LOW GOAL
        kp::intake::slowout();
        if (hood_is_up == true) { kp::hood::togglet(); hood_is_up = false; }
    }
    else if (intake_mode == 3) { 
        // MODE 3: SCORER 
        kp::intake::score();
        if (hood_is_up == false) { kp::hood::togglet(); hood_is_up = true; }
    }
    else if (intake_mode == 4) { 
        // MODE 4: MIDDLE GOAL
        kp::intake::mid();
        if (hood_is_up == true) { kp::hood::togglet(); hood_is_up = false; }
    }
    else if (intake_mode == 5) { 
        // MODE 5: FAST OUTTAKE 
          kp::intake::outtake();
        if (hood_is_up == true) { kp::hood::togglet(); hood_is_up = false; }
    }
    else if (intake_mode == 6) { 
        // MODE 6: Slow Middle Goal 
          kp::intake::slowmid();
        if (hood_is_up == true) { kp::hood::togglet(); hood_is_up = false; }
    }
    else { 
        // MODE 0: STOP 
        kp::intake::stop();
        if (hood_is_up == true) { kp::hood::togglet(); hood_is_up = false; }
    }
    

    // Drive controls 
    #ifdef CONTROLLER_MOVEMENT
    kp::chassis::track_dual_joystick(1.6, 1);
    #endif

    kp::controller::delay();
  }
}