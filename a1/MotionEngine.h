#pragma once

#include "Robot.h"

class MotionEngine
{
	Robot *_robot;

	bool motionRandom, motionShuffle;
	void nextMotionStateRandom();
	void nextMotionStateShuffle();

	int postureChangeMode;

	int deltaAngle;

public:
	MotionEngine(Robot *robot);
	MotionEngine(void);
	~MotionEngine(void);

	void keyboardPressHandler(unsigned char key, int x, int y);
	void setMotionRandom(Robot *obj);
	void setMotionShuffle(Robot *obj);
	void nextMotionState(Robot *obj);
	void reset();
};
