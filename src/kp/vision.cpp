#include "main.h"

namespace kp::vision {

    //Tuner to change values
    static double vision_kP = 0.5; 
    static double vision_kI = 0.0;
    static double vision_kD = 0.5;
    pros::Vision aivis(8, pros::E_VISION_ZERO_TOPLEFT);
    pros::vision_signature_s_t yellow = pros::Vision::signature_from_utility(
        1, 
        4249, 
        4989, 
        4620, 
        -4859, 
        -4273, 
        -4566, 
        3.000, 
        0
    );

    //PID Helper Class
    class PIDclass {
    public:
        double kP, kI, kD;
        double error, sum, prev_error, deriv;

        PIDclass(double init_kP, double init_kI, double init_kD) {
            kP = init_kP; kI = init_kI; kD = init_kD;
            error = 0; sum = 0; prev_error = 0; deriv = 0;
        }

        double adjust(double setpoint, double sensor_value) {
            error = setpoint - sensor_value;
            if (kI != 0) sum += error;
            deriv = error - prev_error;
            prev_error = error;
            return (error * kP) + (sum * kI) + (deriv * kD);
        }
    };

    //Vision Sensor Logic
    void drive_straight_toward_goal(int duration_msec, bool target_small_goal) {
        // Stop any background chassis tasks
        if (kp::chassis::get_chassis() != nullptr) {
            kp::chassis::get_chassis()->cancelMotion();
        }

       //Configuration (Uses the Live Tuner Values)
        double KP = vision_kP;
        double KI = vision_kI;
        double KD = vision_kD;

        PIDclass dir(KP, KI, KD);

        float final_max_rpm = target_small_goal ? -200.0: -200.0; // Negative implies backward driving
        float accel_base    = target_small_goal ? 13.0 : 15.0;

        float current_vel = 0; 
        int last_known_x = 155;
        int lost_frames = 100; 
        int goal_x = 155;
        int factor = 0;
        bool everseen = false;
        float dir_adj = 0;

        long start = pros::millis();
        
        while (pros::millis() - start < duration_msec) {
            
            // Get object from Vision Sensor (Signature 1 = yellow)
            pros::vision_object_s_t obj = aivis.get_by_sig(0, 1);

            // Check if object exists and isn't just noise (width > 5)
            bool has_target = (obj.signature != 0 && obj.width > 5);

            if (has_target) {
                goal_x = obj.x_middle_coord;
                last_known_x = goal_x; 
                lost_frames = 0;
                factor = 1;
                everseen = true;
            } else {
                // Buffer: Keep turning briefly if frame is dropped
                if (lost_frames < 10 && everseen) {
                    goal_x = last_known_x;
                    lost_frames++;
                    factor = 1; 
                } else {
                    goal_x = 155; 
                    factor = 0;
                }
            }

            //Speed Control
            float error = std::abs(155 - goal_x);
            if (factor == 0) error = 350.0; 
            
            float speed_factor = 1.0 - (error / 350.0); 
            if (speed_factor < 0) speed_factor = 0;
            if (speed_factor > 1.0) speed_factor = 1.0;

            float active_speed_limit = final_max_rpm * speed_factor;
            if (factor == 0) active_speed_limit = 0; 

            // Acceleration Ramp
            if (current_vel > active_speed_limit) current_vel -= accel_base; 
            else if (current_vel < active_speed_limit) current_vel += accel_base; 

            //PID Steering
            if (error > 0.5 && factor == 1) {
                dir_adj = dir.adjust(155, goal_x);
            } else {
                dir_adj = 0;
            }

            //Apply to Motors
            if(factor == 1) {
                // If robot turns WRONG way, swap these (+/-)
                kp::chassis::set_tank_velocity(current_vel - dir_adj, current_vel + dir_adj);
            } else {
                kp::chassis::set_tank_velocity(0, 0);
            }

            pros::delay(20);
        }

        // Full stop at the end
        kp::chassis::set_tank_velocity(0, 0);
    }

    //Tuner Task Logic
    void tuner_fn(void* param) {
      kp::controller::short_delay();

      LCD_MAIN_CLEAR
      LCD_1("VISION PID TUNER")
      LCD_2("UP:+kD | DN:-kD | RT:+kP | LF:-kP")
      LCD_3("R2:Test (2s) | X:Exit")

      float kP_step = 0.05;
      float kD_step = 0.5;

      // Lock controller to prevent driving while tuning
      if (kp::controller::lock() == true) {
        while(true) {
          LCD_5("kP: %.2f kD: %.2f", vision_kP, vision_kD);

          if (CTRL_PRESS_ONCE(UP)) {
            vision_kD += kD_step;
          }
          if (CTRL_PRESS_ONCE(DOWN)) {
            vision_kD -= kD_step;
          }
          if (CTRL_PRESS_ONCE(LEFT)) {
            vision_kP -= kP_step;
          }
          if (CTRL_PRESS_ONCE(RIGHT)) {
            vision_kP += kP_step;
          }

          // Runs vision logic for 2 seconds
          if (CTRL_PRESS_ONCE(R2)) {
            CTRL_VIBRATE(".");
            drive_straight_toward_goal(2000, true);
            CTRL_VIBRATE("-");
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

    //Launcher
    void launch_vision_tuner() {
        pros::Task vision_tuner_task(tuner_fn);
    }
}


