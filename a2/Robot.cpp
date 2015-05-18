#include "Robot.h"
#include "functions.h"

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

	Point binormal(1.0, 0, 0);
	Point normal(0.0, 1, 0);
	Point tangent(0, 0, 1);
	frenetFrame[0] = binormal;
	frenetFrame[1] = normal;
	frenetFrame[2] = tangent;
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
	glColor3f(0.7, 0.7, 0.8);
	glPushMatrix();
		const GLfloat matrix[] = {frenetFrame[0].X(), frenetFrame[0].Y(), frenetFrame[0].Z(), 0, 
							frenetFrame[1].X(), frenetFrame[1].Y(), frenetFrame[1].Z(), 0,
							frenetFrame[2].X(), frenetFrame[2].Y(), frenetFrame[2].Z(), 0,
							0, 0, 0, 1};
		//glScalef(4.0, 4.0, 4.0);
		glMultMatrixf(matrix);
		glRotatef(180, 0.0, 1.0, 0.0);
		drawUpper();
		drawLower();
	glPopMatrix();
}

void Robot::generateTextures() {
    Image *image1 = loadTexture("floor.bmp");

	if(image1 == NULL){
        printf("Image was not returned from loadTexture\n");
        return;
    }

    //makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create Texture
    glGenTextures(2, texture);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    //glBindTexture(GL_TEXTURE_2D, texture[0]);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,&checkImage[0][0][0]);
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

void Robot::setFrenetFrame(Point tangent, Point normal, Point binormal) {
	frenetFrame[0] = binormal;
	frenetFrame[1] = normal;
	frenetFrame[2] = tangent;
}

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
	if (this->shoulderAngleZ<=-200)
		this->shoulderAngleZ = -200;
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

		
		drawBox(this->shoulder[0], -this->shoulder[1], this->shoulder[2]);

		glTranslatef(0.0, -jointDistance - this->shoulder[1]/2, 0.0);
		glRotatef(this->elbowAngleX, 1.0, 0.0, 0.0);
		glRotatef(this->elbowAngleY, 0.0, 1.0, 0.0);
		glTranslatef(0.0, -this->forehand[1]/2, 0.0);

		//glColor3f(0.3, 0.0, 0.3);
		drawBox(this->forehand[0], this->forehand[1], this->forehand[2]);

		glTranslatef(0.0, -jointDistance - this->forehand[1]/2, 0.0);
		glRotatef(this->wristAngleX, 1.0, 0.0, 0.0);
		glRotatef(this->wristAngleZ, 0.0, 0.0, 1.0);
		glTranslatef(0.0, -this->wrist[1]/2, 0.0);

		//glColor3f(0.0, 0.3, 0.3);
		drawBox(this->wrist[0], this->wrist[1], this->wrist[2]);
	glPopMatrix();
}

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
	if (this->hipAngleZ<=-150)
		this->hipAngleZ = -150;
	else if (this->hipAngleZ>= 15)
		this->hipAngleZ = 15;
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

RobotTorso::RobotTorso(float* sh, float* ch, float* ws) {
}


RobotTorso::RobotTorso(void)
{
}


RobotTorso::~RobotTorso(void)
{
}



void RobotTorso::turnChest(int angleX, int angleY) {

}