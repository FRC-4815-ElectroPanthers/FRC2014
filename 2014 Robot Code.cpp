#include "WPILib.h"

/**
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 
class Robot4815 : public IterativeRobot
{
	Victor *frontRight, *frontLeft, *backRight, *backLeft, *shooter; // robot drive system
	Joystick *stick; // only joystick
	DigitalInput *limitswitch, *limitswitch2;
	DoubleSolenoid *latch, *angle; 
	Compressor *comp;
	uint32_t n;
	bool difDrive, stop;

public:
	Robot4815()	
	{
		this->SetPeriod(0); 	//Set update period to sync with robot control packets (20ms nominal)
	}
	
/**
 * Robot-wide initialization code should go here.
 * 
 * Use this method for default Robot-wide initialization which will
 * be called when the robot is first powered on.  It will be called exactly 1 time.
 */
void Robot4815::RobotInit() {
	
	frontRight = new Victor(1);
	frontLeft = new Victor(3);
	backRight = new Victor(2);
	backLeft = new Victor(4);
	shooter = new Victor(5);// these must be initialized in the same order
	stick = new Joystick(1);		// as they are declared above.
	limitswitch = new DigitalInput(1);
	limitswitch2 = new DigitalInput(2);
	latch = new DoubleSolenoid(1,2);
	angle = new DoubleSolenoid(3,4);
	comp = new Compressor(1, 2);
	comp->Start();
	
}

/**
 * Initialization code for disabled mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters disabled mode. 
 */
void Robot4815::DisabledInit() {
}

/**
 * Periodic code for disabled mode should go here.
 * 
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in disabled mode.
 */
void Robot4815::DisabledPeriodic() {
}

/**
 * Initialization code for autonomous mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters autonomous mode.
 */
void Robot4815::AutonomousInit() {
	stop = false;
}

/**
 * Periodic code for autonomous mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in autonomous mode.
 */
void Robot4815::AutonomousPeriodic() {
	if (!stop){
		frontRight->Set(0);
		frontLeft->Set(0);
		backRight->Set(0);
		backLeft->Set(0);
	}else{
		if (shooter->Get() == 0)
			shooter->Set(-1.0);
		if (limitswitch){
			latch->Set(DoubleSolenoid::kForward);
			shooter->Set(1.0);
		}
		if (limitswitch2){
			shooter->Set(0);
			latch->Set(DoubleSolenoid::kReverse);
			Wait(1.0);
			latch->Set(DoubleSolenoid::kForward);	
			frontRight->Set(1.0);
			frontLeft->Set(-1.0);
			backRight->Set(1.0);
			backLeft->Set(-1.0);
			Wait(1.0);
			stop = true;
	}
	}	
}

/**
 * Initialization code for teleop mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters teleop mode.
 */
void Robot4815::TeleopInit() {
	n = 1;
	difDrive = false;
}

/**
 * Periodic code for teleop mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in teleop mode.
 */
void Robot4815::TeleopPeriodic() {
	if (stick->GetRawAxis(3)> 0){
		shooter->Set(-1.0);
		latch->Set(DoubleSolenoid::kForward);
	}
	if (limitswitch)
		shooter->Set(1.0);
	if (limitswitch2)
		shooter->Set(0);
	if (stick->GetRawAxis(3)< 0)
		latch->Set(DoubleSolenoid::kReverse);
	
	if (stick->GetRawButton(8) and difDrive != true){
		difDrive = true;
	}	else if(stick->GetRawButton(8) and difDrive == true){
			difDrive = false;
		}
	if (difDrive){
		n = 5;
	}	else{
			n = 1;
		}
	
	float x = stick->GetRawAxis(n);
	float y = stick->GetY();
	float LOUT = y-x;
	float ROUT = y+x;
	frontRight->Set(ROUT);
	frontLeft->Set(-LOUT);
	backRight->Set(ROUT);
	backLeft->Set(-LOUT);
}

/**
 * Initialization code for test mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters test mode.
 */
void Robot4815::TestInit() {
}

/**
 * Periodic code for test mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in test mode.
 */
void Robot4815::TestPeriodic() {
}

};

START_ROBOT_CLASS(Robot4815);