#pragma once

#include "RobotHand.h"
#include "RobotLeg.h"
#include "RobotHead.h"

class Robot
{
private:
	float torso[3];
	float shoulderJoints[3];
	float waist[3];

	int torsoAngleX, torsoAngleY, torsoAngleZ;

	float joint;

	bool animating;
	bool randomAnimation;
	bool shuffleDanceAnimation;

	void drawUpper();
	void drawLower();
	void drawHead();
public:
	RobotHead head;
	RobotHand handLeft, handRight;
	RobotLeg legLeft, legRight;

	Robot(void);
	Robot(float *tor, float *shJ, float *wst, float jointD);
	~Robot(void);

	void initHands(float *sh, float *fh, float *wr, float jointD);
	void initLegs(float *hp, float *shn, float *ft, float jointD);
	void initHead(float *hd, float* nck, float jointD);

	void turnTorso(int angleX, int angleY, int angleZ);

	void draw();

	friend class MotionEngine;
};

