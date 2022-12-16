#include "main.h"

/////
// For instalattion, upgrading, documentations and tutorials, check out website!
// https://ez-robotics.github.io/EZ-Template/
/////

const int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed).  We don't suggest making this 127.
                             // If this is 127 and the robot tries to heading correct, it's only correcting by
                             // making one side slower.  When this is 87%, it's correcting by making one side
                             // faster and one side slower, giving better heading correction.
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;

///
// Constants
///

// It's best practice to tune constants when the robot is empty and with heavier game objects, or with lifts up vs down.
// If the objects are light or the cog doesn't change much, then there isn't a concern here.

void default_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void one_mogo_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void two_mogo_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void exit_condition_defaults()
{
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void modified_exit_condition()
{
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

///
// Swing Example
///
void swing_example()
{
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is target degrees
  // The third parameter is speed of the moving side of the drive

  chassis.set_swing_pid(ez::LEFT_SWING, 45, SWING_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_until(12);

  chassis.set_swing_pid(ez::RIGHT_SWING, 0, SWING_SPEED);
  chassis.wait_drive();
}

void delay_until_target(pros::Motor_Group &motor, int target)
{
  while (!((motor.get_positions()[0] < target + 5) && (motor.get_positions()[0] > target - 5)))
  {
    pros::delay(20);
  }
}

void spin_intake(int units, int velocity)
{
  intake.move_relative(units, velocity);
  delay_until_target(intake, units);
}

void fireDiscs(int discCount)
{
  // make it as easy as possible
  // piston delay of 4s?
  // each disc should fire every 1s after that!
  // so motors spin up for 4s
  // then we enter loop of (fire piston, wait 1s)
  for (int i = 0; i < discCount; i++)
  {
    indexer.set_value(1);
    pros::delay(200);
    indexer.set_value(0);
    pros::delay(1100);
  }
  flywheel.brake();
}

void capybara_run()
{
  flywheel.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

  // drive robot forward 3 inches
  chassis.set_drive_pid(3, DRIVE_SPEED, true);
  chassis.wait_drive();

  // spin intake 320 degrees
  spin_intake(300, 127);

  // reverse 4 inches
  chassis.set_drive_pid(-4, DRIVE_SPEED, true);
  chassis.wait_drive();

  // turn 132 degrees
  chassis.set_turn_pid(132, TURN_SPEED);
  chassis.wait_drive();

  // spin up the flywheel
  flywheel.move_voltage(12000);

  // drive forward 5 feet
  chassis.set_drive_pid(60, DRIVE_SPEED, true);
  chassis.wait_drive();

  // turn 130 degrees
  chassis.set_turn_pid(130, TURN_SPEED);
  chassis.wait_drive();

  // drive robot 5 inches forward
  chassis.set_drive_pid(5, DRIVE_SPEED, true);
  chassis.wait_drive();

  // delay 700ms
  pros::delay(700);

  // fire 3 discs
  fireDiscs(3);
}