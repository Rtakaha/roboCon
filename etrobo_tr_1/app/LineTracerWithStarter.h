#ifndef EV3_APP_LINETRACERWITHSTARTER_H_
#define EV3_APP_LINETRACERWITHSTARTER_H_

#include "LineMonitor.h"
#include "BalancingWalker.h"

class LineTracerWithStarter {
public:
    void LineTracerWithStarter();

    void run();

private:
	const app::LineTracer& mLineTracer;
	const unit::Starter& Starter;

};

#endif  // EV3_APP_LINETRACER_H_
