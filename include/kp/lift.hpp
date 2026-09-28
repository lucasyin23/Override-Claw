#pragma once
#include "api.h"

namespace kp::lift {
    void initialize();
    bool is_bottom();
    void set_power(int input);
    void brake();
    void bump_up(double degrees, int speed);
    void home();
    bool is_homing();
}