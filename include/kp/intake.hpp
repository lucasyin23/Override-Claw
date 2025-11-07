#pragma once
#include "main.h"

namespace kp::intake {
    void initialize();

    void outtake();           // L2: reverse all
    void feed_top();          // R2: lower+middle forward, scorer reverse
    void score();             // R1: all forward
    void pickup();            // L1: lower+middle only
    void stop();              // stop all
}
