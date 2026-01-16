#include "main.h" 
#include <cmath> 
namespace kp::intake {
        pros::Motor lower_motor(3);
        pros::Motor scorer(2);

    void initialize() {
        lower_motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        scorer.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    }

    void outtake() {
        lower_motor.move(-127);
        scorer.move(-127);
    }

    void feed_top() {
        lower_motor.move(127);
        scorer.move(50);
    }

    void score() {
        lower_motor.move(127);
        scorer.move(127);
    }

    void pickup() {
        lower_motor.move(127);
        scorer.move(0);
    }

    void stop() {
        lower_motor.move(0);
        scorer.move(0);
    }

    void slow () {
        lower_motor.move(80);
        scorer.move(90);
    }
}
