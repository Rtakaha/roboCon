#ifndef MOTOR_DRIVE_H
#define MOTOR_DRIVE_H

#include "Motor.h"
#include "Steering.h"

using namespace ev3api;

class MotorDrive
{
private:
  	Motor motor;
  	Steering steer;
public:

	/*not use
	void Drive(int speed, int turn);
	void Turn(int speed, int turn);
	void Stop(); */
	int SteerControl(int to);
	int LineFinder(int lightValue, int to, int direction);
    int LineFinderSetSpeed(int lightValue, int to, int direction,int speed);

};

#endif
