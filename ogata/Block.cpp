//2017,6,13メモ
//基本的にinitを実行して初期化。そのあと、codecalcullationで初期値を打ち込む。BlockLineup実行でブロック並べをするプログラム
//そのためそれ以外の関数はBlockLineupのための関数
//仕組みについては関数名とコメントを見て
//written by O.T
//ver.1.00
//2016年用の中崎先輩のものを変更する形で編集




#include "Block.h"
#include "ev3api.h"

static FILE *bt = ev3_serial_open_file(EV3_SERIAL_BT);

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

void Block::terminate() {
  msg_f("Stopped.", 1);
  leftWheel2.stop();
  rightWheel2.stop();
}

 void Block::liftup(){
    msg_f("Lift upping",1);
    terminate();
    frontarm.setPWM(5);
    clock.wait(300);
    frontarm.stop();
	}

 void Block::liftdown(){
    msg_f("Lift downing",1);
    terminate();
    frontarm.setPWM(-5);
    clock.wait(300);
    frontarm.stop();
    leftWheel2.setPWM(-20);
    rightWheel2.setPWM(-20);
    clock.wait(500);
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

void Block::searchblock(){//nearstがlocationに最短のblockの番号を与える関数
  fprintf(bt, "searchblockStart¥n");
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
 d3=(zahyo(location,0)-zahyo(block[3],0))*(zahyo(location,0)-zahyo(block[3],0))+(zahyo(location,1)-zahyo(block[3],1))*(zahyo(location,1)-zahyo(block[3],1));
 }

  if(block[0]!=-1){ 
  nearest=block[0];
 } 
  if(block[1]!=-1){ 
    if(d0>d1){
    nearest=block[1];
    }
  }
  if(block[2]!=-1){ 
    if(d1>d2&&d0>d2){
    nearest=block[2];
    }
  } 
  if(block[3]!=-1){
   if(d2>d3&&d1>d3&&d0>d3){
      nearest=block[3];
    } 
  }
  fprintf(bt, "searchblockEnd¥n");
}

void Block::direction(int togyro){
  // fprintf(bt,"togyro=%d\r\n",togyro);
  // fprintf(bt,"nowgyro=%d\r\n",nowgyro);
  gyro=togyro-nowgyro; 
  nowgyro=togyro;
  /*if(gyro==180||gyro==-180){
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
  */
}



