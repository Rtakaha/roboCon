#ifndef _Block_H_
#define _Block_H_


#include "Motor.h"
#include "ColorSensor.h"
#include "util.h"
#include "Clock.h"
#include "Steering.h"
#include "LineTrace.h"
#include "ev3api.h"
#include "Localization.h"

using namespace ev3api;



class Block {
public:
  Block();
  void init();
  void run();
  void rotate(int gyro);
  void terminate();
  void avoid();
  void point();
  void goalsearch();
  void targetsearch();
  void direction(int togyro);
  void straight(int dis);
  void blockLineup();
  void linestraight();
  void liftup();
  void liftdown();
  void display();
  void codecalculation(int code);
  void dfs(int n, int goal, int limit);
  void ids(int start, int goal);
  void SetLine(int black, int white, float normalize, int gray);
  int gyro;
  float dis;
  int next2;
  int now;
  int next;
  int course[100];
  int coursecount=0;
  float minus;
  int wheel_gyro;
  int nowgyro;
  int togyro;
  int avoidgyro;
  int nowblock;
  int point_colornumber;
  int aflag;
  int green;
  int black_b;
  int red;
  int yellow;
  int blue;
  int clocktime;
  int linetime;
  colorid_t point_color;

  //int block[16]={0,0,4,0,2,0,0,0,0,0,3,1,0,0,5,0};
    /*ブロック存在の有無と色設定*/
  /*1=green,2=red,3=yellow,4=blue,5=black*/
  int block[16];
  int trace;
int path[6];
  bool visited[15];
  int flag=0;
  int lflag=0;
  int count=0;
  int start;
  int goal;

  int black_flag=0;
  int red_flag=0;
  int yellow_flag=0;
  int blue_flag=0;
  int green_flag=0;

  int goalflag=0;
  int finishflag=0;
  int rflag=0;


  //map(接続情報)    
  int map[15][4]={{5,10,2,0},//1
          {5,6,1,3},//2
          {6,7,2,4},//3
          {7,11,3,0},//4
          {1,2,8,10},//5
          {2,3,8,9},//6
          {3,4,9,11},//7
          {5,6,12,13},//8
          {6,7,14,15},//9
          {5,12,1,0},//10
          {7,15,4,0},//11
          {8,10,13,0},//12
          {8,12,14,0},//13
          {9,13,15,0},//14
          {9,11,14,0}};//15

  FILE *bt = ev3_serial_open_file(EV3_SERIAL_BT);

private:
  const int8_t mThreshold = 20;
 // const int8_t pwm = (Motor::PWM_MAX) / 6;
  const int8_t pwm = 10;


  Clock clock;
  Steering steer;
  Motor frontarm;
  Motor leftWheel2;
  Motor rightWheel2;
  ColorSensor colorSensor;
  LineTrace linetrace2;
  Localization local2;
};

#endif
