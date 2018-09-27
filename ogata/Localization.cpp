//
//  Localization.cpp
//  
//  written by Nakazaki 2017/06/05
//  add     by kimoto   2017/06/30


#include "Localization.h"
// Bluetooth仮想シリアルポートのファイルをオープンする
    static FILE *bt = ev3_serial_open_file(EV3_SERIAL_BT);

Localization::Localization():
rightWheel(PORT_B),leftWheel(PORT_C){
    
    x = 0.0;
    y = 0.0;
    len = 0.0;
    theta = 0.0;
    degree = 0.0;
    
    prePhiL = 0.0;
    prePhiR = 0.0;
    
    mapX = 0;
    mapY = 0;
        
    time = 0;
}

//現在位置の初期化
void Localization::Init(){
    
    x = 0.0;
    y = 0.0;
    len = 0.0;
    theta = 0.0;
    degree = 0.0;
    
    mapX = 0;
    mapY = 0;

    prePhiR = rightWheel.getCount();
    prePhiL = leftWheel.getCount();   
   
}


void Localization::SetPoint(){
    float phiR = 0.0,phiL = 0.0;
    float deltaLR = 0.0,deltaLL = 0.0,deltaL = 0.0,deltaTheta = 0.0;

    phiR = rightWheel.getCount() - prePhiR;  //右車輪の回転角度の変化量
    phiL = leftWheel.getCount() - prePhiL;  //左ry
    
    prePhiR = rightWheel.getCount();
    prePhiL = leftWheel.getCount();


    deltaLR = 2 * PI * WHEEL_R * phiR / 360;        //右車輪の移動距離の変化量
    deltaLL = 2 * PI * WHEEL_R * phiL / 360;        //左ry
    deltaL = (deltaLR + deltaLL) / 2;               //走行距離算出式[mm]
    deltaTheta = (deltaLR - deltaLL) / WHEEL_D;     //進行方向算出式[rad](atanの近似式)
    
    x = x + deltaL * cos(theta + deltaTheta/2);     //x座標の算出式(init時の進行方向が正)
    y = y + deltaL * sin(theta + deltaTheta/2);     //y座標ry(init時の進行方向の左が正)
    len = len + deltaL;                             //総移動距離[mm]
    theta = theta + deltaTheta;                     //初期からの現在の角度
    
    degree = theta * 180.0 / PI;            //deg(左回転で増加)
    /*
    while(degree >= 360.0 ){
        degree = degree - 360.0;
    }*/

    fprintf(bt,"x=%f, y=%f, deg=%f \r\n",x,y,degree);

    XYtoMap();
}


/*
 自己位置を補正(設定)する
 mode(0,1,2,3)によって補正を反映させる値を決める
 0:x,y,theta
 1:x,y
 2:x,theta
 3:y,theta
 x,y;[mm] theta:[deg→rad]
 */
void Localization::CorrectLocalization(float newX,float newY,float newTheta,int mode){
    
    if(mode == 0){
        x = newX;
        y = newY;
        theta = newTheta * PI /180.0;
    }else if(mode == 1){
        x = newX;
        y = newY;
    }else if(mode == 2){
        x = newX;
        theta = newTheta * PI /180.0;
    }else if(mode == 3){
        y = newY;
        theta = newTheta * PI /180.0;
    }else{    }
    
}


float Localization::GetX(){
    return x;
}

float Localization::GetY(){
    return y;
}

int Localization::GetMapX(){
    return mapX;
}

int Localization::GetMapY(){
    return mapY;
}

float Localization::GetTheta(){
    return degree;
}

float Localization::GetLen(){
    return len;
}

void Localization::SetLen(float newLen){
    len = newLen;
}

//コースマップの座標は20mm単位で区切っているのでその補正
void Localization::XYtoMap(){
    mapX = (int)x/3;
    mapY = (int)y/3;
}
