#pragma once
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

