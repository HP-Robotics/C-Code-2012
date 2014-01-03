#include "WPILib.h"
#include "Joystick.h"

class RobotDemo : public SimpleRobot
{
	RobotDrive drivewheels; // Robot driving wheels system
	RobotDrive shooterwheels; //Robot shooting wheels system
	Relay ballloader; //Ball-loading motor operated by a relay
	Joystick leftstick; // The left joystick
	Joystick rightstick; //The right joystick
	Compressor comp; //The compressor
	Solenoid shooterup; //Shooter solenoid (Up)
	Solenoid shooterdown; //Shooter solenoid (Down)
	Solenoid bridgeoutout; //Bridge-out solenoid (Out)
	Solenoid bridgeoutin; //Bridge-out solenoid (In)
	Relay bridgedown; //Bridge-down solenoid controlled by relay
	float shooterwheelsspeed; //The speed of shooting wheels as determined by the Z-Axis of the left joystick
	float leftSpeed;
	float rightSpeed;
	
public:
	RobotDemo(void): // these must be initialized in the same order as they are declared above.
		drivewheels(1, 2), //Driving motors are on ports 1 and 2
		shooterwheels(3, 4), //Shooting motors are on ports 3 and 4
		ballloader(2), //Ball-loading motor relay is on port 2
		leftstick(2), //Left joystick is on port 2
		rightstick(1), //Right joystick is on port 1
		comp(1, 1), //The compressor is on port 1, 1
		shooterup(1), //The shooter solenoid (up) is on port 1
		shooterdown(2), //The shooter solenoid (down) is on port 2
		bridgeoutout(3), //The bridge-out solenoid (out) is on port 3
		bridgeoutin(4), //The bridge-out solenoid (in) is on port 4
		bridgedown(3) //The bridge-down relay is on port 3
	{
		comp.Start(); //Start compressor
		shooterdown.Set(true); //Bring shooter piston down (If it wasn't already)
		shooterup.Set(false);
		bridgedown.Set(Relay::kOff); //Bring bridge-down solenoid up (If it wasn't already)
		Wait (1.0); //Waits 1 second so the bridge-down piston doesn't get smashed by the bridge-in piston
		bridgeoutin.Set(true); //Bring bridge mechanism in (If it wasn't already)
		bridgeoutout.Set(false);
		shooterwheels.SetSafetyEnabled(false);
		AxisCamera &camera = AxisCamera::GetInstance();
	}

	void Autonomous(void) //Autonomous mode
	{
		shooterwheels.Drive(0.5, 0.0); //Drive forwards half speed...
		Wait(2.0); //...for 2 seconds...
		shooterwheels.Drive(0.0, 0.0); //... then stop
	}

	void OperatorControl(void) //Teleop mode
	{
		drivewheels.SetExpiration(0.1); //Set Watchdog expiration to 0.1
		drivewheels.SetSafetyEnabled(true); //Set Watchdog to On
		while (IsOperatorControl())
		{
			leftSpeed = leftstick.GetY();
			rightSpeed = rightstick.GetY();
			shooterwheelsspeed = leftstick.GetZ();
            	if (leftstick.GetRawButton(1)) //When the left trigger is pushed...
            	{
            		shooterwheels.Drive(-(shooterwheelsspeed), 0.0); //Drive the shooter wheels at speed 2
            		Wait (2.0); //Do this for 2 seconds
            		shooterup.Set(true); //Make shooter piston go up
            		shooterdown.Set(false);
            		Wait (1.0); //Wait 1 second
            		shooterdown.Set(true); //Make shooter piston go down
            		shooterup.Set(false);
            		shooterwheels.Drive(0.0, 0.0); //Stop shooter wheels
            	}
            	if (leftstick.GetRawButton(2)) //When left-button 2 is held
            	{
            		bridgeoutout.Set(true); //Push bridge mechanism out
            		bridgeoutin.Set(false);
            	}
            	if (!leftstick.GetRawButton(2)) //When left-button 2 is not held
            	{
            		bridgeoutin.Set(true); //Bring bridge mechanism in
            		bridgeoutout.Set(false);
            	}
            	if (leftstick.GetRawButton(3)) //When left-button 3 is held
            	{
            		ballloader.Set(Relay::kForward); //Bring balls into robot
            	}	
            	if (!leftstick.GetRawButton(3)) //When left-button 3 is not held
            	{
            		ballloader.Set(Relay::kOff); //Stop ball-capture motor
            	}
            	if (rightstick.GetRawButton(2)) //When right-button 2 is held
            	{
            		bridgedown.Set(Relay::kForward); //Make bridge-down piston go down
            	}
            	if (!rightstick.GetRawButton(2)) //When right-button 2 is not held
            	{
            		bridgedown.Set(Relay::kOff); //Make bridge-down piston go up
            	}
        	Wait(0.005); //Waits a bit so Wtchdog can catch up
        	drivewheels.TankDrive(-(leftSpeed), -(rightSpeed));
		}
	}
};
START_ROBOT_CLASS(RobotDemo);
