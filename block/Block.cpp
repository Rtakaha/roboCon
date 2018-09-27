//2017,6,13メモ
//基本的にinitを実行して初期化。そのあと、codecalcullationで初期値を打ち込む。BlockLineup実行でブロック並べをするプログラム
//そのためそれ以外の関数はBlockLineupのための関数
//仕組みについては関数名とコメントを見て
//written by O.T
//ver.1.00
//2016年用の中崎先輩のものを変更する形で編集




#include "Block.h"


using namespace ev3api;

Block::Block():
frontarm(PORT_A),rightWheel2(PORT_B),leftWheel2(PORT_C), steer(leftWheel2,rightWheel2),
colorSensor(PORT_3){

}

void Block::init(){
  init_f("Block");
  leftWheel2.reset();
  rightWheel2.reset();
  location=10;
  nowgyro=90;
  local2.Init();
  clock.wait(1000);
}

void Block::rotate(int gyro){//gyroは角度情報　nowblock=0でブロックを持っているというフラグ

  msg_f("Rotating.", 1);
  int angle=gyro*5/3;/*タイヤの回転角=回転したい角度*5/3*/
  if(nowblock>0){
  ev3_motor_rotate(EV3_PORT_C,angle,10,false);//ブロック非所持
  ev3_motor_rotate(EV3_PORT_B,-angle,10,true);
  }
  else{
  ev3_motor_rotate(EV3_PORT_C,angle,15,false);//ブロック所持
  ev3_motor_rotate(EV3_PORT_B,-angle,15,true);
  }
}
void Block::direction(int togyro){
  fprintf(bt,"togyro=%d\r\n",togyro);
  fprintf(bt,"nowgyro=%d\r\n",nowgyro);
  gyro=togyro-nowgyro; 
  nowgyro=togyro;
  if(gyro==180||gyro==-180){
    rflag=1;
  }
  else{
    rflag=0;
  }

  if(gyro>180){
    gyro=360-gyro; 
    gyro=-gyro;
  }
  else if(gyro<-180){
    gyro=360+gyro;
  }

  if(gyro==0){
    avoidgyro=180;
    clocktime=3200;
  }
  else if(gyro>0){
    avoidgyro=180-gyro;
    clocktime=avoidgyro*40/3+400;

  }
  else{
    avoidgyro=180+gyro;
    clocktime=avoidgyro*40/3+600;
  }

  if(avoidgyro<=90&&avoidgyro>0){
    clocktime=clocktime-200;
  }

  fprintf(bt,"gyro=%d\r\n",gyro);


}
/*ブロック持ち上げ*/
  void Block::liftup(){
    msg_f("Lift upping",1);
    // terminate();
    frontarm.setPWM(5);
    clock.wait(300);
    frontarm.stop();
    // nowblock=block[next];  // 現在持っているブロックの色を設定
    // block[next]=0;
    
  }

/*ブロック設置*/
  void Block::liftdown(){
    msg_f("Lift downing",1);
    terminate();
    frontarm.setPWM(-5);
    clock.wait(300);
    frontarm.stop();
    leftWheel2.setPWM(-20);
    rightWheel2.setPWM(-20);
    clock.wait(500);
    block[next]=nowblock;
    nowblock=0;
    lflag=1;
    goalflag++;
    coursecount=0;
    start=goal;

        //ブロック並べが終わった時
        //現時点では１つ並べた時(goalflag==5が正しい値)
    if(goalflag>0){
      finishflag=1;
      fprintf(bt,"finishflag1=%d\r\n",finishflag);
      goal=11;
      ids(start,goal);
      
    }
     else{
      
        goalsearch();//最初の１回だけ読んで経路設定
    }
       now=course[coursecount];/*現在の点*/
       next=course[coursecount+1];/*次に進む点*/
       next2=course[coursecount+2];
    point();
    direction(togyro);

      fprintf(bt,"finishflag2=%d\r\n",finishflag);


    fprintf(bt,"liftavoidjudge\r\n");
    fprintf(bt,"block[%d]=%d,rflag=%d\r\n",now,block[now],rflag);
    if(block[now]>0&&rflag==0){
          fprintf(bt,"liftavoid\r\n");
          avoid();
        }
        else{
          rflag=0;
          rotate(gyro);
        }
        coursecount=coursecount-1;
        lflag=0;
  }
    
