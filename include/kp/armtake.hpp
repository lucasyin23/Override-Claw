#pragma once
#include "api.h"

namespace kp::armtake {
    void initialize();
    void set_power(int input);
    void intake();
    void outtake();
    void stop();
    int get_state();
    void toggle_intake();
    void toggle_outtake();
}