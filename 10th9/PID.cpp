/*
* PID.cpp
* written by kimoto on 2017/06/16
*/

#include "PID.h"

//FILE *bt = ev3_serial_open_file(EV3_SERIAL_BT);

//値の上限と下限を制限する
float Limit(float target,int max, int min){
	if(target > max){
		return max;
	}
	else if(target < min){
		return min;
	}
	else{
		return target;
	}
}

PID::PID():
rightWheel(PORT_B),leftWheel(PORT_C){

	Kp = 1.0;
	Ki = 0;
	Kd = 1.0;

	delta = 0.004;//周期4msec

	diff[0] = 0;
	diff[1] = 0;

	integral = 0.0;
    
    motordiff[0] = 0;
    motordiff[1] = 0;
    Kph = 0.13;
    Kdh = 0.01;

}

//モータの速度を使ったPID制御
float PID::PIDSetRun(int sensorval,int targetval,float newp,float newi,float newd/*,float curve*/){
    /*位置型pid制御*/
    Kp = newp;
    Ki = newi;
    Kd = newd;
    
    float p,i,d;
    /*float Ti, Td;*/

    /*Ti = 0.5 * 0.6;
    Td = 0.125 * 0.6; //右の変数はマシンの振動する周期の時間
    Ki = Kp / Ti;*/
    
    diff[2] = diff[0]; //一番古い情報なので注意
    diff[0] = diff[1];
    diff[1] = targetval - sensorval;
    
    integral += (diff[1]+diff[0]) / 2.0 * delta;
    
    p = Kp * (float)diff[1];							//比例部
    i = Ki * integral;									//積分部
    d = Kd * ( (float)(diff[1] - diff[0]) / delta );	//微分部

    //fprintf(bt, "Pの値 %f 　Iの値 %f　Dの値 %f\r\n", p, i, d);

    u[0] = u[1];
    u[1] = p + i + d;

    /*return Limit(p + i + d  ,100.0,-100.0);*/

    /*速度型pid制御*/
    //float ph,dh;

    //左右のタイヤの速度の偏差を計算
    //motordiff[0] = motordiff[1];
    //motordiff[1] = ((-1 * leftWheel.getCount())  - (curve * (-1 * rightWheel.getCount())));
    //S16,F36がエラーだったので消したが有効数字が不安
    //motordiff[1] = ((-1 * motor_l.ReturnSpeed())  - (curve * (F32)(-1 * motor_r.ReturnSpeed())));
    //motordiff[1] = (S16)((-1 * motor_l.ReturnSpeed())  - (S16)(curve * (F32)(-1 * motor_r.ReturnSpeed())));
   
    //以下自分の内容
    motordiff[0] = motordiff[1];
    /*motordiff[1] = Kp * ((diff[1] - diff[0]) + (delta / Ti) * diff[1] + (Td / delta) * (diff[1] - 2 * diff[0] + diff[2]));*/
    motordiff[1] = Kp * (diff[1] - diff[0]) + Ki * diff[1] + Kd * (diff[1] - (2 * diff[0]) + diff[2]);

    //比例部と微分部を調整
    //ph = Kph * (float)motordiff[1];
    //dh = Kdh * ((float)(motordiff[1] - motordiff[0]) / 2 * delta);
    
    //return Limit(p + i + d + ph + dh ,100.0,-100.0);
    return Limit(u[0] + motordiff[1] ,100.0,-100.0);
}

/*速度無しPID制御計算 bluetooth込み　使わない予定
float PID::PIDCurveSlow(int sensorVal,int targetVal){
    
    KP = 1.51;
    KI = 0;
    KD = 0.153;
    
    //bluetoothで受信したらその値でKpidを決定
    if(bluetooth.ReturnData7() != 0){
        KP = bluetooth.ReturnData7()/1000.0;
        KI = bluetooth.ReturnData8()/1000.0;
        KD = bluetooth.ReturnData9()/1000.0;
    }
    
    float p,i,d;
    
    diff[0] = diff[1];
    diff[1] = sensorVal - targetVal;
    integral += (diff[1]+diff[0]) / 2*DELTA;
    
    p = KP * diff[1];
    i = KI * integral;
    d = KD * ( (float)(diff[1] - diff[0]) / DELTA );
    
    return Limit(p + d + i, 80,-80);
}*/

