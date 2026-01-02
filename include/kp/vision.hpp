#pragma once
#include "api.h"

namespace kp::vision {
    void drive_straight_toward_goal(int duration_msec, bool target_small_goal);
    void launch_vision_tuner();
}


