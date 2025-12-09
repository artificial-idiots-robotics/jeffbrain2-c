#include "main.h"

#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

extern lv_obj_t * main_tabview;
extern lv_obj_t * drivebase_lf_tempbar;
extern lv_obj_t * drivebase_rf_tempbar;
extern lv_obj_t * drivebase_lb_tempbar;
extern lv_obj_t * drivebase_rb_tempbar;
extern lv_obj_t * intake_motor_a_tempbar;
extern lv_obj_t * intake_motor_b_tempbar;
extern lv_obj_t * chain_motor_tempbar;
extern lv_obj_t * drivebase_lf_templabel;
extern lv_obj_t * drivebase_rf_templabel;
extern lv_obj_t * drivebase_lb_templabel;
extern lv_obj_t * drivebase_rb_templabel;
extern lv_obj_t * intake_motor_a_templabel;
extern lv_obj_t * intake_motor_b_templabel;
extern lv_obj_t * chain_motor_templabel;
extern lv_obj_t * config_dropdown;
extern lv_obj_t * test_in_move_function_label;
extern int control_mode;
extern int selected_auton;

extern pros::MotorGroup drivebase;

extern const lv_img_dsc_t creature;

extern double inchesToDegrees(double inches, double gear_ratio, double wheel_diameter);
extern void driveForwardInches(pros::MotorGroup& targetMotorGroup, double inches, double gear_ratio, double wheel_diameter);
void initialize_interface();
void temp_update_task(void* param);
void legacy_temp_update_task(void* param);

void create_auton_tab(lv_obj_t * parent_tab);
void create_temp_tab(lv_obj_t * parent_tab);

#endif // _INTERFACE_HPP_