void Block::terminate() {
  msg_f("Stopped.", 1);
  leftWheel2.stop();
  rightWheel2.stop();
}
int Block::zahyo(int location,int z){//mapを二次元座標に見立ててx座標、y座標を返すプログラム
  
  switch (location){
      case 0:
    if(z==0){
      return 0;
    }
    else{
      return 0;
    }

    case 1:
    if(z==0){
      return 1;
    }
    else{
      return 0;
    }

    case 2:
    if(z==0){
      return 2;
    }
    else{
      return 0;
    }

    case 3:
    if(z==0){
      return 3;
    }
    else{
      return 0;
    }

    case 4:
    if(z==0){
      return 0;
    }
    else{
      return 1;
    }

    case 5:
    if(z==0){
      return 1;
    }
    else{
      return 1;
    }

    case 6:
        if(z==0){
      return 2;
    }
    else{
      return 1;
    }

    case 7:
        if(z==0){
      return 3;
    }
    else{
      return 1;
    }

    case 8:
      if(z==0){
      return 0;
    }
    else{
      return 2;
    }

    case 9:
    if(z==0){
      return 1;
    }
    else{
      return 2;
    }

    case 10:
    if(z==0){
      return 2;
    }
    else {
      return 2;
    }

    case 11:
    if(z==0){
      return 3;
    }
    else{
      return 2;
    }

    case 12:
    if(z==0){
      return 0;
    }
    else{
      return 3;
    }

    case 13:
    if(z==0){
      return 1;
    }
    else{
      return 3;
    }
    case 14:
        if(z==0){
      return 2;
    }
    else{
      return 3;
    }
    case 15:
        if(z==0){
      return 1;
    }
    else{
      return 3;
    }
  }//locationに座標ナンバー、zに０でx座標、1でy座標を返すプログラム
}
void Block::goalsearch(){ //nearstがlocationに最短のblockの番号を与える関数goalsearchと同義ブロック非所持のフラグ設定の関数
 if(block[0]!=-1){
 d0=(zahyo(location,0)-zahyo(block[0],0))*(zahyo(location,0)-zahyo(block[0],0))+(zahyo(location,1)-zahyo(block[0],1))*(zahyo(location,1)-zahyo(block[0],1));
 }
 else{
  d0=10000;
 }
 if(block[1]!=-1){
 d1=(zahyo(location,0)-zahyo(block[1],0))*(zahyo(location,0)-zahyo(block[1],0))+(zahyo(location,1)-zahyo(block[1],1))*(zahyo(location,1)-zahyo(block[1],1));
 }
 else{
  d1=10000;
 }
 if(block[2]!=-1){
 d2=(zahyo(location,0)-zahyo(block[2],0))*(zahyo(location,0)-zahyo(block[2],0))+(zahyo(location,1)-zahyo(block[2],1))*(zahyo(location,1)-zahyo(block[2],1));
 }
 else{
  d2=100000;
 }
 if(block[3]!=-1){
 d3=(zahyo(location,0)-zahyo(block[3],0))*(zahyo(location,0)-zahyo(block[3],0))+(zahyo(location,1)-zahyo(block[3],1))*(zahyo(location,1)-zahyo(block[13],1));
 }

 if(block[0]!=-1){ 
  nearest=block[0];
 } 
  if(block[1]=!-1){ 
    if(d0>d1){
    nearest=block[1];
    }
  }
  if(block[2]=!-1){ 
    if(d1>d2&d0>d2){
    nearest=block[2];
    }
  } 
  if(block[3]!=-1){
   if(d2>d3&d1>d3&d0>d3){
      nearest=block[3];
    } 
  } 
}

void Block::codecalculation(int code){//codeを解析してblock[]の配列にblockのナンバーを返す関数

  pb=code%16;
  block[3]=((code-pb)%256)/16;
  block[2]=((code-pb-block[3]*16)%4096)/256;
  block[1]=((code-pb-block[3]*16-block[2]*256)%65536)/4096;
  block[0]=(code-pb-block[3]*16-block[2]*256-block[1]*4096)/65536;
  map[zahyo(block[3],0)][zahyo(block[3],1)]=1;
  map[zahyo(block[2],0)][zahyo(block[2],1)]=1;
  map[zahyo(block[1],0)][zahyo(block[1],1)]=1;
  map[zahyo(block[0],0)][zahyo(block[0],1)]=1;
  //mapは4*4の配列にblockの位置情報を詰め込んだ配列

}

void Block::point(){//現在地と持ってるブロックの色によって移動すべき場所、togyroを計算する関数
}

