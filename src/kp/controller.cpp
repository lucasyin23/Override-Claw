#include "main.h"

namespace kp::controller {
  namespace {
    lemlib::Chassis* chassis;
    bool _is_ready = false;
    pros::Mutex controllerMutex = pros::Mutex();
    pros::Mutex controllerLCDMutex = pros::Mutex();
    pros::Controller master(pros::E_CONTROLLER_MASTER);
  }

  void initialize() {
    pros::Task screen_task([&]() {
      master.clear();
      pros::delay(50);

      while (true) {
        if (_is_ready == true) {
          master.clear();
          pros::delay(50);
          master.print(0, 0, "%s", kp::automation::title());
          pros::delay(50);
          break;
        }

        pros::delay(50);
        if (controllerLCDMutex.take(100)) {
          lemlib::Pose p = kp::chassis::current_pose();

          master.print(0, 0, "X %+06.1f %02d%%", p.x, pros::battery::get_capacity());
          pros::delay(50);
          master.print(1, 0, "Y %+06.1f", p.y);
          pros::delay(50);
          master.print(2, 0, "T %+06.1f %s", p.theta, kp::automation::title());
          pros::delay(50);
          controllerLCDMutex.give();
        }
      }
    });
  }

  void rumble(std::string pattern) {
    if (controllerLCDMutex.take(500)) {
      fmt::println("CONTROLLER RUMBLE PATTERN {}", pattern);
      kp::controller::get_master().rumble(pattern.c_str());
      pros::delay(pattern.length() * 500);
      controllerLCDMutex.give();
    }
  }

  bool lock() {
    return controllerMutex.take();
  }

  void release() {
    controllerMutex.give();
  }

  void delay() {
    pros::delay(25);
  }

  void short_delay() {
    pros::delay(10);
  }

  pros::Controller get_master() {
    return master;
  }

  void tank() {
    int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    int rightY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    chassis -> tank(leftY, rightY);
  }

  void curvature() {
    int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    chassis -> curvature(leftY, rightX);
  }

  bool is_ready() {
    return _is_ready;
  }

  void ready() {
    _is_ready = true;
  }
}