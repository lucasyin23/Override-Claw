#include "lemlib/api.hpp"

namespace kp::chassis {
  void initialize();
  lemlib::Chassis* get_chassis();
  lemlib::Chassis* get_setup_chassis();
  void launch_angular_adjustment();
  void launch_linear_adjustment();

  lemlib::Pose current_pose();
  void reset();
  bool is_pose_origin();
  
  void track_dual_joystick(double base, double p);

  void forward_by_time(int milliseconds, int voltage = 127);
  void forward(int voltage = 60);
  void stop();
  void backwards_by_time(int milliseconds, int voltage = 127);
  void backwards(int voltage = 60);
  void turn_left_by_time(int milliseconds, int voltage = 127);
  void turn_right_by_time(int milliseconds, int voltage = 127);
  void swing_left_by_time(int milliseconds, int voltage = 127);
  void swing_right_by_time(int milliseconds, int voltage = 127);

  double get_left_velocity();
  double get_right_velocity();
  double get_velocity();
  double get_t();
  void wait_until_greater_than(double t);
  void wait_until_less_than(double t);
  void wait_until_blocked(double velocity = 3);
  void wait_until_unblocked(double velocity = 5);
  void cancel_motion_until_by_time(int milliseconds);
  float get_vertical_traveled();
  int get_vertical_raw();

  void move_gyro(double inch);
  void refresh_xy();


}