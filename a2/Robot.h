#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#include "types.h"

class RobotLeg
{
private:
	float hip[3];
	float shin[3];
	float foot[3];

	int hipAngleX, hipAngleZ;
	int kneeAngleX, kneeAngleY;
	int footAngleX, footAngleZ;

	float jointDistance;

	bool right;
public:
	RobotLeg(float* hip, float *shin, float *foot, float jointD, bool right);
	RobotLeg(void);
	~RobotLeg(void);

	void turnHip(int angleX, int angleZ);
	void turnShin(int angleX, int angleY);
	void turnFoot(int angleX, int angleZ);
	void draw();

	friend class MotionEngine;
};

class RobotTorso
{
public:

	float shoulderJoints[3];
	float chest[3];
	float waist[3];

	int waistAngleX, waistAngleY;

	RobotTorso(float* sh, float* ch, float* ws);

	RobotTorso(void);
	~RobotTorso(void);

	void turnChest(int angleX, int angleY);
};


class RobotHand
{
private:
	float shoulder[3];
	float forehand[3];
	float wrist[3];

	int shoulderAngleX, shoulderAngleZ;
	int elbowAngleX, elbowAngleY;
	int wristAngleX, wristAngleZ;

	float jointDistance;

	bool right;
public:
	RobotHand(void);
	RobotHand(float* sh, float *fh, float* wr, float jointD, bool right);
	~RobotHand(void);

	void turnShoulder(int angleX, int angleZ);
	void turnForeHand(int angleX, int angleY);
	void turnWrist(int angleX, int angleZ);
	void draw();

	friend class MotionEngine;
};

class RobotHead
{
private:
	float head[3];
	float neck[3];

	int headAngleX, headAngleY, headAngleZ;

	float jointDistance;
public:
	RobotHead(void);
	RobotHead(float* h, float *n, float jointD);
	~RobotHead(void);

	void draw();
	void turnHead(int angleX, int angleY, int angleZ);

	friend class MotionEngine;
};

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
	bool waveHandsAnimation;

	void drawUpper();
	void drawLower();
	void drawHead();

	Point frenetFrame[3];

	GLuint texture[2];
	void generateTextures();
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

	void setFrenetFrame(Point tangent, Point normal, Point binormal);
};

