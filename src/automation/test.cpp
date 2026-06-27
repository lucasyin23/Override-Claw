#include "kp/matchload.hpp"
#include "kp/hood.hpp"
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
        lemlib::Pose first_blocks = lemlib::Pose(4.06, 17.41,17.24);
        lemlib::Pose other_blocks = lemlib::Pose(4.44, 18.31, 26.94);
        lemlib::Pose long_goalpos = lemlib::Pose(35.5, 2.51, 180);
        lemlib::Pose long_goal = lemlib::Pose(35.5, 27.51, 180);
        lemlib::Pose matchload = lemlib::Pose(35, -10.2, 180);
        lemlib::Pose long_goalpos1 = lemlib::Pose(35.5, 18, 180);
        lemlib::Pose wingpos = lemlib::Pose(48, 10, 180);
        lemlib::Pose wing_control= lemlib::Pose(50, 38, 170);

   // pick up middle 3 blocks
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 1200, {.minSpeed = 20}); // move to block
            pros::delay(600);
            kp::matchload::toggle(); // bring down matchload mec
            chassis->waitUntilDone();
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 300,{.minSpeed = 20}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(200);

        // move to long goal pos
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,2000, {.maxSpeed = 80}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(100);
            chassis->turnToHeading(180,500, {.maxSpeed = 70}); // face towards long goal
            chassis->waitUntilDone();

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,1000, {.forwards = false, .maxSpeed = 50}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(200);
            kp::intake::score(); // score on the long goal 
            chassis->waitUntilDone();
            pros::delay(2000); 
            kp::intake::pickup();
            pros::delay(200);

        // pick up matchload
            pros::delay(200);
            chassis->moveToPoint(matchload.x, matchload.y, 1500, {.maxSpeed = 60}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(700);

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,1000, {.forwards = false, .maxSpeed = 50}); // back into long goal again
            chassis->waitUntilDone();
            kp::matchload::toggle(); // bring up matchload mec
            pros::delay(200);
            kp::intake::score(); // score on the long goal 
            chassis->waitUntilDone();
            pros::delay(1500);
            kp::intake::stop(); // stop intake  
            pros::delay(200);

        // move to wing position
            chassis->moveToPoint(long_goalpos1.x, long_goalpos1.y,700, {.maxSpeed = 60}); // move to long goal position
            chassis->waitUntilDone();
            chassis->moveToPoint(wingpos.x,wingpos.y,1500, {.maxSpeed=70});
            chassis->waitUntilDone();
            chassis->turnToHeading(wingpos.theta,500, {.minSpeed = 40});
            chassis->waitUntilDone();

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
        lemlib::Pose long_goalpos1 = lemlib::Pose(-36, 18, 180);
        lemlib::Pose long_goal = lemlib::Pose(-31.38, 26.51, -180);
        lemlib::Pose matchload = lemlib::Pose(-30.44, -16.92, -180);
        lemlib::Pose wingpos = lemlib::Pose(-43.04, 30.70, -180);
        lemlib::Pose wing_control= lemlib::Pose(-45.18, 42.84, -180);

           // pick up middle 3 blocks
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 1200, {.minSpeed = 20}); // move to block
            pros::delay(200);
            kp::matchload::toggle(); // bring down matchload mec
            chassis->waitUntilDone();
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 300,{.minSpeed = 20}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(200);

        // move to long goal pos
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,1500, {.maxSpeed = 60}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(100);
            chassis->turnToHeading(180,500, {.maxSpeed = 70}); // face towards long goal
            chassis->waitUntilDone();

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,1000, {.forwards = false, .maxSpeed = 50}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(200);
            kp::intake::score(); // score on the long goal 
            chassis->waitUntilDone();
            pros::delay(2000); 
            kp::intake::pickup();
            pros::delay(200);

        // pick up matchload
            pros::delay(200);
            chassis->moveToPoint(matchload.x, matchload.y, 1500, {.maxSpeed = 60}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(700);

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,800, {.forwards = false, .maxSpeed = 60}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(200);
            kp::intake::score(); // score on the long goal 
            chassis->waitUntilDone();
            pros::delay(1500);
            kp::intake::stop(); // stop intake  
            kp::matchload::toggle(); // bring up matchload mec
            pros::delay(200);

        // move to wing position
            chassis->moveToPoint(long_goalpos1.x, long_goalpos1.y,700, {.maxSpeed = 60}); // move to long goal position
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

    void left_fourthree() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose first_blocks = lemlib::Pose(-4, 18.01,17.24);
        lemlib::Pose other_blocks = lemlib::Pose(-4.5, 20.31, 26.94);
        lemlib::Pose long_goalpos = lemlib::Pose(-34, 2.51, 180);
        lemlib::Pose long_goal = lemlib::Pose(-34, 27.51, 180);
        lemlib::Pose matchload = lemlib::Pose(-34, -10.2, 180);
        lemlib::Pose long_goalpos1 = lemlib::Pose(-34, 180);
        lemlib::Pose wingpos = lemlib::Pose(-23, 10, 180);
        lemlib::Pose wing_control= lemlib::Pose(-25, 40, 160);

        // pick up middle 3 blocks
            kp::intake::pickup(); // start intake
            kp::wing::toggle();
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 1200, {.minSpeed = 20}); // move to block
            pros::delay(700);
            kp::matchload::toggle(); // bring down matchload mec
            chassis->waitUntilDone();
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 300,{.minSpeed = 20}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(200);
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 1200, {.maxSpeed = 80}); // move to block

        // move to long goal pos
            kp::matchload::toggle();
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,2000, {.maxSpeed = 70}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(100);
            chassis->turnToHeading(180,500, {.maxSpeed = 70}); // face towards long goal
            chassis->waitUntilDone();

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,1000, {.forwards = false, .maxSpeed = 50}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(200);
            kp::intake::score(); // score on the long goal 
            chassis->waitUntilDone();
            pros::delay(2000); 
            kp::intake::pickup();
            pros::delay(200);
            kp::wing::toggle();
            pros::delay(500);
            kp::matchload::toggle();

        // move to wing position
            chassis->moveToPoint(long_goalpos.x, 5,700, {.maxSpeed = 50}); // move to long goal position
            chassis->waitUntilDone();
            chassis->moveToPoint(wingpos.x,wingpos.y,1500, {.maxSpeed=70});
            chassis->waitUntilDone();
            chassis->turnToHeading(wingpos.theta,500, {.minSpeed = 40});
            chassis->waitUntilDone();

        // get long goal control
            chassis->moveToPoint(wing_control.x,wing_control.y,800, {.forwards = false, .minSpeed = 50});
            chassis->waitUntilDone();  
            chassis->turnToHeading(150,  250, {.minSpeed = 80});
            chassis->waitUntilDone(); 
    }


    void right_fourthree() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose first_blocks = lemlib::Pose(4.06, 17.41,17.24);
        lemlib::Pose other_blocks = lemlib::Pose(4.44, 18.31, 26.94);
        lemlib::Pose long_goalpos = lemlib::Pose(35.5, 2.51, 180);
        lemlib::Pose long_goal = lemlib::Pose(35.5, 27.51, 180);
        lemlib::Pose matchload = lemlib::Pose(35, -10.2, 180);
        lemlib::Pose long_goalpos1 = lemlib::Pose(35.5, 18, 180);
        lemlib::Pose wingpos = lemlib::Pose(48, 10, 180);
        lemlib::Pose wing_control= lemlib::Pose(50, 38, 170);

        // pick up middle 3 blocks
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 1200, {.minSpeed = 20}); // move to block
            pros::delay(600);
            kp::matchload::toggle(); // bring down matchload mec
            chassis->waitUntilDone();
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 300,{.minSpeed = 20}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(200);

        // move to long goal pos
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,2000, {.maxSpeed = 80}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(100);
            chassis->turnToHeading(180,500, {.maxSpeed = 70}); // face towards long goal
            chassis->waitUntilDone();

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,1000, {.forwards = false, .maxSpeed = 50}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(200);
            kp::intake::score(); // score on the long goal 
            chassis->waitUntilDone();
            pros::delay(2000); 
            kp::intake::pickup();
            pros::delay(200);
            kp::matchload::toggle();

        // move to wing position
            chassis->moveToPoint(long_goalpos1.x, long_goalpos1.y,700, {.maxSpeed = 60}); // move to long goal position
            chassis->waitUntilDone();
            chassis->moveToPoint(wingpos.x,wingpos.y,1500, {.maxSpeed=70});
            chassis->waitUntilDone();
            chassis->turnToHeading(wingpos.theta,500, {.minSpeed = 40});
            chassis->waitUntilDone();

        // get long goal control
            chassis->moveToPoint(wing_control.x,wing_control.y,800, {.forwards = false, .minSpeed = 50});
            chassis->waitUntilDone();  
            chassis->turnToHeading(150,  250, {.minSpeed = 80});
            chassis->waitUntilDone(); 
    }

    void skills() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose leftclosematchload = lemlib::Pose(0, 0, -90);
        lemlib::Pose leftclosegoalpos = lemlib::Pose(0, 31.57, -90);  
        lemlib::Pose leftclosealley = lemlib::Pose(-0, 0, 90); 
        lemlib::Pose leftfaralley = lemlib::Pose(-0, 0, 90); 
        lemlib::Pose leftfargoalpos = lemlib::Pose(0, -0, 90);
        lemlib::Pose leftlonggoal = lemlib::Pose(-0, -0, 90);
        lemlib::Pose leftfarmatchload = lemlib::Pose(0, 0, 90);
        lemlib::Pose farleftblocks = lemlib::Pose(0, 0, 0);
        lemlib::Pose farrightblocks = lemlib::Pose(0, 0, 0);
        lemlib::Pose center_high = lemlib::Pose(0, 0, 135);
        lemlib::Pose rightfarmatchload = lemlib::Pose(0, 0, -90);
        lemlib::Pose rightgoalpos = lemlib::Pose(-0, 0, -90);  
        lemlib::Pose rightfarealley = lemlib::Pose(-0, 0, 90); 
        lemlib::Pose rightclosealley = lemlib::Pose(-0, 0, 90); 
        lemlib::Pose rightclosegoalpos = lemlib::Pose(0, 0, 90);
        lemlib::Pose rightgoal = lemlib::Pose(-0, 0, 90);
        lemlib::Pose rightclosematchload = lemlib::Pose(0, 0, 90);
        lemlib::Pose closerightblocks = lemlib::Pose(0, 0, 0);
        lemlib::Pose center_low = lemlib::Pose(0, 0, 45);
        lemlib::Pose parkpos = lemlib::Pose(0, 0, -90);
        lemlib::Pose parkzone = lemlib::Pose(0, 0, -90);

        // pick up left side matchload
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(leftclosegoalpos.x, leftclosegoalpos.y,1400, {.minSpeed = 20}); // move to position 
            chassis->waitUntilDone();
            chassis->turnToHeading(leftclosematchload.theta, 1000, {.maxSpeed = 50}); // turn to matchload tubes
            chassis->waitUntilDone();
            kp::matchload::toggle(); // bring down matchload mech
            pros::delay(200);
            chassis->moveToPoint(leftclosematchload.x, leftclosematchload.y, 1500, {.minSpeed = 50}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(500);
        
    }

    void Solo_AWP() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0,0);
        lemlib::Pose rightmatchload = lemlib::Pose(18, 32, 90);
        lemlib::Pose rightlong_goalpos = lemlib::Pose(0, 32, 90);
        lemlib::Pose rightlong_goal = lemlib::Pose(-19, 32, 90);
        lemlib::Pose rightfirst_blocks = lemlib::Pose(-17, 10, 227);
        lemlib::Pose leftfirst_blocks = lemlib::Pose(-20, -34, 180);    
        lemlib::Pose center_high = lemlib::Pose(-33, -25, 135);
        lemlib::Pose leftlong_goalpos = lemlib::Pose(0, -57, 90);
        lemlib::Pose leftlong_goal = lemlib::Pose(-19, -57, 90);
        lemlib::Pose leftmatchload = lemlib::Pose(0, 0, 90);

        // pick up right side matchload
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(rightlong_goalpos.x, rightlong_goalpos.y,1400, {.minSpeed = 20}); // move to position 
            chassis->waitUntilDone();
            chassis->turnToHeading(rightmatchload.theta, 1000, {.maxSpeed = 50}); // turn to matchload tubes
            chassis->waitUntilDone();
            kp::matchload::toggle(); // bring down matchload mech
            pros::delay(200);
            chassis->moveToPoint(rightmatchload.x, rightmatchload.y, 1500, {.minSpeed = 50}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(500);

        // score in right long goal
            chassis->moveToPoint(rightlong_goal.x, rightlong_goal.y,1000, {.forwards = false, .maxSpeed = 70}); // back into long goal
            chassis->waitUntilDone();
            kp::hood::togglet(); // open hood
            kp::intake::score(); // score on the long goal
            pros::delay(1100);
            kp::matchload::toggle(); // bring up matchload mech
            kp::intake::stop(); // stop intake

        // pick up right middle 3 blocks
            chassis->moveToPoint(0,rightlong_goalpos.y ,1200, {.minSpeed = 50}); // move to position again
            chassis->waitUntilDone();
            kp::hood::togglet(); // close hood
            pros::delay(100);
            kp::intake::pickup(); // start intake
            chassis->turnToHeading(225, 500, {.maxSpeed = 57}); // turn towards middle blocks
            chassis->moveToPoint(rightfirst_blocks.x, rightfirst_blocks.y, 2000, {.minSpeed = 20}); // move to block
            pros::delay(500);
            kp::matchload::toggle(); // bring down matchload mech
            pros::delay(400);

        // pick up left middle 3 blocks
            kp::matchload::toggle(); // bring up matchload mech
            chassis->turnToHeading(180, 500, {.maxSpeed = 57}); // turn towards left middle blocks
            chassis->waitUntilDone();
            chassis->moveToPoint(leftfirst_blocks.x, leftfirst_blocks.y, 1200, {.minSpeed = 50}); // move to block
            pros::delay(900);
            kp::matchload::toggle(); // bring down matchload mech

        // score in Center High Goal
            chassis->turnToHeading(center_high.theta,700, {.maxSpeed = 40}); // turn to center high goal
            chassis->waitUntilDone();
            chassis->moveToPoint(center_high.x, center_high.y,900, {.forwards = false,.maxSpeed = 50,}); // move to center high goal
            chassis->waitUntilDone();
            kp::intake::mid(); // score on the center high goal
            pros::delay(1000);
            kp::intake::stop();

/*
        // pick up left side matchload
            chassis->moveToPoint(leftlong_goalpos.x, leftlong_goalpos.y, 1300, {.minSpeed = 40}); // move to position
            chassis->waitUntilDone();
            chassis->turnToHeading(leftmatchload.theta, 500, {.minSpeed = 50}); // turn to matchload tubes
            chassis->waitUntilDone(); 
                                /*
            chassis->moveToPoint(leftmatchload.x, leftmatchload.y, 1000, {.maxSpeed = 70}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(450); 
    
        // score in left long goal
            chassis->moveToPoint(leftlong_goal.x, leftlong_goal.y,1000, {.forwards = false, .maxSpeed = 60}); // back into long goal
            chassis->waitUntilDone();
            kp::intake::score(); // score on the long goal
            chassis->waitUntilDone();
            pros::delay(3000); */
    }
    void alt_SAWP() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0, 0);
        lemlib::Pose first_blocks = lemlib::Pose(4.06, 17.41,17.24);
        lemlib::Pose other_blocks = lemlib::Pose(4.44, 18.31, 26.94);
        lemlib::Pose long_goalpos = lemlib::Pose(35.5, 2.51, 180);
        lemlib::Pose long_goal = lemlib::Pose(35.5, 27.51, 180);
        lemlib::Pose matchload = lemlib::Pose(35, -10.2, 180);
        lemlib::Pose long_goalpos1 = lemlib::Pose(35, 18, 180);
        lemlib::Pose wingpos = lemlib::Pose(48, 10, 180);
        lemlib::Pose wing_control= lemlib::Pose(50, 38, 170);

        // pick up middle 3 blocks
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(first_blocks.x, first_blocks.y, 1200, {.minSpeed = 20}); // move to block
            pros::delay(600);
            kp::matchload::toggle(); // bring down matchload mec
            chassis->waitUntilDone();
            chassis->moveToPoint(other_blocks.x, other_blocks.y, 300,{.minSpeed = 20}); // move to other blocks
            chassis->waitUntilDone();
            pros::delay(200);

        // move to long goal pos
            chassis->moveToPoint(long_goalpos.x, long_goalpos.y,2000, {.maxSpeed = 80}); // move to long goal position
            chassis->waitUntilDone();
            pros::delay(100);
            chassis->turnToHeading(180,500, {.maxSpeed = 70}); // face towards long goal
            chassis->waitUntilDone();

        // score in long goal
            chassis->moveToPoint(long_goal.x, long_goal.y,1000, {.forwards = false, .maxSpeed = 50}); // back into long goal again
            chassis->waitUntilDone();
            pros::delay(200);
            kp::intake::score(); // score on the long goal 
            chassis->waitUntilDone();
            pros::delay(2000); 
            kp::intake::pickup();
            pros::delay(200);
            kp::matchload::toggle();