void Block::straight(int dis) { 
  msg_f("Straight.", 1);

  //float ang=(dis-5)*14.28;/*距離を角位置に換算*/
  ev3_motor_reset_counts(EV3_PORT_B);/*角位置リセット*/

      leftWheel2.reset();
      rightWheel2.reset();
      local2.Init();
   /*   if(lflag==1){
        if(block[now]>0&&rflag==0){
          fprintf(bt,"liftavoid\r\n");
          avoid();
        }
        else{
          rflag=0;
          rotate(gyro);
        }
        lflag=0;
      }*/
      //else{
        rotate(gyro);
      leftWheel2.setPWM(5);
      rightWheel2.setPWM(8);
      clock.wait(800);
      //}
      ev3_sta_cyc(CYCLE_CYC);
      //slp_tsk();
      //ev3_stp_cyc(CYCLE_CYC);;
      clock.wait(linetime);
      ev3_stp_cyc(CYCLE_CYC);;
      linetime=1800;
     
    while(1){
         //int wheel_gyro=ev3_motor_get_counts(EV3_PORT_B);
         // linetrace2.PIDSetRun(5, 0.15, 0, 0.006, 1);
          
        //PIDで調整するとこ
        //0.004でも若干ぶれる
        //0.004以下にするとcolorsensorを読み込まないかも?
        //pid.cppでlogをとっている、dが１桁だと安定？
          // linetrace2.PIDSetRun(7, 0.07, 0, 0.003, 1);    
          if(colorSensor.getColorNumber()==point_color){

            if(goal==next&&block[next]>0){
              msg_f("up",1);
              liftup();
              
             }
             else if(block[next]>0){
              fprintf(bt, "nextavoid\r\n");
              now=course[coursecount+1];
              next=course[coursecount+2];
              point();
              direction(togyro);
              avoid();

              break;
             }


            else if(point_colornumber==nowblock||nowblock==5){
              msg_f("down",1);

              minus=1;
              liftdown();
              rflag=0;
              break;
            }
        
            leftWheel2.setPWM(7);
            rightWheel2.setPWM(7); 
            clock.wait(1200);
            break;
          }     
        //else if(wheel_gyro<ang&&wheel_gyro>0){
        //msg_f("Running.", 1);
        //}

        }
      
        local2.SetPoint();
        clock.wait(4);
        rflag=0;
      



        fprintf(bt,"------finish------\r\n\n");
}


void Block::blockLineup(){
  point_color=COLOR_BLUE;
 liftup();
 direction(90);
 rotate(90);
 straight(1000);
 // terminate();


/*  if(map[2][0]>0){
  liftup();
  Block_color=ColorSensor.getColorNumber();
  liftdown();
  goalsearch(); 
  nowblock=1;
  }
  else{
  goalsearch(); 
  }
   leftWheel2.setPWM(7);
   rightWheel2.setPWM(7);
   clock.wait(1000);
   if(nowblock==0){
   }
   else{
    direction(togyro);
　　rotate(gyro);

   }
*/

}

void Block::avoid(){
  if(lflag==0){
    if(avoidgyro<=45){
    leftWheel2.setPWM(-40);
    rightWheel2.setPWM(-40);
    clock.wait(500);

   }
   else{
    leftWheel2.setPWM(-20);
    rightWheel2.setPWM(-20);
    clock.wait(500);
   }
  }
  else if(lflag==1&&avoidgyro<=45){
     leftWheel2.setPWM(-20);
    rightWheel2.setPWM(-20);
    clock.wait(500);
  }


  fprintf(bt,"\navoid!!!!\r\n");
  fprintf(bt,"clocktime=%d\r\n",clocktime);
  fprintf(bt,"avoidgyro=%d\r\n\n",avoidgyro);

  if(gyro>=0){

    rotate(80);
    steer.setPower(30,50);
    clock.wait(clocktime);
    rotate(80);
  }
  else{
    rotate(-80);
    steer.setPower(30,-50);
    clock.wait(clocktime);
    rotate(-90);
  }



  linetime=100;

}

void Block::dfs(int n, int goal, int limit)
{ 
  int x = path[n - 1];
  if (limit == n) {
   if(flag==1){
      flag=0;
      return;
    }
  if (x == goal) {
    flag=1;
    for (int i = 0; i < n; i++){
      course[i]=path[i];
      count++;
   //   printf("count=%d\r\n",count); 

    }


   }
  } 
  else {
  

    for (int i = 0; i < 4; i++) {
      if(flag==1){
        break;
      }

      int y = map[x-1][i];
 //     printf("map[%d][%d]=%d\r\n",x,i,y);
      if (y == 0) break;
      if (!visited[y]) {
        path[n] = y;
      //  printf("path[%d]=%d\r\n",n,y);
        visited[y] = true;
        dfs(n + 1, goal, limit);
        visited[y] = false;
     }
    }
  }
}

void Block::ids(int start, int goal){ 
  for (int j=0;j<=15;j++){
    visited[j]=0;
  }


  path[0] = start;;
  visited[start] = true;
  for (int limit = 2; limit <= 7; limit++) {
   
    if(flag==1){
      flag=0;
      return;
    }
  //  printf("---- %d -----\r\n", limit);
  
    dfs(1, goal, limit);
  }
}
void Block::SetLine(int black, int white, float normalize, int gray){
  linetrace2.Setblack(black);
  linetrace2.Setwhite(white);
  linetrace2.Setnormalize(normalize);
  linetrace2.Setgray(gray);
}


