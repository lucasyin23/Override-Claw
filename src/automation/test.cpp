#include "kp/vision.hpp"
#include "main.h"
#include "pros/rtos.hpp"

namespace automation {
    void test() {
    }
    
    // kp::vision::drive_straight_toward_goal(2000, true);
    void drive(){
        lemlib::Chassis *chassis = kp::chassis::get_chassis();
        chassis->setPose(0, 0, 0);

        chassis->moveToPoint( 0,7, 2000, {.maxSpeed = 10});
    }
}