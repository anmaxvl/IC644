#include "RobotHead.h"
#include "functions.h"

#include <gl/glut.h>

RobotHead::RobotHead(void){}

RobotHead::~RobotHead(void){}

RobotHead::RobotHead(float* h, float *n, float jointD) {
	for (int i=0; i<3; i++) {
		this->head[i] = h[i];
		this->neck[i] = n[i];
	}

	this->headAngleX = 0;
	this->headAngleY = 0;
	this->headAngleZ = 0;
	this->jointDistance = jointD;
}

void RobotHead::turnHead(int angleX, int angleY, int angleZ){
	this->headAngleX += angleX;
	this->headAngleY += angleY;
	this->headAngleZ += angleZ;

	this->headAngleY %= 360;
}

void RobotHead::draw() {
	glPushMatrix();
		glTranslatef(0.0, this->neck[1]/2, 0.0);

		drawBox(this->neck[0], this->neck[1], this->neck[2]);//draw neck

		glTranslatef(0.0, this->neck[1]/2 + this->jointDistance, 0.0);
		glRotatef(this->headAngleX, 1.0, 0.0, 0.0);
		glRotatef(this->headAngleY, 0.0, 1.0, 0.0);
		glRotatef(this->headAngleZ, 0.0, 0.0, 1.0);
		glTranslatef(0.0, this->head[1]/2, 0.0);

		drawBox(this->head[0], this->head[1], this->head[2]);//draw head
	glPopMatrix();
}

