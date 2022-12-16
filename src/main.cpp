#include "main.h"

// ratio could be 0.714285714

// Chassis constructor
Drive chassis(
    // Left Chassis Ports (negative port will reverse it!)
    //   the first port is the sensored port (when trackers are not used!)
    {-4, -1}

    // Right Chassis Ports (negative port will reverse it!)
    //   the first port is the sensored port (when trackers are not used!)
    ,
    {20, 2}

    // IMU Port
    ,
    19

    // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
    //    (or tracking wheel diameter)
    ,
    4.125

    // Cartridge RPM
    //   (or tick per rotation if using tracking wheels)
    ,
    200

    // External Gear Ratio (MUST BE DECIMAL)
    //    (or gear ratio of tracking wheel)
    // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
    // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
    ,
    1.4

    // Uncomment if using tracking wheels
    /*
    // Left Tracking Wheel Ports (negative port will reverse it!)
    // ,{1, 2} // 3 wire encoder
    // ,8 // Rotation sensor

    // Right Tracking Wheel Ports (negative port will reverse it!)
    // ,{-3, -4} // 3 wire encoder
    // ,-9 // Rotation sensor
    */

    // Uncomment if tracking wheels are plugged into a 3 wire expander
    // 3 Wire Port Expander Smart Port
    // ,1
);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
  // Print our branding over your terminal :D
  // ez::print_ez_template();

  pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  // Configure your chassis controls
  // chassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.set_active_brake(0);     // Sets the active brake kP. We recommend 0.1.
  chassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)
  default_constants();             // Set the drive to your own constants from autons.cpp!
  exit_condition_defaults();       // Set the exit conditions to your own constants from autons.cpp!

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.set_left_curve_buttons (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used.
  // chassis.set_right_curve_buttons(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

  // Initialize chassis and auton selector
  chassis.initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled()
{
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize()
{
  // . . .
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
  chassis.reset_pid_targets();               // Resets PID targets to 0
  chassis.reset_gyro();                      // Reset gyro position to 0
  chassis.reset_drive_sensor();              // Reset drive sensors to 0
  chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.

  // ez::as::auton_selector.call_selected_auton(); // Calls selected auton from autonomous selector.
  capybara_run();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
  // This is preference to what you like to drive on.
  chassis.set_drive_brake(MOTOR_BRAKE_COAST);

  // chassis.tank(); // Tank control
  //  chassis.arcade_standard(ez::SPLIT); // Standard split arcade
  //  chassis.arcade_standard(ez::SINGLE); // Standard single arcade
  //  chassis.arcade_flipped(ez::SPLIT); // Flipped split arcade
  //  chassis.arcade_flipped(ez::SINGLE); // Flipped single arcade

  // create drive motors!
  pros::Motor FL_mtr = pros::Motor(4, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true);
  pros::Motor FR_mtr = pros::Motor(20, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, false);
  pros::Motor BL_mtr = pros::Motor(1, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, true);
  pros::Motor BR_mtr = pros::Motor(2, pros::motor_gearset_e_t::E_MOTOR_GEARSET_18, false);

  // set brake modes
  FL_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  FR_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  BL_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  BR_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  // to avoid using a task for piston delays, we'll have a variable
  // that keeps track of how many ms have ellapsed since the last
  // time we pressed the button

  uint32_t last_press_indexer = 0;
  bool indexer_state = false;

  while (true)
  {

    // fire discs when L1!
    if (master.get_digital(DIGITAL_L1))
    {
      indexer.set_value(1);
      indexer_state = true;
      last_press_indexer = pros::millis();
    }

    if (indexer_state && pros::millis() - last_press_indexer > 100)
    {
      // if >100 ms have passed
      indexer.set_value(0);
      indexer_state = false;
    }

    // expansion pneumatics
    if (master.get_digital(DIGITAL_A))
    {
      expansion_left.set_value(1);
      expansion_right.set_value(1);
    }

    // flywheel
    if (master.get_digital(DIGITAL_L2))
    {
      flywheel = 127;
    }
    else
    {
      flywheel.brake();
    }

    // intake
    if (master.get_digital(DIGITAL_R2))
    {
      intake.move_voltage(12000);
    }
    else if (master.get_digital(DIGITAL_R1))
    {
      intake.move_voltage(-12000);
    }
    else
    {
      intake.brake();
    }

    // drive!
    int axis1 = master.get_analog(ANALOG_RIGHT_X);
    int axis3 = master.get_analog(ANALOG_LEFT_Y);
    int axis4 = master.get_analog(ANALOG_LEFT_X);

    FL_mtr = (axis3 + axis1 + axis4);
    BL_mtr = (axis3 + axis1 - axis4);
    FR_mtr = (axis3 - axis1 - axis4);
    BR_mtr = (axis3 - axis1 + axis4);

    pros::delay(20);

    // . . .
    // Put more user control code here!
    // . . .

    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}