void Block::gopurpose(int location,int nearest){//周りのblockを避けながら最短経路でlocationからnearestに移動する関数
  fprintf(bt, "gopurposeStart¥n");
  while(1){
  int x=zahyo(location,0);
  int y=zahyo(location,1);
  int nearestx=zahyo(nearest,0);
  int nearesty=zahyo(nearest,1);
  if(x==nearestx){
    break;
  }
  else if(nearestx>x&&(location+1)!=block[0]&&(location+1)!=block[1]&&(location+1)!=block[2]&&(location+1)!=block[3])
  {
    location+=1;
    togyro=0;
    direction(togyro);
    rotate(gyro);
    ev3_led_set_color(LED_GREEN);
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
      fprintf(bt, "gopurpose, COLOR_WHITE");
      fprintf(bt, "location%d", location);
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);
      //白を読み込むとなんかするやつ
    }
  }
  else if(nearestx<x&&(location-1)!=block[0]&&(location-1)!=block[1]&&(location-1)!=block[2]&&(location-1)!=block[3])
  {
    location-=1;
    togyro=180;
    direction(togyro);
    rotate(gyro);

    ev3_led_set_color(LED_GREEN);
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);
      //白を読み込むとストップするなんか
    }
 }
 else if(y!=nearesty){
  if(nearesty>y&&(location+4)!=block[0]&&(location+4)!=block[1]&&(location+4)!=block[2]&&(location+4)!=block[3])
  {
    location+=4;
    togyro=-90;
    direction(togyro);
    rotate(gyro);
    ev3_led_set_color(LED_GREEN);
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);
      //白を読み込むとなんかするやつ
    }
  }
  else if(nearesty<x&&(location-4)!=block[0]&&(location-4)!=block[1]&&(location-4)!=block[2]&&(location-4)!=block[3])
  {
    location-=4;
    togyro=90;
    direction(togyro);
    rotate(gyro);

    ev3_led_set_color(LED_GREEN);
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);
      //白を読み込むとストップするなんか
        }
      }
  
    }  

  }

  while(1){
  int x=zahyo(location,0);
  int y=zahyo(location,1);
  int nearestx=zahyo(nearest,0);
  int nearesty=zahyo(nearest,1);
  if(nearesty==y){
    break;
  }
  else if(nearesty>y&&(location+4)!=block[0]&&(location+4)!=block[1]&&(location+4)!=block[2]&&(location+4)!=block[3])
  {
    location+=4;
    togyro=-90;
    direction(togyro);
    rotate(gyro);
    ev3_led_set_color(LED_GREEN);
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);
      //白を読み込むとなんかするやつ
    }
  }
  else if(nearesty<x&&(location-4)!=block[0]&&(location-4)!=block[1]&&(location-4)!=block[2]&&(location-4)!=block[3])
  {
    location-=4;
    togyro=90;
    direction(togyro);
    rotate(gyro);

    ev3_led_set_color(LED_GREEN);
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);
      //白を読み込むとストップするなんか
        }
      }
      else if(x!=nearestx){if(nearestx>x&&(location+1)!=block[0]&&(location+1)!=block[1]&&(location+1)!=block[2]&&(location+1)!=block[3])
  {
    location+=1;
    togyro=0;
    direction(togyro);
    rotate(gyro);
    ev3_led_set_color(LED_GREEN);
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);
      //白を読み込むとなんかするやつ
    }
  }
  else if(nearestx<x&&(location-1)!=block[0]&&(location-1)!=block[1]&&(location-1)!=block[2]&&(location-1)!=block[3])
  {
    location-=1;
    togyro=180;
    direction(togyro);
    rotate(gyro);

    ev3_led_set_color(LED_GREEN);
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);
      //白を読み込むとストップするなんか
    }
 }

      }
  }
  fprintf(bt, "gopurposeEnd¥n");
}
void Block::Blockcolor(){
  fprintf(bt, "BlockcolorStart¥n");
  liftup();
  Block_color=colorSensor.getColorNumber();
  liftdown();
  fprintf(bt, "BlockcolorEnd¥n");
}
void Block::urochoro(int location){//置いた後戻る動作はまだ書いていない
  fprintf(bt, "urochoroStart¥n");
   while(1){
    if(location==block[0]){
      block[0]=-1;
    }
    if(location==block[1]){
      block[1]=-1;
    }
    if(location==block[2]){
      block[2]=-1;
    }
    if(location==block[3]){
      block[3]=-1;
    }

    if(color[zahyo(location,0)][zahyo(location,1)]==Block_color){
      break;
    }
    else if(color[zahyo(location,0)][zahyo(location,1)]==color[zahyo(location,0)+1][zahyo(location,1)]&&map[zahyo(location,0)+1][zahyo(location,1)]!=1)
   {
    togyro=0;
    direction(togyro);
    rotate(gyro);
    location+=1;
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
      leftWheel2.setPWM(20);
      rightWheel2.setPWM(20);
      clock.wait(20); 
    }
  }
  else if(color[zahyo(location,0)][zahyo(location,1)]==color[zahyo(location,0)-1][zahyo(location,1)]&&map[zahyo(location,0)-1][zahyo(location,1)]!=1)
  {
    togyro=180;
    direction(togyro);
    rotate(gyro);
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
      leftWheel2.setPWM(20);
      rightWheel2.setPWM(20);
      clock.wait(20);  
    }
  }
  else if(color[zahyo(location,0)][zahyo(location,1)]==color[zahyo(location,0)][zahyo(location,1)+4]&&map[zahyo(location,0)][zahyo(location,1)+4]!=1)
 {
  togyro=-90;
  direction(togyro);
  rotate(gyro);
 leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);  
 }
 }
 else if(color[zahyo(location,0)][zahyo(location,1)]==color[zahyo(location,0)+1][zahyo(location,1)-4]&&map[zahyo(location,0)][zahyo(location,1)-4]!=1)
 {
  togyro=90;
  direction(togyro);
  rotate(gyro);
 leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);  
 }
 }
 else {
  if (map[zahyo(location,0)-1][zahyo(location,1)]!=1){
      togyro=0;
  direction(togyro);
  rotate(gyro);
  location+=1;
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);  
     }
   }
       if(map[zahyo(location,0)+1][zahyo(location,1)]!=1){
      togyro=180;
  direction(togyro);
  rotate(gyro);
  location-=1;
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);  
     }
   }
       if (map[zahyo(location,0)][zahyo(location,1)+4]!=1){
      togyro=-90;
  direction(togyro);
  rotate(gyro);
  location+=4;
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);  
     }
   }
       if (map[zahyo(location,0)+1][zahyo(location,1)-4]!=1){
      togyro=90;
  direction(togyro);
  rotate(gyro);
  location-=4;;
    leftWheel2.setPWM(20);
    rightWheel2.setPWM(20);
    if(colorSensor.getColorNumber()==COLOR_WHITE){
      terminate();
       leftWheel2.setPWM(20);
       rightWheel2.setPWM(20);
       clock.wait(20);  
     }
   }
 } 



  
  }
  fprintf(bt, "urochoroEnd¥n");

}

void Block::SetLine(int black, int white, float normalize, int gray){
  linetrace2.Setblack(black);
  linetrace2.Setwhite(white);
  linetrace2.Setnormalize(normalize);
  linetrace2.Setgray(gray);
}



void Block::blockLineup(){//実行関数
  while((block[0]!=-1)&&(block[1]!=-1)&&(block[2]!=-1)&&(block[3]!=-1)){
  searchblock();
  gopurpose(location,nearest);
  Blockcolor();
  urochoro(location);
}
}
