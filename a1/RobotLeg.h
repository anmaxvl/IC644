#pragma once
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

