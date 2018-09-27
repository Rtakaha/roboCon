#include "app.h"

#include "Tracer.h"
#include "AreaManager.h"
//#include "Block.h"

Tracer tracer;
AreaManager areamanager;
//Block block;

void tracer_cyc(intptr_t exinf) {
  act_tsk(TRACER_TASK);
}

void cycle_cyc(intptr_t exinf){
  act_tsk(CYCLE);
}

void tracer_task(intptr_t exinf) {
  if (ev3_button_is_pressed(UP_BUTTON)) {
    wup_tsk(MAIN_TASK);  // 上ボタン押下でメインを起こす（緊急停止用）
  } else {
    //tracer.trace();  //走行

    areamanager.Manager();
    //tracer.debug();

    //areamanager.Manager(); //周期呼び出し

  }
  ext_tsk();
}

void cycle(intptr_t exinf){

    areamanager.regularLine();

    ext_tsk();
}

void main_task(intptr_t unused) {
  
  //ここでキャリブレーション処理以外の各種初期化を行う
  areamanager.Init(); 
  
  ev3_sta_cyc(TRACER_CYC);
  slp_tsk();  // 起きたら走行をやめる
  ev3_stp_cyc(TRACER_CYC);

  areamanager.Manager(); //周期処理じゃない

  //block.init();
  //block.blockLineup();

  //tracer.debug();

  tracer.terminate();
  ext_tsk();
}
