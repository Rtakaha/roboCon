//9.21メモ
//case文がやたら無駄
//フラグの立てすぎ
//fprintfはデバッグ用
//参考非推奨 
//written by T.N




#include "Block.h"


using namespace ev3api;

Block::Block():
frontarm(PORT_A),rightWheel2(PORT_B),leftWheel2(PORT_C), steer(leftWheel2,rightWheel2),
colorSensor(PORT_3){

}


/*初期値設定*/
void Block::init() {
  init_f("Block");
  
  nowgyro=30;
  nowblock=0;
  start=10;

  leftWheel2.reset();
  rightWheel2.reset();

  local2.Init();

  //linetrace2.Calibration();
  clock.wait(1000);


}


/*引数は回転する角度
ev3_motor_rotate(port,degrees,speed,blocking);
blocking=true:回転してから次を読む
        false:回転を待たずに次を読む
*/
void Block::rotate(int gyro){
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








void Block::blockLineup(){
  //int course[ ]={1,3,2,7,11,15,12,9,8,5,4,6,10,13,14,0};/*経路格納*/
  //int course[ ]={1,3,7,3,5,8,5,3,5,8,11,8,11,15,14,13,9,12,0};
 start=10;
 if(block[10]>0){//初期位置にブロック有り
  next=10;
  liftup();
  goalsearch();//フラグ設定
  targetsearch();

 }
 else{
 goalsearch();
 }
 leftWheel2.setPWM(7);
 rightWheel2.setPWM(7);
 clock.wait(1000);

 fprintf(bt,"{");
 for(int j=0;j<10;j++){
 fprintf(bt,"%d,",course[j]);
 }
 fprintf(bt,"}\r\n");

  while(1){



       
    now=course[coursecount];/*現在の点*/
    next=course[coursecount+1];/*次に進む点*/
    next2=course[coursecount+2];
    
    fprintf(bt,"now=%d\r\n",now);
    fprintf(bt,"next=%d\r\n",next);
   

    
  /*  if(next==0){
     break;
     }経路0を読んだら終了*/
  
    /*画面表示*/
    display();
    
    /*運搬なし*/
    if(nowblock==0){
    

      //ブロック並べが終わり11にたどり着いた時停止
      


    /*  if(now==0||next==0){
        coursecount=0;
        start=goal;

        //ブロック並べが終わった時
        //現時点では１つ並べた時(goalflag==5が正しい値)
        if(goalflag>0){
          goal=11;
          ids(start,goal);
          goalflag==6;
        }
        else{
      
        goalsearch();//最初の１回だけ読んで経路設定
        }
       now=course[coursecount];//現在の点
       next=course[coursecount+1];//次に進む点
       next2=course[coursecount+2];
      }*/

      fprintf(bt,"course={");
    for(int j=0;j<10;j++){
    fprintf(bt,"%d,",course[j]);
    }
    fprintf(bt,"}\r\n");

    fprintf(bt,"finishflag=%d\r\n",finishflag);
    if(next==0&&finishflag==1){
        fprintf(bt,"goal!\r\n");
        togyro=60;
        gyro=togyro-nowgyro;
        rotate(gyro);
        terminate();
        break;

      }


        display();
        point();
        direction(togyro);
        straight(dis);
       
      }
    /*運搬中*/
    else{

      fprintf(bt,"blockup!\r\n");

      //初回のみcourse設定
      if(now==0||next==0){
      coursecount=0;
      
      for(int j=0;j<10;j++){
        course[j]=0;
      }//courseの初期化


      start=goal;

      fprintf(bt,"start=%d\r\n",start);
      fprintf(bt,"nowblock=%d\r\n",nowblock);

      targetsearch();

      now=course[coursecount];/*現在の点*/
      next=course[coursecount+1];/*次に進む点*/
      next2=course[coursecount+2];




    fprintf(bt,"{");
    for(int j=0;j<10;j++){
    fprintf(bt,"%d,",course[j]);
    }
    fprintf(bt,"}\r\n");


      }


      display();
      point();
      direction(togyro);
        
      
      straight(dis);
      
    }

    coursecount++;


  }
  
}




/*方向計算*/
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
    terminate();
    frontarm.setPWM(5);
    clock.wait(300);
    frontarm.stop();
    nowblock=block[next];/*現在持っているブロックの色を設定*/
    block[next]=0;
    
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
    


/*引数は距離(cm)
 直進、停止、ブロックの上げ下げを行う
*/
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

void Block::codecalculation(int code){


//初期位置コード = (黒ブロック位置番号 - 1) x 11 x 11 x 11 +
// (赤ブロック位置番号 - 1) x 11 x 11 +
// (黄ブロック位置番号 - 1) x 11 +
// (青ブロック位置番号 - 1) 
  
//黒3赤10黄11青11緑9=3871
   green=9;
   black_b=code/1331+1;
  code=code % 1331;
   red=code/121+1;
  code=code % 121;
   yellow=code/11+1;
  code=code % 11;
   blue=code+1;

switch(red){
  case 1:
    red=2;
    break;
  case 2:
    red=3;
    break;
  case 3:
    red=4;
    break;
  case 4:
    red=5;
    break;
  case 5:
    red=6;
    break;
  case 6:
    red=9;
    break;
  case 7:
    red=10;
    break;
  case 8:
    red=11;
    break;
  case 9:
    red=12;
    break;
  case 10:
    red=13;
    break;
  case 11:
    red=15;
    break;
  }

switch(yellow){
  case 1:
    yellow=1;
    break;
  case 2:
    yellow=2;
    break;
  case 3:
    yellow=4;
    break;
  case 4:
    yellow=6;
    break;
  case 5:
    yellow=7;
    break;
  case 6:
    yellow=8;
    break;
  case 7:
    yellow=9;
    break;
  case 8:
    yellow=10;
    break;
  case 9:
    yellow=11;
    break;
  case 10:
    yellow=12;
    break;
  case 11:
    yellow=14;
    break;
  }

  switch(blue){
  case 1:
    blue=1;
    break;
  case 2:
    blue=3;
    break;
  case 3:
    blue=5;
    break;
  case 4:
    blue=6;
    break;
  case 5:
    blue=7;
    break;
  case 6:
    blue=8;
    break;
  case 7:
    blue=10;
    break;
  case 8:
    blue=11;
    break;
  case 9:
    blue=13;
    break;
  case 10:
    blue=14;
    break;
  case 11:
    blue=15;
    break;
  }



  block[green]=1;
  block[red]=2;
  block[yellow]=3;
  block[blue]=4;
  block[black_b]=5;

  fprintf(bt,"block={");
  for(int j=0;j<=14;j++){
    fprintf(bt,"%d,",block[j]);
  }
  fprintf(bt,"}");
} 


//停止

void Block::terminate() {
  msg_f("Stopped.", 1);
  leftWheel2.stop();
  rightWheel2.stop();
}


/*値調整*/
void Block::point(){
  int n=now*100+next;/*数を連結させる（例：1415→現在地14,次点15）*/

/*点の色設定*/
  /*修正後*/
switch(next){
  case  6:
  case 10:
  case 11:
    point_colornumber=1;
    point_color=COLOR_GREEN;
    break;
  case  1:
  case  7:
  case  8:
  case 14:
    point_colornumber=2;
    point_color=COLOR_RED;
    break;
  case  3:
  case  5:
  case 13:
  case 15:
    point_colornumber=3;
    point_color=COLOR_YELLOW;
    break;

  case  2:
  case  4:
  case  9:
  case 12:
    point_colornumber=4;
    point_color=COLOR_BLUE;
    break;
  
  }
  

/*角度設定,togyro=(目標点までの角)*/
switch(n){

  case 1104:
      togyro=15;
      break;

  case 1005:
  case 1208:
  case 1409:
      togyro=30;
      break;

  case  502:
  case  806:
  case  603:
  case  907:
  case  704:
  case 1511:
      togyro=60;
      break;

  case  102:
  case 1213:
  case 1314:
  case  203:
  case 1415:
  case  304:
      togyro=90;
      break;

  case 1012:
  case  105:
  case  508:
  case  206:
  case  609:
  case  307: 
      togyro=120;
      break;

  case  813:
  case  915:
  case  711:
      togyro=150;
      break;

  case  110:
      togyro=165;
      break;

    case 411:
      togyro=195;
      break;

  case  510:
  case  812:
  case  914:
      togyro=210;
      break;

  case  205:
  case  608:
  case  306:
  case  709:
  case  407:
  case 1115:
      togyro=240;
      break;

  case  201:
  case 1312:
  case 1413:
  case  302:
  case 1514:
  case  403:
      togyro=270;
      break;

  case 1210:
  case  501:
  case  805:
  case  602:
  case  906:
  case  703: 
      togyro=300;
      break;

  case 1308:
  case 1509:
  case 1107:
      togyro=330;
      break;

  case  1001:
      togyro=345;
      break;

  default:
      togyro=0;
      break;

}


/*距離設定,dis=(2点の中心間の距離)(単位:cm)*/
switch(n){

  case 1001:
  case  110:
  case  411:
  case 1104:
      dis=45.0000*1.4142;
      break;

  
  case  102:
  case  201:
  case  302:
  case  203:
  case  304:
  case  403:
      dis=45.0000*1.7320;
      break;

  case 1314:
  case 1413:
      dis=45.0000*0.7320;
      break;
  default:
      dis=45.0000;
      break;


  }
}

void Block::display(){

/*色表示用,直前に読んだ色を表示*/
  switch(point_color){
      case COLOR_GREEN:
        msg_f("green",2);
        break;
      case COLOR_RED:
        msg_f("red",2);
        break;
      case COLOR_YELLOW:
        msg_f("yellow",2);
        break;
      case COLOR_BLUE:
        msg_f("blue",2);
        break;
      default:
        break;
    }

/*運んでいるブロックの色を表示*/
  switch(nowblock){
       case 1:
        msg_f("Green carrying",3);
        break;
       case 2:
        msg_f("red carrying",3);
        break;
       case 3:
        msg_f("Yellow carrying",3);
        break;
       case 4:
        msg_f("Blue carrying",3);
        break;
       case 5:
        msg_f("Black carrying",3);
        break;   
       case 0:
        msg_f("No carrying",3);
        break;
       default:
        msg_f("Error",3);
        
    }

//   いらないやつ
    switch(block[next]){
      case 1:
        msg_f("Green ",5);
        break;
       case 2:
        msg_f("red ",5);
        break;
       case 3:
        msg_f("Yellow ",5);
        break;
       case 4:
        msg_f("Blue ",5);
        break;
       case 5:
        msg_f("Black ",5);
        break;
       case 0:
        msg_f("Nothing",5);
        break;
      default:
        msg_f("Error! ",5);
        break;
        
    }

    switch(next){
       case 1:
        msg_f("next:1",4);
        break;
       case 2:
        msg_f("next:2",4);
        break;
       case 3:
        msg_f("next:3",4);
        break;
       case 4:
        msg_f("next:4",4);
        break;
       case 5:
        msg_f("next:5",4);
        break;   
       case 6:
        msg_f("next:6",4);
        break;
       case 7:
        msg_f("next:7",4);
        break;
       case 8:
        msg_f("next:8",4);
        break;
       case 9:
        msg_f("next:9",4);
        break;
       case 10:
        msg_f("next:10",4);
        break;
       case 11:
        msg_f("next:11",4);
        break;   
       case 12:
        msg_f("next:12",4);
        break;
       case 13:
        msg_f("next:13",4);
        break;
       case 14:
        msg_f("next:14",4);
        break;
       case 15:
        msg_f("next:15",4);
        break;
       case 0:
        msg_f("next:0",4);
        break;
      }

       switch(now){
       case 1:
        msg_f("now:1",3);
        break;
       case 2:
        msg_f("now:2",3);
        break;
       case 3:
        msg_f("now:3",3);
        break;
       case 4:
        msg_f("now:4",3);
        break;
       case 5:
        msg_f("now:5",3);
        break;   
       case 6:
        msg_f("now:6",3);
        break;
       case 7:
        msg_f("now:7",3);
        break;
       case 8:
        msg_f("now:8",3);
        break;
       case 9:
        msg_f("now:9",3);
        break;
       case 10:
        msg_f("now:10",3);
        break;
       case 11:
        msg_f("now:11",3);
        break;   
       case 12:
        msg_f("now:12",3);
        break;
       case 13:
        msg_f("now:13",3);
        break;
       case 14:
        msg_f("now:14",3);
        break;
       case 15:
        msg_f("now:15",3);
        break;
       case 0:
        msg_f("now:0",3);
        break;
     
    }



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


//ブロック所持時の経路設定
void Block::targetsearch(){
  switch(nowblock){
    case 1:
      switch(green){
        case 1:
        case 5:
        case 12:
        case 13:
          goal=10;
          ids(start,goal);
          break;
        case 2:
        case 3:
        case 8:
        case 9:
          goal=6;
          ids(start,goal);
          break;
        case 4:
        case 7:
        case 14:
        case 15:
          goal=6;
          ids(start,goal);
          break;
      }
      break;
    case 2:
      switch(red){
        case 2:
        case 5:
        case 10:
          goal=1;
          if(block[1]>0){
            goal=8;
          }
          ids(start,goal);
          break;
        case 6:
        case 12:
        case 13:
          goal=8;
          if(block[8]>0){
            goal=1;
          }
          ids(start,goal);
          break;
        case 3:
        case 4:
        case 9:
        case 11:
        case 15:
          goal=7;
          if(block[7]>0){
            goal=14;
          }
          ids(start,goal);
          break;
        
      }
      break;
    case 3:
      switch(yellow){
        case 1:
        case 2:
        case 8:
        case 10:
          goal=5;
          if(block[5]>0){
            goal=3;
          }
          ids(start,goal);
          break;
        case 4:
        case 6:
        case 7:
          goal=3;
          if(block[3]>0){
            goal=5;
          }
          ids(start,goal);
          break;
        case 9:
        case 11:
          goal=15;
          if(block[15]>0){
            goal=13;
          }
          ids(start,goal);
          break;
        case 12:
        case 14:
          goal=13;
          if(block[13]>0){
            goal=15;
          }
          ids(start,goal);
          break;
      }
      break;
    case 4:
     switch(blue){
        case 1:
        case 3:
        case 5:
        case 6:
          goal=2;
          if(block[2]>0){
            goal=11;
          }
          ids(start,goal);
          break;
        case 7:
        case 11:
          goal=4;
          if(block[4]>0){
            goal=9;
          }
          ids(start,goal);
          break;
        case 8:
        case 10:
        case 13:
          goal=11;
          if(block[11]>0){
            goal=2;
          }
          ids(start,goal);
          break;
        case 14:
        case 15:
          goal=9;
          if(block[9]>0){
            goal=4;
          }
          ids(start,goal);
          break;
      }
      break;
    case 5:
      for(int j=0;j<4;j++){
        int tmp=map[black_b-1][j];
        if(block[tmp]==0){
          goal=map[black_b-1][j];
          break;
        }
      }
      ids(start,goal);
      break;

  }
  count=0;
}


//ブロック非所持時の経路設定
void Block::goalsearch(){
 int compare[5]={100,100,100,100,100};
 int tmp=0;

  if(black_flag==0){

    ids(start,black_b);
    compare[0]=count;
    count=0;

  }
  if(red_flag==0){
    ids(start,red);
    compare[1]=count;
    count=0;
  }
  if(yellow_flag==0){
    ids(start,yellow);
    compare[2]=count;
    count=0;
  }
  if(blue_flag==0){
    ids(start,blue);
    compare[3]=count;
    count=0;
  }
  if(green_flag==0){
  ids(start,green);
  compare[4]=count;
  count=0;
  }
  int min=compare[0];
  for(int j=0;j<=4;j++){
    if(compare[j]!=0&&compare[j]<=min){
      min=compare[j];
      tmp=j;
    }
  }

  fprintf(bt,"{");
  for(int j=0;j<=4;j++){
    fprintf(bt,"compare[%d]=%d",j,compare[j]);
  }
  fprintf(bt,"}\r\n");


    for(int j=0;j<10;j++){
      course[j]=0;
    }


   switch(tmp){
    case 0:
      goal=black_b;
      ids(start,goal);
      black_flag=1;
      break;
    case 1:
      goal=red;
      ids(start,goal);
      red_flag=1;
      break;
    case 2:
      goal=yellow;
      ids(start,goal);
      yellow_flag=1;
      break;
    case 3:
      goal=blue;
      ids(start,goal);
      blue_flag=1;
      break;
    case 4:
      goal=green;
      ids(start,goal);
      green_flag=1;
      break;
    }
    count=0;
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





