/*
* Linetrace.h
* written by kimoto on 2017/05/16
*/

#include "LineTrace.h"

//コンストラクタ
LineTrace::LineTrace():
    rightWheel(PORT_B),leftWheel(PORT_C),
    touch(PORT_1),color(PORT_3),steer(rightWheel,leftWheel){
    edge = 1;
    ColorHistoryReset();
}

//曲がる方向を設定
void LineTrace::SetEdge(int newedge){
    if(newedge == 1 || newedge == -1)
        edge = newedge;
}

//PID制御しながらライントレース
void LineTrace::PIDSetRun(int pwm,float newp,float newi,float newd,float curve){

  //  msg_f("PIDstart", 2);

    int ave_color = 0;
    int sum_color = 0;

    for(int i = 0; i < 9; i++){
        colorHistory[i] = colorHistory[i+1];//最も古いデータを消している
    }
    colorHistory[9] = NormalizeLine(color.getBrightness());

 //   msg_f("PIDhistory", 3);

    if(colorHistory[0] == -1) return;//Lコースではなぜか出来ない

  //  msg_f("PIDhistory2", 3);

    for(int i = 0;i < 10;i++){
        sum_color = sum_color + colorHistory[i];
    }

    ave_color = sum_color / 10;

   // msg_f("PID!PID!", 4);

    float turn = pid.PIDSetRun(ave_color,gray/*+10*/,newp,newi,newd/*,curve*/);
    
    turn = turn * curve;
    
    int pwm_l = pwm - turn;
    int pwm_r = pwm + turn;

  //  msg_f("setPWM", 3);
    
    leftWheel.setPWM(pwm_l);
    rightWheel.setPWM(pwm_r);
    
    /*steer.setPower(pwm, turn);*/
}

//written by nagao
void LineTrace::Calibration(){

    /*while(ev3_button_is_pressed(RIGHT_BUTTON)){
        msg_f("Set brack", 2);
        ev3_speaker_play_tone(NOTE_CS4,1000);
        black = color.getBrightness();
        break;
    }

    while(ev3_button_is_pressed(LEFT_BUTTON)){
        msg_f("Set white", 3);
        ev3_speaker_play_tone(NOTE_GS4,1000);
        white = color.getBrightness();
        break;
    }*/

    while(1){

        msg_f("Set brack", 2);
        if(ev3_button_is_pressed(RIGHT_BUTTON)){
            ev3_speaker_play_tone(NOTE_CS4,300);
            black = color.getBrightness();
            break;
        }
     }

    while(1){

        msg_f("Set white", 3);
        if(ev3_button_is_pressed(LEFT_BUTTON)){
            ev3_speaker_play_tone(NOTE_GS4,300);
            white = color.getBrightness();
            break;
        }
    }

    //grayは閾値になる
    normalize = 100 / (white - black);

    gray = 40;

    /*gray = black + (white - black) / 2;*/

    //スタート時は黒と白の境界となる場所からライントレースを開始すること
}

float LineTrace::NormalizeLine(int value/*ラインの値*/){
    value = (value - black) * normalize;

    if(value > 100){
        return 100;
    }
    else if(value < 0){
        return 0;
    }
    else{
        return value;
    }
}

//written by nakazaki
void LineTrace::Rotate(int gyr){

    /*gyr:回転する角度[DEG] 右が正の値
    ev3_motor_rotate(port,degrees,speed,blocking);
    blocking=true:回転してから次を読む
        false:回転を待たずに次を読む
    */

    int angle = gyr * 5 / 3;/*タイヤの回転角=回転したい角度*5/3*/
    ev3_motor_rotate(EV3_PORT_B,angle,50,false);
    ev3_motor_rotate(EV3_PORT_C,-angle,50,true);

}

/*
int LineTrace::PIDCurveSlow(int speed){
    float a = pid.PIDCurveSlow(light.Normalization(light.GetData()),60);    //カラーセンサの値と目標値を使ってPID値を計算
    a = a * (float)edge;                                                    //曲がる方向を決めて
    
    motordrive.Drive(speed,(int)a);                                         //モーターに渡す
    
    return (int)a;
}*/

int LineTrace::Getblack(){
    return black;
}

int LineTrace::Getwhite(){
    return white;
}

float LineTrace::Getnormalize(){
    return normalize;
}

int LineTrace::Getgray(){
    return gray;
}

void LineTrace::Setblack(int black2){
    black = black2;
}

void LineTrace::Setwhite(int white2){
    white = white2;
}

void LineTrace::Setnormalize(float normalize2){
    normalize = normalize2;
}
