#include "ev3api.h"
#include "Motor.h"
#include "Clock.h"
#include "ColorSensor.h"
//#include "TouchSensor.h"
//#include "GyroSensor.h"
//#include "SonarSensor.h"
#include "util.h"

#include "LineTrace.h"
#include "Detect.h"
#include "Localization.h"
#include "ETsumou.h"


using namespace ev3api;

//#include "BoundaryDefine.h"

class Tracer {
public:
  Tracer();
  void run();
  void init();
  void terminate();
  void trace();
  void stop();
  void debug();

private:
  Motor rightWheel;
  Motor leftWheel;
  Clock clock;
  //ColorSensor color;// -> Lintrace
  //TouchSensor touch;
  //GyroSensor gyro;
  //SonarSensor sonar;
  LineTrace linetrace;
  Detect detect;
  Localization local;
  ETsumou sumou;


  const int8_t mThreshold = 20;
  const int8_t pwm = (Motor::PWM_MAX) / 4;

  float calc_prop_value();      // <1>
};
