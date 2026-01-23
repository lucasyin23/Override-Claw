#include "main.h"

namespace kp::chassis {
  namespace {
    pros::Imu imu(4);
    pros::Controller master(pros::E_CONTROLLER_MASTER);

    // motor groups
    pros::MotorGroup left_motors({11, 12, 13}, pros::MotorGearset::blue); 
    pros::MotorGroup right_motors({14, 15, 16}, pros::MotorGearset::blue); 

    // tracking wheels
    /* pros::Rotation horizontal_encoder(-9);
    pros::Rotation vertical_encoder(-10);

    lemlib::TrackingWheel horizontal(&horizontal_encoder, lemlib::Omniwheel::NEW_275, -3.25);
    lemlib::TrackingWheel vertical(&vertical_encoder, lemlib::Omniwheel::NEW_275, 0); */

    // drivetrain settings
    lemlib::Drivetrain drivetrain(&left_motors, // left motor group
                                  &right_motors, // right motor group
                                  10.75, // track width
                                  lemlib::Omniwheel::NEW_325,
                                  450, // drivetrain rpm on the wheels
                                  8 // horizontal drift is 2. If we had traction wheels, it would have been 8
    );

    // angular motion controller
    lemlib::ControllerSettings angular_controller(3.2, // proportional gain (kP)
                                                0, // integral gain (kI)
                                                20, // derivative gain (kD)
                                                0, // anti windup
                                                0.5, // small error ··range, in degrees
                                                100, // small error range timeout, in milliseconds
                                                1, // large error range, in degrees
                                                500, // large error range timeout, in milliseconds
                                                0 // maximum acceleration (slew)
    );

    // lateral motion controller
    lemlib::ControllerSettings linear_controller(8, // proportional gain (kP)
                                                0, // integral gain (kI)
                                                60, // derivative gain (kD)
                                                3, // anti windup
                                                1, // small error range, in inches
                                                100, // small error range timeout, in milliseconds
                                                2, // large error range, in inches
                                                500, // large error range timeout, in milliseconds
                                                0 // maximum acceleration (slew)
    );

    // sensors for odometry
    lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel dont have
                                nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                                nullptr, // horizontal tracking wheel dont have
                                nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                                &imu // inertial sensor
    );

