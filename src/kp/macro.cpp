#include "main.h"

namespace kp::macro {
    static bool is_running = false;
    static bool macro_requested = false;

    bool is_active() {
        return is_running;
    }

    void score_action() {
        is_running = true;

        // keep intake spinning
        kp::armtake::intake();
        kp::lift::bump_up(300.0, 100);
        pros::delay(300);
        kp::arm::move_to_target(900.0, 90);
        pros::delay(1000);

        is_running = false;
    }

    static void macro_task_fn(void* param) {
        while (true) {
            if (macro_requested) {
                macro_requested = false;
                score_action();
            }
            pros::delay(20);
        }
    }

    // background worker task
    static pros::Task macro_worker_task(macro_task_fn, nullptr, "MacroWorker");

    void score() {
        macro_requested = true;
    }

    void stop() {
        macro_requested = false;
        is_running = false;
    }
}