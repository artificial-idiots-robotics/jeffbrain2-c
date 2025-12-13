#include "interface.hpp";

lv_obj_t * toggle_display_image = NULL;

static void image_button_action(lv_event_t * e) {
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e); 
    
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        if (lv_obj_has_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_clear_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(btn, LV_COLOR_MAKE(0, 200, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            lv_obj_add_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(btn, LV_COLOR_MAKE(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void create_image_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_ROW);

    lv_obj_t * image_btn = lv_btn_create(cont);
    lv_obj_set_pos(image_btn, 10, 10);
    lv_obj_set_size(image_btn, 100, 40);
    lv_obj_add_event_cb(image_btn, image_button_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(image_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(image_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(image_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * image_label = lv_label_create(image_btn);
    lv_label_set_text(image_label, "Minnow");

    lv_obj_set_style_bg_color(image_btn, LV_COLOR_MAKE(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);

    extern const lv_img_dsc_t creature; 

    toggle_display_image = lv_img_create(cont);
    lv_img_set_src(toggle_display_image, &creature);
    lv_obj_move_foreground(toggle_display_image);

    lv_obj_align(toggle_display_image, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN);
}