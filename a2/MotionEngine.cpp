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
	motionWaveHands = false;
}

void MotionEngine::setMotionShuffle(Robot *obj) {
	motionRandom = false;
	motionShuffle = true;
	motionWaveHands = false;
}

void MotionEngine::setMotionWaveHands(Robot *obj) {
	motionRandom = false;
	motionShuffle = false;
	motionWaveHands = true;
	_robot->handLeft.turnShoulder(0, -180);
	_robot->handRight.turnShoulder(0, -180);
	_robot->handLeft.turnForeHand(0, 90);
	_robot->handRight.turnForeHand(0, 90);
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
	else if (motionWaveHands) {
		this->nextMotionStateWaveHands();
	}
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

void MotionEngine::nextMotionStateWaveHands() {
	int delta = (int) 4*cos(deltaAngle*20*3.1415/360);
	int delta2 = (int) 5*cos(deltaAngle*15*3.1415/360);
	int delta3 = (int) 2*cos(deltaAngle*10*3.1415/360);
	
	_robot->handLeft.turnShoulder(0, -delta);
	_robot->handRight.turnShoulder(0, delta);
	_robot->handLeft.turnForeHand(delta, 0);
	_robot->handRight.turnForeHand(delta, 0);
	_robot->handLeft.turnForeHand(0, 2);
	_robot->handRight.turnForeHand(0, -2);

	_robot->legLeft.turnHip(0, delta2);
	_robot->legRight.turnHip(0, -delta2);
	_robot->legLeft.turnShin(delta2, 0);
	_robot->legRight.turnShin(delta2, 0);
	
	_robot->turnTorso(0, 0, delta3);
	_robot->turnTorso(0, 2*delta3, 0);

	deltaAngle++;
	deltaAngle%=360;
}

void MotionEngine::nextMotionStateShuffle() {

}

void MotionEngine::reset() {
	this->motionRandom = true;

	deltaAngle = 2;
}