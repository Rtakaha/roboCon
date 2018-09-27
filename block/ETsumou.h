//
// ETsumou.h
//
//
// written by kimoto 2017/07/04
//

#ifndef ETsumou_H__
#define ETsumou_H__


#include "ev3api.h"
#include "Clock.h"
#include "Motor.h"
#include "Steering.h"
#include "ColorSensor.h"
#include "LineTrace.h"
#include "Localization.h"
#include "Detect.h"
//#include "BoundaryDefine.h"

using namespace ev3api;

class ETsumou{

private:

	colorid_t circle_color[8];
	int kensho;			//懸賞を取ったかどうか 1:取った 0:取ってない
	int wall;			//壁を検出したら１
	int wallflag;		//壁の最も近かった時の距離を保存
	int block;			//ブロックが同じ色なら１
	int circle;			//指定した色の円を読んだら１

	Clock clock;
	Motor frontarm;
	Motor rightWheel;
	Motor leftWheel;
    Steering steer;
    SonarSensor sonar;
    ColorSensor color;
    LineTrace linetrace;
    Localization local;
    Detect detect;

public:
	ETsumou();
	void Init();
	int  PlayETsumou();
	int  BlockCheck(colorid_t circle);
	int  GetKensho();
	void SetLine(int black, int white, float normalize, int gray);
	
};

#endif //ETsumou_H__
