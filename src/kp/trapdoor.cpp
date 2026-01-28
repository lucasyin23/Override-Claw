#include "main.h"

namespace kp::trapdoor {
    
    bool open = false;

    pros::adi::Pneumatics openSolenoid('g', false, false);
    pros::adi::Pneumatics closeSolenoid('h', false, false);

    void setDoor(bool state) {
        if (state) {
            // OPEN
            openSolenoid.extend();
            closeSolenoid.retract();
        } else {
            // CLOSE
            openSolenoid.retract();
            closeSolenoid.extend();
        }
    }

    void togglet() {
        open = !open;
        setDoor(open);
    }
}
