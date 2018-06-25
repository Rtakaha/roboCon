/*
AreaManager.h
コースを区間ごとに分け正確に走れるように管理

written by kimoto 2017/06/29
*/

#ifndef AreaManager_H_
#define AreaManager_H_

//#include "BoundaryDefine.h"

#include "LineTrace.h"
#include "Detect.h"
#include "Localization.h"
#include "ETsumou.h"
#include "Block.h"

#include "ev3api.h"
#include "util.h"
#include "Motor.h"
#include "Steering.h"
#include "Clock.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
//#include "GyroSensor.h"

using namespace ev3api;


enum cource{
	L_cource = 1,
	R_cource = 2,
};

enum R_state{
	START,			//スタート動作
	CALIBRATION,	//各種初期化
	STRAIGHT1,		//スタート直後の直線
	CURVE1,			//右カーブ
	CURVE2,			//ゲート１通過から２通過後の曲線
	STRAIGHT2,		//スピードコースゴールまで
	CURVE3,			//ゴールから相撲まで
	ETSUMOU,		//相撲エリアへ入るところから出るところまで
	KENSHO,			//懸賞を置いてラインに復帰まで
	PARKING,		//直角駐車
	END             //終了
};

enum L_state{
	START_L,		//スタート動作
	STRAIGHT_L1,	//スタート直後の直線
	CURVE_L1,		//右カーブ
	STRAIGHT_L2,	//スピードコースゴールまで
	CURVE_L2,		//ゲート１通過から２通過までの曲線
	STRAIGHT_L3,	//スピードコースゴールまで
	CURVE_L3,		//直線までの急なカーブ
	STRAIGHT_L4,	//スピードコースゴールまで
	CURVE_L4,		//直線までの急なカーブ
	STRAIGHT_L5,	//スピードコースゴールまで
	BRANCH,			//黒線分岐
	BLOCK,			//ブロック並べ開始位置から終了位置まで
	PARKING_L,		//縦列駐車
	END_L			//終了
};

/*
enum L_state{
	START,			//スタート動作
	CALIBRATION,	//各種初期化
	STRAIGHT1,		//スタート直後の直線
	CURVE1,			//右カーブ
	CURVE2,			//ゲート１通過から２通過後の曲線
	STRAIGHT2,		//スピードコースゴールまで
	CURVE3,			//ゴールからブロック並べスタート地点まで
	BLOCK,			//ブロック並べ開始位置から終了位置まで
	PARKING			//縦列駐車
};*/

class AreaManager{
private:
	int currentcource;
	int currentstate;
	int curve_f = 0;
	int correct = 0;
	int impact_on = 0;
	int search = 0;
	int color_on = 0;
	int detect_on = 0;
	int speed = 0;//速度を落とす(上げる)時に変化を滑らかにする
	int black_on = 0;
	int second = 0;
	int park = 0;
	int park_l = 0;
	int park_l2 = 0;
	int park_l3 = 0;
	int park_l4 = 0;

	int black; //キャリブレーション後の黒の値
	int white;
	float normalize;
	int gray;
	int code = 0;
	int codecount = 0;
	int kensho = 0; //懸賞を持っているか否か

	TouchSensor touch;
	SonarSensor sonar;
	ColorSensor color; //Lコース開始の緑色検知用
	//GyroSensor gyro;
	Motor frontarm;
  	Motor rightWheel;
  	Motor leftWheel;
  	Steering steer;
	Clock clock;
	Block block;

	LineTrace linetrace;
	Detect detect;
	Localization local;
	ETsumou sumou;

public:
	AreaManager();
	void Init();
	void Manager();
	void NumberDisplay(int codecount);
	void regularLine();
};

#endif //AreaManager_H_