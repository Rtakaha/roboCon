//
// ETsumou.h
//
//
// written by kimoto 2017/07/04
// 値とかは適当です
//

#include "ETsumou.h"

//debug
// Bluetooth仮想シリアルポートのファイルをオープンする
	//FILE *bt = ev3_serial_open_file(EV3_SERIAL_BT);

ETsumou::ETsumou():
frontarm(PORT_A),rightWheel(PORT_B),leftWheel(PORT_C), steer(leftWheel,rightWheel),
sonar(PORT_2),color(PORT_3){

//当日朝確定するブロックのサークルの順番（適宜変更）
	circle_color[0] = COLOR_GREEN;		//１列目左
	circle_color[1] = COLOR_YELLOW;		//１列目右
	circle_color[2] = COLOR_BLUE;		//２列目左
	circle_color[3] = COLOR_RED;		//２列目右
	circle_color[4] = COLOR_RED;		//３列目左
	circle_color[5] = COLOR_BLUE;		//３列目右
	circle_color[6] = COLOR_YELLOW;		//４列目左
	circle_color[7] = COLOR_GREEN;		//４列目右

	kensho = 0;
	wall = 0;
	wallflag = 255;
	block = 0;
	circle = 0;

}

void ETsumou::Init(){
	//frontarm.setPWM(-5);
	//clock.wait(1000);
	//frontarm.reset();

	rightWheel.reset();
	leftWheel.reset();
	
	local.Init();
	detect.Init();

	//linetrace.Calibration();
	//clock.wait(1000);

	/*while(1){

		if(ev3_button_is_pressed(RIGHT_BUTTON)){
			msg_f("R Cource", 1);
			//currentcource = R_cource;
			//currentstate = START;
			linetrace.SetEdge(1);	//右回転から（合ってる？）
		
			break;
		}

		if(ev3_button_is_pressed(LEFT_BUTTON)){
			msg_f("L Cource", 1);
			//currentcource = L_cource;
			//currentstate = START2;
			linetrace.SetEdge(1);

			break;
		}
	}*/

}

