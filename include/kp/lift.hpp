#pragma once
#include "api.h"

namespace kp::lift {
    void initialize();
    void set_power(int input);
    void brake();
    void home();
    void bump_up();
    bool is_homing();
    bool is_bottom();
    void tare_position();    
    double get_position();   
}