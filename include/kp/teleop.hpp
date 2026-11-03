#pragma once

#define CONTROLLER_MOVEMENT

namespace kp::teleop {
    // One pass of driver control: drive, lift, claw, toggler, homing.
    // Shared by opcontrol and the path recorder, so recording drives the real controls.
    void tick();
}
