#include <iostream>
#include "MotionEngine.h"

using namespace std;

MotionEngine::MotionEngine(void){
}

MotionEngine::~MotionEngine(void){
}

MotionEngine::MotionEngine(Robot *robot) {
	this->motionRandom = true;

	deltaAngle = 3;

	_robot = robot;
}

void MotionEngine::setMotionRandom(Robot *obj) {
	motionRandom = true;
	motionShuffle = false;
}

void MotionEngine::setMotionShuffle(Robot *obj) {
	motionRandom = false;
	motionShuffle = true;
}

void MotionEngine::nextMotionState(Robot *obj) {
	if (_robot != obj) {
		_robot = obj;
		this->reset();
	}
	if (motionRandom)
		this->nextMotionStateRandom();
	else if (motionShuffle)
		this->nextMotionStateShuffle();
}

void MotionEngine::nextMotionStateRandom () {
	if (_robot->legRight.hipAngleX>=40)
		deltaAngle = -deltaAngle;
	else if (_robot->legRight.hipAngleX<=-40)
		deltaAngle = -deltaAngle;

	_robot->legRight.turnHip(deltaAngle, 0);
	_robot->legLeft.turnHip(-deltaAngle, 0);
	_robot->legRight.turnShin(deltaAngle, 0);
	_robot->legLeft.turnShin(-deltaAngle, 0);
	_robot->legRight.turnFoot(-deltaAngle, 0);
	_robot->legLeft.turnFoot(deltaAngle, 0);

	_robot->handRight.turnShoulder(-deltaAngle, 0);
	_robot->handLeft.turnShoulder(deltaAngle, 0);
	_robot->handRight.turnForeHand(-deltaAngle, 0);
	_robot->handLeft.turnForeHand(deltaAngle, 0);

	_robot->turnTorso(deltaAngle/5, deltaAngle/2, 0);
}

void MotionEngine::nextMotionStateShuffle() {

}

void MotionEngine::reset() {
	this->motionRandom = true;

	deltaAngle = 2;
}