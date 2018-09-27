//
//  Detect.h
//
//
//  Written by kimoto on 2017/06/25.
//
//

#ifndef ____Detect__
#define ____Detect__

#include "ev3api.h"
#include "Clock.h"
#include "GyroSensor.h"
#include "ColorSensor.h"
#include "SonarSensor.h"

using namespace ev3api;
//#include "BoundaryDefine.h"

#define LIGHTDATA_N 10//光センサの記録回数
#define SONARDATA_N 10//ソナーセンサの記録回数
#define GRAY_DETECT 10//グレー検知の値

class Detect{
    
private:
    int angularVelocityHistory[10]; //段差検出に使用 角速度を記憶
    int rightAngleHistory[10];      //直角検出に使用 光センサ値を記憶
    int lightHistory[LIGHTDATA_N];  //灰色検出に使用 光センサ値を記憶
    int sonarHistory[SONARDATA_N];  //障害物検知に使用
    int sonarvalue;                 //超音波センサの値を格納
    int sonarflag;                  //障害物検知誤作動防止

    Clock clock;
    SonarSensor sonar;
    ColorSensor color;
    GyroSensor gyro;
    

public:
    Detect();
    void Init();
    int StepDetection();            //段差検出関数　return1 検出
    int RightAngleDetection();      //直角検出関数　return1 検出
    int GrayDetection();            //灰色検出関数　return1 検出
    int WallDetection(int distance);//壁検出関数 return1 検出
    int GetSonarvalue();            //壁検知関数を使用した時、読み取った最新のsonarvalueを読み出す（WallDetectionとセットで使う）
    
    void RightAngleReset(){
        for(int i = 0; i < 10; i++){
            rightAngleHistory[i] = 0;//光センサの値を初期化
        }
    }
    void StepDetectionReset(){
        for(int i = 0; i < 10; i++){
            angularVelocityHistory[i] = 600;//アンプ値600
        }
    }
    void GrayDetectionReset(){
        for(int i = 0; i < LIGHTDATA_N; i++){
            lightHistory[i] = 0;//初期化
        }
    }
    void WallDetectionReset(){
        for(int i = 0; i < SONARDATA_N; i++){
            sonarHistory[i] = 0;//初期化
        }
        sonarflag = 0;
        sonarvalue = 255;
    }
    
    
};

#endif