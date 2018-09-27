//
//  Localization.h
//  
//
//  written by kimoto 2017/06/30
//
//

#ifndef ____Localization__
#define ____Localization__

#include <stdlib.h>
#include <math.h>

#include "Motor.h"

using namespace ev3api;
//#include "BoundaryDefine.h"


#define WHEEL_R 40.0        //車輪の半径
#define WHEEL_D 128.0       //車輪間の距離

#define PI 3.141592653589793

class Localization{
private:

    float x,y,len,theta,degree;
    float prePhiR;
    float prePhiL;  //prePhiは各車輪の１つ前の測定時の回転角度
    int mapX,mapY;
    
    int time;

    Motor rightWheel;
    Motor leftWheel;
    
public:
    Localization();
    
    void Init();
    void SetPoint();
    
    float GetX();
    float GetY();
    int GetMapX();
    int GetMapY();
    float GetTheta();
    float GetLen();
    void SetLen(float newLen);
    
    void XYtoMap();
    
    void CorrectLocalization(float newX,float newY,float newTheta,int mode);
    
    
};

#endif /* defined(____Localization__) */