// orera
/*
AreaManager.h
written by kimoto 2017/06/29
memo
値は適当　マップを前提とした調整が必要
記述途中　ぶっちゃけ値がわかんないので適当書くしかできない
体裁はこんな感じ
*/

#include "AreaManager.h"

FILE *bt = ev3_serial_open_file(EV3_SERIAL_BT);

AreaManager::AreaManager():
touch(PORT_1),sonar(PORT_2),color(PORT_3),/*,gyro(PORT_4),*/
frontarm(PORT_A),rightWheel(PORT_B),leftWheel(PORT_C),steer(leftWheel,rightWheel){

}

int count = 0;
int loop = 0;
int16_t val;
int hey = 0;
char msgbuf[100];


void AreaManager::Init(){

	//初期値の設定や変数の初期化処理はここで

	init_f("AreaManager");

	leftWheel.reset();
	rightWheel.reset();


	//アームを確実に下に向ける
	frontarm.setPWM(0); //回転方向？
	clock.wait(1000);
	frontarm.reset();

	local.Init();
	detect.Init();

	//fprintf(bt, "BlueTooth接続成功\r\n");

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
			currentstate = START_R;
			linetrace.SetEdge(1);	//右回転から（合ってる？）
			break;
		}

		if(ev3_button_is_pressed(LEFT_BUTTON)){
			msg_f("L Cource", 1);
			currentcource = L_cource;
			currentstate = START;

			linetrace.SetEdge(1);

			break;
		}
	}
}
void AreaManager::Brightness(){
	bright = color.getBrightness();
}

