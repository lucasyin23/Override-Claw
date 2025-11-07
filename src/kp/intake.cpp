#include "main.h" 
#include <cmath> 
namespace kp::intake {
        pros::Motor lower_motor(-1);
        pros::Motor middle_motor(2);
        pros::Motor scorer(3);
        pros::Controller master(pros::E_CONTROLLER_MASTER);

    void initialize() {
        lower_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        middle_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        scorer.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    }

    void outtake() {
        lower_motor.move(-127);
        middle_motor.move(-127);
        scorer.move(-127);
    }

    void feed_top() {
        lower_motor.move(127);
        middle_motor.move(127);
        scorer.move(-127);
    }

    void score() {
        lower_motor.move(127);
        middle_motor.move(127);
        scorer.move(127);
    }

    void pickup() {
        lower_motor.move(127);
        middle_motor.move(127);
        scorer.move(0);
    }

    void stop() {
        lower_motor.move(0);
        middle_motor.move(0);
        scorer.move(0);
    }
}
