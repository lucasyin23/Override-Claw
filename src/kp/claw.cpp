#include "main.h"

namespace kp::claw {
    namespace {
        bool extended = false;

        // Extended = closed, retracted (internal spring) = open
        pros::adi::Pneumatics clawPiston('b', false);
    }

    void initialize() {
        set(false);
    }

    void set(bool closed) {
        extended = closed;
        clawPiston.set_value(extended);
    }

    void toggle() {
        set(!extended);
    }

    bool is_extended() {
        return extended;
    }
}
