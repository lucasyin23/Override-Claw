#include "main.h" 
#include <cmath> 
namespace kp::intake {
        pros::Motor lower_motor(5);
        pros::Motor middle_motor(2);
        pros::Motor scorer(1);
    void initialize() {
        lower_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        middle_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        scorer.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

        lower_motor.set_current_limit(2500);
        middle_motor.set_current_limit(2500);
        scorer.set_current_limit(2500);
    }

    void outtake() {
        scorer.set_current_limit(2500);
        lower_motor.move(127);
        middle_motor.move(127);
        scorer.move(127);
    }

    void mid() {
        scorer.set_current_limit(2500);
        lower_motor.move(-127);
        middle_motor.move(-127);
        scorer.move(127);
    }

    void slowmid() {
        scorer.set_current_limit(2500);
        lower_motor.move(-127);
        middle_motor.move(-127);
        scorer.move(70);
    }

    void score() {
        scorer.set_current_limit(2500);
        lower_motor.move(-127);
        middle_motor.move(-127);
        scorer.move(-127);
    }

    void pickup() {
        scorer.set_current_limit(750);
        lower_motor.move(-127);
        middle_motor.move(-127);
        scorer.move(-127);
    }

    void stop() {
        lower_motor.move(0);
        middle_motor.move(0);
        scorer.move(0);
    }

    void slowout() {
        scorer.set_current_limit(2500);
        lower_motor.move(100);
        middle_motor.move(110);
        scorer.move(127);
    }
}
