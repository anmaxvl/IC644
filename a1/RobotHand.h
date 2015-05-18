#pragma once
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

