#ifndef _KP_AUTOMATION_H
#define _KP_AUTOMATION_H

#define XY(distance, degree) sin(M_PI / 180.0 * degree) * distance, cos(M_PI / 180.0 * degree) * distance

namespace kp::automation {
  void set(const char *title, void (*f)());
  void run();
  void launch_selection();
  const char* title();
  void clear();
  void next();
}

#endif