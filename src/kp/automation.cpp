#include "main.h"
#include "automation.hpp"

namespace kp::automation {
  namespace {
    int index = 0;
    int size = 0;
    const char * titles[10] = {};
    void (*automations[10])() = {};

    void _next() {
      index++;
    }


    void selection_fn(void* param) {
      kp::controller::short_delay();

      LCD_MAIN_CLEAR
      LCD_1("AUTOMATION TESTER")
      LCD_3("R1:NEXT | R2:TEST | Y:RESET")
      LCD_4("X:EXIT")

      
      if (kp::controller::lock() == true) {
        while(true) {
          LCD_2("= %s =", titles[index % size])
          
          if (CTRL_PRESS_ONCE(R1)) {
            next();
          }

          if (CTRL_PRESS_ONCE(B)) {
            kp::chassis::reset();
          }

          if (CTRL_PRESS_ONCE(R2)) {
            CTRL_VIBRATE(".")
            uint32_t start = pros::millis();
            run();
            LCD_6("RUNTIME: %f", (pros::millis() - start) / 1000.0);
            CTRL_VIBRATE("-")
          }

          if (CTRL_PRESS_ONCE(X)) {
            LCD_CLEAR
            break;
          }

          kp::chassis::refresh_xy();
          kp::controller::delay();
        }

        kp::controller::release();
      }
    }
  }

  void run() {
    void (*f)() = automations[index % size];
    (*f)();
  }

  void set(const char *title, void (*f)()) {
    index++;
    titles[index - 1] = title;
    automations[index - 1] = f;
    size = index;
  }

  void clear() {
    index = 0;
    size = 0;
  }

  void next() {
    _next();
  }

  void launch_selection() {
    pros::Task automation_selection(selection_fn);
  }

  const char* title() {
    return titles[index % size];
  }
}

