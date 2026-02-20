#include "main.h" 
#include <cmath> 
namespace kp::intake {
        pros::Motor intake_motor(5);
        pros::Motor scorer(1);
    void initialize() {
        intake_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        scorer.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

        intake_motor.set_current_limit(2500);
        scorer.set_current_limit(2500);
    }

    void outtake() {
        scorer.set_current_limit(2500);
        intake_motor.move(127);
        scorer.move(127);
    }

    void mid() {
        scorer.set_current_limit(2500);
        intake_motor.move(-127);
        scorer.move(127);
    }

    void score() {
        scorer.set_current_limit(2500);
        intake_motor.move(-127);
        scorer.move(-127);
    }

    void pickup() {
        scorer.set_current_limit(2500);
        intake_motor.move(-127);
        scorer.move(-127);
    }

    void stop() {
        lower_motor.move(0);
        scorer.move(0);
    }

    void slowout() {
        scorer.set_current_limit(2500);
        intake_motor.move(100);
        scorer.move(127);
    }
}
