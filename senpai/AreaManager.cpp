/*
AreaManager.h
written by kimoto 2017/06/29
memo
値は適当　マップを前提とした調整が必要
記述途中　ぶっちゃけ値がわかんないので適当書くしかできない
体裁はこんな感じ
*/

#include "AreaManager.h"

//FILE *bt = ev3_serial_open_file(EV3_SERIAL_BT);

AreaManager::AreaManager():
touch(PORT_1),sonar(PORT_2),color(PORT_3),/*,gyro(PORT_4),*/
frontarm(PORT_A),rightWheel(PORT_B),leftWheel(PORT_C),steer(leftWheel,rightWheel){

}

void AreaManager::Init(){

	//初期値の設定や変数の初期化処理はここで

	init_f("AreaManager");

	leftWheel.reset();
	rightWheel.reset();

	//アームを確実に下に向ける
	frontarm.setPWM(-5); //回転方向？
	clock.wait(1000);
	frontarm.reset();

	local.Init();
	detect.Init();

	//fprintf(bt, "BlueTooth接続成功\r\n");

	/*//コード入力
	while(1){
		NumberDisplay(codecount);

		msg_f("Set 5-figures",2);
		if(ev3_button_is_pressed(UP_BUTTON)){
			codecount=codecount+1;
		}
		clock.wait(500);
		if(ev3_button_is_pressed(DOWN_BUTTON)){
			codecount=codecount-1;
		}

		if(ev3_button_is_pressed(LEFT_BUTTON)){
			code=codecount*10000;
			codecount=0;
			break;
		}
    }
    while(1){
    	NumberDisplay(codecount);

		msg_f("Set 4-figures",2);
		if(ev3_button_is_pressed(UP_BUTTON)){
			codecount=codecount+1;
		}
		clock.wait(500);
		if(ev3_button_is_pressed(DOWN_BUTTON)){
			codecount=codecount-1;
		}
		if(ev3_button_is_pressed(LEFT_BUTTON)){
			code=codecount*1000 + code;
			codecount=0;
			break;
		}
    }
    while(1){
    	NumberDisplay(codecount);

		msg_f("Set 3-figures",2);
		if(ev3_button_is_pressed(UP_BUTTON)){
			codecount=codecount+1;
		}
		clock.wait(500);
		if(ev3_button_is_pressed(DOWN_BUTTON)){
			codecount=codecount-1;
		}
		if(ev3_button_is_pressed(LEFT_BUTTON)){
			code=codecount*100 + code;
			codecount=0;
			break;
		}
    }
    while(1){
    	NumberDisplay(codecount);

		msg_f("Set 2-figures",2);
		if(ev3_button_is_pressed(UP_BUTTON)){
			codecount=codecount+1;
		}
		clock.wait(500);
		if(ev3_button_is_pressed(DOWN_BUTTON)){
			codecount=codecount-1;
		}
		if(ev3_button_is_pressed(LEFT_BUTTON)){
			code=codecount*10 + code;
			codecount=0;
			break;
		}
    }
    while(1){
    	NumberDisplay(codecount);

		msg_f("Set 1-figures",2);
		if(ev3_button_is_pressed(UP_BUTTON)){
			codecount=codecount+1;
		}
		clock.wait(500);
		if(ev3_button_is_pressed(DOWN_BUTTON)){
			codecount=codecount-1;
		}
		if(ev3_button_is_pressed(LEFT_BUTTON)){
			code=codecount + code;
			codecount=0;
			break;
		}
    }

    block.codecalculation(code);


    //コード入力終了*/



	//キャリブレーション処理
	linetrace.Calibration();
	black = linetrace.Getblack();
	white = linetrace.Getwhite();
	normalize = linetrace.Getnormalize();
	gray = linetrace.Getgray();

	sumou.SetLine(black, white, normalize, gray);
	block.SetLine(black, white, normalize, gray);
	//Blockも同様に
	

	clock.wait(1000);

	//コース選択　R：右ボタン　L：左ボタン
	msg_f("Select Cource", 1);
	ev3_speaker_play_tone(NOTE_AS4,300);
	
	while(1){

		if(ev3_button_is_pressed(RIGHT_BUTTON)){
			msg_f("R Cource", 1);
			currentcource = R_cource;
			currentstate = START;
			//currentstate = CURVE2;
			//currentstate = STRAIGHT2;
			//currentstate = ETSUMOU;
			linetrace.SetEdge(1);	//右回転から（合ってる？）
			//linetrace.Calibration();
			break;
		}

		if(ev3_button_is_pressed(LEFT_BUTTON)){
			msg_f("L Cource", 1);
			currentcource = L_cource;
			currentstate = START_L;

			/*while(1){
				NumberDisplay(codecount);

				msg_f("Set 5-figures",2);
				if(ev3_button_is_pressed(UP_BUTTON)){
					codecount=codecount+1;
				}
				clock.wait(500);
				if(ev3_button_is_pressed(DOWN_BUTTON)){
					codecount=codecount-1;
				}

				if(ev3_button_is_pressed(LEFT_BUTTON)){
					code=codecount*10000;
					codecount=0;
					break;
				}
    		}
    		while(1){
    			NumberDisplay(codecount);

				msg_f("Set 4-figures",2);
				if(ev3_button_is_pressed(UP_BUTTON)){
					codecount=codecount+1;
				}
				clock.wait(500);
				if(ev3_button_is_pressed(DOWN_BUTTON)){
					codecount=codecount-1;
				}
				if(ev3_button_is_pressed(LEFT_BUTTON)){
					code=codecount*1000 + code;
					codecount=0;
					break;
				}
  		  	}
   			while(1){
   			 	NumberDisplay(codecount);

				msg_f("Set 3-figures",2);
				if(ev3_button_is_pressed(UP_BUTTON)){
					codecount=codecount+1;
				}
				clock.wait(500);
				if(ev3_button_is_pressed(DOWN_BUTTON)){
					codecount=codecount-1;
				}
				if(ev3_button_is_pressed(LEFT_BUTTON)){
					code=codecount*100 + code;
					codecount=0;
					break;
				}
    		}
   		 	while(1){
    			NumberDisplay(codecount);

				msg_f("Set 2-figures",2);
				if(ev3_button_is_pressed(UP_BUTTON)){
					codecount=codecount+1;
				}
				clock.wait(500);
				if(ev3_button_is_pressed(DOWN_BUTTON)){
					codecount=codecount-1;
				}
				if(ev3_button_is_pressed(LEFT_BUTTON)){
					code=codecount*10 + code;
					codecount=0;
					break;
				}
   		 	}
    		while(1){
    			NumberDisplay(codecount);

				msg_f("Set 1-figures",2);
				if(ev3_button_is_pressed(UP_BUTTON)){
					codecount=codecount+1;
				}
				clock.wait(500);
				if(ev3_button_is_pressed(DOWN_BUTTON)){
					codecount=codecount-1;
				}
				if(ev3_button_is_pressed(LEFT_BUTTON)){
					code=codecount + code;
					codecount=0;
					break;
				}
   			}

   			block.codecalculation(code);*/


   			 //コード入力終了

			//currentstate = STRAIGHT_L2;
			linetrace.SetEdge(1);

			break;
		}
	}
}