//周期関数外で実行する
int ETsumou::PlayETsumou(){

	//線路の手前で競技台の正面を見て
	Init();
	steer.setPower(0,0);
	
	while(wall == 0){				//壁が通り過ぎたら
		wall = detect.WallDetection(140);
	}
	detect.WallDetectionReset();
	wall = 0;						//初期化
	
	clock.wait(500);
	frontarm.setPWM(20);			//アームを上げる
	clock.wait(500);
	frontarm.stop();
	steer.setPower(40,0);			//線路を渡る
	clock.wait(1200);
	steer.setPower(0,0);			//止まる
	frontarm.setPWM(-20);			//アームを下げる
	clock.wait(500);
	frontarm.stop();

	//相撲開始
	//PIDするときの閾値を設定し直す必要あり？?

	/*********************************
	１列目左
	**********************************/
	//左を向く
	leftWheel.setPWM(-20);
    rightWheel.setPWM(20);
    clock.wait(500);
	while(color.getColorNumber() != COLOR_BLACK){
		leftWheel.setPWM(-10);
    	rightWheel.setPWM(10);
	}
	/*ラインを探しながら左を向く
	leftWheel.setPWM(-20);
    rightWheel.setPWM(20);
    clock.wait(300);
	while(color.getColorNumber() != COLOR_BLACK){
		leftWheel.setPWM(-5);
    	rightWheel.setPWM(10);
    	if(local.GetTheta() > 95){
    		while(local.GetTheta() > 85){
    			leftWheel.setPWM(10);
    			rightWheel.setPWM(-5);
    			if(color.getColorNumber() == COLOR_BLACK){ break; break; }
    		}
    	}
	}*/

	steer.setPower(0,0);

	//正確に黒をライントレースしながらサークルの色を検知するまで前進
	//アームにブロックをはめるまで進める
	while(circle == 0){
		// linetrace.PIDSetRun(5,0.20,0,0.009,1);

		if(color.getColorNumber() == circle_color[0]){
			steer.setPower(0,0);
			circle = 1;
		}
	}

	circle = 0;

	//少し後退
	local.Init();
	while(local.GetX() > -35){
		steer.setPower(-20,0);
		local.SetPoint();
	}
	steer.setPower(0,0);

	//色検知
	block = BlockCheck(circle_color[0]);
	if(block == 1){							//色が同じ

		while(local.GetX() < 40){			//ブロックを掴む
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 50){		//寄り切り
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}		
		
		steer.setPower(-20,0);				//ブロックを放す
		clock.wait(500);

		while(local.GetTheta() < 175){		//中央を向く
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}

		while(local.GetX() > -160){			//中央まで
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > 95){		//横ラインを向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		while(local.GetY() < 5){			//ライン上へ
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 175){		//中央を向く
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}

		steer.setPower(0,0);	

	}

	else if(block == 0){					//色が違う
		while(local.GetX() < 80){			//押し出し
			steer.setPower(30,0);
			local.SetPoint();
		}

		while(local.GetX() > -160){			//後退
			steer.setPower(-30,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 155){		//180度旋回
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}				

		steer.setPower(0,0);

	}

	block = 0;


	/*********************************
	１列目右
	**********************************/
	//アームにブロックをはめるまで進める
	while(circle == 0){
		// linetrace.PIDSetRun(5,0.20,0,0.009,-1);

		if(color.getColorNumber() == circle_color[1]){
			steer.setPower(0,0);
			circle = 1;
		}
	}

	circle = 0;

	//少し後退
	local.Init();
	while(local.GetX() > -35){
		steer.setPower(-20,0);
		local.SetPoint();
	}
	steer.setPower(0,0);

	//色検知
	block = BlockCheck(circle_color[1]);
	if(block == 1){							//色が同じ

		while(local.GetX() < 40){			//ブロックを掴む
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > -50){		//寄り切り
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}		
		
		steer.setPower(-20,0);				//ブロックを放す
		clock.wait(500);

		while(local.GetTheta() > -175){		//中央を向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		while(local.GetX() > -160){			//中央まで
			steer.setPower(20,0);
			local.SetPoint();
		}


		while(local.GetTheta() > -265){		//２列目を向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		local.CorrectLocalization(local.GetX(),0.0,90.0,2);

		steer.setPower(0,0);	

	}

	else if(block == 0){					//色が違う
		while(local.GetX() < 80){			//押し出し
			steer.setPower(30,0);
			local.SetPoint();
		}

		while(local.GetX() > -160){			//後退
			steer.setPower(-30,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 85){		//旋回
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}				

		steer.setPower(0,0);

	}

	block = 0;



	//2列目へ移動
	while(local.GetY() < 300){			//交差点まで進んで停止
		//linetrace.PIDSetRun(5,0.20,0,0.01,1);
		steer.setPower(35,0);
		local.SetPoint();
	}

	steer.setPower(0,0);

	/*********************************
	２列目左
	**********************************/
	while(local.GetTheta() < 175){
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
	}

	steer.setPower(0,0);

	//アームにブロックをはめるまで進める
	while(circle == 0){
		// linetrace.PIDSetRun(5,0.20,0,0.009,1);

		if(color.getColorNumber() == circle_color[2]){
			steer.setPower(0,0);
			circle = 1;
		}
	}

	circle = 0;

	//少し後退
	local.Init();
	while(local.GetX() > -35){
		steer.setPower(-20,0);
		local.SetPoint();
	}
	steer.setPower(0,0);


	//色検知
	block = BlockCheck(circle_color[2]);
	if(block == 1){							//色が同じ

		while(local.GetX() < 40){			//ブロックを掴む
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > -50){		//寄り切り
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}		
		
		steer.setPower(-20,0);				//ブロックを放す
		clock.wait(500);

		while(local.GetTheta() > -175){		//中央を向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		while(local.GetX() > -160){			//中央まで
			steer.setPower(20,0);
			local.SetPoint();
		}


		while(local.GetTheta() < -95){		//横ラインを向く
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}

		while(local.GetY() > 5){			//ライン上へ
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > -175){		//中央を向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		steer.setPower(0,0);	

	}

	else if(block == 0){					//色が違う
		while(local.GetX() < 80){			//押し出し
			steer.setPower(30,0);
			local.SetPoint();
		}

		while(local.GetX() > -160){			//後退
			steer.setPower(-30,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 155){		//180度旋回
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}				

		steer.setPower(0,0);

	}

	block = 0;

	/*********************************
	２列目右
	**********************************/
	//アームにブロックをはめるまで進める
	while(circle == 0){
		// linetrace.PIDSetRun(5,0.20,0,0.009,-1);

		if(color.getColorNumber() == circle_color[3]){
			steer.setPower(0,0);
			circle = 1;
		}
	}

	circle = 0;

	//少し後退
	local.Init();
	while(local.GetX() > -35){
		steer.setPower(-20,0);
		local.SetPoint();
	}
	steer.setPower(0,0);


	//色検知
	block = BlockCheck(circle_color[3]);
	if(block == 1){							//色が同じ

		while(local.GetX() < 40){			//ブロックを掴む
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 50){		//寄り切り
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}		
		
		steer.setPower(-20,0);				//ブロックを放す
		clock.wait(500);

		while(local.GetTheta() < 175){		//中央を向く
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}

		while(local.GetX() > -160){			//中央まで
			steer.setPower(20,0);
			local.SetPoint();
		}


		while(local.GetTheta() > 95){		//横ラインを向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);
			local.SetPoint();
		}
		steer.setPower(0,0);

		while(local.GetY() < 0){			//ライン上へ
			steer.setPower(20,0);
			local.SetPoint();
		}


		steer.setPower(0,0);	

	}

	else if(block == 0){					//色が違う
		while(local.GetX() < 80){			//押し出し
			steer.setPower(30,0);
			local.SetPoint();
		}

		while(local.GetX() > -150){			//後退
			steer.setPower(-30,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 85){		//90度旋回
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}				

		steer.setPower(0,0);

	}

	block = 0;


	//3列目へ
	/*while(local.GetY() < 30){
		linetrace.PIDSetRun(5,0.20,0,0.009,1);
		local.SetPoint();
	}*/
	steer.setPower(0,0);
	local.Init();

	//壁が通り過ぎたら
	while(wall == 0){
		wall = detect.WallDetection(90);
		if(wallflag > detect.GetSonarvalue()){
			wallflag = detect.GetSonarvalue();	//壁を検出するまでに読み取った最も近い距離を保存
		}
		//fprintf(bt,"wallflag = %d\r\n",wallflag);
	}
	detect.WallDetectionReset();
	wall = 0;

	clock.wait(500);
	frontarm.setPWM(20);			//アームを上げる
	clock.wait(500);
	frontarm.stop();
	steer.setPower(40,0);			//線路を渡る
	clock.wait(1100);
	steer.setPower(0,0);			//止まる
	frontarm.setPWM(-20);			//アームを下げる
	clock.wait(500);
	frontarm.stop();


	/*********************************
	3列目左
	**********************************//*
	//左を向く
	leftWheel.setPWM(-20);
    rightWheel.setPWM(20);
    clock.wait(500);
	while(color.getColorNumber() != COLOR_BLACK){
		leftWheel.setPWM(-10);
    	rightWheel.setPWM(10);
	}
	/*ラインを探しながら左を向く
	leftWheel.setPWM(-20);
    rightWheel.setPWM(20);
    clock.wait(300);
	while(color.getColorNumber() != COLOR_BLACK){
		leftWheel.setPWM(-5);
    	rightWheel.setPWM(10);
    	if(local.GetTheta() > 95){
    		while(local.GetTheta() > 85){
    			leftWheel.setPWM(10);
    			rightWheel.setPWM(-5);
    			if(color.getColorNumber() == COLOR_BLACK){ break; break; }
    		}
    	}
	}*

	steer.setPower(0,0);

	//アームにブロックをはめるまで進める
	while(circle == 0){
		linetrace.PIDSetRun(5,0.20,0,0.009,1);

		if(color.getColorNumber() == circle_color[4]){
			steer.setPower(0,0);
			circle = 1;
		}
	}

	circle = 0;

	//少し後退
	local.Init();
	while(local.GetX() > -35){
		steer.setPower(-20,0);
		local.SetPoint();
	}
	steer.setPower(0,0);

	//色検知
	block = BlockCheck(circle_color[4]);
	if(block == 1){							//色が同じ

		while(local.GetX() < 40){			//ブロックを掴む
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 50){		//寄り切り
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}		
		
		steer.setPower(-20,0);				//ブロックを放す
		clock.wait(500);

		while(local.GetTheta() < 175){		//中央を向く
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}

		while(local.GetX() > -160){			//中央まで
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > 95){		//横ラインを向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		while(local.GetY() < 10){			//ライン上へ
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 175){		//中央を向く
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}

		steer.setPower(0,0);	

	}

	else if(block == 0){					//色が違う
		while(local.GetX() < 80){			//押し出し
			steer.setPower(30,0);
			local.SetPoint();
		}

		while(local.GetX() > -160){			//後退
			steer.setPower(-30,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 170){		//180度旋回
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}				

		steer.setPower(0,0);

	}

	block = 0;


	/*********************************
	3列目右
	**********************************
	//アームにブロックをはめるまで進める
	while(circle == 0){
		linetrace.PIDSetRun(5,0.20,0,0.009,-1);

		if(color.getColorNumber() == circle_color[5]){
			steer.setPower(0,0);
			circle = 1;
		}
	}

	circle = 0;

	//少し後退
	local.Init();
	while(local.GetX() > -35){
		steer.setPower(-20,0);
		local.SetPoint();
	}
	steer.setPower(0,0);

	//色検知
	block = BlockCheck(circle_color[5]);
	if(block == 1){							//色が同じ

		while(local.GetX() < 40){			//ブロックを掴む
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > -50){		//寄り切り
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}		
		
		steer.setPower(-20,0);				//ブロックを放す
		clock.wait(500);

		while(local.GetTheta() > -175){		//中央を向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		while(local.GetX() > -160){			//中央まで
			steer.setPower(20,0);
			local.SetPoint();
		}


		while(local.GetTheta() > -265){		//4列目を向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		//-270 -> 90
		local.CorrectLocalization(local.GetX(),0.0,90.0,2);

		/*while(local.GetY() < 50){			//懸賞位置手前まで
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetY() < 140){			//懸賞位置まで
			linetrace.PIDSetRun(5,0.20,0,0.009,1);
			//steer.setPower(20,0);
			local.SetPoint();
		}*

		steer.setPower(0,0);	

	}

	else if(block == 0){					//色が違う
		while(local.GetX() < 80){			//押し出し
			steer.setPower(30,0);
			local.SetPoint();
		}

		while(local.GetX() > -160){			//後退
			steer.setPower(-30,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 85){		//旋回
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}

		/*while(local.GetY() < 50){			//懸賞位置横まで
			steer.setPower(30,0);
			local.SetPoint();
		}

		while(local.GetY() < 140){			
			linetrace.PIDSetRun(5,0.20,0,0.009,1);
			//steer.setPower(20,0);
			local.SetPoint();
		}*				

		steer.setPower(0,0);

	}

	block = 0;


	//4列目へ移動
	while(local.GetY() < 300){			//交差点まで進んで停止
		//linetrace.PIDSetRun(5,0.20,0,0.01,1);
		steer.setPower(20,0);
		local.SetPoint();
	}

	steer.setPower(0,0);


	/*********************************
	4列目左
	**********************************
	while(local.GetTheta() < 175){
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
	}

	steer.setPower(0,0);

	//アームにブロックをはめるまで進める
	while(circle == 0){
		linetrace.PIDSetRun(5,0.20,0,0.009,1);

		if(color.getColorNumber() == circle_color[6]){
			steer.setPower(0,0);
			circle = 1;
		}
	}

	circle = 0;

	//少し後退
	local.Init();
	while(local.GetX() > -35){
		steer.setPower(-20,0);
		local.SetPoint();
	}
	steer.setPower(0,0);


	//色検知
	block = BlockCheck(circle_color[6]);
	if(block == 1){							//色が同じ

		while(local.GetX() < 40){			//ブロックを掴む
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > -50){		//寄り切り
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}		
		
		steer.setPower(-20,0);				//ブロックを放す
		clock.wait(500);

		while(local.GetTheta() > -175){		//中央を向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		while(local.GetX() > -160){			//中央まで
			steer.setPower(20,0);
			local.SetPoint();
		}


		while(local.GetTheta() < -95){		//横ラインを向く
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}

		while(local.GetY() > 10){			//ライン上へ
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() > -175){		//中央を向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		steer.setPower(0,0);	

	}

	else if(block == 0){					//色が違う
		while(local.GetX() < 80){			//押し出し
			steer.setPower(30,0);
			local.SetPoint();
		}

		while(local.GetX() > -160){			//後退
			steer.setPower(-30,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 170){		//180度旋回
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}				

		steer.setPower(0,0);

	}

	block = 0;

	/*********************************
	4列目右
	**********************************
	//アームにブロックをはめるまで進める
	while(circle == 0){
		linetrace.PIDSetRun(5,0.20,0,0.009,-1);

		if(color.getColorNumber() == circle_color[7]){
			steer.setPower(0,0);
			circle = 1;
		}
	}

	circle = 0;

	//少し後退
	local.Init();
	while(local.GetX() > -35){
		steer.setPower(-20,0);
		local.SetPoint();
	}
	steer.setPower(0,0);


	//色検知
	block = BlockCheck(circle_color[7]);
	if(block == 1){							//色が同じ

		while(local.GetX() < 40){			//ブロックを掴む
			steer.setPower(20,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 50){		//寄り切り
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}		
		
		steer.setPower(-20,0);				//ブロックを放す
		clock.wait(500);

		while(local.GetTheta() < 175){		//中央を向く
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}

		while(local.GetX() > -160){			//中央まで
			steer.setPower(20,0);
			local.SetPoint();
		}


		while(local.GetTheta() > 98){		//横ラインを向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

		while(local.GetY() < 0){			//ライン上へ
			steer.setPower(20,0);
			local.SetPoint();
		}


		steer.setPower(0,0);	

	}

	else if(block == 0){					//色が違う
		while(local.GetX() < 80){			//押し出し
			steer.setPower(30,0);
			local.SetPoint();
		}

		while(local.GetX() > -160){			//後退
			steer.setPower(-30,0);
			local.SetPoint();
		}

		while(local.GetTheta() < 85){		//90度旋回
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}				

		steer.setPower(0,0);

	}

	block = 0;*/

	//3.4列目を無視して移動
	while(local.GetX() < 200){
		//linetrace.PIDSetRun(5,0.20,0,0.01,1);
		steer.setPower(50,0);
		local.SetPoint();
	}
	while(local.GetX() < 550){
		// linetrace.PIDSetRun(5,0.20,0,0.01,-1);
		//steer.setPower(20,0);
		local.SetPoint();
	}
	/*while(local.GetX() < 950){
		//linetrace.PIDSetRun(5,0.20,0,0.01,1);
		steer.setPower(50,0);
		local.SetPoint();
	}*/

	steer.setPower(0,0);

	kensho = 0;//GetKensho();

	//壁が通り過ぎたら(２回目の線路横断で一番奥を新幹線が通過した時はぶつかる可能性がないため省略)
	if(wallflag <= 30){
		while(wall == 0){		
			wall = detect.WallDetection(60);
		}
		detect.WallDetectionReset();
		wall = 0;
	}

	
	clock.wait(500);

	steer.setPower(40,0);			//線路を渡る
	clock.wait(1500);
	steer.setPower(0,0);			//止まる

	return kensho;
}


int ETsumou::BlockCheck(colorid_t circle){
	int ans = 0;

	//ブロックが正面にある状態で
	frontarm.setPWM(20);
	clock.wait(500);
	frontarm.stop();

	while(frontarm.getCount() < 100){
		if(color.getColorNumber() == circle){
			ans = 1;
			break;
		}
		else{
			frontarm.setPWM(10);
		}
	}

	while(1){
		frontarm.setPWM(-20);
		if(frontarm.getCount() <= 3){ break; }
	}

	clock.wait(100);
	frontarm.stop();

	return ans;
	
}


int ETsumou::GetKensho(){

	local.Init();
	//懸賞取得位置に移動
	while(local.GetTheta() > -80){			//懸賞方向を向く
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}

	while(local.GetY() > -180){			
			steer.setPower(20,0);
			local.SetPoint();
		}

	steer.setPower(0,0);

	//微調整
	if(local.GetTheta() > -90){
		while(local.GetTheta() > -85){
			leftWheel.setPWM(15);
        	rightWheel.setPWM(-15);

			local.SetPoint();
		}
	}
	else if(local.GetTheta() < -90){
		while(local.GetTheta() < -95){
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}
	}
	else{}

	steer.setPower(0,0);

	while(local.GetY() > -230){			
			steer.setPower(20,0);
			local.SetPoint();
		}


	while(frontarm.getCount() < 100){		//懸賞取得
			frontarm.setPWM(20);
		}
	frontarm.stop();
		
	while(local.GetY() < 0){				//後退
			steer.setPower(-20,0);	
			local.SetPoint();
		}

	while(local.GetTheta() < -10){			//ゴール方向を向く
			leftWheel.setPWM(-15);
        	rightWheel.setPWM(15);

			local.SetPoint();
		}

	steer.setPower(0,0);

	//fprintf(bt, "%d\n",(int)color.getColorNumber());
	if(color.getColorNumber() == COLOR_NONE){ 
		while(frontarm.getCount() > 3){ frontarm.setPWM(-20); }
		return 0; 
	}
	else{ 
		return 1; }	

}


void ETsumou::SetLine(int black, int white, float normalize, int gray){
	linetrace.Setblack(black);
	linetrace.Setwhite(white);
	linetrace.Setnormalize(normalize);
	linetrace.Setgray(gray-25);
}