/*
        // pick up matchload
            pros::delay(200);
            chassis->moveToPoint(matchload.x, matchload.y, 1500, {.maxSpeed = 60}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(700);
/*
        // move to wing position
            chassis->moveToPoint(long_goalpos1.x, long_goalpos1.y,700, {.maxSpeed = 60}); // move to long goal position
            chassis->waitUntilDone();
            chassis->moveToPoint(wingpos.x,wingpos.y,1500, {.maxSpeed=70});
            chassis->waitUntilDone();
            chassis->turnToHeading(wingpos.theta,500, {.minSpeed = 40});
            chassis->waitUntilDone();

        // get long goal control
            chassis->moveToPoint(wing_control.x,wing_control.y,800, {.forwards = false, .minSpeed = 50});
            chassis->waitUntilDone();  
            chassis->turnToHeading(150,  250, {.minSpeed = 80});
            chassis->waitUntilDone(); */
    }    

    void Half_AWP() {
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        // define positions
        lemlib::Pose start_pose = lemlib::Pose(0, 0,0);
        lemlib::Pose rightmatchload = lemlib::Pose(18, 32, 90);
        lemlib::Pose rightlong_goalpos = lemlib::Pose(0, 32, 90);
        lemlib::Pose rightlong_goal = lemlib::Pose(-19, 32, 90);
        lemlib::Pose rightfirst_blocks = lemlib::Pose(-17, 10, 227);
        lemlib::Pose leftfirst_blocks = lemlib::Pose(-20, -34, 180);    
        lemlib::Pose center_high = lemlib::Pose(-33, -25, 135);
        lemlib::Pose leftlong_goalpos = lemlib::Pose(0, -57, 90);
        lemlib::Pose leftlong_goal = lemlib::Pose(-19, -57, 90);
        lemlib::Pose leftmatchload = lemlib::Pose(0, 0, 90);

        // pick up right side matchload
            kp::intake::pickup(); // start intake
            chassis->moveToPoint(rightlong_goalpos.x, rightlong_goalpos.y,1400, {.minSpeed = 20}); // move to position 
            chassis->waitUntilDone();
            chassis->turnToHeading(rightmatchload.theta, 1000, {.maxSpeed = 50}); // turn to matchload tubes
            chassis->waitUntilDone();
            kp::matchload::toggle(); // bring down matchload mech
            pros::delay(200);
            chassis->moveToPoint(rightmatchload.x, rightmatchload.y, 1500, {.minSpeed = 50}); // move to matchload tubes
            chassis->waitUntilDone();
            pros::delay(500);

        // score in right long goal
            chassis->moveToPoint(rightlong_goal.x, rightlong_goal.y,1000, {.forwards = false, .maxSpeed = 70}); // back into long goal
            chassis->waitUntilDone();
            kp::hood::togglet(); // open hood
            kp::intake::score(); // score on the long goal
            pros::delay(1100);
            kp::matchload::toggle(); // bring up matchload mech
            kp::intake::stop(); // stop intake

        // pick up right middle 3 blocks
            chassis->moveToPoint(0,rightlong_goalpos.y ,1200, {.minSpeed = 50}); // move to position again
            chassis->waitUntilDone();
            kp::hood::togglet(); // close hood
            pros::delay(100);
            kp::intake::pickup(); // start intake
            chassis->turnToHeading(225, 500, {.maxSpeed = 57}); // turn towards middle blocks
            chassis->moveToPoint(rightfirst_blocks.x, rightfirst_blocks.y, 2000, {.minSpeed = 20}); // move to block
            pros::delay(500);
            kp::matchload::toggle(); // bring down matchload mech
            pros::delay(400);

        // pick up left middle 3 blocks
            kp::matchload::toggle(); // bring up matchload mech
            chassis->turnToHeading(180, 500, {.maxSpeed = 57}); // turn towards left middle blocks
            chassis->waitUntilDone();
            chassis->moveToPoint(leftfirst_blocks.x, leftfirst_blocks.y, 1200, {.minSpeed = 50}); // move to block
            pros::delay(900);
            kp::matchload::toggle(); // bring down matchload mech

        // score in Center High Goal
            chassis->turnToHeading(center_high.theta,700, {.maxSpeed = 40}); // turn to center high goal
            chassis->waitUntilDone();
            chassis->moveToPoint(center_high.x, center_high.y,900, {.forwards = false,.maxSpeed = 50,}); // move to center high goal
            chassis->waitUntilDone();
            kp::intake::mid(); // score on the center high goal
            pros::delay(500);
    }

    void drive(){
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        chassis->moveToPoint( 0,7, 2000, {.maxSpeed = 10});
    }
}