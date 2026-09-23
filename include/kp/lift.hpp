#pragma once

namespace kp::lift {
    void initialize();
    void set_power(int input);
    void brake();
    bool is_bottom();
    void home(int timeout_ms = 3000);
}