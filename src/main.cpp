#include "main.h"
#include "pros/apix.h"
#include "globals.hpp"
#include "interface.hpp"

// Initial function
void initialize() {
	drivebase_left.set_gearing(pros::v5::MotorGears::green);
    drivebase_right.set_gearing(pros::v5::MotorGears::green);
    arm_motor.set_gearing(pros::v5::MotorGears::red);
    claw_motor.set_gearing(pros::v5::MotorGears::green);

    drivebase_left.set_reversed(false);
    drivebase_right.set_reversed(false);
    arm_motor.set_reversed(false);
    claw_motor.set_reversed(false);

    initialize_interface();
}

// DISABLED mode
void disabled() {}

// Runs after INITIALIZE when connected to VEX Competition
void competition_initialize() {
	
}

void autonomous() {}


void opcontrol() {
	while (true) {
        int dir = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		drivebase_left.move(turn + dir);
		drivebase_right.move(turn - dir);
		pros::delay(5);
	}
}