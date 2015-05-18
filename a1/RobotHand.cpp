#include "RobotHand.h"
#include "functions.h"
#include <GL/glut.h>

RobotHand::RobotHand(float* sh, float *fh, float* wr, float jointD, bool right) {
	for (int i=0; i<3; i++) {
		this->shoulder[i] = sh[i];
		this->forehand[i] = fh[i];
		this->wrist[i] = wr[i];
	}
	this->elbowAngleX = 0;
	this->elbowAngleY = 0;
	this->shoulderAngleX = 0;
	this->shoulderAngleZ = 0;
	this->wristAngleX = 0;
	this->wristAngleZ = 0;
	this->jointDistance = jointD;
	this->right = right;
}

RobotHand::RobotHand(void){
}


RobotHand::~RobotHand(void){
}

void RobotHand::turnShoulder(int angleX, int angleZ) {
	this->shoulderAngleX += angleX;
	if (this->shoulderAngleX<=-360)
		this->shoulderAngleX+=360;
	else if (this->shoulderAngleX>=360)
		this->shoulderAngleX -= 360;
	
	this->shoulderAngleZ += angleZ;
	if (this->shoulderAngleZ<=-120)
		this->shoulderAngleZ = -120;
	else if (this->shoulderAngleZ >= 0)
		this->shoulderAngleZ = 0;
}

void RobotHand::turnForeHand(int angleX, int angleY){
	this->elbowAngleX += angleX;
	if (this->elbowAngleX<=0)
		this->elbowAngleX = 0;
	if (this->elbowAngleX>=160)
		this->elbowAngleX = 160;

	this->elbowAngleY += angleY;
	if (this->elbowAngleY <= -360)
		this->elbowAngleY += 360;
	else if (this->elbowAngleY >= 360)
		this->elbowAngleY -= 360;
}

void RobotHand::turnWrist(int angleX, int angleZ) {
	this->wristAngleX += angleX;
	if (this->wristAngleX <= -30)
		this->wristAngleX = -30;
	else if (this->wristAngleX >= 30)
		this->wristAngleX = 30;

	this->wristAngleZ += angleZ;
	if (this->wristAngleZ <= -90)
		this->wristAngleZ = -90;
	else if (this->wristAngleZ >= 90)
		this->wristAngleZ = 90;
}


void RobotHand::draw() {
	glPushMatrix();
		//glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(this->shoulderAngleX, 1.0, 0.0, 0.0);
		if (right) {
			glTranslatef(-this->shoulder[0]/2, 0.0, 0.0);
			glRotatef(this->shoulderAngleZ, 0.0, 0.0, 1.0);
			glTranslatef(0.0, -this->shoulder[1]/2, 0.0);
		}
		else {
			glTranslatef(this->shoulder[0]/2, 0.0, 0.0);
			glRotatef(this->shoulderAngleZ, 0.0, 0.0, -1.0);
			glTranslatef(0.0, -this->shoulder[1]/2, 0.0);
		}

		glColor3f(0.3, 0.3, 0.0);
		drawBox(this->shoulder[0], -this->shoulder[1], this->shoulder[2]);

		glTranslatef(0.0, -jointDistance - this->shoulder[1]/2, 0.0);
		glRotatef(this->elbowAngleX, 1.0, 0.0, 0.0);
		glRotatef(this->elbowAngleY, 0.0, 1.0, 0.0);
		glTranslatef(0.0, -this->forehand[1]/2, 0.0);

		glColor3f(0.3, 0.0, 0.3);
		drawBox(this->forehand[0], this->forehand[1], this->forehand[2]);

		glTranslatef(0.0, -jointDistance - this->forehand[1]/2, 0.0);
		glRotatef(this->wristAngleX, 1.0, 0.0, 0.0);
		glRotatef(this->wristAngleZ, 0.0, 0.0, 1.0);
		glTranslatef(0.0, -this->wrist[1]/2, 0.0);

		glColor3f(0.0, 0.3, 0.3);
		drawBox(this->wrist[0], this->wrist[1], this->wrist[2]);
	glPopMatrix();
}