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
  void gopurpose(int location,int nearest);
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
  int zahyo(int location,int z);
  void searchblock();
  void Blockcolor(); 
  void urochoro(int location);
  int gyro;
  int pb;
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
  int d0,d1,d2,d3;
  int location ;
  int nearest;
  int nearestx;
  int nearesty;
  int Block_color;
  int color[4][4]={{5,4,2,4},{2,3,5,4},{5,4,2,3},{2,3,5,4}};
  /*COLOR_NONE   = 0, //!< \~English None         \~Japanese 無色
    COLOR_BLACK  = 1, //!< \~English Black        \~Japanese 黒
    COLOR_BLUE   = 2, //!< \~English Blue         \~Japanese 青
    COLOR_GREEN  = 3, //!< \~English Green        \~Japanese 緑
    COLOR_YELLOW = 4, //!< \~English Yellow       \~Japanese 黄
    COLOR_RED    = 5, //!< \~English Red          \~Japanese 赤
    COLOR_WHITE  = 6, //!< \~English White          \~Japanese 白
    COLOR_BROWN  = 7,*/
  //int block[16]={0,0,4,0,2,0,0,0,0,0,3,1,0,0,5,0};
    /*ブロック存在の有無と色設定*/
  /*1=green,2=red,3=yellow,4=blue,5=black*/
  int block[4];
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
  int map[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

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
