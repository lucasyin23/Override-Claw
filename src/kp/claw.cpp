#include "main.h"

namespace kp::claw {
    namespace {
        bool extended = false;

        pros::adi::Pneumatics clawPiston('b', false);

        void task_fn() {
            pros::Task this_task = pros::Task::current();

            while (true) {
                if (this_task.notify_take(true, TIMEOUT_MAX)) {
                    // Toggle the piston state
                    extended = !extended;

                    // Push out or retract via internal spring
                    clawPiston.set_value(extended);
                }

                pros::delay(10);
            }
        }

        pros::Task task(task_fn, "CLAW_PISTON_TASK");
    }
    

    void toggle() {
        task.notify();
    }
}