    // input curve for throttle input during driver control
    lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                        10, // minimum output where drivetrain will move out of 127
                                        1.019 // expo curve gain
    );

    // input curve for steer input during driver control
    lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                      10, // minimum output where drivetrain will move out of 127
                                      1.019 // expo curve gain
    );

      lemlib::Chassis* chassis;

    void angular_adjustment_fn(void* param) {
      kp::controller::short_delay();

      LCD_MAIN_CLEAR
      LCD_1("ANGULAR PID ADJUSTMENT")
      LCD_2("UP:+kD | DN:-kD | RT:+kP | LF:-kP")
      LCD_3("R2:Test | X:Exit")

      // angular motion controller
      lemlib::ControllerSettings ac(angular_controller.kP, // proportional gain (kP)
                                    0, // integral gain (kI)
                                    angular_controller.kD, // derivative gain (kD)
                                    0, // anti windup
                                    0, // small error ··range, in degrees
                                    0, // small error range timeout, in milliseconds
                                    0, // large error range, in degrees
                                    0, // large error range timeout, in milliseconds
                                    0 // maximum acceleration (slew)
      );

      float kP_step = 0.02;
      float kD_step = 0.5;

      int adjustment_angle = 90;

      if (kp::controller::lock() == true) {
        while(true) {
          LCD_5("kP: %.2f kD: %.2f", ac.kP, ac.kD);

          if (CTRL_PRESS_ONCE(UP)) {
            ac.kD += kD_step;
          }

          if (CTRL_PRESS_ONCE(DOWN)) {
            ac.kD -= kD_step;
          }

          if (CTRL_PRESS_ONCE(LEFT)) {
            ac.kP -= kP_step;
          }

          if (CTRL_PRESS_ONCE(RIGHT)) {
            ac.kP += kP_step;
          }

          if (CTRL_PRESS_ONCE(R2)) {
            lemlib::Chassis* c = new lemlib::Chassis(drivetrain, linear_controller, ac, sensors, &throttle_curve, &steer_curve);
            
            adjustment_angle = adjustment_angle == 90 ? -90 : 90;

            c->setPose(0, 0, 0);
            c->turnToHeading(adjustment_angle, 3000, {}, false);
            CTRL_VIBRATE(".")
            delete c;
          }

          if (CTRL_PRESS_ONCE(X)) {
            LCD_MAIN_CLEAR
            break;
          }

          kp::controller::delay();
        }

        kp::controller::release();
      }
    }

    
  
    void linear_adjustment_fn(void* param) {
      kp::controller::short_delay();

      LCD_MAIN_CLEAR
      LCD_1("LINEAR PID ADJUSTMENT")
      LCD_2("UP:+kD | DN:-kD | RT:+kP | LF:-kP")
      LCD_3("R2:Test | X:Exit")

      // angular motion controller
      lemlib::ControllerSettings lc(linear_controller.kP, // proportional gain (kP)
                                    0, // integral gain (kI)
                                    linear_controller.kD, // derivative gain (kD)
                                    0, // anti windup
                                    0, // small error ··range, in degrees
                                    0, // small error range timeout, in milliseconds
                                    0, // large error range, in degrees
                                    0, // large error range timeout, in milliseconds
                                    0 // maximum acceleration (slew)
      );

      float adjustment_step = 0.5;
      int adjustment_distance = 24;

      if (kp::controller::lock() == true) {
        while(true) {
          LCD_5("kD: %.2f kP: %.2f", lc.kD, lc.kP);

          if (CTRL_PRESS_ONCE(UP)) {
            lc.kD += adjustment_step;
          }

          if (CTRL_PRESS_ONCE(DOWN)) {
            lc.kD -= adjustment_step;
          }

          if (CTRL_PRESS_ONCE(LEFT)) {
            lc.kP -= adjustment_step;
          }

          if (CTRL_PRESS_ONCE(RIGHT)) {
            lc.kP += adjustment_step;
          }

          if (CTRL_PRESS_ONCE(R2)) {
            lemlib::Chassis* c = new lemlib::Chassis(drivetrain, lc, angular_controller, sensors, &throttle_curve, &steer_curve);
            
            if (adjustment_distance > 0) {
              c -> setPose(0, 0, 0);
              c -> moveToPose(0, adjustment_distance, 0, 3000, {}, false);
              adjustment_distance = 0 - adjustment_distance;
            } else {
              c -> setPose(0, 0, 0);
              c -> moveToPose(0, adjustment_distance, 0, 3000, {.forwards = false}, false);
              adjustment_distance = abs(adjustment_distance);
            }

            CTRL_VIBRATE(".")
            delete c;
          }

          if (CTRL_PRESS_ONCE(X)) {
            LCD_MAIN_CLEAR
            break;
          }

          kp::controller::delay();
        }

        kp::controller::release();
      }
    }

    double get_position() {
      return 0.0;
    }

    void _move_gyro(double enc) {
      double previous_error, integral, error, derivative, power;
      double power_initial = 0;
      double timer = 0;

      int dir = enc > 0 ? 1 : -1;
      double position_initial = get_position();
      double kp = 1.5, ki = 0.004, kd = 2.2; // 0.65,4.2
      double kp_gyro = 1.8, kd_gyro = 2;
      int time_out = fabs(enc) < 500 ? 700 : 2 * fabs(enc);

      while (true) {
        timer += 15;
        power_initial += dir * 10; // 5

        if (fabs(power_initial) > 127) {
          power_initial = dir * 127;
        }
        
        if (dir > 0) {
          error = enc - get_position();
        } else {
          error = enc - get_position();
          position_initial = get_position();
        }
        if (fabs(error) < 50){
          integral += error;
        } else {
          integral = 0;
        }
        if ((error / fabs(error)) != (previous_error / fabs(previous_error))) {
          integral = 0;
        }
        
        derivative = error - previous_error;
        power = kp * error + ki * integral + kd * derivative;
        previous_error = error;

        // if (fabs(error) > 350) {
        //   integral = 0;
        // } else if (fabs(error) < 1) {
        //   integral = 0;
        // }

        if (dir > 0 && (fabs(power) > 127)) {
          power = 127;
        } else if (dir < 0 && (fabs(power) > 127)) {
          power = -127;
        } else {
          power = power;
        }

        left_motors.move(power);
        right_motors.move(power);
        
        if (dir > 0) {
          if (error < 3) {
            break;
          }
        } else if (error > -3) {
          break;
        } if (timer > time_out) {
          break;
        }

        pros::delay(15);
      }

      left_motors.brake();
      right_motors.brake();
    }
  }

  void initialize() {
    chassis = new lemlib::Chassis(drivetrain, linear_controller, angular_controller, sensors, &throttle_curve, &steer_curve);
    chassis -> calibrate();
  }

  void refresh_xy() {
    LCD_7(LCD_HORIZONTAL_LINE);
    LCD_8("X: %7.2f Y: %7.2f T: %7.2f", chassis -> getPose().x, chassis -> getPose().y, chassis -> getPose().theta);
  }

  lemlib::Chassis* get_chassis() {
    return chassis;
  }

  lemlib::Chassis* get_setup_chassis() {
    lemlib::ControllerSettings lc(45, // proportional gain (kP)
                                  0, // integral gain (kI)
                                  150, // derivative gain (kD)
                                  0, // anti windup
                                  0, // small error ··range, in degrees
                                  0, // small error range timeout, in milliseconds
                                  0, // large error range, in degrees
                                  0, // large error range timeout, in milliseconds
                                  0 // maximum acceleration (slew)
    );

    lemlib::ControllerSettings ac(6, // proportional gain (kP)
                                  0, // integral gain (kI)
                                  16, // derivative gain (kD)
                                  0, // anti windup
                                  0, // small error ··range, in degrees
                                  0, // small error range timeout, in milliseconds
                                  0, // large error range, in degrees
                                  0, // large error range timeout, in milliseconds
                                  0 // maximum acceleration (slew)
    );

    lemlib::Chassis* c = new lemlib::Chassis(drivetrain, lc, ac, sensors, &throttle_curve, &steer_curve);
    return c;
  }

  void reset() {
    chassis -> setPose(0, 0, 0, true);
  }

  lemlib::Pose current_pose() {
    return chassis -> getPose();
  }

  bool is_pose_origin() {
    lemlib::Pose p = chassis -> getPose();

    // fmt::println("{} {} {}", p.x, p.y, p.theta);

    if ((p.x > -0.01 && p.x < 0.01) && 
        (p.y > -0.01 && p.y < 0.01) &&
        (p.theta > -0.01 && p.theta < 0.01)) {
      return true;
    }

    return false;
  }

  void launch_angular_adjustment() {
    pros::Task angular_adjustment(angular_adjustment_fn);
  }

  void launch_linear_adjustment() {
    pros::Task linear_adjustment(linear_adjustment_fn);
  }

  void track_dual_joystick(double base, double p) {
    int32_t straight_vector = master.get_analog(ANALOG_LEFT_Y);
    int32_t turn_vector = master.get_analog(ANALOG_RIGHT_X);

    if (abs(straight_vector) > 5) {
      left_motors.move(base * straight_vector + base * p * turn_vector);
      right_motors.move(base * straight_vector - base * p * turn_vector);
    } else {
      left_motors.move(base * straight_vector + base * p * 0.5 * turn_vector);
      right_motors.move(base * straight_vector - base * p * 0.5 * turn_vector);
    }
  }

  void move_by_time(int milliseconds, int voltage) {
    if (milliseconds == 0){
      left_motors.move(voltage);
      right_motors.move(voltage);
    } else {
      left_motors.move(voltage);
      right_motors.move(voltage);
      pros::delay(milliseconds);
      left_motors.brake();
      right_motors.brake();
    }
  }

  void turn_by_time(int milliseconds, int voltage) {
    if (milliseconds == 0){
      left_motors.move(voltage);
      right_motors.move(0 - voltage);
    } else {
      left_motors.move(voltage);
      right_motors.move(0 - voltage);
      pros::delay(milliseconds);
      left_motors.brake();
      right_motors.brake();
    }
  }

  void swing_by_time(int milliseconds, int voltage, lemlib::DriveSide side) {
    if (milliseconds == 0){
      if (side == lemlib::DriveSide::LEFT) {
        left_motors.move(voltage);
      } else {
        right_motors.move(voltage);
      }
    } else {
      if (side == lemlib::DriveSide::LEFT) {
        left_motors.move(voltage);
      } else {
        right_motors.move(voltage);
      }
      pros::delay(milliseconds);
      left_motors.brake();
      right_motors.brake();
    }
  }

  void turn_left_by_time(int milliseconds, int voltage) {
    turn_by_time(milliseconds, 0 - abs(voltage));
  }

  void turn_right_by_time(int milliseconds, int voltage) {
    turn_by_time(milliseconds, abs(voltage));
  }

  void swing_left_by_time(int milliseconds, int voltage) {
    swing_by_time(milliseconds, voltage, lemlib::DriveSide::LEFT);
  }

  void swing_right_by_time(int milliseconds, int voltage) {
    swing_by_time(milliseconds, voltage, lemlib::DriveSide::RIGHT);
  }


  void forward_by_time(int milliseconds, int voltage) {
    move_by_time(milliseconds, abs(voltage));
  }

  void forward(int voltage) {
    move_by_time(0, abs(voltage));
  }

  void stop() {
    move_by_time(0, 0);
  }

  void backwards_by_time(int milliseconds, int voltage) {
    move_by_time(milliseconds, 0 - abs(voltage));
  }

  void backwards(int voltage) {
    move_by_time(0, 0 - abs(voltage));
  }
  void move_gyro(double inch) {
    //     lemlib::Omniwheel::NEW_275 * M_PI * ((double)vertical_encoder.get_position() / 100.00 / 360.0);
    _move_gyro(get_position() + inch);
}
  double get_left_velocity() {
    return left_motors.get_actual_velocity();
  }

  double get_right_velocity() {
    return right_motors.get_actual_velocity();
  }

  double get_velocity() {
    return (get_left_velocity() + get_right_velocity()) / 2.0;
  }

  double get_t() {
    return imu.get_rotation();
  }

  void wait_until_greater_than(double t) {
    while(get_t() > t) {
      pros::delay(10);
    }
  }

  void wait_until_less_than(double t) {
    while(get_t() < t) {
      pros::delay(10);
    }
  }

  void wait_until_blocked(double velocity) {
    pros::delay(100);
    while(get_velocity() > velocity) {
      pros::delay(10);
    }
  }

  void wait_until_unblocked(double velocity) {
    pros::delay(100);
    while(get_velocity() < velocity) {
      pros::delay(10);
    }
  }

  void cancel_motion_until_by_time(int milliseconds) {
    pros::delay(milliseconds);
    chassis->cancelMotion();
  }

  float get_vertical_traveled() {
    return 0.0; // vertical.getDistanceTraveled();
  }

  int get_vertical_raw() {
    return 0.0; // vertical_encoder.get_position();
  }

  void set_tank_velocity(double left, double right) {
        left_motors.move_velocity(left);
        right_motors.move_velocity(right);
    }
}