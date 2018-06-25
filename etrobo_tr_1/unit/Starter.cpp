#include "Starter.h"

Starter::Starter(
	const ev3api::TouchSensor& TouchSensor)
		: mTouchSensor(TouchSensor){

	}

bool Starter::isPushed() const {
	return mTouchSensor.isPressed();
}