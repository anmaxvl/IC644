#include <iostream>
#include "RobotLeg.h"
#include "functions.h"
#include <GL\glut.h>

using namespace std;

RobotLeg::RobotLeg(void){}

RobotLeg::RobotLeg(float* hi, float *sh, float *ft, float jointD, bool right) {
	for (int i=0; i<3; i++) {
		this->hip[i] = hi[i];
		this->shin[i] = sh[i];
		this->foot[i] = ft[i];
	}

	this->hipAngleX = 0;
	this->hipAngleZ = 0;
	this->kneeAngleX = 0;
	this->kneeAngleY = 0;
	this->footAngleX = 0;
	this->footAngleZ = 0;
	this->right = right;
	this->jointDistance = jointD;
}

RobotLeg::~RobotLeg(void){}

void RobotLeg::turnHip(int angleX, int angleZ) {
	this->hipAngleX += angleX;
	if (this->hipAngleX <= -60)
		this->hipAngleX = -60;
	else if (this->hipAngleX>=150)
		this->hipAngleX = 150;

	this->hipAngleZ += angleZ;
	if (this->hipAngleZ>=150)
		this->hipAngleZ = 150;
	else if (this->hipAngleZ<=-15)
		this->hipAngleZ = -15;
}

void RobotLeg::turnShin(int angleX, int angleY) {
	this->kneeAngleX += angleX;
	if (this->kneeAngleX <=0)
		this->kneeAngleX = 0;
	this->kneeAngleY += angleY;
	this->kneeAngleY %= 360;
}

void RobotLeg::turnFoot(int angleX, int angleZ) {
	this->footAngleX += angleX;
	if (this->footAngleX>=40)
		this->footAngleX = 40;
	else if (this->footAngleX<=-60)
		this->footAngleX = -60;
	
	this->footAngleZ += angleZ;
	if (this->footAngleZ>=10)
		this->footAngleZ = 10;
	else if (this->footAngleZ<=-20)
		this->footAngleZ = -20;
}

	
void RobotLeg:: draw() {
	glPushMatrix();
		//glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(this->hipAngleX, 1.0, 0.0, 0.0);
		if (right) {
			glTranslatef(-this->hip[0]/2, 0.0, 0.0);
			glRotatef(this->hipAngleZ, 0.0, 0.0, 1.0);
			glTranslatef(0.0, -this->hip[1]/2, 0.0);
		}
		else {
			glTranslatef(this->hip[0]/2, 0.0, 0.0);
			glRotatef(this->hipAngleZ, 0.0, 0.0, -1.0);
			glTranslatef(0.0, -this->hip[1]/2, 0.0);
		}

		drawBox(this->hip[0], this->hip[1], this->hip[2]);//draw hip

		glTranslatef(0.0, -jointDistance - this->hip[1]/2, 0.0);
		glRotatef(this->kneeAngleX, -1.0, 0.0, 0.0);
		glRotatef(this->kneeAngleY, 0.0, 1.0, 0.0);
		glTranslatef(0.0, -this->shin[1]/2, 0.0);

		drawBox(this->shin[0], this->shin[1], this->shin[2]);//draw shin

		glTranslatef(0.0, -jointDistance - this->shin[1]/2, 0.0);

		glRotatef(this->footAngleX, 1.0, 0.0, 0.0);
		glRotatef(this->footAngleZ, 0.0, 0.0, 1.0);
		glTranslatef(0.0, -this->foot[1]/2, -(this->foot[2] - this->shin[2])/2);

		drawBox(this->foot[0],this->foot[1],this->foot[2]);//draw foot
	glPopMatrix();
}