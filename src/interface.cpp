#include "interface.hpp"
#include "globals.hpp"

const lv_color_t M3_ACCENT_COLOR = lv_color_hex(0x6750A4);
const lv_color_t M3_SURFACE_COLOR = lv_color_hex(0x201A2B);
const lv_color_t M3_BACKGROUND_COLOR = lv_color_hex(0x1C1B1F);
const int M3_RADIUS = 20;

static lv_style_t style_base;
static lv_style_t style_m3_btn;
static lv_style_t style_tempbar_main;
static lv_style_t style_tempbar_indicator;

lv_obj_t * main_tabview = NULL;

lv_obj_t * drivebase_lf_tempbar = NULL;
lv_obj_t * drivebase_rf_tempbar = NULL;
lv_obj_t * drivebase_lb_tempbar = NULL;
lv_obj_t * drivebase_rb_tempbar = NULL;
lv_obj_t * intake_motor_a_tempbar = NULL;
lv_obj_t * intake_motor_b_tempbar = NULL;
lv_obj_t * chain_motor_tempbar = NULL;

lv_obj_t * drivebase_lf_templabel = NULL;
lv_obj_t * drivebase_rf_templabel = NULL;
lv_obj_t * drivebase_lb_templabel = NULL;
lv_obj_t * drivebase_rb_templabel = NULL;
lv_obj_t * intake_motor_a_templabel = NULL;
lv_obj_t * intake_motor_b_templabel = NULL;
lv_obj_t * chain_motor_templabel = NULL;

lv_obj_t * auton_status_label = NULL;
lv_obj_t * test_in_move_function_label = NULL;
lv_obj_t * toggle_display_image = NULL;
lv_obj_t * config_dropdown = NULL;

AutonRoutine selected_auton = AutonRoutine::NONE;
ControlMode control_mode = ControlMode::ARCADE;

typedef struct {
    lv_obj_t * container;
    lv_obj_t * bar;
    lv_obj_t * label;
} motor_gauge_t;

typedef struct {
    const char * label_text;
    lv_coord_t x_pos;
    lv_coord_t y_pos;
    int user_id;
} auton_button_data_t;

lv_obj_t * create_tab_content_container(lv_obj_t * parent_tab, lv_flex_flow_t flow) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, flow);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    return cont;
}

void create_stats_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_ROW);
}

static void auton_btn_click_action(lv_event_t * e) {
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e);

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        uint8_t id = (uint8_t)(intptr_t)lv_obj_get_user_data(btn);

        selected_auton = static_cast<AutonRoutine>(id);

        const char *auton_names[] = {"None", "RED Left", "RED Right", "BLU Left", "BLU Right", "Skills"};

        if (id >= 0 && id <= 5) {
            static char buffer[64];
            snprintf(buffer, sizeof(buffer), "Selected: %s", auton_names[id]);
            lv_label_set_text(auton_status_label, buffer);
        }

    }
}

motor_gauge_t create_motor_gauge(lv_obj_t * parent, const char * label_text) {
    motor_gauge_t motor_gauge;

    motor_gauge.container = lv_obj_create(parent);
    lv_obj_set_width(motor_gauge.container, LV_PCT(100));
    lv_obj_set_height(motor_gauge.container, LV_SIZE_CONTENT);
    lv_obj_set_layout(motor_gauge.container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(motor_gauge.container, LV_FLEX_FLOW_ROW);

    motor_gauge.bar = lv_bar_create(motor_gauge.container);
    lv_obj_set_size(motor_gauge.bar, 240, 10);
    lv_bar_set_range(motor_gauge.bar, 0, 100);

    motor_gauge.label = lv_label_create(motor_gauge.container);
    lv_label_set_text(motor_gauge.label, label_text);

    return motor_gauge;
}

void temp_update_task(void* param) {
    pros::Motor *motors[] = {
        &drivebase_lf,
        &drivebase_rf,
        &drivebase_lb,
        &drivebase_rb,
        &intake_motor_a,
        &intake_motor_b,
        &chain_motor
    };

    lv_obj_t *bars[] = {
        drivebase_lf_tempbar,
        drivebase_rf_tempbar,
        drivebase_lb_tempbar,
        drivebase_rb_tempbar,
        intake_motor_a_tempbar,
        intake_motor_b_tempbar,
        chain_motor_tempbar
    };

    while (true) {
        for (int i = 0; i < 7; i++) {
            double current_temp = motors[i]->get_temperature();
            // double current_temp = 42.0; <- Declare static value to debug task code.
            lv_obj_t *current_bar = bars[i];
            
            lv_bar_set_value(bars[i], current_temp, LV_ANIM_ON);

            if (current_temp > 999.0) {
                lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);
            } else if (current_temp > 55.0) {
                lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
            } else {
                lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
            }
        }

        pros::delay(200);
    }
}

