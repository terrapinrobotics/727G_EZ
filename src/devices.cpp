#include "devices.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"

// our controller
pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

// chassis motors!
/*pros::Motor FL_mtr = pros::Motor(4, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true);
pros::Motor FR_mtr = pros::Motor(20, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, false);
pros::Motor BL_mtr = pros::Motor(1, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true);
pros::Motor BR_mtr = pros::Motor(2, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, false);
*/

// flywheel
pros::Motor flywheel_A = pros::Motor(3, pros::motor_gearset_e_t::E_MOTOR_GEARSET_06, false, pros::motor_encoder_units_e_t::E_MOTOR_ENCODER_DEGREES);
pros::Motor flywheel_B = pros::Motor(7, pros::motor_gearset_e_t::E_MOTOR_GEARSET_06, true, pros::motor_encoder_units_e_t::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group flywheel = pros::Motor_Group({flywheel_A, flywheel_B});

// intake
pros::Motor intake_A = pros::Motor(6, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true, pros::motor_encoder_units_e_t::E_MOTOR_ENCODER_DEGREES);
pros::Motor intake_B = pros::Motor(9, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true, pros::motor_encoder_units_e_t::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group intake = pros::Motor_Group({intake_A, intake_B});
// pneumatics
pros::ADIDigitalOut indexer = pros::ADIDigitalOut('A');
pros::ADIDigitalOut expansion_left = pros::ADIDigitalOut('B');
pros::ADIDigitalOut expansion_right = pros::ADIDigitalOut('C');

// inertial
pros::Imu inertial = pros::Imu(19);