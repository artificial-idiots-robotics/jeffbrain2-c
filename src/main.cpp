#include "main.h"
#include "pros/apix.h"
#include "globals.hpp"
#include "interface.hpp"

// Initial function
void initialize() {
    chassis.setPose(0, 0, 0);

    // Set gearings
	drivebase_lf.set_gearing(pros::v5::MotorGears::blue);
    drivebase_rf.set_gearing(pros::v5::MotorGears::blue);
    drivebase_lb.set_gearing(pros::v5::MotorGears::blue);
    drivebase_rb.set_gearing(pros::v5::MotorGears::blue);
    intake_motor_a.set_gearing(pros::v5::MotorGears::red);
    intake_motor_b.set_gearing(pros::v5::MotorGears::red);
    chain_motor.set_gearing(pros::v5::MotorGears::red);

    // Set encoder units
    drivebase_lf.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    drivebase_rf.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    drivebase_lb.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    drivebase_rb.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    intake_motor_a.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    intake_motor_b.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    chain_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

    // Set reverse states
    drivebase_lf.set_reversed(false);
    drivebase_rf.set_reversed(false);
    drivebase_lb.set_reversed(false);
    drivebase_rb.set_reversed(false);
    intake_motor_a.set_reversed(false);
    intake_motor_b.set_reversed(false);
    chain_motor.set_reversed(false);

    initialize_interface();
}

// DISABLED mode
void disabled() {}

// Runs after INITIALIZE when connected to VEX Competition
void competition_initialize() {
	
}

void autonomous() {
    switch (selected_auton) {
        case AutonRoutine::NONE: {
            break;
        }
        
        case AutonRoutine::RED_LEFT: {
            break;
        }
        
        case AutonRoutine::RED_RIGHT: {
            break;
        }
        
        case AutonRoutine::BLU_LEFT: {
            break;
        }
        
        case AutonRoutine::BLU_RIGHT:  {
            break;
        }

        case AutonRoutine::SKILLS: {
            break;
        }

        default: {
            // In case of variable somehow reaching values beyond our comprehension. Also known as 6.
            break;
        }
    }
}


void opcontrol() {
    extern ControlMode control_mode;

	while (true) {
        switch (control_mode) {
            case ControlMode::ARCADE: {
                int dir = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
                int turn = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

                chassis.arcade(dir, turn);
                break;
            }

            case ControlMode::TANK: {
                int leftdrive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
                int rightdrive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

                chassis.tank(leftdrive, rightdrive);
                break;
            }

            default: {
                // Default to ARCADE mode if control_mode is somehow invalid
                control_mode = ControlMode::ARCADE;
                break;
            }
        }

        pros::delay(5);
	}
}