#include "main.h"

namespace kp::wing {
    namespace {
        bool extended = false;

        // One wire for extend, one wire for retract
        pros::adi::Pneumatics extendSolenoid('a', false, false);
        pros::adi::Pneumatics retractSolenoid('b', false, false);

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

                    // Toggle piston state
                    extended = !extended;

                    // Apply correct valve configuration
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
