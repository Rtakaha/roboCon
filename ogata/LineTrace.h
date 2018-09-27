/*
* Linetrace.h
* written by kimoto on 2017/06/16
*/

#ifndef LINETRACE_H_				
#define LINETRACE_H_


#include "ev3api.h"
#include "util.h"
#include "TouchSensor.h"
#include "ColorSensor.h"
#include "Motor.h"
#include "Steering.h"
#include "PID.h"
#include "util.h"

//#include "BoundaryDefine.h"
//#include "MotorDrive.h"

using namespace ev3api;		

class LineTrace {					
private:
	Motor rightWheel;
	Motor leftWheel;
	TouchSensor touch;
    ColorSensor color;
    Steering steer;
	PID pid;

    int colorHistory[10];    
    int white;
    int black;
    int gray;
    float normalize;
    
    int edge;	//曲がる方向(±1)

public:
	LineTrace();
    
    void SetEdge(int newedge);		

    void PIDSetRun(int pwm,float newp,float newi,float newd,float curve);
    void Calibration();
    float NormalizeLine(int value);
    void Rotate(int gyr);
    void ColorHistoryReset(){
        for(int i = 0; i < 10; i++){
            colorHistory[i] = -1;//初期化
        }
    }
    //int PIDCurveSlow(int speed);

    int Getblack();
    int Getwhite();
    float Getnormalize();
    int Getgray();
    void Setblack(int black2);
    void Setwhite(int white2);
    void Setnormalize(float normalize2);
    void Setgray(int point){
        gray = point;
    }
};

#endif /* LINETRACE_H_ */