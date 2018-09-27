/*
* PID.h
* written by kimoto on 2017/06/16
*/

#ifndef PID_H_
#define PID_H_

//#include "BoundaryDefine.h"
#include "Motor.h"
using namespace ev3api;

class PID {
private:
    Motor rightWheel;
    Motor leftWheel;
    
    float Kp,Ki,Kd;				//PID制御係数
	float delta;				//測定間隔
    float u[2];

	int diff[3];				//偏差
	float integral;				//積分値
    
    int motordiff[2];			//モーターの偏差
    float Kph,Kdh;				//モーターの偏差によるPID制御の係数*/

public:
	PID();
    
    float PIDSetRun(int sensorval,int targetval,float newp,float newi,float newd/*,float curve*/);
    //float PIDCurveSlow(int sensorval,int targetval);	使わない予定

};

#endif /* PID_H_ */

