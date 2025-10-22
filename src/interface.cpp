#include "interface.hpp"
#include "globals.hpp"

lv_obj_t * main_tabview = NULL;
lv_obj_t * temp_label_dbl = NULL;
lv_obj_t * temp_label_dbr = NULL;
lv_obj_t * temp_label_arm = NULL;
lv_obj_t * temp_label_clw = NULL;

lv_obj_t * auton_status_label = NULL;
lv_obj_t * toggle_display_image = NULL;

int selected_auton = 0;


static void auton_btn_click_action(lv_event_t * e) {
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e);

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        uint8_t id = (uint8_t)(intptr_t)lv_obj_get_user_data(btn);

        selected_auton = id;

        const char *auton_names[] = {"None", "RED Left", "RED Right", "BLU Left", "BLU Right", "Skills"};

        if (id >= 0 && id <= 5) {
            static char buffer[64];
            snprintf(buffer, sizeof(buffer), "Selected: %s", auton_names[id]);
            lv_label_set_text(auton_status_label, buffer);
        }

    }
}

void temp_update_task(void* param) {
    pros::Motor *motors[] = {
        &drivebase_left,
        &drivebase_right,
        &arm_motor,
        &claw_motor
    };

    lv_obj_t *labels[] = {
        temp_label_dbl,
        temp_label_dbr,
        temp_label_arm,
        temp_label_clw
    };

    const char *names[] = {
        "DBL", "DBR", "ARM", "CLW"
    };

    static char buffer[32];

    while (true) {
        for (int i = 0; i < 4; i++) {
            double current_temp = motors[i]->get_temperature();
            // double current_temp = 42.0; <- Declare static value to debug task code.
            lv_obj_t *current_label = labels[i];
            
            snprintf(buffer, sizeof(buffer), "%s: %.1f C", names[i], current_temp);
            lv_label_set_text(current_label, buffer);

            if (current_temp > 999.0) {
                snprintf(buffer, sizeof(buffer), "%s: ERR", names[i]);
                lv_obj_set_style_text_color(current_label, LV_COLOR_MAKE(255, 165, 0), LV_PART_MAIN);
            } else if (current_temp > 55.0) {
                lv_obj_set_style_text_color(current_label, LV_COLOR_MAKE(255, 0, 0), LV_PART_MAIN);
            } else {
                lv_obj_set_style_text_color(current_label, lv_color_white(), LV_PART_MAIN);
            }
        }

        pros::delay(200);
    }
}

void create_temp_tab(lv_obj_t * parent_tab) {
    temp_label_dbl = lv_label_create(parent_tab);
    lv_label_set_text(temp_label_dbl, "DBL: -- C");
    lv_obj_set_pos(temp_label_dbl, 10, 10);

    temp_label_dbr = lv_label_create(parent_tab);
    lv_label_set_text(temp_label_dbr, "DBR: -- C");
    lv_obj_set_pos(temp_label_dbr, 10, 40);

    temp_label_arm = lv_label_create(parent_tab);
    lv_label_set_text(temp_label_arm, "ARM: -- C");
    lv_obj_set_pos(temp_label_arm, 200, 10);

    temp_label_clw = lv_label_create(parent_tab);
    lv_label_set_text(temp_label_clw, "CLW: -- C");
    lv_obj_set_pos(temp_label_clw, 200, 40);

    pros::Task temp_task(temp_update_task, (void*)"TEMP_TASK");
}

