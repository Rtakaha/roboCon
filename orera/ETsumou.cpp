//
// ETsumou.h
//
//
// written by kimoto 2017/07/04
// 値とかは適当です
//

#include "ETsumou.h"

ETsumou::ETsumou():
frontarm(PORT_A),rightWheel(PORT_B),leftWheel(PORT_C), steer(leftWheel,rightWheel),
sonar(PORT_2),color(PORT_3){

	//colorid_t circle = COLOR_NONE;
	//colorid_t block = COLOR_NONE;
	
	//int kensho = 0;
	wall = 0;
	block = 0;

}

//周期関数外で実行する
void ETsumou::PlayETsumou(){

	/*ETsumou台に対し正面を向く　とやりやすそう
	  スタート以降loc.initは使わずSetlen＝０でスピードコースを走り
		ET相撲直前でスタート位置から左90度回転した位置に向くようにする？
		低速ライントレースと直角検知、locの座標を駆使（こわい）*/

	local.Init();

	/*while(detect.StepDetection() == 0){
		steer.setPower(10,0);

		if(sonar.getDistance() < 20){		//進んでる間に壁にぶつかりそうになったら停止
			steer.setPower(0,0);		
			clock.wait(3000);
		}
	}*/
	
	steer.setPower(0,0);				//段差にぶつかったら一旦停止

	while(wall == 0){					//壁が通り過ぎたら
		wall = detect.WallDetection();
	}

	wall = 0;						//初期化

	frontarm.setPWM(10);			//アームを上げる
	clock.wait(500);
	frontarm.stop();
	steer.setPower(25,0);			//登る
	clock.wait(1800);
	steer.setPower(0,0);			//止まる
	frontarm.setPWM(-10);			//下げる
	clock.wait(1000);
	frontarm.stop();

	/*登る動作が安定しなかった場合
	　登りきれなかったことを検知してやり直す仕組みも考慮すべき?
	　ジャイロかdetectかlocalで検知して後退して壁待ちからやり直し*/

	//相撲開始

	//左下
	while(local.GetTheta() < 82){
			leftWheel.setPWM(-20);
        	rightWheel.setPWM(20);
			local.SetPoint();
	}

	steer.setPower(0,0);

	local.Init();
	clock.wait(1000);

	while(local.GetX() < 90){
		steer.setPower(20,0);
		local.SetPoint();
	}

	steer.setPower(0,0);

	block = BlockCheck(COLOR_GREEN);
	if(block == 1){							//色が同じ

		while(local.GetX() < 150){
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 60){		//寄り切り
			leftWheel.setPWM(-20);
        	rightWheel.setPWM(20);
			local.SetPoint();
		}		
		
		steer.setPower(-20,0);				//ブロックを放す
		clock.wait(500);

		while(local.GetTheta() < 180){
			leftWheel.setPWM(-20);
        	rightWheel.setPWM(20);
			local.SetPoint();
		}

		while(local.GetX() > 10){
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > 100){
			leftWheel.setPWM(20);
        	rightWheel.setPWM(-20);
			local.SetPoint();
		}

		while(local.GetY() < 10){
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 180){
			leftWheel.setPWM(-20);
        	rightWheel.setPWM(20);
			local.SetPoint();
		}

		steer.setPower(0,0);	

	}

	else if(block == 0){					//色が違う
		while(local.GetX() < 250){
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetX() > 110){
			steer.setPower(-20,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 180){		//180旋回
			leftWheel.setPWM(-20);
        	rightWheel.setPWM(20);
			local.SetPoint();
		}				

		while(local.GetX() > 30){			//線の交点まで戻る
			steer.setPower(20,0);
			local.SetPoint();
		}

		steer.setPower(0,0);

	}

	block = 0;


	//右下
	while(local.GetX() > -100){
		steer.setPower(20,0);
		local.SetPoint();
	}

	steer.setPower(0,0);

	block = BlockCheck(COLOR_YELLOW);
	if(block == 1){							//色が同じ

		while(local.GetX() > -170){
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > 120){		//寄り切り
			leftWheel.setPWM(20);
        	rightWheel.setPWM(-20);
			local.SetPoint();
		}		
		
		steer.setPower(-20,0);				//ブロックを放す
		clock.wait(500);

		while(local.GetTheta() < 360){		
			leftWheel.setPWM(-20);
        	rightWheel.setPWM(20);
			local.SetPoint();
		}

		while(local.GetX() < 0){
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > 280){		
			leftWheel.setPWM(20);
        	rightWheel.setPWM(-20);
			local.SetPoint();
		}	
		
	}

	else if(block == 0){					//色が違う
		while(local.GetX() > -250){
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetX() < 0){
			steer.setPower(-20,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 270){
			leftWheel.setPWM(-20);
        	rightWheel.setPWM(20);
			local.SetPoint();
		}

		while(local.GetY() > -100){
			steer.setPower(20,0);
			local.SetPoint();
		}		
	}

	block = 0;

/*
	//2列目へ移動
	while(local.GetX() >= 300){			//線の交点まで戻る
			steer.setPower(10,0);
			//linetrace.PIDSetRun(10,0.38, 0.06, 0.025);
	}

	//左上
	linetrace.Rotate(-90);

	while(local.GetY() >= 70){
		steer.setPower(10,0);
	}

	steer.setPower(0,0);

	if(BlockCheck(COLOR_BLUE) == 1){	//色が同じ

		while(local.GetY()  >= 100){
			steer.setPower(10,0);
		}

		linetrace.Rotate(-45);			//左30度旋回して寄り切り
		
		steer.setPower(-10,0);			//ブロックを放す
		clock.wait(1000);

		linetrace.Rotate(-90);			//線の交点まで戻る

		while(local.GetY()  <= 0){
			steer.setPower(10,0);
		}

		linetrace.Rotate(-45);
	}

	else if(BlockCheck(COLOR_BLUE) == 0){	//色が違う
		steer.setPower(10,0);				//押し出し
		clock.wait(3000);

		steer.setPower(-10,0);
		clock.wait(5000);

		linetrace.Rotate(180);				//180旋回

		while(local.GetY() <= 0){			//線の交点まで戻る
			steer.setPower(10,0);
		}
		
	}

	//右上
	while(local.GetY() <= -70){			
			steer.setPower(10,0);
	}
	steer.setPower(0,0);

	if(BlockCheck(COLOR_RED) == 1){	//色が同じ

		while(local.GetY()  >= -100){
			steer.setPower(10,0);
		}

		linetrace.Rotate(45);			//左30度旋回して寄り切り
		
		steer.setPower(-10,0);			//ブロックを放す
		clock.wait(1000);

		linetrace.Rotate(90);			//線の交点まで戻る

		while(local.GetY()  >= 0){
			steer.setPower(10,0);
		}

		linetrace.Rotate(225);
	}

	else if(BlockCheck(COLOR_RED) == 0){	//色が違う
		steer.setPower(10,0);				//押し出し
		clock.wait(3000);

		steer.setPower(-10,0);
		clock.wait(5000);

		linetrace.Rotate(180);				//180旋回

		while(local.GetY() >= 0){			//線の交点まで戻る
			steer.setPower(10,0);
		}

		linetrace.Rotate(90);
		
	}

*/

}

int ETsumou::BlockCheck(colorid_t circle){
	int ans = 0;

	//ブロックが正面にある状態で
	frontarm.setPWM(5);
	clock.wait(2000);
	frontarm.stop();

	while(frontarm.getCount() < 100){
		if(color.getColorNumber() == circle){
			ans = 1;
			break;
		}
		else{
			frontarm.setPWM(5);
		}
	}

	while(1){
		frontarm.setPWM(-5);
		if(frontarm.getCount() <= 0){ break; }
	}

	return ans;
	
}




