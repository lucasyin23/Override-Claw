#pragma once
#include "api.h"

namespace kp::lift {
    constexpr double BUMP_DEG = 120.0;

    void initialize();
    void set_power(int input);
    void brake();
    void home();
    void bump_up();
    int bump_count();        // bumps since power-on; the path recorder reads this
    bool bump_active();
    bool is_homing();
    bool is_bottom();
    void tare_position();
    double get_position();

    // Autonomous: start moving to a height (does not block), then optionally wait.
    void go_to(double degrees);
    void wait_until_there(int timeout_ms);

    void log_status();       // one line per call to `pros terminal`
}
