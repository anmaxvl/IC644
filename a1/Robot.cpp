#include "Robot.h"
#include "functions.h"
#include <GL\glut.h>

Robot::Robot(void){}

Robot::~Robot(void){}

Robot::Robot(float *tor, float *shJ, float *wst, float jointD) {
	for (int i=0; i<3; i++) {
		torso[i] = tor[i];
		shoulderJoints[i] = shJ[i];
		waist[i] = wst[i];
	}

	this->joint = jointD;
	this->torsoAngleX = 0;
	this->torsoAngleY = 0;
	this->torsoAngleZ = 0;
}

void Robot::initHands(float *sh, float *fh, float *wr, float jointD) {
	this->handLeft = RobotHand(sh, fh, wr, jointD, false);

	this->handRight = RobotHand(sh, fh, wr, jointD, true);
}

void Robot::initLegs(float *hp, float *shn, float *ft, float jointD) {
	this->legLeft = RobotLeg(hp, shn, ft, jointD, false);

	this->legRight = RobotLeg(hp, shn, ft, jointD, true);
}

void Robot::initHead(float *hd, float* nck, float jointD) {
	this->head = RobotHead(hd, nck, jointD);
}

void Robot::turnTorso(int angleX, int angleY, int angleZ) {
	this->torsoAngleX += angleX;
	this->torsoAngleY += angleY;
	this->torsoAngleZ += angleZ;
}

void Robot::draw() {
//	glPushMatrix();
		glRotatef(180, 0.0, 1.0, 0.0);
		drawUpper();
		drawLower();
		glRotatef(-180, 0.0, 1.0, 0.0);
//	glPopMatrix();
}

//drawing
void Robot::drawUpper() {
//	glPushMatrix();

		glTranslatef(0.0, waist[1]/2 + joint, 0.0);
		
		//rotate torso
		glRotatef(torsoAngleX, 1.0, 0.0, 0.0);
		glRotatef(torsoAngleY, 0.0, 1.0, 0.0);
		glRotatef(torsoAngleZ, 0.0, 0.0, 1.0);
		glTranslatef(0.0, torso[1]/2, 0.0);

		drawBox(torso[0], torso[1], torso[2]);//draw torso

		glTranslatef(0.0, torso[1]/2, 0.0);
		glTranslatef(torso[0]/2 + shoulderJoints[0]/2, -shoulderJoints[1]/2, 0.0);
		
		drawBox(shoulderJoints[0], shoulderJoints[1], shoulderJoints[2]);//draw left hand shoulder joint

		glTranslatef(shoulderJoints[0]/2, shoulderJoints[1]/2, 0.0);
		handLeft.draw();//draw left hand

		glTranslatef(-shoulderJoints[0] - torso[0] - shoulderJoints[0]/2, -shoulderJoints[1]/2, 0.0);
		
		drawBox(shoulderJoints[0], shoulderJoints[1], shoulderJoints[2]);//draw right hand shoulder joint

		glTranslatef(-shoulderJoints[0]/2, shoulderJoints[1]/2, 0.0);
		handRight.draw();//draw right hand

		glTranslatef(shoulderJoints[0] + torso[0]/2, 0.0, 0.0);

		head.draw();//draw neck and head

		glTranslatef(-shoulderJoints[0] - torso[0]/2, 0.0, 0.0);
		glTranslatef(shoulderJoints[0]/2, -shoulderJoints[1]/2, 0.0);
		glTranslatef(shoulderJoints[0] + torso[0] + shoulderJoints[0]/2, shoulderJoints[1]/2, 0.0);
		glTranslatef(-shoulderJoints[0]/2, -shoulderJoints[1]/2, 0.0);
		glTranslatef(-(torso[0]/2 + shoulderJoints[0]/2), shoulderJoints[1]/2, 0.0);
		glTranslatef(0.0, -torso[1], 0.0);
		
		//rotate torso
		glRotatef(-torsoAngleZ, 0.0, 0.0, 1.0);
		glRotatef(-torsoAngleY, 0.0, 1.0, 0.0);
		glRotatef(-torsoAngleX, 1.0, 0.0, 0.0);

		glTranslatef(0.0, -waist[1]/2 - joint, 0.0);

//	glPopMatrix();
}

void Robot::drawLower() {
	glPushMatrix();
		drawBox(waist[0], waist[1], waist[2]);//draw waist

		glTranslatef(waist[0]/2 + joint, 0.0, 0.0);//draw left leg
		legLeft.draw();

		glTranslatef(-waist[0] - 2*joint, 0.0, 0.0);//draw right leg
		legRight.draw();
	glPopMatrix();
}