void AreaManager::Manager(){

	/*int curve_f = 0;
	int gray_on = 0;*/

	/*モータの位置調整などは呼び出し前に済ませておく*/

	if(currentcource == R_cource){

		switch(currentstate){

			case START:

				local.Init();

				msg_f("Ready ...", 1);

				if(touch.isPressed() == 1){
					msg_f("Go !", 1);
					clock.wait(1500);	//数秒待つ
					currentstate = STRAIGHT1;
					//currentstate = STRAIGHT2;
					//currentstate = CURVE3;
					//currentstate = KENSHO;
					//currentstate = PARKING;
					speed = 0;
				}

				break;


			case STRAIGHT1:
				//速め Motorでただの直進指示でもいいかも
				msg_f("1", 3);
				ev3_led_set_color(LED_RED);
				/*linetrace.PIDSetRun(50, 0.35, 0.03, 0.025, 1);*/
				if(speed < 80){
					speed = speed + 1;
				}//ここはいらないかも？
				else if(speed > 80){
					speed = speed - 1;
				}
				linetrace.PIDSetRun(speed, 0.365, 0.05, 0.120, 1);//ずれてる
				//linetrace.PIDSetRun(speed ,0.45, 0.05, 0.120, 1); 

				local.SetPoint();

				if(local.GetLen() >= 2300){/*単位はmm*/
					local.Init();
					currentstate = CURVE1;
				}
				
				break;


			case CURVE1:
				//中速 ただの円弧なのでsteerの関数でもいいかも
				msg_f("2", 3);
				ev3_led_set_color(LED_GREEN);

				if(speed > 50){
					speed = speed - 1;
				}
				else if(speed < 50){
					speed = speed + 1;
				}

				/*linetrace.PIDSetRun(25,0.32, 0.06, 0.025); */
				linetrace.PIDSetRun(speed, 0.40, 0.03, 0.085, 1); 

				local.SetPoint();

				/*if(local.GetLen() >= 750){ 
					local.Init();
					currentstate = CURVE2;
				}*/

				if(local.GetX() < 0){
					local.Init();
					currentstate = CURVE2;
					/*linetrace.SetEdge(-1);*/
				}

				break;


			case CURVE2:
				msg_f("3", 3);
				//ev3_led_set_color(LED_ORANGE);
				//中速 途中で曲がる方向が変わるのでedge切り替えするといいかも
				/*linetrace.PIDSetRun(25,0.32, 0.06, 0.025);*/
				
				if(curve_f == 0){
					ev3_led_set_color(LED_ORANGE);
					if(speed > 50){
						speed = speed - 1;
					}
					else if(speed < 50){
						speed = speed + 1;
					}
					linetrace.PIDSetRun(speed, 0.40, 0.03, 0.085, 1);//30, 0.20, 0.070
				}
				else{
					//linetrace.PIDSetRun(speed, 0.32, 0.03, 0.080, 1);
					ev3_led_set_color(LED_GREEN);
					if(speed > 40){
						speed = speed - 1;
					}
					else if(speed < 40){
						speed = speed + 1;
					}
					linetrace.PIDSetRun(speed, 0.30, 0.03, 0.065, 1);
				}

				local.SetPoint();

				if(local.GetX() > 800 && curve_f == 0){
					curve_f = 1;
					msg_f("curve_finish", 4);
				}
				//if(local.GetY() > 550){
				if(local.GetY() > 550){
					local.Init();
					currentstate = STRAIGHT2;
					/*linetrace.SetEdge(1);*/
				}

				break;

			
			case STRAIGHT2:
				msg_f("4", 3);
				ev3_led_set_color(LED_RED);
				//速め
				//linetrace.PIDSetRun(70, 0.365, 0.05, 0.120, 1);
				if(speed < 80){
					speed = speed + 1;
				}
				else if(speed > 80){
					speed = speed - 1;
				}

				linetrace.PIDSetRun(speed, 0.375, 0.05, 0.120, 1);//p 1.2  d 0.15

				local.SetPoint();

				/*if(local.GetLen() >= 1450 && gray_on == 0){
					msg_f("gray_on", 4);
					linetrace.Setgray(65);
					gray_on = 1;
					linetrace.SetEdge(-1);
				}*/

				if(local.GetLen() >= 1800/*1700*/){
					local.Init();
					linetrace.Setgray(45);
					currentstate = CURVE3;
				}


				//Localizationで走行距離を測って直線が終わったら
				//灰色のラインがあるのでそれを目安でもいいかも(灰色検知きつい)
				break;


			case CURVE3:

				msg_f("5", 3);
				ev3_led_set_color(LED_GREEN);

				/*ev3_motor_stop(EV3_PORT_A, false);
				ev3_motor_set_power(EV3_PORT_A, 0); //frontarm使うなら*/

				/*ev3_motor_reset_counts(EV3_PORT_A);*/
				//ev3_motor_rotate(EV3_PORT_A, 2, 30, false);

				//中速
				/*linetrace.PIDSetRun(15,0.30, 0.06, 0.025);*/

				if(local.GetY() < 500 && detect_on == 0/* && local.GetLen() < 100*/){
					//linetrace.PIDSetRun(20, 0.28, 0.03, 0.035, 1);
					if(speed > 30){
						speed = speed - 1;
					}
					else if(speed < 30){
						speed = speed + 1;
					}
					linetrace.PIDSetRun(speed, 0.30, 0.00, 0.065, 1);//p 0.28 i 0.03 d 0.050
					//steer.setPower(20, 0); //試験用
					local.SetPoint();
				}

				//detect_onがないとelse内のlocal.Initでおかしな挙動になる
				else if(local.GetY() >= 500 && detect_on == 0/* && local.GetLen() >= 100*/){
					detect_on = 1;
					//ev3_motor_reset_counts(EV3_PORT_A);
					//ev3_motor_rotate(EV3_PORT_A, 13, 100, true);//不安定
					//ev3_motor_rotate(EV3_PORT_A, 9, 100, true);
					/*ev3_motor_reset_counts(EV3_PORT_A);*/
				}

				else{

					/*clock.wait(1000);//frontarmの稼動が終わるまで*/

					if(impact_on == 0){

						ev3_led_set_color(LED_ORANGE);
						//fprintf(bt, "角位置 %d \r\n", ev3_motor_get_counts(EV3_PORT_A));

						/*if(/*(ev3_motor_get_power(EV3_PORT_B) >= 22 || ev3_motor_get_power(EV3_PORT_C) >= 22) && //ev3_motor_get_counts(EV3_PORT_A) > 8 || second < 10000){
							//ev3_motor_reset_counts(EV3_PORT_A);
							//linetrace.PIDSetRun(20, 0.28, 0.03, 0.035, 1);
							if(speed > 25){
								speed = speed - 1;
							}
							else if(speed < 25){
								speed = speed + 1;
							} 
							linetrace.PIDSetRun(speed, 0.25, 0.00, 0.040, 1);//d 0.045
							//steer.setPower(20, 0); //試験用
							second++;
							local.SetPoint();
						}
					
						else{

							if(ev3_motor_get_power(EV3_PORT_B) < 22 && ev3_motor_get_power(EV3_PORT_C) < 22){
								msg_f("by motor", 4);
							}
							if(ev3_motor_get_counts(EV3_PORT_A) <= -1){
								msg_f("by frontarm", 3);
							}

							steer.setPower(0, 0);
							impact_on = 1;
							msg_f("impact_on", 4);
							local.Init();
						}*/
					
						if(/*ev3_motor_get_counts(EV3_PORT_A) <= 8 &&*/ second > 870){

							if(ev3_motor_get_power(EV3_PORT_B) < 22 && ev3_motor_get_power(EV3_PORT_C) < 22){
								msg_f("by motor", 4);
							}
							if(ev3_motor_get_counts(EV3_PORT_A) <= -1){
								msg_f("by frontarm", 3);
							}

							steer.setPower(0, 0);
							impact_on = 1;
							msg_f("impact_on", 4);
							local.Init();
						}

						else{
							/*ev3_motor_reset_counts(EV3_PORT_A);*/
							//linetrace.PIDSetRun(20, 0.28, 0.03, 0.035, 1);
							if(speed > 25){
								speed = speed - 1;
							}
							else if(speed < 25){
								speed = speed + 1;
							} 
							linetrace.PIDSetRun(speed, 0.25, 0.00, 0.040, 1);//d 0.045
							//steer.setPower(20, 0); //試験用
							second++;
							local.SetPoint();
							
							/*衝突回避
							if(detect.WallDetection(60) == 1){
								steer.setPower(0, 0);
								detect.Init();
								clock.wait(500);
								linetrace.PIDSetRun(speed, 0.25, 0.00, 0.040, 1);
							}*/
						}

					}

					else{

						if(local.GetX() >= -100){
							steer.setPower(-20, 0);
							local.SetPoint();
						}

						else{
							local.Init();
							leftWheel.setPWM(0);
        					rightWheel.setPWM(0);
        					/*frontarm.setPWM(-10);
							clock.wait(300);
							frontarm.reset();//ETsumouに入る前のfrontarmの固定*/
							currentstate = ETSUMOU;
						/*while(1){
							steer.setPower(0,0);
						}*/
						}

					}
				}

				/*if(local.GetY() <= 700){
					linetrace.PIDSetRun(20, 0.35, 0.03, 0.080, 1);
					local.SetPoint();
				}

				else if(local.GetY() > 700){

					//linetrace.PIDSetRun(20, 0.35, 0.03, 0.080, 1);
					//local.SetPoint();

					if(detect.StepDetection() == 0){
						//steer.setPower(10,0);
						linetrace.PIDSetRun(20, 0.35, 0.03, 0.040, 1);
						local.SetPoint();

						if(sonar.getDistance() < 20){		//進んでる間に壁にぶつかりそうになったら停止
							steer.setPower(0,0);		
							clock.wait(3000);
							steer.setPower(0, 0);
							currentstate = ETSUMOU;
						}

					}
					else{
						leftWheel.setPWM(-20);
    					rightWheel.setPWM(-20);
    					clock.wait(700);
    					steer.setPower(0, 0);
						currentstate = ETSUMOU;
					}

					if(local.GetX() < 0){
						steer.setPower(0, 0);
						currentstate = ETSUMOU;
						linetrace.SetEdge(1);
					}

				}*/

				//Localizationで走行距離を測って直線が終わったら
				break;


			case ETSUMOU:
				msg_f("6", 3);
				ev3_led_set_color(LED_ORANGE);

				ev3_stp_cyc(TRACER_CYC);//ここから周期処理じゃなくなるため、caseの移動はgotoで

				kensho = sumou.PlayETsumou();
				//ETsumou内でkenshoを持っているかどうかの値を0(持ってない)か1(持っている)で返す

				msg_f("ETsumou finish", 3);
				/*while(1){
  					leftWheel.stop();
 					rightWheel.stop();
				}*/

				//sumou.PlayETsumou();

				//懸賞置き場横にいる状態を想定
				local.Init();
				//currentstate = KENSHO;
				goto Kensho;
				break;


			case KENSHO:

Kensho:
				msg_f("7", 3);
				ev3_led_set_color(LED_GREEN);

				/*kensho = 1;//デバッグ用
				frontarm.setPWM(20);			//アームを上げる
				clock.wait(500);
				frontarm.stop();*/

				if(kensho == 1){

					while(local.GetTheta() > -80){
						leftWheel.setPWM(20);
        				rightWheel.setPWM(-20);
						local.SetPoint();
					}

					while(local.GetY() > -150){
						leftWheel.setPWM(20);
        				rightWheel.setPWM(20);
        				local.SetPoint();
					}

					steer.setPower(0,0);
					frontarm.setPWM(-20);
					clock.wait(500);
					frontarm.stop();

					while(local.GetY() < 0){
						leftWheel.setPWM(-20);
        				rightWheel.setPWM(-20);
        				local.SetPoint();
					}

					while(local.GetTheta() < 0){
						leftWheel.setPWM(-20);
        				rightWheel.setPWM(20);
        				local.SetPoint();
					}

					steer.setPower(0,0);

				}

				else if(kensho == 0){steer.setPower(0, 0);}

				//懸賞を持っているかの状態を表す変数をETSUMOU内で用意
				//持っていないならそのまま駐車入り口へ
				//持っているなら所定の場所に移動して置いて帰ってきて駐車入り口へ
				local.Init();
				//goto Parking;
				currentstate = PARKING;
				ev3_sta_cyc(TRACER_CYC);
				break;

			case PARKING:
Parking:
				msg_f("8", 3);
				ev3_led_set_color(LED_ORANGE);

				/*while(local.GetX() < 420){
					//linetrace.PIDSetRun(15, 0.35, 0.00, 0.020, 1);
					linetrace.PIDSetRun(15, 0.25, 0.00, 0.009, -1);
					local.SetPoint();
				}

				while(local.GetTheta() > -90){
					leftWheel.setPWM(20);
        			rightWheel.setPWM(-20);
					local.SetPoint();
				}

				while(local.GetY() > -450){
					//linetrace.PIDSetRun(15, 0.35, 0.00, 0.020, 1);
					linetrace.PIDSetRun(15, 0.25, 0.00, 0.009, -1);
        			local.SetPoint();
				} //非周期処理時

				//停める 直角検知を利用

				leftWheel.stop(); rightWheel.stop();
				//goto End;*/

				if(local.GetX() < 420 && park == 0){
					linetrace.PIDSetRun(15, 0.35, 0.00, 0.030, -1);//d 0.020
					local.SetPoint();
				}
				else if(local.GetX() >= 420 && park == 0){
					ev3_led_set_color(LED_GREEN);
					leftWheel.setPWM(20);
        			rightWheel.setPWM(-20);
					local.SetPoint();
					if(local.GetTheta() < -90){
						park = 1;
					}
				}
				else{//park == 1の時
					linetrace.PIDSetRun(15, 0.35, 0.00, 0.027, 1);//p 0.35
        			local.SetPoint();
				}

				if(local.GetY() < -450){
					local.Init();
					currentstate = END;
				}

				break;

			case END:
End:
				msg_f("9_end", 3);
				ev3_stp_cyc(TRACER_CYC);
				while(1){
					leftWheel.stop(); rightWheel.stop();
				}

				//wup_tsk(MAIN_TASK);

				break;

			default:
				msg_f("error", 4);
				leftWheel.stop(); rightWheel.stop();
				break;

		}
		
	}

	else if(currentcource == L_cource){

		switch(currentstate){

			case START_L:

				local.Init();

				msg_f("Ready ...", 1);

				if(touch.isPressed() == 1){
					msg_f("Go !", 1);
					clock.wait(1500);	//数秒待つ
					currentstate = STRAIGHT_L1;
					//currentstate = CURVE_L1;
					//currentstate = STRAIGHT_L2;
					//currentstate = PARKING_L;
				}

				break;

				//msg_f("Ready ...", 1);

				//if(touch.isPressed() == 1){
				//	msg_f("Go !", 1);
				//	clock.wait(1500);	//数秒待つ
				/*currentstate = BLOCK;
				wup_tsk(MAIN_TASK);*/
				//}

			case STRAIGHT_L1:
				ev3_led_set_color(LED_GREEN);
				//速め Motorでただの直進指示でもいいかも
				msg_f("1", 3);
				/*linetrace.PIDSetRun(50, 0.35, 0.03, 0.025, 1);*/
				//linetrace.PIDSetRun(60, 0.365, 0.05, 0.09, 1);
				/*linetrace.PIDSetRun(60 ,0.45, 0.00, 0.025); */
				if(speed < 80){
					speed = speed + 1;
				}//ここはいらないかも？
				else if(speed > 80){
					speed = speed - 1;
				}
				//linetrace.PIDSetRun(speed, 0.365, 0.05, 0.120, 1);//ずれてる
				linetrace.PIDSetRun(speed, 0.450, 0.05, 0.120, 1);//ずれてる 0.460, 0.60

				local.SetPoint();

				if(local.GetX() >= 2260){/*単位はmm 2260*/
					//local.Init();
					int theta, x;
					x = local.GetX();
					theta = local.GetTheta();
					local.CorrectLocalization(x, 0, theta, 3);
					currentstate = CURVE_L1;
				}

				break;

			case CURVE_L1:
				//中速 ただの円弧なのでsteerの関数でもいいかも
				ev3_led_set_color(LED_RED);
				msg_f("2", 3);
				if(speed < 38){//36
					speed = speed + 1;
				}//ここはいらないかも？
				else if(speed > 38){
					speed = speed - 1;
				}
				/*linetrace.PIDSetRun(25,0.32, 0.06, 0.025); */
				//linetrace.PIDSetRun(speed, 0.38, 0.03, 0.080, 1);
				//linetrace.PIDSetRun(speed, 0.35, 0.03, 0.080, 1);
				//linetrace.PIDSetRun(speed, 0.70, 0.00, 0.080, 1);//イマココ
				//linetrace.PIDSetRun(speed, 0.70, 0.00, 0.060, 1);
				//linetrace.PIDSetRun(speed, 0.720, 0.00, 0.080, 1);良さげ
				//linetrace.PIDSetRun(speed, 0.45, 0.03, 0.065, 1);//0.45, 0.070 折原
				linetrace.PIDSetRun(speed, 0.55, 0.03, 0.070, 1);//いいんじゃないか？

				local.SetPoint();

				/*if(local.GetLen() >= 750){ 
					local.Init();
					currentstate = CURVE2;
				}*/

				if(local.GetX() < 2260){
					//local.Init();
					currentstate = CURVE_L2;
					/*linetrace.SetEdge(-1);*/
				}
				break;

			case CURVE_L2:
				ev3_led_set_color(LED_GREEN);
				msg_f("3", 3);
				//中速 途中で曲がる方向が変わるのでedge切り替えするといいかも
				/*linetrace.PIDSetRun(25,0.32, 0.06, 0.025);*/
				if(speed < 43){//40
					speed = speed + 1;
				}//ここはいらないかも？
				else if(speed > 43){//40
					speed = speed - 1;
				}

				/*if(local.GetX() <= 900 && correct == 0){
					int x2, theta2;
					x2 = local.GetX();
					theta2 = local.GetTheta();
					local.CorrectLocalization(x2, 0, theta2, 3);
					correct == 1;
				}*/

				if(local.GetY() > -500){
					correct = 1;
				}

				//linetrace.PIDSetRun(speed, 0.60, 0.00, 0.05, 1);
				//linetrace.PIDSetRun(speed, 0.55, 0.00, 0.055, 1);
				//linetrace.PIDSetRun(speed, 0.40, 0.00, 0.055, 1);//良さげ
				linetrace.PIDSetRun(speed, 0.35, 0.03, 0.060, 1);//良さげ 0.35, 0.50 折原
				//linetrace.PIDSetRun(speed, 0.38, 0.00, 0.065, 1);//良さげ 0.35, 0.50


				local.SetPoint();

				if(local.GetY() < /*250*/-500 && correct == 1){//-900
					//steer.setPower(0,0);
					local.Init();
					currentstate = CURVE_L3;
					/*linetrace.SetEdge(1);*/
				}

				break;

			case CURVE_L3:
				ev3_led_set_color(LED_RED);

				//while(1){steer.setPower(0, 0);}

				msg_f("4", 3);
				if(speed < 35){//30ok
					speed = speed + 1;
				}//ここはいらないかも？
				else if(speed > 35){
					speed = speed - 1;
				}
				/*if(local.GetY() < 100){
					linetrace.PIDSetRun(speed, 0.38, 0.00, 0.065, 1);
				}*/
				//else{
					//linetrace.PIDSetRun(speed, 0.38, 0.02, 0.065, -1);
				//}
				/*if(black_on == 0){
					leftWheel.setPWM(15);//20
 				    rightWheel.setPWM(-5);
 				    if(color.getColorNumber() != COLOR_BLACK){
 				    	clock.wait(100);
 				    	black_on = 1;
 				    	speed = 0;
 				    }
				}*/
				//else{
					ev3_led_set_color(LED_ORANGE);
					/*if(speed < 35){
						speed = speed + 1;
					}*/
					linetrace.PIDSetRun(speed, 0.47, 0.03, 0.075, 1);//0.55, 0.080ok
				//}

				local.SetPoint();

				if(local.GetX() > 1070){//1000
					local.Init();
					currentstate = STRAIGHT_L2;
					/*linetrace.SetEdge(1);*/
				}

				break;

			case STRAIGHT_L2:
				msg_f("5", 3);

				//linetrace.PIDSetRun(70, 0.36, 0.03, 0.12, 1);
				ev3_led_set_color(LED_RED);
				if(speed < 70){
					speed = speed + 1;
				}//ここはいらないかも？
				else if(speed > 70){
					speed = speed - 1;
				}

				if(black_on == 0){
					leftWheel.setPWM(40);//30
 				    rightWheel.setPWM(38);//28
 				    if(color.getColorNumber() == COLOR_BLACK){
 				    	clock.wait(100);
 				    	black_on = 1;
 				    	speed = 70;
 				    	second++;
 				    }
				}
				else{
					//linetrace.PIDSetRun(60, 0.36, 0.03, 0.12, -1);
					if(second > 100){
						leftWheel.setPWM(38);//28
 				  		rightWheel.setPWM(40);//30
 				  		second--;
					}
					else{
						linetrace.PIDSetRun(speed, 0.450, 0.05, 0.100, -1);//p 0.500 d 0.100
					}
				}

				local.SetPoint();

				if(local.GetLen() >= /*1700*/ 1800){
					local.Init();
					currentstate = BRANCH;
					linetrace.SetEdge(-1); //意味ない(linetrace.PIDSetRunの引数curveをマイナスに設定するべし)
				}

				break;

			case BRANCH:

				msg_f("6", 3);

				//local.SetPoint();

				if(local.GetLen() < 1100){//1000
					linetrace.PIDSetRun(25, 0.320, 0, 0.060, -1);
        			local.SetPoint();
				}
				else{
					ev3_stp_cyc(TRACER_CYC);//この時点で複数のサイクルが残ってるかも？

					ev3_led_set_color(LED_ORANGE);

				/*rightWheel.setPWM(0);
				leftWheel.setPWM(0);
				local.Init();
				clock.wait(200);

				while(local.GetTheta() > -20){
					rightWheel.setPWM(-10);
					leftWheel.setPWM(10);
					local.SetPoint();
				}

				local.Init();
				clock.wait(200);

				while(local.GetLen() < 20){
					rightWheel.setPWM(10);
					leftWheel.setPWM(10);
					local.SetPoint();
				}

				local.Init();
				clock.wait(200);

				while(local.GetTheta() < 20){
					rightWheel.setPWM(10);
					leftWheel.setPWM(-10);
					local.SetPoint();
				}

				rightWheel.setPWM(0);
				leftWheel.setPWM(0);
				local.Init();
				linetrace.ColorHistoryReset();
				clock.wait(400);*/

				//ev3_led_set_color(LED_GREEN);

				/*linetrace.Setgray(gray - 15);

				while(local.GetLen() < 400){

					local.SetPoint();
					linetrace.PIDSetRun(15, 0.25, 0.02, 0.03, -1);
					clock.wait(4);
				}


				linetrace.Setgray(gray);*/

				/*while(local.GetLen() < 700){

					local.SetPoint();
					linetrace.PIDSetRun(50, 0.35, 0.02, 0.07, -1);
					clock.wait(4);//PIDSetRunは4m秒周期のため

				}*/

					//ev3_stp_cyc(TRACER_CYC);

					while(color_on == 0){

					/*if(speed < 15){
						speed = speed + 1;
					}//ここはいらないかも？
					else if(speed > 15){
						speed = speed - 1;
					}*/

						local.SetPoint();
					//linetrace.PIDSetRun(5, 0.15, 0, 0.006, -1);
					//linetrace.PIDSetRun(25, 0.23, 0, 0.02, -1);//元々
						//linetrace.PIDSetRun(15, 0.105, 0, 0.01, -1);//0.2フレすぎ 0.007
						linetrace.PIDSetRun(5,0.05,0,0.009,-1);//5,0.20,0,0.009


						if(color.getColorNumber() == 3){

							rightWheel.setPWM(0);
							leftWheel.setPWM(0);
							color_on = 1;

						}

					//clock.wait(4);

					}

					local.Init();
					goto Block;

				}

				//local.Init();


					/*while(1){

						if(color_on == 0 && search == 0){

							ev3_led_set_color(LED_ORANGE);

							linetrace.PIDSetRun(20, 0.36, 0.03, 0.08, -1);

							clock.wait(1000);
							search = 1;

						}
						else if(color_on == 0 && search == 1){

							rightWheel.setPWM(0);
							leftWheel.setPWM(0);

							if(color.getColorNumber() == 3){
								color_on = 1;
							}

							clock.wait(600);
							search = 0;

						}
						else{

							local.Init();
							//currentstate = BLOCK;
							break;
						}

						clock.wait(4);//擬似周期処理的な

					}*/

				//goto Block;

					/*if(color.getColorNumber() == 3){
						local.Init();
						currentstate = BLOCK;
						linetrace.SetEdge(1);
					}
					else{
						linetrace.PIDSetRun(20, 0.36, 0.03, 0.08, -1);
					}*/

				break;

			case BLOCK:

Block:
				msg_f("block", 5);
				msg_f("Ready ...", 1);
				block.init();

   				block.codecalculation(16210);

				/*while(1){
					if(touch.isPressed() == 1){
						msg_f("Go !", 1);
						clock.wait(1500);	//数秒待つ
						break;
					}
				}*/

				block.blockLineup();

				//wup_tsk(MAIN_TASK);
				//goto Parking_L;
				local.Init();
				currentstate = PARKING_L;
				ev3_sta_cyc(TRACER_CYC);
				break;

			case PARKING_L:
Parking_L:
				msg_f("parking_L", 5);

				/*while(local.GetX() < 500){//ライン上に行くまで直進
					leftWheel.setPWM(20);
        			rightWheel.setPWM(20);
					local.SetPoint();
				}

				local.Init();

				while(local.GetLen() < 700){//駐車場までライントレース
					linetrace.PIDSetRun(10, 0.25, 0.00, 0.02, 1);
					local.SetPoint();
				}

				local.Init();

				while(local.GetTheta() < 90){//駐車場に向けて回転
					leftWheel.setPWM(-20);
        			rightWheel.setPWM(20);
					local.SetPoint();
				}

				while(local.GetY() < 400){//駐車場に入る
					leftWheel.setPWM(20);
        			rightWheel.setPWM(20);
					local.SetPoint();
				}

				while(local.GetTheta() > 0){//縦に駐車
					leftWheel.setPWM(20);
        			rightWheel.setPWM(-20);
					local.SetPoint();
				}

				leftWheel.stop(); rightWheel.stop();
				goto End_L;*/

				if(local.GetX() < 350 && park_l == 0){//350
					leftWheel.setPWM(30);//20
        			rightWheel.setPWM(30);
					local.SetPoint();
				}
				else if(local.GetX() >= 350 && park_l == 0){
					/*local.Init();
					park_l = 1;*/
					leftWheel.setPWM(-20);
        			rightWheel.setPWM(20);
        			local.SetPoint();
        			if(local.GetTheta() > 50){
        				park_l = 1;
        				local.Init();
        			}

				}
				else if(local.GetLen() < 750 && park_l2 ==  0){
					linetrace.PIDSetRun(30, 0.30, 0.00, 0.040, -1);//15 0.35 0.00 0.030 -1
					local.SetPoint();
				}
				else if(local.GetLen() >= 750 && park_l2 ==  0){
					local.Init();
					park_l2 = 1;
				}
				else if(local.GetTheta() < 82 && park_l3 == 0){
					leftWheel.setPWM(-20);
        			rightWheel.setPWM(20);
					local.SetPoint();
				}
				else if(local.GetTheta() >= 82 && park_l3 == 0){
					park_l3 = 1;
				}
				else if(local.GetY() < 150 && park_l4 == 0){
					leftWheel.setPWM(20);
        			rightWheel.setPWM(20);
					local.SetPoint();
				}
				else if(local.GetY() >= 150 && park_l4 == 0){
					leftWheel.setPWM(20);
        			rightWheel.setPWM(-20);
					local.SetPoint();
					if(local.GetTheta() < 0){
						park_l4 = 1;
					}
				}
				else{//park == 1の時
					local.Init();
					currentstate = END_L;
				}

				break;

			case END_L:
End_L:
				msg_f("9_end", 3);
				ev3_stp_cyc(TRACER_CYC);
				while(1){
					leftWheel.stop(); rightWheel.stop();
				}

				//wup_tsk(MAIN_TASK);

				break;

			default:
				msg_f("error", 4);
				leftWheel.stop(); rightWheel.stop();
				break;
		}

	}

/*
	else if(currentcource == L_cource){
		switch(currentstate){
			case START:
				//タッチセンサ？による指定のスタート動作
				currentstate = CALIBRATION;
				break;
			case CALIBRATION:
				//キャリブレーション処理
				currentstate = STRAIGHT1;
				break;
			case STRAIGHT1:
				//速め Motorから直進指示でもいいかも
				linetrace.PIDSetRun(40,0.38, 0.06, 0.025); 
				//Localizationで走行距離を測って直線が終わったら
				currentstate = CURVE1;
				break;
			case CURVE1:
				//中速 ただの円弧なのでsteerの関数でもいいかも
				linetrace.PIDSetRun(25,0.38, 0.06, 0.025); 
				//Localizationで走行距離を測って直線が終わったら
				currentstate = CURVE2;
				break;
			case CURVE2:
				//中速 途中で曲がる方向が変わるのでedge切り替えするといいかも
				//Lコースだとやたら迂回してるのでライン無視しても良さそう
				linetrace.PIDSetRun(25,0.38, 0.06, 0.025); 
				//Localizationで走行距離を測って直線が終わったら
				currentstate = STRAIGHT2;
				break;
			
			case STRAIGHT2:
				//速め
				linetrace.PIDSetRun(40,0.38, 0.06, 0.025); 
				//Localizationで走行距離を測って直線が終わったら
				//灰色のラインがあるのでそれを目安でもいいかも
				currentstate = CURVE3;
				break;
			case CURVE3:
				//中速
				linetrace.PIDSetRun(25,0.38, 0.06, 0.025); 
				//Localizationで走行距離を測って直線が終わったら
				currentstate = BLOCK;
				break;
			case BLOCK:
				//ブロック並べ攻略用プログラムに飛ぶのが妥当か
				currentstate = PARKING;
				break;
			case PARKING:
				//停める 灰色検知を利用
				leftWheel.stop(); rightWheel.stop();
				break;
			default:
				leftWheel.stop(); rightWheel.stop();
				break;
		}
		
	}*/

	else{ leftWheel.stop(); rightWheel.stop(); }

}

void AreaManager::NumberDisplay(int codecount){
	switch(codecount){
		case 0:
			msg_f("0",3);
			break;
		case 1:
			msg_f("1",3);
			break;
		case 2:
			msg_f("2",3);
			break;
		case 3:
			msg_f("3",3);
			break;
		case 4:
			msg_f("4",3);
			break;
		case 5:
			msg_f("5",3);
			break;
		case 6:
			msg_f("6",3);
			break;
		case 7:
			msg_f("7",3);
			break;
		case 8:
			msg_f("8",3);
			break;
		case 9:
			msg_f("9",3);
			break;
		default:
			msg_f("e",3);
			break;
		}
}

void AreaManager::regularLine(){
	//if(local.GetLen() < 300){
		//linetrace.PIDSetRun(30, 0.30, 0.00, 0.040, 1);//15 0.35 0.00 0.030 -1
		//linetrace.PIDSetRun(15, 0.35, 0.00, 0.030, 1);//p 0.35
		linetrace.PIDSetRun(25, 0.320, 0, 0.060, 1);
		//local.SetPoint();
	//}
	//else{
		//local.Init();
		//ev3_stp_cyc(CYCLE_CYC);
	//}
}
