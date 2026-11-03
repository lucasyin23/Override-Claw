#pragma once

namespace kp::claw {
    void initialize();
    void set(bool closed);
    void toggle();
    bool is_extended(); // extended = closed
}
