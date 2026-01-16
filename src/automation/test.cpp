#include "kp/matchload.hpp"
#include "kp/trapdoor.hpp"
#include "kp/vision.hpp"
#include "kp/wing.hpp"
#include "main.h"
#include "pros/rtos.hpp"

namespace automation {
    void test() {
    }
    
    // kp::vision::drive_straight_toward_goal(2000, true);

    void right_seven() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose first_blocks = lemlib::Pose(3.91, 18.78,19.12);
        lemlib::Pose other_blocks = lemlib::Pose(9.54, 22.35, 22.56);
        lemlib::Pose long_goalpos = lemlib::Pose(36, 2.51, 180);
        lemlib::Pose long_goal = lemlib::Pose(36, 26.51, 180);
        lemlib::Pose long_goal1 = lemlib::Pose(36, 35.51, 180);
        lemlib::Pose matchload = lemlib::Pose(36, -10, 180);
        lemlib::Pose long_goalpos1 = lemlib::Pose(36, 18, 180);
        lemlib::Pose wingpos = lemlib::Pose(27, 4.32, 180);
        lemlib::Pose wing_control= lemlib::Pose(30, 40, 180);

    // pick up middle 3 blocks
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 1200, {.maxSpeed = 60}); // move to block
            chassis->waitUntilDone();
            pros::delay(200);
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 300,{.maxSpeed = 50}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(200);

        // move to long goal pos
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,1500, {.minSpeed = 10}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(100);
            chassis->turnToHeading(180,500, {.maxSpeed = 80}); // face towards long goal
            chassis->waitUntilDone();
            kp::intake::stop(); // stop intake
            pros::delay(100);

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,800, {.forwards = false, .maxSpeed = 60}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(200);
            kp::intake::score(); // score on the long goal again
            chassis->waitUntilDone();
            pros::delay(1500);
            kp::intake::stop(); // stop intake  
            kp::matchload::toggle(); // bring down matchload mec
            pros::delay(200);

        // pick up matchload
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(matchload.x, matchload.y, 1500, {.maxSpeed = 70}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(700);

        // score in long goal 
            chassis->moveToPoint(long_goal1.x, long_goal1.y,700, {.forwards = false, .maxSpeed = 60}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(350);
            kp::intake::score(); // score on the long goal again
            chassis->waitUntilDone();
            pros::delay(1500);
            kp::intake::stop(); // stop intake
            kp::wing::toggle(); // close wings

        // move to wing position
            chassis->moveToPoint(long_goalpos1.x, long_goalpos1.y,700, {.minSpeed = 80}); // move to long goal position
            chassis->waitUntilDone();
            kp::matchload::toggle(); // bring up matchload mech
            chassis->moveToPoint(wingpos.x,wingpos.y,1000, {.minSpeed = 70});
            chassis->waitUntilDone();
            chassis->turnToHeading(wingpos.theta,500, {.minSpeed = 40});
            chassis->waitUntilDone();
            kp::wing::toggle(); // open wings

        // get long goal control
            chassis->moveToPoint(wing_control.x,wing_control.y,800, {.forwards = false, .minSpeed = 50});
            chassis->waitUntilDone();  
            chassis->turnToHeading(150,  250, {.minSpeed = 80});
            chassis->waitUntilDone();
    }

    void left_seven() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose first_blocks = lemlib::Pose(-3.91, 18.78,-19.12);
        lemlib::Pose other_blocks = lemlib::Pose(-9.54, 22.35, -22.56);
        lemlib::Pose long_goalpos = lemlib::Pose(-31.38, 3.69, -180);
        lemlib::Pose long_goalpos1 = lemlib::Pose(36, 18, 180);
        lemlib::Pose long_goal = lemlib::Pose(-31.38, 26.51, -180);
        lemlib::Pose long_goal1 = lemlib::Pose(-31.38, 33.51, -180);
        lemlib::Pose matchload = lemlib::Pose(-30.44, -15.92, -180);
        lemlib::Pose wingpos = lemlib::Pose(43.04, 30.70, 00);
        lemlib::Pose wing_control= lemlib::Pose(45.18, 42.84, 0);

        // pick up middle 3 blocks
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 1200, {.maxSpeed = 60}); // move to block
            chassis->waitUntilDone();
            pros::delay(200);
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 300,{.maxSpeed = 50}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(200);

        // move to long goal pos
            kp::intake::stop(); // stop intake
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,1500, {.minSpeed = 10}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(100);
            chassis->turnToHeading(180,500, {.maxSpeed = 80}); // face towards long goal
            chassis->waitUntilDone();
            pros::delay(100);

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,800, {.forwards = false, .maxSpeed = 60}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(200);
            kp::intake::score(); // score on the long goal again
            chassis->waitUntilDone();
            pros::delay(1500);
            kp::intake::stop(); // stop intake  
            kp::matchload::toggle(); // bring down matchload mec
            pros::delay(200);

        // pick up matchload
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(matchload.x, matchload.y, 1500, {.maxSpeed = 70}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(100);
            kp::wing::toggle(); // open wings

        // score in long goal 
            chassis->moveToPoint(long_goal1.x, long_goal1.y,800, {.forwards = false, .maxSpeed = 70}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(300);
            kp::intake::score(); // score on the long goal again
            chassis->waitUntilDone();
            pros::delay(1500);
            kp::intake::stop(); // stop intake
            kp::wing::toggle(); // close wings
        
        /*
        // move to wing position
            kp::matchload::toggle(); // bring up matchload mech
            chassis->moveToPoint(wingpos.x,wingpos.y,5000, {.maxSpeed = 80});
            chassis->waitUntilDone();
            pros::delay(100);
            chassis->turnToHeading(wingpos.theta,5000, {.minSpeed = 40});
            chassis->waitUntilDone();
            pros::delay(100);

        // get long goal control
            chassis->moveToPoint(wing_control.x,wing_control.y,5000, {.minSpeed = 60});
            chassis->waitUntilDone();  */
    }

    void left_fourthree() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose first_blocks = lemlib::Pose(-3.91, 18.78,-19.12);
        lemlib::Pose other_blocks = lemlib::Pose(-9.54, 22.35, -22.56);
        lemlib::Pose long_goalpos = lemlib::Pose(-31.38, 3.69, -180);
        lemlib::Pose long_goal = lemlib::Pose(-31.38, 26.51, -180);
        lemlib::Pose long_goal1 = lemlib::Pose(-31.38, 33.51, -180);
        lemlib::Pose matchload = lemlib::Pose(-30.44, -15.92, -180);
        lemlib::Pose center_high = lemlib::Pose(4.82, 27.5, -135);
        lemlib::Pose wingpos = lemlib::Pose(-43.04, 30.70, 0);
        lemlib::Pose wing_control= lemlib::Pose(-45.18, 42.84, 0);
        
         // pick up middle 3 blocks
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 2000, {.maxSpeed = 60}); // move to block
            chassis->waitUntilDone();
            pros::delay(200);
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 5000,{.maxSpeed = 50}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(200);

        // move to long goal pos
            kp::intake::stop(); // stop intake
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,4000, {.minSpeed = 10}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(100);
            chassis->turnToHeading(180,500, {.maxSpeed = 80}); // face towards long goal
            chassis->waitUntilDone();
            pros::delay(100);

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,500, {.forwards = false, .maxSpeed = 60}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(200);
            kp::intake::score(); // score on the long goal again
            chassis->waitUntilDone();
            pros::delay(1500);
            kp::intake::stop(); // stop intake  
            kp::matchload::toggle(); // bring down matchload mec
            pros::delay(200);

        // pick up matchload
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(matchload.x, matchload.y, 3000, {.maxSpeed = 60}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(100);
        
        // score in center high goal
            kp::intake::stop(); // stop intake
            chassis->moveToPoint(center_high.x, center_high.y,5000, {.minSpeed = 50}); // move to center low goal
            chassis->turnToHeading(center_high.theta,5000, {.minSpeed = 50}); // turn to center low goal
            chassis->waitUntilDone();
            kp::trapdoor::togglet(); // open trapdoor 
            kp::intake::score(); // score on the center high goal
            pros::delay(500);
            kp::trapdoor::togglet(); // close trapdoor
            kp::intake::pickup(); // pickup intake */
    }
        


    void right_fourthree() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose first_blocks = lemlib::Pose(4.17, 19.74, 20.24);
        lemlib::Pose other_blocks = lemlib::Pose(6.12, 26.41, 16.38);
        lemlib::Pose center_low = lemlib::Pose(7.72, 17.64, 137.97);
        lemlib::Pose long_goalpos = lemlib::Pose(36.03, 2.45, 180);
        lemlib::Pose long_goal = lemlib::Pose(33.21, 19.39, 180);
        lemlib::Pose matchload = lemlib::Pose(32.48, -10, 180);
        lemlib::Pose wingpos = lemlib::Pose(-43.04, 30.70, 0);
        lemlib::Pose wing_control= lemlib::Pose(-45.18, 42.84, 0);

        // pick up middle 3 blocks
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 2000, {.maxSpeed = 90}); // move to block
            chassis->waitUntilDone();
            pros::delay(200);
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 5000,{.maxSpeed = 40}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(400);

        // score in center low goal
            kp::intake::stop(); // stop intake
            chassis->moveToPoint(center_low.x, center_low.y,5000, {.minSpeed = 50}); // move to center low goal
            chassis->turnToHeading(center_low.theta,5000, {.minSpeed = 50}); // turn to center low goal
            chassis->waitUntilDone();
            kp::intake::outtake(); // score on the center low goal
            chassis->waitUntilDone();
            pros::delay(400);

        // move to long goal pos
            kp::intake::stop(); // stop intake
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,4000, {.maxSpeed = 80}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(200);
            chassis->turnToHeading(long_goalpos.theta,10000, {.maxSpeed = 60}); // face towards long goal

        // pick up matchload
            kp::matchload::toggle(); // bring down matchload mech
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(matchload.x, matchload.y, 3000, {.maxSpeed = 70}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(450);

        // score in long goal 
            chassis->moveToPoint(long_goal.x, long_goal.y,5000, {.forwards = false, .maxSpeed = 60}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(300);
            kp::intake::score(); // score on the long goal again
            chassis->waitUntilDone();
            pros::delay(1000);
            kp::intake::stop(); // stop intake
        
        // move to wing position
            kp::matchload::toggle(); // bring up matchload mech
            chassis->moveToPoint(wingpos.x,wingpos.y,5000, {.maxSpeed = 80});
            chassis->waitUntilDone();
            pros::delay(100);
            chassis->turnToHeading(wingpos.theta,5000, {.minSpeed = 40});
            chassis->waitUntilDone();
            pros::delay(100);

        // get long goal control
            chassis->moveToPoint(wing_control.x,wing_control.y,5000, {.minSpeed = 60});
            chassis->waitUntilDone();  
    }

    void skills() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose rightmatchload = lemlib::Pose(14, 34, 90);
        lemlib::Pose rightlong_goalpos = lemlib::Pose(0, 32, 90);
        lemlib::Pose rightlong_goal = lemlib::Pose(-19, 34, 90);
        lemlib::Pose rightfirst_blocks = lemlib::Pose(-17, 10, 227);
        lemlib::Pose leftfirst_blocks = lemlib::Pose(-20, -34, 180);    
        lemlib::Pose center_high = lemlib::Pose(-31.62, -21.35, 135);
        lemlib::Pose leftlong_goalpos = lemlib::Pose(0, -57, 90);
        lemlib::Pose leftlong_goal = lemlib::Pose(-19, -57, 90);
        lemlib::Pose leftmatchload = lemlib::Pose(14, -57;[=], 90);
        lemlib::Pose parkzone_pos = lemlib::Pose(-5, 13, 90);
        lemlib::Pose parkzone = lemlib::Pose(-5, -50, 180);

         // pick up right side matchload
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(rightlong_goalpos.x, rightlong_goalpos.y,1200, {.minSpeed = 20, .earlyExitRange = 1}); // move to position 
            chassis->waitUntilDone();
            chassis->turnToHeading(rightmatchload.theta, 1000, {.maxSpeed = 50}); // turn to matchload tubes
            chassis->waitUntilDone();
            kp::matchload::toggle(); // bring down matchload mech
            chassis->moveToPoint(rightmatchload.x, rightmatchload.y, 1200, {.minSpeed = 70}); // move to matchload tubes
            chassis->waitUntilDone();
            chassis->moveToPoint(17.5, rightmatchload.y, 1000, {.minSpeed = 80}); // move to matchload tubes
            chassis->moveToPoint(16, rightmatchload.y, 1000, {.minSpeed = 85}); // move to matchload tubes
            pros::delay(3000);
            chassis->moveToPoint(17.5, rightmatchload.y, 1000, {.minSpeed = 80}); // move to matchload tubes
            chassis->moveToPoint(16, rightmatchload.y, 1000, {.minSpeed = 85}); // move to matchload tubes

        // score in right long goal
            chassis->moveToPoint(rightlong_goal.x, rightlong_goal.y,1000, {.forwards = false, .maxSpeed = 70}); // back into long goal
            chassis->waitUntilDone();
            kp::intake::score(); // score on the long goal
            pros::delay(1100);
            kp::matchload::toggle(); // bring up matchload mech
            kp::intake::stop(); // stop intake

        // pick up right middle 3 blocks
            chassis->moveToPoint(0, 35,1500, {.minSpeed = 40}); // move to position again
            chassis->waitUntilDone();
            pros::delay(100);
            kp::intake::pickup(); // start intake
            chassis->turnToHeading(225, 500, {.maxSpeed = 40}); // turn towards middle blocks
            chassis->moveToPoint(rightfirst_blocks.x, rightfirst_blocks.y, 2000, {.maxSpeed = 70}); // move to block
            pros::delay(1267);
            kp::matchload::toggle(); // bring down matchload mech
            pros::delay(400);

        // pick up left middle 3 blocks
            kp::matchload::toggle(); // bring up matchload mech
            chassis->turnToHeading(180, 500, {.maxSpeed = 57}); // turn towards left middle blocks
            chassis->waitUntilDone();
            chassis->moveToPoint(leftfirst_blocks.x, leftfirst_blocks.y, 1200, {.minSpeed = 50,.earlyExitRange = 1}); // move to block
            pros::delay(900);
            kp::matchload::toggle(); // bring down matchload mech

        // score in Center High Goal
            chassis->turnToHeading(center_high.theta,300, {.minSpeed = 50}); // turn to center high goal
            chassis->waitUntilDone();
            chassis->moveToPoint(center_high.x, center_high.y,1000, {.forwards = false,.minSpeed = 50, .earlyExitRange = 1}); // move to center high goal
            chassis->waitUntilDone();
            kp::trapdoor::togglet(); // open trapdoor 
            pros::delay(700);
            kp::trapdoor::togglet(); // close trapdoor

        // pick up left side matchload
            chassis->moveToPoint(leftlong_goalpos.x, leftlong_goalpos.y, 2000, {.minSpeed = 40}); // move to position
            chassis->waitUntilDone();
            chassis->turnToHeading(leftmatchload.theta, 500, {.minSpeed = 50}); // turn to matchload tubes
            chassis->waitUntilDone(); 
            kp::matchload::toggle(); // bring down matchload mech
            pros::delay(100);
            chassis->moveToPoint(leftmatchload.x, leftmatchload.y, 3000, {.maxSpeed = 70}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(450);
    
        // score in left long goal
            chassis->moveToPoint(leftlong_goal.x, leftlong_goal.y,1000, {.forwards = false, .maxSpeed = 60}); // back into long goal
            chassis->waitUntilDone();
            kp::intake::score(); // score on the long goal
            chassis->waitUntilDone();
            pros::delay(3000); 
        // move to park zone position
            chassis->moveToPoint(parkzone_pos.x, parkzone_pos.y,5000, {.minSpeed = 40});
            chassis->waitUntilDone();

        // park in zone
            chassis->moveToPoint(parkzone.x, parkzone.y,5000, {.minSpeed = 90});
    }

    void Solo_AWP() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose rightmatchload = lemlib::Pose(14, 34, 90);
        lemlib::Pose rightlong_goalpos = lemlib::Pose(0, 32, 90);
        lemlib::Pose rightlong_goal = lemlib::Pose(-19, 34, 90);
        lemlib::Pose rightfirst_blocks = lemlib::Pose(-17, 10, 227);
        lemlib::Pose leftfirst_blocks = lemlib::Pose(-20, -34, 180);    
        lemlib::Pose center_high = lemlib::Pose(-31.62, -21.35, 135);
        lemlib::Pose leftlong_goalpos = lemlib::Pose(0, -57, 90);
        lemlib::Pose leftlong_goal = lemlib::Pose(-19, -57, 90);
        lemlib::Pose leftmatchload = lemlib::Pose(0, 0, 90);

        // pick up right side matchload
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(rightlong_goalpos.x, rightlong_goalpos.y,1200, {.minSpeed = 20, .earlyExitRange = 1}); // move to position 
            chassis->waitUntilDone();
            chassis->turnToHeading(rightmatchload.theta, 1000, {.maxSpeed = 50}); // turn to matchload tubes
            chassis->waitUntilDone();
            kp::matchload::toggle(); // bring down matchload mech
            chassis->moveToPoint(rightmatchload.x, rightmatchload.y, 1200, {.minSpeed = 70}); // move to matchload tubes
            chassis->waitUntilDone();
            chassis->moveToPoint(17.5, rightmatchload.y, 1000, {.minSpeed = 80}); // move to matchload tubes
            chassis->moveToPoint(16, rightmatchload.y, 1000, {.minSpeed = 85}); // move to matchload tubes
            pros::delay(500);

        // score in right long goal
            chassis->moveToPoint(rightlong_goal.x, rightlong_goal.y,1000, {.forwards = false, .maxSpeed = 70}); // back into long goal
            chassis->waitUntilDone();
            kp::intake::score(); // score on the long goal
            pros::delay(1100);
            kp::matchload::toggle(); // bring up matchload mech
            kp::intake::stop(); // stop intake

        // pick up right middle 3 blocks
            chassis->moveToPoint(0, 35,1200, {.minSpeed = 50}); // move to position again
            chassis->waitUntilDone();
            pros::delay(100);
            kp::intake::pickup(); // start intake
            chassis->turnToHeading(225, 500, {.maxSpeed = 57}); // turn towards middle blocks
            chassis->moveToPoint(rightfirst_blocks.x, rightfirst_blocks.y, 2000, {.maxSpeed = 70}); // move to block
            pros::delay(1267);
            kp::matchload::toggle(); // bring down matchload mech
            pros::delay(400);

        // pick up left middle 3 blocks
            kp::matchload::toggle(); // bring up matchload mech
            chassis->turnToHeading(180, 500, {.maxSpeed = 57}); // turn towards left middle blocks
            chassis->waitUntilDone();
            chassis->moveToPoint(leftfirst_blocks.x, leftfirst_blocks.y, 1200, {.minSpeed = 50,.earlyExitRange = 1}); // move to block
            pros::delay(900);
            kp::matchload::toggle(); // bring down matchload mech

        // score in Center High Goal
            chassis->turnToHeading(center_high.theta,300, {.minSpeed = 50}); // turn to center high goal
            chassis->waitUntilDone();
            chassis->moveToPoint(center_high.x, center_high.y,500, {.forwards = false,.minSpeed = 50, .earlyExitRange = 1}); // move to center high goal
            chassis->waitUntilDone();
            kp::trapdoor::togglet(); // open trapdoor 
            pros::delay(250);
            kp::trapdoor::togglet(); // close trapdoor

        // pick up left side matchload
            chassis->moveToPoint(leftlong_goalpos.x, leftlong_goalpos.y, 2000, {.minSpeed = 40}); // move to position
            chassis->waitUntilDone();
            chassis->turnToHeading(leftmatchload.theta, 500, {.minSpeed = 50}); // turn to matchload tubes
            chassis->waitUntilDone(); /*
            kp::matchload::toggle(); // bring down matchload mech
            pros::delay(100);
            chassis->moveToPoint(leftmatchload.x, leftmatchload.y, 3000, {.maxSpeed = 70}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(450); */
    
        // score in left long goal
            chassis->moveToPoint(leftlong_goal.x, leftlong_goal.y,1000, {.forwards = false, .maxSpeed = 60}); // back into long goal
            chassis->waitUntilDone();
            kp::intake::score(); // score on the long goal
            chassis->waitUntilDone();
            pros::delay(3000); 
    }
}