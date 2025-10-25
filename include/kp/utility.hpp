#ifndef _KP_UTILITY_H_
#define _KP_UTILITY_H_

#define LCD_CLEAR for (int i = 0; i <= 7; i++) { pros::lcd::clear_line(i); }
#define LCD_MAIN_CLEAR for (int i = 0; i <= 5; i++) { pros::lcd::clear_line(i); }
#define LCD_1(...) pros::lcd::print(0, __VA_ARGS__);
#define LCD_2(...) pros::lcd::print(1, __VA_ARGS__);
#define LCD_3(...) pros::lcd::print(2, __VA_ARGS__);
#define LCD_4(...) pros::lcd::print(3, __VA_ARGS__);
#define LCD_5(...) pros::lcd::print(4, __VA_ARGS__);
#define LCD_6(...) pros::lcd::print(5, __VA_ARGS__);
#define LCD_7(...) pros::lcd::print(6, __VA_ARGS__);
#define LCD_8(...) pros::lcd::print(7, __VA_ARGS__);
#define LCD_HORIZONTAL_LINE "======================================"

#endif