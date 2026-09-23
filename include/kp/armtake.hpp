#pragma once

namespace kp::armtake {
    void initialize();
    void set_power(int input);
    void intake();   // Max voltage intake
    void outtake();  // Max voltage outtake
    void stop();     // Coast to stop
    void toggle_intake();
    void toggle_outtake();
    int get_state(); // 1 = Intaking, -1 = Outtaking, 0 = Stopped
}