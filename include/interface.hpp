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

extern ControlMode control_mode;
extern AutonRoutine selected_auton;

extern pros::MotorGroup drivebase;
extern pros::MotorGroup intake_motors;

extern const lv_img_dsc_t creature;

void initialize_interface();
void temp_update_task(void* param);
void create_auton_tab(lv_obj_t * parent_tab);
void create_temp_tab(lv_obj_t * parent_tab);
void create_image_tab(lv_obj_t * parent_tab);
void create_settings_tab(lv_obj_t * parent_tab);
void create_stats_tab(lv_obj_t * parent_tab);

#endif // _INTERFACE_HPP_