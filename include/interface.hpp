#include "main.h"

#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

extern lv_obj_t * main_tabview;
extern lv_obj_t * temp_label_dbl;
extern lv_obj_t * temp_label_dbr;
extern lv_obj_t * temp_label_arm;
extern lv_obj_t * temp_label_clw;

void initialize_interface();
void temp_update_task(void* param);

void create_auton_tab(lv_obj_t * parent_tab);
void create_temp_tab(lv_obj_t * parent_tab);

#endif // _INTERFACE_HPP_