void create_temp_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

    motor_gauge_t drivebase_lf_gauge = create_motor_gauge(cont, "Drivebase front left");
    drivebase_lf_tempbar = drivebase_lf_gauge.bar;
    drivebase_lf_templabel = drivebase_lf_gauge.label;
    
    motor_gauge_t drivebase_rf_gauge = create_motor_gauge(cont, "Drivebase front right");
    drivebase_rf_tempbar = drivebase_rf_gauge.bar;
    drivebase_rf_templabel = drivebase_rf_gauge.label;

    motor_gauge_t drivebase_lb_gauge = create_motor_gauge(cont, "Drivebase back left");
    drivebase_lb_tempbar = drivebase_lb_gauge.bar;
    drivebase_lb_templabel = drivebase_lb_gauge.label;

    motor_gauge_t drivebase_rb_gauge = create_motor_gauge(cont, "Drivebase back right");
    drivebase_rb_tempbar = drivebase_rb_gauge.bar;
    drivebase_rb_templabel = drivebase_rb_gauge.label;

    motor_gauge_t intake_motor_a_gauge = create_motor_gauge(cont, "Intake motor A");    
    intake_motor_a_tempbar = intake_motor_a_gauge.bar;
    intake_motor_a_templabel = intake_motor_a_gauge.label;  

    motor_gauge_t intake_motor_b_gauge = create_motor_gauge(cont, "Intake motor B");
    intake_motor_b_tempbar = intake_motor_b_gauge.bar;
    intake_motor_b_templabel = intake_motor_b_gauge.label;

    motor_gauge_t chain_motor_gauge = create_motor_gauge(cont, "Chain motor");
    chain_motor_tempbar = chain_motor_gauge.bar;
    chain_motor_templabel = chain_motor_gauge.label;

    lv_obj_t *bars[] = {
        drivebase_lf_tempbar,
        drivebase_rf_tempbar,
        drivebase_lb_tempbar,
        drivebase_rb_tempbar,
        intake_motor_a_tempbar,
        intake_motor_b_tempbar,
        chain_motor_tempbar
    };

    for (size_t i = 0; i < sizeof(bars) / sizeof(bars[0]); i++) {
        lv_obj_t* obj = bars[i];

        lv_obj_add_style(obj, &style_tempbar_main, LV_PART_MAIN);
        lv_obj_add_style(obj, &style_tempbar_indicator, LV_PART_INDICATOR);
    }
    
    pros::Task temp_task(temp_update_task, (void*)"TEMP_TASK");
}

lv_obj_t * create_auton_button(lv_obj_t * parent, const auton_button_data_t * data) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_pos(btn, data->x_pos, data->y_pos);
    lv_obj_set_size(btn, LV_PCT(45), 40);
    lv_obj_set_user_data(btn, (void*)(intptr_t)data->user_id);
    lv_obj_add_event_cb(btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, data->label_text);

    return btn;
}

void create_auton_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_column(cont, 15, 0);
    lv_obj_set_style_pad_row(cont, 15, 0); 
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    auton_status_label = lv_label_create(cont);
    lv_label_set_text(auton_status_label, "Selected: None");
    lv_obj_set_pos(auton_status_label, 10, 10);
    lv_obj_set_width(auton_status_label, LV_PCT(90));

    const auton_button_data_t auton_buttons[] = {
        {"None", 10, 50, static_cast<int>(AutonRoutine::NONE)},
        {"Skills", 230, 50, static_cast<int>(AutonRoutine::SKILLS)},
        {"RED Left", 10, 110, static_cast<int>(AutonRoutine::RED_LEFT)},
        {"RED Right", 230, 110, static_cast<int>(AutonRoutine::RED_RIGHT)},
        {"BLU Left", 10, 170, static_cast<int>(AutonRoutine::BLU_LEFT)},
        {"BLU Right", 230, 170, static_cast<int>(AutonRoutine::BLU_RIGHT)}
    };

    for (size_t i = 0; i < sizeof(auton_buttons) / sizeof(auton_buttons[0]); i++) {
        create_auton_button(cont, &auton_buttons[i]);
    }
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

static void drive_mode_dropdown_action(lv_event_t * e) {
    lv_obj_t * dropdown = (lv_obj_t *)lv_event_get_target(e);
    
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        int selected_index = lv_dropdown_get_selected(dropdown);
        control_mode = static_cast<ControlMode>(selected_index);
    }
}

