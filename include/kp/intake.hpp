#pragma once
#include "pros/motors.hpp"

namespace kp::intake {
    extern pros::Motor lower_motor;
    extern pros::Motor middle_motor;
    extern pros::Motor scorer;

    void initialize();
    void outtake();
    void feed_top();
    void score();
    void pickup();
    void only();
    void stop();
}
