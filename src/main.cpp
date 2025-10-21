#include "main.h"
#include <lv_types.h>
#include <liblvgl/lv_api_map_v8.h>
#include <liblvgl/widgets/label/lv_label.h>
#include <liblvgl/widgets/tabview/lv_tabview.h>

void create_gui() {
	lv_obj_t * screen = lv_scr_act();
	lv_obj_t * tabview = lv_tabview_create(screen);
	pros::delay(10);
}

void initialize() {
	create_gui();
}