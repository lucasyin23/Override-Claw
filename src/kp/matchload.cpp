#include "main.h"

namespace kp::matchload {
    namespace {
        bool extended = false;

        // one wire for extend and one wire for retract
        pros::adi::Pneumatics extendSolenoid('c', false, false);
        pros::adi::Pneumatics retractSolenoid('d', false, false);

        void setPiston(bool extend) {
            if (extend) {
                extendSolenoid.extend();
                retractSolenoid.retract();
            } else {
                extendSolenoid.retract();
                retractSolenoid.extend();
            }
        }

        void task_fn() {
            pros::Task this_task = pros::Task::current();

            while (true) {
                if (this_task.notify_take(true, TIMEOUT_MAX)) {

                    // toggle piston state
                    extended = !extended;

                    // apply correct valve configuration
                    setPiston(extended);
                }

                pros::delay(10);
            }
        }

        pros::Task task(task_fn, "MATCHLOAD_PISTON_TASK");
    }

    void toggle() {
        task.notify();
    }
}
