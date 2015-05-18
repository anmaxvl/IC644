#pragma once
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

