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
    }

    void outtake() {
        lower_motor.move(127);
        middle_motor.move(127);
        scorer.move(127);
    }

    void mid() {
        lower_motor.move(-127);
        middle_motor.move(-127);
        scorer.move(127);
    }

    void slowmid() {
        lower_motor.move(-127);
        middle_motor.move(-115);
        scorer.move(70);
    }

    void score() {
        lower_motor.move(-127);
        middle_motor.move(-127);
        scorer.move(-127);
    }

    void pickup() {
        lower_motor.move(-127);
        middle_motor.move(-127);
        scorer.move(-50);
    }

    void stop() {
        lower_motor.move(0);
        middle_motor.move(0);
        scorer.move(0);
    }

    void slowout() {
        lower_motor.move(-100);
        middle_motor.move(-110);
        scorer.move(127);
    }
}
