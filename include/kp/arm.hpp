#pragma once
#include "api.h"

namespace kp::arm {
    void initialize();
    void set_power(int input);
    void brake();
    void tare_position();
    void home_to_hardstop(int timeout_ms = 2500);
    void move_to_target(double target, int speed);
    void update();
}