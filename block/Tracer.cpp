#include "Tracer.h"

Tracer::Tracer():
  rightWheel(PORT_B),leftWheel(PORT_C){
}

void Tracer::init() {
  init_f("Tracer");
}

void Tracer::terminate() {
  msg_f("Stopped.", 3);
  leftWheel.stop();
  rightWheel.stop();
}

/*
// tag::tracer_calc_prop[]
float Tracer::calc_prop_value() {
  const float Kp = 0.83;        // <1>
  const int target = 10;        // <2>
  const int bias = 0;5
  int diff = color.getBrightness() - target; // <3>
  return (Kp * diff + bias);                       // <4>
}
// end::tracer_calc_prop[]
// tag::tracer_run[]
void Tracer::run() {
  msg_f("running...", 1);
  float turn = calc_prop_value(); // <1>
  int pwm_l = pwm - turn;      // <2>
  int pwm_r = pwm + turn;      // <2>
  leftWheel.setPWM(pwm_l);
  rightWheel.setPWM(pwm_r);
}
// end::tracer_run[]
*/

void Tracer::trace(){
  msg_f("running...", 1);
  /*linetrace.PIDSetRun(pwm,0.38, 0.06, 0.025);*/

}

void Tracer::stop() {
  leftWheel.stop();
  rightWheel.stop();
}

//debug
void Tracer::debug(){

      /*local. debug*/
      /*msg_f("local", 1);

      //trace();  //走行
      while(1){

      leftWheel.setPWM(20);
      rightWheel.setPWM(20);

      local.SetPoint();

      if(local.GetX() >= 100){
        stop();
        linetrace.Rotate(90);
        msg_f("Xend", 1);
        break;
      }
    }

  while(1){

      leftWheel.setPWM(20);
      rightWheel.setPWM(20);

      local.SetPoint();

      if(local.GetY() >= 100){
        stop();
        linetrace.Rotate(90);
        msg_f("Yend", 2);
        break;
      }*/
     /* }*/


      /*detect.step debug*/
      while(1){
      msg_f("step", 1);

      //trace();  //走行
      leftWheel.setPWM(40);
      rightWheel.setPWM(40);

      //clock.wait(500); //3秒待つ

      if(detect.StepDetection() == 1){

        stop();
        ev3_speaker_play_tone(NOTE_AS4,1000);

        msg_f("end", 1);
        wup_tsk(MAIN_TASK);

      }
    }


      /*detect.90 debug*
      msg_f("90", 1);

      trace();  //走行

      if(detect.RightAngleDetection() == 1){

        //90度旋回？

        ev3_speaker_play_tone(NOTE_AS4,1000);

        msg_f("end", 1);
        wup_tsk(MAIN_TASK);

      }*/

      /*detect.grey debug*
      msg_f("grey", 1);

      trace();  //走行

      if(detect.GrayDetection() == 1){

        stop();
        ev3_speaker_play_tone(NOTE_AS4,1000);

        msg_f("end", 1);
        wup_tsk(MAIN_TASK);

      }*/

      /*detect.wall debug*
      msg_f("wall", 1);

      if(detect.WallDetection() == 1){

        ev3_speaker_play_tone(NOTE_AS4,1000);

        msg_f("end", 1);
        wup_tsk(MAIN_TASK);

      }*/

      /*ETsumou.block debug*
      msg_f("block", 1);
      int ans = sumou.BlockCheck(COLOR_RED);
      if(ans == 1){
          ev3_speaker_play_tone(NOTE_AS4,1000);

          msg_f("end", 2);
      }*/
  
  }
