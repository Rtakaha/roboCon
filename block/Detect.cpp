//
//  Detect.cpp
//
//
//  Written by kimoto on 2017/06/25.
//
//

#include "Detect.h"
#include <stdlib.h>

// Bluetooth仮想シリアルポートのファイルをオープンする
//FILE *bt = ev3_serial_open_file(EV3_SERIAL_BT);

Detect::Detect():
sonar(PORT_2),color(PORT_3),gyro(PORT_4){
}

//配列をまとめて初期化
void Detect::Init(){

    RightAngleReset();
    StepDetectionReset();
    GrayDetectionReset();
    WallDetectionReset();
}

int Detect::StepDetection(){    //段差検出関数　return1　検出
    float add = 0;              //角速度の記録１０回分の合計値
    float v  = 0;               //分散値
    int ans = 0;                //検出フラグ
    static int c = 0;           //角速度や光センサ値の記録に使う
    
    angularVelocityHistory[c] = gyro.getAnglerVelocity();//角速度を取得
    c++;
    if(c >= 10){//角速度は１０回分まで記録
        c = 0;
    }
    
    for(int i = 0; i < 10; i++){
        add = add + (float)angularVelocityHistory[i]; //角速度の記録１０回分を合計した値をaddに入れる→平均の計算に使う
    }
    for(int i = 0; i < 10; i++){//∑(平均 - 値)**2
        v = v + ((add / (float)10) - (float)angularVelocityHistory[i])*((add / (float)10) - (float)angularVelocityHistory[i]);
    }
    v = v / 10;                 //v = 1/n*∑(平均-値)**2
    
    if(v > 40)ans = 1;         //分散がある値以上ならば段差検出
    return ans;
}

int Detect::RightAngleDetection(){//直角検出関数 return1  DELTA_T * RIGHTANGLE_N (ms) 間黒 retrun0 未検出
    int ans = 1;
    static int c = 0;
    
    if(color.getColorNumber() == (colorid_t)1 ){
        rightAngleHistory[c] = 1;//光センサの値が閾値を超えていれば１を記録
    }
    else{
        rightAngleHistory[c] = 0;//超えていなければ０を記録
    }
    c++;
    if(c >= 10){//１０回分まで記録する
        c = 0;
    }
    for(int i = 0; i < 10; i++){//過去１０回分の記録が全て閾値を超えていた場合検出、1を返す
        ans = ans * rightAngleHistory[i];
    }
    return ans;
}

//要変更
int Detect::GrayDetection(){//灰色検出関数　return1　検出 
    float sum = 0;//光センサ値の合計
    float ave = 0;//光センサ値の平均
    float v   = 0;//光センサ値の分散
    
    for(int i = 0; i < LIGHTDATA_N - 1; i++){//LIGHTDATA_N=10(光センサの記録回数)
        lightHistory[i] = lightHistory[i+1];//最も古いデータを消している
    }
    lightHistory[LIGHTDATA_N - 1] = color.getBrightness();//光センサからとった値をいじって格納してる
    
    if(lightHistory[0] == 0) return 0;//光センサの値がひとつも記録されていなければ０を返す(未検出)
    
    for(int i = 0; i < LIGHTDATA_N; i++){
        sum = sum + lightHistory[i];//記録されている光センサの値の合計を計算する
    }
    ave = sum/LIGHTDATA_N;//光センサの値の平均を計算

    for(int i = 0; i < 10; i++){//分散
        v = v + (ave - (float)lightHistory[i])*(ave - (float)lightHistory[i]);
    }
    v = v / 10;                 //v = 1/n*∑(平均-値)^2
    
    if(ave > 50 && v > 200 && v < 700 && lightHistory[LIGHTDATA_N -1] < 50){
        return 1;
    }else{
        return 0;
    }
}

//壁検知関数
//return 0->未検出　1->検出
int Detect::WallDetection(int distance)
{
     for(int i = 0; i < SONARDATA_N - 1; i++){
     sonarHistory[i] = sonarHistory[i+1];
     }

     sonarvalue = sonar.getDistance();
     
     if (sonarvalue <= distance){
        sonarHistory[SONARDATA_N - 1] = 1;
        sonarflag++;
     }
     else{
        sonarHistory[SONARDATA_N - 1] = 0;
     }

     //fprintf(bt, "dis: %d, flag: %d\r\n",(int)sonar.getDistance(),sonarflag);
    
    if((sonarHistory[0] - sonarHistory[SONARDATA_N - 1]) == 1 && sonarflag >= 500){//検出（１）→未検出（０）に変化した時、壁が通過したとみなす
        return 1;
    }
    else if(sonarflag >= 10000){  //センサの誤認識で値が閾値以下から動かなかった場合を検出
        //clock.wait(500);
        return 1;
    } 
    else{ return 0; }

}

int Detect::GetSonarvalue()
{
    return sonarvalue;
}

