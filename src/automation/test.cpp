#include "kp/intake.hpp"
#include "main.h"
#include "pros/rtos.hpp"

namespace automation {
    void test() {

    }

    void right_seven() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose first_blocks = lemlib::Pose(4.17, 19.74, 20.24);
        lemlib::Pose other_blocks = lemlib::Pose(6.12, 26.41, 16.38);
        lemlib::Pose long_goalpos = lemlib::Pose(36.03, 2.45, 180);
        lemlib::Pose long_goal = lemlib::Pose(33.21, 19.39, 180);
        lemlib::Pose matchload = lemlib::Pose(32.48, -7.77, 180);

        // pick up middle 3 blocks
            kp::matchload::toggle(); // bring down matchload mech
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 2000, {.maxSpeed = 70}); // move to block
            chassis->waitUntilDone();
            pros::delay(450);
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 2000,{.maxSpeed = 40}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(1000);

        // move to in long goal
            kp::intake::stop(); // stop intake
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,4000, {.maxSpeed = 80}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(450);
            chassis->turnToHeading(long_goalpos.theta,10000, {.minSpeed = 60}); // face towards long goal
            chassis->waitUntilDone();
            pros::delay(200);
        
        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,1000, {.forwards = false,.minSpeed = 40}); // back into long goal
            chassis->waitUntilDone();
            pros::delay(400);
            kp::intake::score(); // score on the long goal
            chassis->waitUntilDone();

        // pick up matchload
            kp::intake::stop(); // stop intake
            chassis->turnToHeading(matchload.theta, 2000, {.minSpeed = 40}); // move to matchload tubes
            chassis->waitUntilDone();
            kp::matchload::toggle(); // bring down matchload mech
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(matchload.x, matchload.y, 1000, {.minSpeed = 40}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(500);

        // score in long goal again
            chassis->moveToPoint(long_goal.x, long_goal.y,500, {.forwards = false, .minSpeed = 40}); // back into long goal again
            kp::intake::score(); // score on the long goal again
            chassis->waitUntilDone();
            pros::delay(500);
            kp::intake::stop(); // stop intake
            
    }
    void left_seven() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose first_blocks = lemlib::Pose(0, 0, 0);
        lemlib::Pose other_blocks = lemlib::Pose(0, 0, 0);
        lemlib::Pose long_goal = lemlib::Pose(0, 0, 180);
        lemlib::Pose long_goalpos = lemlib::Pose(0, 0, 180);
        lemlib::Pose matchload = lemlib::Pose(0, 0, 180);

        // pick up middle 3 blocks
            kp::matchload::toggle(); // bring down matchload mech
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 2000, {.maxSpeed = 70}); // move to block
            chassis->waitUntilDone();
            pros::delay(450);
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 2000,{.maxSpeed = 40}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(1000);

        // move to in long goal
            kp::intake::stop(); // stop intake
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,4000, {.maxSpeed = 80}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(450);
            chassis->turnToHeading(long_goalpos.theta,10000, {.minSpeed = 60}); // face towards long goal
            chassis->waitUntilDone();
            pros::delay(200);
        
        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,1000, {.forwards = false,.minSpeed = 40}); // back into long goal
            chassis->waitUntilDone();
            pros::delay(400);
            kp::intake::score(); // score on the long goal
            chassis->waitUntilDone();

        // pick up matchload
            kp::intake::stop(); // stop intake
            chassis->turnToHeading(matchload.theta, 2000, {.minSpeed = 40}); // move to matchload tubes
            chassis->waitUntilDone();
            kp::matchload::toggle(); // bring down matchload mech
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(matchload.x, matchload.y, 1000, {.minSpeed = 40, .earlyExitRange = 0.7}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(500);

        // score in long goal again
            chassis->moveToPoint(long_goal.x, long_goal.y,500, {.forwards = false, .minSpeed = 40}); // back into long goal again
            kp::intake::score(); // score on the long goal again
            chassis->waitUntilDone();
            pros::delay(500);
            kp::intake::stop(); // stop intake  
    }
    void left_fourthree() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose first_blocks = lemlib::Pose(0, 0, 0);
        lemlib::Pose other_blocks = lemlib::Pose(0, 0, 0);
        lemlib::Pose center_high = lemlib::Pose(0, 0, 0);
        lemlib::Pose matchload = lemlib::Pose(0, 0, 180);
        lemlib::Pose long_goal = lemlib::Pose(0, 0, 180);
        lemlib::Pose long_goalpos = lemlib::Pose(0, 0, 180);
        
        // pick up middle 3 blocks
            kp::intake::pickup(); // startm intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 1000, {.minSpeed = 40}); // move to blocks
            chassis->waitUntilDone();
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 1000,{.minSpeed = 40}); // move to other blocks
            pros::delay(500);

        // score in Center High Goal
            kp::intake::stop(); // stop intake
            chassis->turnToHeading(center_high.theta,300, {.minSpeed = 50}); // turn to center high goal
            chassis->moveToPoint(center_high.x, center_high.y,500, {.minSpeed = 50, .earlyExitRange = 1}); // move to center high goal
            kp::intake::feed_top(); // score on the center high goal
            chassis->waitUntilDone();
            pros::delay(400);

        // Go to Long Goal
            kp::intake::stop(); // stop intake
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,2000, {.minSpeed = 50}); // move to long goal position
            chassis->turnToHeading(long_goal.theta,500, {.minSpeed = 50}); // face towards long goal
            chassis->waitUntilDone();

        // pick up matchload
            kp::matchload::toggle(); // bring down matchload mech
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(matchload.x, matchload.y, 500, {.minSpeed = 40, .earlyExitRange = 0.7}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(500);

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,500, {.forwards = false, .minSpeed = 40, .earlyExitRange = 1}); // back into long goal
            kp::intake::score(); // score on the long goal
            chassis->waitUntilDone();
            pros::delay(500);
            kp::intake::stop(); // stop intake    
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
        lemlib::Pose matchload = lemlib::Pose(32.48, -6.77, 180);

        // pick up middle 3 blocks
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 1000, {.minSpeed = 40}); // move to blocks
            chassis->waitUntilDone();
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 1000,{.minSpeed = 40}); // move to other blocks
            pros::delay(500);

        // score in Center Low Goal
            kp::intake::stop(); // stop intake
            chassis->turnToHeading(center_low.theta,500, {.minSpeed = 50}); // turn to center low goal
            chassis->waitUntilDone();
            chassis->moveToPoint(center_low.x, center_low.y,500, {.minSpeed = 50}); // move to center low goal
            kp::intake::outtake(); // score on the center low goal
            chassis->waitUntilDone();
            pros::delay(400);

        // Go to Long Goal
            kp::intake::stop(); // stop intake
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,2000, {.minSpeed = 50}); // move to long goal position
            chassis->turnToHeading(long_goal.theta,500, {.minSpeed = 50}); // face towards long goal
            chassis->waitUntilDone();

        // pick up matchload
            kp::matchload::toggle(); // bring down matchload mech
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(matchload.x, matchload.y, 500, {.minSpeed = 40}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(500);

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,500, {.forwards = false, .minSpeed = 40}); // back into long goal
            kp::intake::score(); // score on the long goal 
            chassis->waitUntilDone();
            pros::delay(500);
            kp::intake::stop(); // stop intake         
    }
}