void create_auton_tab(lv_obj_t * parent_tab) {
    auton_status_label = lv_label_create(parent_tab);
    lv_label_set_text(auton_status_label, "Selected: None");
    lv_obj_set_pos(auton_status_label, 10, 10);

    lv_obj_t * none_btn = lv_btn_create(parent_tab);
    lv_obj_set_pos(none_btn, 10, 50);
    lv_obj_set_size(none_btn, 100, 40);
    lv_obj_set_user_data(none_btn, (void*)0);
    lv_obj_add_event_cb(none_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);

    lv_obj_t * none_label = lv_label_create(none_btn);
    lv_label_set_text(none_label, "None");

    lv_obj_t * skills_btn = lv_btn_create(parent_tab);
    lv_obj_set_pos(skills_btn, 230, 50);
    lv_obj_set_size(skills_btn, 100, 40);
    lv_obj_set_user_data(skills_btn, (void*)5);
    lv_obj_add_event_cb(skills_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);

    lv_obj_t * skills_label = lv_label_create(skills_btn);
    lv_label_set_text(skills_label, "Skills");

    lv_obj_t * red_left_btn = lv_btn_create(parent_tab);
    lv_obj_set_pos(red_left_btn, 10, 110);
    lv_obj_set_size(red_left_btn, 100, 40);
    lv_obj_set_user_data(red_left_btn, (void*)1);
    lv_obj_add_event_cb(red_left_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);

    lv_obj_t * red_left_label = lv_label_create(red_left_btn);
    lv_label_set_text(red_left_label, "RED Left");

    lv_obj_t * red_right_btn = lv_btn_create(parent_tab);
    lv_obj_set_pos(red_right_btn, 230, 110);
    lv_obj_set_size(red_right_btn, 100, 40);
    lv_obj_set_user_data(red_right_btn, (void*)2);
    lv_obj_add_event_cb(red_right_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);

    lv_obj_t * red_right_label = lv_label_create(red_right_btn);
    lv_label_set_text(red_right_label, "RED Right");

    lv_obj_t * blu_left_btn = lv_btn_create(parent_tab);
    lv_obj_set_pos(blu_left_btn, 10, 170);
    lv_obj_set_size(blu_left_btn, 100, 40);
    lv_obj_set_user_data(blu_left_btn, (void*)3);
    lv_obj_add_event_cb(blu_left_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);

    lv_obj_t * blu_left_label = lv_label_create(blu_left_btn);
    lv_label_set_text(blu_left_label, "BLU Left");

    lv_obj_t * blu_right_btn = lv_btn_create(parent_tab);
    lv_obj_set_pos(blu_right_btn, 230, 170);
    lv_obj_set_size(blu_right_btn, 100, 40);
    lv_obj_set_user_data(blu_right_btn, (void*)4);
    lv_obj_add_event_cb(blu_right_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);

    lv_obj_t * blu_right_label = lv_label_create(blu_right_btn);
    lv_label_set_text(blu_right_label, "BLU Right");
}

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
    lv_obj_t * image_btn = lv_btn_create(parent_tab);
    lv_obj_set_pos(image_btn, 10, 10);
    lv_obj_set_size(image_btn, 100, 40);
    lv_obj_add_event_cb(image_btn, image_button_action, LV_EVENT_CLICKED, NULL);

    lv_obj_t * image_label = lv_label_create(image_btn);
    lv_label_set_text(image_label, "Minnow");

    lv_obj_set_style_bg_color(image_btn, LV_COLOR_MAKE(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);

    extern const lv_img_dsc_t creature; 

    toggle_display_image = lv_img_create(parent_tab);
    lv_img_set_src(toggle_display_image, &creature);
    lv_obj_move_foreground(toggle_display_image);

    lv_obj_align(toggle_display_image, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN);
}

void initialize_interface() {
    main_tabview = lv_tabview_create(lv_scr_act());
    lv_obj_t * tab_bar = lv_tabview_get_tab_bar(main_tabview);
    lv_obj_set_height(tab_bar, 30);
    
    lv_obj_t * temp_tab = lv_tabview_add_tab(main_tabview, "Monitor");
    create_temp_tab(temp_tab);
    
    lv_obj_t * auton_tab = lv_tabview_add_tab(main_tabview, "Auton");
    create_auton_tab(auton_tab);
    
    lv_obj_t * image_tab = lv_tabview_add_tab(main_tabview, "Images");
    create_image_tab(image_tab);

    lv_tabview_set_act(main_tabview, 1, LV_ANIM_OFF); 
}