void AreaManager::Manager(){

	/*int curve_f = 0;
	int gray_on = 0;*/

	/*モータの位置調整などは呼び出し前に済ませておく */
	if(currentcource == L_cource){

		switch(currentstate){

			case START:
				//fprintf(bt, "START\r\n");

				local.Init();

				msg_f("Ready ...", 1);

				if(touch.isPressed() == 1){
					msg_f("Go !", 1);
					clock.wait(1000);	//数秒待つ
					currentstate = STRAIGHT1;
					//currentstate = STRAIGHT2;
					//currentstate = CURVE3;
					//currentstate = KENSHO;
					//currentstate = PARKING;
					speed = 0;
				}

				break;
				// local.Init();

				// msg_f("Ready ...", 1);
				// if(touch.isPressed() == 1){
				// 	msg_f("Go !", 1);
				// 	clock.wait(1000);	//数秒待つ
				// 	currentstate = STRAIGHT_L1;
				// }
				// break;


			case STRAIGHT1:
				ev3_led_set_color(LED_GREEN);
				//速め Motorでただの直進指示でもいいかも
				msg_f("3", 2);

				// linetrace.PIDSetRun(bright, 30, 0.5, 0.5, 0.049, 1);
				// linetrace.PIDSetRun(bright, 40, 0.2, 0.02, 0.1, 1);
				linetrace.PIDSetRun(bright, 60, 0.04, 0.05, 0.05, 1);

				linetrace.PIDSetRun(bright, 40, 0.25, 0.05, 0.05, 1);  //　これもめちゃ良い
				// linetrace.PIDSetRun(bright, 40, 0.3, 0.05, 0.05, 1);  // 最強説
				// linetrace.PIDSetRun(bright, 40, 0.325, 0.05, 0.05, 1);  // かなり綺麗
				// linetrace.PIDSetRun(bright, 60, 0.450, 0.05, 0.120, 1);
				//linetrace.PIDSetRun(bright, 60, 0.04, 0.02, 0.08, 1); // これも滑らか
				// linetrace.PIDSetRun(bright, 90, 0.05, 0.02, 0.100, 1);  // これも滑らか
				// linetrace.PIDSetRun(bright, 110, 0.075, 0.02, 0.120, 1);  // ハイスピード
				//　左カーブは　0.450, 0.05, 0.120, 1 -> 最後のカーブ

				local.SetPoint();

				if(local.GetLen() >= 5000){//単位はmm
					ev3_speaker_play_tone(NOTE_C5,500);
					speed = 110;
					currentstate = CURVE1;
				}
				break;
				

			case 20:  // 駐車用
				// if (val < 20)
				// {
				// 	leftWheel.setPWM(0);
    //     			rightWheel.setPWM(0);
				// }
				// else
				// {
				linetrace.PIDSetRun(bright, 20, 0.20, 0.05, 0.03, 1);
				// }
				local.SetPoint();
				if(hey == 60)
				{
					currentstate = 30;
				}

				// int8_t val = ev3_infrared_sensor_get_distance(EV3_PORT_2);
				// sprintf(msgbuf, "Distance: %-3d %%", val.distance);
				// ev3_lcd_draw_string(msgbuf, 0, 20);

				break;

			case 30:
				val = ev3_ultrasonic_sensor_get_distance(EV3_PORT_2);
				if (val > 10)
				{
					leftWheel.setPWM(20);
					rightWheel.setPWM(20);
					count++;
					sprintf(msgbuf, "Distance: %-3d cm", val);
					ev3_lcd_draw_string(msgbuf, 0, 20);
					break;
				}

				leftWheel.setPWM(0);
				rightWheel.setPWM(0);
				break;



			case CURVE1:
				ev3_led_set_color(LED_GREEN);
				msg_f("3", 2);
				if(speed > 40){
					speed = speed - 1;
				}
				linetrace.PIDSetRun(bright, 30, 0.25, 0.05, 0.05, 1);  //　これもめちゃ良い
				// linetrace.PIDSetRun(bright, 60, 0.450, 0.05, 0.120, 1);
				// linetrace.PIDSetRun(bright, speed, 0.2, 0.05, 0.12, 1); 
				// linetrace.PIDSetRun(bright, speed, 0.2, 0.02, 0.08, 1); //一応曲がる
				// linetrace.PIDSetRun(bright, 60, 0.02, 0.05, 0.12, 1); // まあまあ綺麗
				// linetrace.PIDSetRun(bright, 30, 0.005, 0.05, 0.08, 1);  // 右カーブにはかなり良い
				// linetrace.PIDSetRun(bright, speed, 0.075, 0.02, 0.120, 1); //かなり綺麗に曲がった（バッテリー入れ替えたらどうなるかわからん,speedは80)
				// linetrace.PIDSetRun(bright, speed(80), 0.1, 0.02, 0.120, 1);  //割と上手く曲がった
				//　左カーブは　0.450, 0.05, 0.120, 1 -> 最後のカーブ

				local.SetPoint();

				// if(local.GetLen() >= 3700){//単位はmm
				// 	ev3_speaker_play_tone(NOTE_C5,500);
				// 	currentstate = STRAIGHT2;
				// }				
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

/*
Lコース
modified by Takahashi 2018/06/11
STRAIGHT_L1
CURVE_L1 (左回転)
STRAIGHT_L2
CURVE_L2 (右回転)
STRAIGHT_L3
CURVE_L3 (右回転)
STRAIGHT_L4
CURVE_L4 (左回転)
STRAIGHT_L5

LineTrace::PIDSetRun(int pwm,float newp,float newi,float newd,float curve)
	ラインの左側を通る
	左折の時はP小さめ、右折の時はP大きめ
*/

	else if(currentcource == R_cource){//Rコース

		switch(currentstate){

			case START_R:

				local.Init();

				msg_f("Ready ...", 1);
				if(touch.isPressed() == 1){
					msg_f("Go !", 1);
					clock.wait(1500);	//数秒待つ
					speed = 0;
					leftWheel.setPWM(20);
            	    rightWheel.setPWM(30);
               		clock.wait(100);
					//leftWheel.setPWM(50);
        			//rightWheel.setPWM(50);
        			//clock.wait(1000);

					
					currentstate = STRAIGHT_R1;

					// leftWheel.setPWM(100);
     //    			rightWheel.setPWM(100);
					// clock.wait(4000);

					// leftWheel.setPWM(44);
     //    			rightWheel.setPWM(86);
     //    			clock.wait(3800);
					
					// leftWheel.setPWM(60);
     //    			rightWheel.setPWM(60);
     //    			clock.wait(300);
					
					// leftWheel.setPWM(44);
     //    			rightWheel.setPWM(87);
     //    			clock.wait(1500);
					
					// leftWheel.setPWM(90);
     //    			rightWheel.setPWM(40);
     //    			clock.wait(1000);
					
					// leftWheel.setPWM(60);
     //    			rightWheel.setPWM(60);
     //    			clock.wait(1500);
					
					// leftWheel.setPWM(90);
     //    			rightWheel.setPWM(40);



				}
				break;

			case STRAIGHT_R1:
//				ev3_speaker_play_tone(NOTE_AS4,300);
				speed = 30;
				//linetrace.PIDSetRun(bright, 80, 0.05, 0.02, 0.1, 1);//なめらか
				linetrace.PIDSetRun(bright,60,0.04,0.05,0.05,1);
				//linetrace.PIDSetRun(bright,110,0.075,0.02,0.12,1); //バッテリー交換後いい感じ (9/17) a


				ev3_led_set_color(LED_GREEN);
				//速め Motorでただの直進指示でもいいかも
				msg_f("R_S1", 3);
				if(speed < 50){
					speed = speed + 1;
				}//ここはいらないかも？
				else if(speed > 50){
					speed = speed - 1;
				}
				//linetrace.PIDSetRun(speed, 0.100, 0.05, 0.100, 1);
				//左カーブは　0.450, 0.05, 0.120, 1 -> 最後のカーブ



				local.SetPoint();

				if(local.GetLen() >= 1800){//減速
					ev3_led_set_color(LED_RED);
					leftWheel.setPWM(20);
                    rightWheel.setPWM(30);
                    clock.wait(500);
					//linetrace.PIDSetRun(bright,55,0.18,0.04,0.12,1);// a
					linetrace.PIDSetRun(bright,50,0.25,0.04,0.12,1);//なめらか
				}

				local.SetPoint();

				// //現在のxとthetaの値を再セットする
				if(local.GetLen() >= 1900){//単位はmm
					
					ev3_led_set_color(LED_GREEN);
					currentstate = 20;
				 }                
				break;


			case 20://カーブ
//				ev3_speaker_play_tone(NOTE_AS4,300);
				msg_f("R_LC", 3);
				
				if(speed < 50){
					speed = speed + 1;
				}//ここはいらないかも？
				else if(speed > 50){
					speed = speed - 1;
				}

				//linetrace.PIDSetRun(bright,55, 0.2, 0.055, 0.2, 1);//(9/17)
				//linetrace.PIDSetRun(bright,55, 0.14, 0.055, 0.2, 1);//(9/18)
				//linetrace.PIDSetRun(bright,50, 0.134, 0.08, 0.2, 1);//最良 a
				linetrace.PIDSetRun(bright,30, 0.14, 0.05, 0.055, 1);//lカーブ 
				currentstate = 20;
                

				 local.SetPoint();

				if(local.GetLen() >= 4000){
					ev3_led_set_color(LED_RED);
					msg_f("R_RC", 3);
					//linetrace.PIDSetRun(bright,45, 0.15, 0.065, 0.5, 1);//rカーブ a
					linetrace.PIDSetRun(bright,30, 0.125, 0.05, 0.065, 1);
				 }

                local.SetPoint();

				if(local.GetLen() >= 7400){
					ev3_speaker_play_tone(NOTE_C4,300);
					ev3_led_set_color(LED_GREEN);
					currentstate = CURVE_R1;
				}
			    break;


			case CURVE_R1://最後の直線
				ev3_led_set_color(LED_GREEN);
				msg_f("R_s_l", 3);
				if(speed < 38){//36
					speed = speed + 1;
				}//ここはいらないかも？
				else if(speed > 38){
					speed = speed - 1;
				}
				linetrace.PIDSetRun(bright,35, 0.135, 0.05, 0.06, 1);//(9/17)

				local.SetPoint();

				if(local.GetLen() >= 8300){//ゴール後減速
					linetrace.PIDSetRun(bright,20, 0.2, 0.05, 0.05, 1);
				 }

				 local.SetPoint();

				 if(local.GetLen() >= 9950){//ブロック並べる
					leftWheel.setPWM(0);
					rightWheel.setPWM(0);
					while(color.getColorNumber() != COLOR_RED){
						leftWheel.setPWM(10);
						rightWheel.setPWM(10);
						local.SetPoint();
					}
					
					leftWheel.setPWM(10);
					rightWheel.setPWM(-10);
					clock.wait(1055);
					leftWheel.setPWM(10);
					rightWheel.setPWM(10);
					clock.wait(3280);
					leftWheel.setPWM(-10);
					rightWheel.setPWM(-10);
					clock.wait(3300);
					leftWheel.setPWM(-10);
					rightWheel.setPWM(10);
					clock.wait(1075);
					// leftWheel.setPWM(20);
					// rightWheel.setPWM(20);
					// clock.wait(8000);

					while(color.getColorNumber() != COLOR_BLACK){
						leftWheel.setPWM(10);
						rightWheel.setPWM(9);
						local.SetPoint();
					}
					leftWheel.setPWM(0);
					rightWheel.setPWM(0);
					clock.wait(100);
					
					currentstate = STRAIGHT_R2;

					break;
				 }

				// 一定距離曲がれば次のステージへ行くようにプログラムを変更
				// local.GetY() > 1000 とか
				break;

			case STRAIGHT_R2://少しだけライントレースする
				msg_f("L_min", 3);

				ev3_led_set_color(LED_RED);
				local.SetPoint();
				linetrace.PIDSetRun(bright,10, 0.2, 0.05, 0.05, 1);
				clock.wait(1000);
				leftWheel.setPWM(0);
				rightWheel.setPWM(0);

				break;

			case END_R:
End_R:
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

	else{ 
		leftWheel.stop(); 
		rightWheel.stop();
	}

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