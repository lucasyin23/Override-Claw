#ifndef _KP_CONTROLLER_H_
#define _KP_CONTROLLER_H_

namespace kp::controller {
  bool lock();
  void release();
  void delay();
  void short_delay();
  pros::Controller get_master();
  void initialize();
  void tank();
  void curvature();
  bool is_ready();
  void ready();
  void rumble(std::string pattern);
}

#define CTRL_PRESS(button) kp::controller::get_master().get_digital(DIGITAL_##button)
#define CTRL_PRESS_ONCE(button) kp::controller::get_master().get_digital_new_press(DIGITAL_##button)
#define CTRL_VIBRATE(pattern) kp::controller::rumble(pattern);

#endif