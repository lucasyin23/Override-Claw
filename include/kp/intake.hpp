#pragma once
#include "api.h"

namespace kp::intake {
    void initialize();
    void tintake();
    void touttake();
    void stop();
    void stop_intake_only();
    bool is_running();
}