void create_settings_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * config_dropdown = lv_dropdown_create(cont);
    lv_obj_set_pos(config_dropdown, 50, 10);
    lv_dropdown_set_options(config_dropdown, "Arcade\nTank");
    lv_obj_add_event_cb(config_dropdown, drive_mode_dropdown_action, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_set_style_radius(config_dropdown, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(config_dropdown, lv_color_hex(0x49454F), LV_PART_MAIN);    
    lv_obj_set_style_bg_color(config_dropdown, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(config_dropdown, lv_color_white(), LV_PART_MAIN);

    lv_obj_t * list = lv_dropdown_get_list(config_dropdown);

    if (list != NULL) {
        lv_obj_set_style_bg_color(list, lv_color_hex(0x49454F), LV_PART_MAIN);
        lv_obj_set_style_radius(list, 15, LV_PART_MAIN);
        lv_obj_set_style_bg_color(list, M3_ACCENT_COLOR, LV_PART_SELECTED);
        lv_obj_set_style_text_color(list, lv_color_white(), LV_PART_SELECTED);
    }
}

void testmove_button_action(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        chassis.moveToPoint(0, 5, 1000);
    }
}

void create_test_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);
    
    lv_obj_t * testmove = lv_btn_create(cont);
    lv_obj_set_pos(testmove, 10, 10);
    lv_obj_set_size(testmove, 150, 40);
    lv_obj_add_event_cb(testmove, testmove_button_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(testmove, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(testmove, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(testmove, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    
    lv_obj_t * testmove_label = lv_label_create(cont);
    lv_label_set_text(testmove_label, "Test MOVE");
}

void initialize_interface() {
    lv_style_init(&style_base);
    lv_style_set_bg_color(&style_base, M3_BACKGROUND_COLOR);
    lv_style_set_text_color(&style_base, lv_color_white());

    lv_style_init(&style_m3_btn);
    lv_style_set_radius(&style_m3_btn, M3_RADIUS);
    lv_style_set_shadow_width(&style_m3_btn, 8);
    lv_style_set_shadow_opa(&style_m3_btn, LV_OPA_50);
    lv_style_set_bg_color(&style_m3_btn, lv_color_hex(0x49454F));
    lv_style_set_text_color(&style_m3_btn, lv_color_white());

    lv_style_init(&style_tempbar_main);
    lv_style_set_radius(&style_tempbar_main, 8);
    lv_style_set_bg_color(&style_tempbar_main, lv_color_hex(0x353139));
    lv_style_set_border_width(&style_tempbar_main, 1);
    lv_style_set_border_color(&style_tempbar_main, lv_color_hex(0x49454F));

    lv_style_init(&style_tempbar_indicator);
    lv_style_set_radius(&style_tempbar_indicator, 8);

    lv_obj_t * screen = lv_scr_act();
    lv_obj_add_style(screen, &style_base, 0);

    main_tabview = lv_tabview_create(screen);
    lv_obj_t * tab_bar = lv_tabview_get_tab_bar(main_tabview);
    lv_obj_set_height(tab_bar, 40);

    lv_obj_set_style_bg_color(tab_bar, M3_SURFACE_COLOR, LV_PART_ITEMS);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xE6E0E9), LV_PART_ITEMS);
    lv_obj_set_style_border_width(tab_bar, 0, LV_PART_ITEMS);
    lv_obj_set_style_pad_all(tab_bar, 5, LV_PART_ITEMS);

    lv_obj_set_style_radius(tab_bar, M3_RADIUS, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(tab_bar, M3_ACCENT_COLOR, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xFFFBFE), LV_STATE_CHECKED | LV_PART_ITEMS);

    lv_obj_set_style_width(tab_bar, LV_PCT(100), LV_STATE_CHECKED | LV_PART_INDICATOR);
    lv_obj_set_style_height(tab_bar, LV_PCT(100), LV_STATE_CHECKED | LV_PART_INDICATOR);

    lv_obj_set_style_bg_color(main_tabview, M3_SURFACE_COLOR, LV_PART_MAIN);
    lv_obj_set_style_pad_all(main_tabview, 0, LV_PART_MAIN);

    // lv_obj_t * stats_tab = lv_tabview_add_tab(main_tabview, "Stats");
    // create_stats_tab(stats_tab);

    lv_obj_t * temp_tab = lv_tabview_add_tab(main_tabview, "Monitor");
    create_temp_tab(temp_tab);

    lv_obj_t * auton_tab = lv_tabview_add_tab(main_tabview, "Auton");
    create_auton_tab(auton_tab);

    lv_obj_t * image_tab = lv_tabview_add_tab(main_tabview, "Images");
    create_image_tab(image_tab);

    lv_obj_t * settings_tab = lv_tabview_add_tab(main_tabview, "Settings");
    create_settings_tab(settings_tab);

    lv_obj_t * test_tab = lv_tabview_add_tab(main_tabview, "Test");
    create_test_tab(test_tab);

    lv_tabview_set_act(main_tabview, 1, LV_ANIM_ON); 
}