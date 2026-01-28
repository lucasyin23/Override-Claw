#include "main.h"

namespace kp::hood {
    
    bool open = false;

    pros::adi::Pneumatics openSolenoid('a', false, false);
    // pros::adi::Pneumatics closeSolenoid('h', false, false);

    void setHood(bool state) {
        if (state) {
            // OPEN
            openSolenoid.extend();
            // closeSolenoid.retract();
        } else {
            // CLOSE
            openSolenoid.retract();
            // closeSolenoid.extend();
        }
    }

    void togglet() {
        open = !open;
        setHood(open);
    }
}
