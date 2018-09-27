
#include <stdlib.h>
#include <math.h>

#include "MotorDrive.h"

/*
drive,turnはSteering.hのsetPowerでまかなえるはず
stopはMotor.hで
確認できたら消していい 残った関数の意味的にcontrol的な名前の方が良いかも

void MotorDrive::Drive(int speed, int turn)/*speed = -100~100, turn = -85 ~ 85
{
	int motor_count;
	motor_count = turn * 8;
	speed = speed * (-1);
	
    if(motor_count - motor_steer.GetCount() > 50){
    	motor_steer.SetPower(100);
    	//motor_steer.Brake(1);
    }        
	else if(motor_count - motor_steer.GetCount() < -50){
		motor_steer.SetPower(-100);
    	//motor_steer.Brake(1);
    }
    else{
    	motor_steer.SetPower(0);
    	//motor_steer.Brake(1);
    }
        

	if(motor_count - motor_steer.GetCount() > 50){
		motor_l.SetPower(speed - (turn*speed/-90));
		//motor_l.Brake(1);

		motor_r.SetPower(speed);
		//motor_r.Brake(1);
	}
	else if(motor_count - motor_steer.GetCount() < -50){
		motor_r.SetPower(speed - (turn*speed/90));
		//motor_r.Brake(1);

		motor_l.SetPower(speed);
		//motor_l.Brake(1);
	}
	else{
		motor_l.SetPower(speed);
		//motor_l.Brake(1);

		motor_r.SetPower(speed);
		//motor_r.Brake(1);
	}
}

void MotorDrive::Turn(int speed, int turn)
{
	int motor_count;
	motor_count = turn * 8;
	speed = speed * (-1);
    
    if(motor_count - motor_steer.GetCount() > 50){
    	motor_steer.SetPower(100);
    	motor_steer.Brake(1);
    }
    else if(motor_count - motor_steer.GetCount() < -50){
    	motor_steer.SetPower(-100);
    	motor_steer.Brake(1);
    }

    else{
    	motor_steer.SetPower(0);
    	motor_steer.Brake(1);
    }
        
	if(motor_count - motor_steer.GetCount() <= 50 && motor_count - motor_steer.GetCount() >= -50){
		if(turn > 0){
			motor_l.SetPower(speed);
			motor_l.Brake(1);

			motor_r.SetPower(-speed);
			motor_r.Brake(1);

		}else if(turn < 0){
			motor_l.SetPower(-speed);
			motor_l.Brake(1);

			motor_r.SetPower(speed);
			motor_r.Brake(1);
		}else{
		}
	}
	
}

void MotorDrive::Stop()
{
	motor_steer.SetPower(0);
	motor_steer.Brake(1);
	motor_l.SetPower(0);
	motor_l.Brake(1);
	motor_r.SetPower(0);
	motor_r.Brake(1);
}*/

/*
motor_steer.GetCount() > 0　右回転 
SteerControlは前輪の向きを正面にリセットできます
返り値が1になるまで呼び出し続けてください
*/
/*setCount()を使う方法もありそう*/

int MotorDrive::SteerControl(int to){
	int ans = 0;
	int now = motor.GetCount();
	
	if(!(abs(now - to) < 20)){// | 現在の値　－　目標値 | < 20
		if(now < to){//左向き
			motor.SetPWM(100);//右回転
			motor.stop();
		}else{
			 motor.SetPWM(-100);//左回転
			 motor.stop();
		}
	}else{
		ans = 1;
		motor.stop();
	}
	return ans;
}
/*
LineFinderはライン復帰用関数です
引数　(ライトセンサー値　目標値　向き)
ライトセンサー値
	正規化されたライトセンサー値を渡してください 値の有効範囲0~100
目標値
	復帰したい時の正規化されたライトセンサーの値を渡してください 値の有効範囲0~100
	基本的に50でOKです
向き
	右旋回でラインを探すか左旋回でラインを探すかを決定します
	0 右旋回
	1　左旋回
	
戻り値	0 移動中
		1 到着
	   -1 エラー
*/
int MotorDrive::LineFinder(int lightValue, int to, int direction){
	int ans = 0;
	int ang = 0;
	double v = 0;//分散値
	
	if((0 > lightValue) || (lightValue > 100) || (0 > to) || (to > 100) || ((direction != 0) && (direction != 1))){
		ans = -1;
	}else{
		if(direction == 0)ang = 25;
		else ang = -25;
		
		v = pow((double)lightValue - (double)to,2);//(センサー値 - 目標値)**2
		
		if(sqrt(v) < 10){
			motor.stop();
			ans = 1;
		}else{
			steer.setPower(15,ang);
		}
	}
	return ans;
}

int MotorDrive::LineFinderSetSpeed(int lightValue, int to, int direction,int speed){
    int ans = 0;
    int ang = 0;
    double v = 0;//分散値
    
    if((0 > lightValue) || (lightValue > 100) || (0 > to) || (to > 100) || ((direction != 0) && (direction != 1))){
        ans = -1;
    }else{
        if(direction == 0)ang = 25;
        else ang = -25;
        
        v = pow((double)lightValue - (double)to,2);//(センサー値 - 目標値)**2
        
        if(sqrt(v) < 10){
            motor.stop();
            ans = 1;
        }else{
            steer.setPower(speed,ang);
        }
    }
    return ans;
}



