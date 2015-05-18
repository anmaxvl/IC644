#include <iostream>

#include <gl/glut.h>
#include <math.h>
#include <string.h>

using namespace std;

#include "MotionEngine.h"
#include "Robot.h"
#include "functions.h"

int viewAngleX = 90, viewAngleY = 0;
int deltaAngleX = 0, deltaAngleY = 0;

int originS1X = -1, originS1Y = -1;
int originS2X = -1, originS2Y = -1;

bool animate = false;

char displayString[20];

int postureChangeMode = 1;

Robot _robot;
MotionEngine _motionEngine;

int mainWindow, subwindow1, subwindow2;

float radiusXZ = 3, eyePosition[3], atPosition[3], upVector[3], zoomScale = 1;

int h, w;

int border = 5;

void setProjection(int w1, int h1){
	float ratio;
	if (h1==0)
		h1 = 1;
	ratio = 1.0f * w1 / h1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w1, h1);

	gluPerspective(45,ratio,0.1,1000);
	glMatrixMode(GL_MODELVIEW);
}

void changeSize(int w1,int h1) {

	if(h1 == 0)
		h1 = 1;

	w = w1;
	h = h1;

	glutSetWindow(subwindow1);
	glutPositionWindow(border, border);
	glutReshapeWindow(w - w/12 - 3*border/2, h-2*border);
	setProjection(w- w/12 - 3*border/2, h-2*border);

	glutSetWindow(subwindow2);
	glutPositionWindow(w-w/12 + border/2,border);
	glutReshapeWindow(w/12 - 3*border/2, (h-2*border)/2);
	setProjection(w/12-3*border/2,(h-2*border)/2);
}

void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void renderMainWindow() {
	glutSetWindow(mainWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void renderSubwindow1() {
	glutSetWindow(subwindow1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.2, 0.2, 1.0);
	glMatrixMode(GL_PROJECTION);

	GLfloat qaAmbientLight[]	= {0.2, 0.2, 0.2, 1.0};
	GLfloat qaDiffuseLight[]	= {0.8, 0.8, 0.8, 1.0};
	GLfloat qaSpecularLight[]	= {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat qaGreen[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat qaRed[] = {1.0, 0.0, 0.0, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaWhite);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaGreen);
	glMaterialf(GL_FRONT, GL_SHININESS, 40.0);
	
	glShadeModel(GL_SMOOTH);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat qaLightPosition[] = {0.0, -1.0, 2.0, 1.0};
	gluLookAt(	eyePosition[0], eyePosition[1], eyePosition[2], 
				0.0, 0.0, 0.0, 
				0.0, upVector[1], 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

	glScalef(zoomScale, zoomScale, zoomScale);

	glBegin(GL_QUADS);
		glVertex3f(-1.0, -0.8, 1.0);
		glVertex3f(-1.0, -0.8, -1.0);
		glVertex3f(1.0, -0.8, -1.0);
		glVertex3f(1.0, -0.8, 1.0);
	glEnd();

	_robot.draw();

	glMatrixMode(GL_PROJECTION);
	glutSwapBuffers();
}

void renderSubwindow2() {
	glutSetWindow(subwindow2);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.2, 0.2, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(	0, 0, 1, 
				0.0, 0.0, 0.0, 
				0.0, 1, 0.0);
	
	//draw plus
	glTranslatef(0.0, 0.3, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	for(int i=0; i<40; i++) {
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.05*cos(2*3.14159265/40*i), 0.05*sin(2*3.14159265/40*i), 0.0);
			glVertex3f(0.05*cos(2*3.14159265/40*(i+1)), 0.05*sin(2*3.14159265/40*(i+1)), 0.0);
		glEnd();
	}
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
		glVertex3f(-0.05/5, 0.035, 0.0);
		glVertex3f(-0.05/5, -0.035, 0.0);
		glVertex3f(0.05/5, -0.035, 0.0);
		glVertex3f(0.05/5, 0.035, 0.0);
	glEnd();

	glBegin(GL_QUADS);
		glVertex3f(-0.035, 0.05/5, 0.0);
		glVertex3f(-0.035, -0.05/5, 0.0);
		glVertex3f(0.035, -0.05/5, 0.0);
		glVertex3f(0.035, 0.05/5, 0.0);
	glEnd();

	//draw minus
	glTranslatef(0.0, -0.15, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	for(int i=0; i<40; i++) {
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.05*cos(2*3.14159265/40*i), 0.05*sin(2*3.14159265/40*i), 0.0);
			glVertex3f(0.05*cos(2*3.14159265/40*(i+1)), 0.05*sin(2*3.14159265/40*(i+1)), 0.0);
		glEnd();
	}

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
		glVertex3f(-0.035, 0.05/5, 0.0);
		glVertex3f(-0.035, -0.05/5, 0.0);
		glVertex3f(0.035, -0.05/5, 0.0);
		glVertex3f(0.035, 0.05/5, 0.0);
	glEnd();

	//draw start
	glTranslatef(0.0, -0.15, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	for(int i=0; i<40; i++) {
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.05*cos(2*3.14159265/40*i), 0.05*sin(2*3.14159265/40*i), 0.0);
			glVertex3f(0.05*cos(2*3.14159265/40*(i+1)), 0.05*sin(2*3.14159265/40*(i+1)), 0.0);
		glEnd();
	}

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_TRIANGLES);
		glVertex3f(-0.024, 0.028, 0.0);
		glVertex3f(-0.024, -0.028, 0.0);
		glVertex3f(0.035, 0.0, 0.0);
	glEnd();

	//draw stop
	glTranslatef(0.0, -0.15, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	for(int i=0; i<40; i++) {
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.05*cos(2*3.14159265/40*i), 0.05*sin(2*3.14159265/40*i), 0.0);
			glVertex3f(0.05*cos(2*3.14159265/40*(i+1)), 0.05*sin(2*3.14159265/40*(i+1)), 0.0);
		glEnd();
	}

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
		glVertex3f(-0.028, 0.028, 0.0);
		glVertex3f(-0.028, -0.028, 0.0);
		glVertex3f(0.028, -0.028, 0.0);
		glVertex3f(0.028, 0.028, 0.0);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glutSwapBuffers();
}

void renderAll() {
	renderMainWindow();
	renderSubwindow1();
	renderSubwindow2();
}

void Timer(int value) {
	glutSetWindow(mainWindow);
	glutPostRedisplay();
	glutSetWindow(subwindow1);
	glutPostRedisplay();
	glutSetWindow(subwindow2);
	glutPostRedisplay();

	glutTimerFunc(30, Timer, 0);

	if (animate)
		_motionEngine.nextMotionState(&_robot);
}


void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glutKeyboardFunc(keyboardPressHandler);
}

void initSubWindows() {
	glClearColor(0.0, 0.2, 0.2, 1.0);
	glOrtho(-1, 1, -1, 1, 0, 0);
}

void mouseBarPressHandler(int button, int state, int x, int y);
void mouseBarMoveHandler(int x, int y);

int main(int iArgc, char** cppArgv) {
	float sh[] = {0.06, 0.2, 0.06};
	float fh[] = {0.06, 0.2, 0.06};
	float wr[] = {0.07, 0.06, 0.03};

	float hip[] = {0.08, 0.25, 0.08};
	float shin[] = {0.08, 0.25, 0.08};
	float foot[] = {0.08, 0.025, 0.12};

	float torso[] = {0.25, 0.35, 0.1};
	float shJ[] = {0.04, 0.04, 0.07};
	float waist[] = {0.15, 0.05, 0.08};

	float head[] = {0.2, 0.2, 0.15};
	float neck[] = {0.06, 0.07, 0.06};

	_robot = Robot(torso, shJ, waist, 0.00);
	_robot.initLegs(hip, shin, foot, 0.02);
	_robot.initHands(sh, fh, wr, 0.01);
	_robot.initHead(head, neck, 0.01);

	_motionEngine = MotionEngine(&_robot);
	_motionEngine.setMotionRandom(&_robot);
	for (int i=0; i<3; i++) {
		eyePosition[i] = 0;
		atPosition[i] = 0;
		upVector[i] = 0;
	}
	eyePosition[2] = 3;
	upVector[1] = 1;

	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	mainWindow = glutCreateWindow("Computer Graphics OpenGL. Assignment 1");
	
	glutDisplayFunc(renderMainWindow);
	glutReshapeFunc(changeSize);
	init();
	
	subwindow1 = glutCreateSubWindow(mainWindow, 0, 0, 300, 500);
	glutDisplayFunc(renderSubwindow1);
	glutMouseFunc(mousePressHandler);
	glutMotionFunc(mouseMoveHandler);
	init();

	subwindow2 = glutCreateSubWindow(mainWindow, 300, 0, 200, 500);
	glutDisplayFunc(renderSubwindow2);
	glutMouseFunc(mouseBarPressHandler);
	glutMotionFunc(mouseBarMoveHandler);
	initSubWindows();

	Timer(0);
	glutMainLoop();
	
	return 0;
}

void mousePressHandler (int button, int state, int x, int y) {
	cout<<x<<" "<<y<<endl;
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			viewAngleX += deltaAngleX;
			viewAngleY += deltaAngleY;
			viewAngleX %= 360;
			viewAngleY %= 360;
			originS1X = -1;
			originS1Y = -1;
		}
		else {
			deltaAngleX = 0;
			deltaAngleY = 0;
			originS1X = x;
			originS1Y = y;
		}
	}
}

void mouseMoveHandler (int x, int y) {
	if (originS1X>=0) {
		deltaAngleX = (x - originS1X);
		deltaAngleY = (y - originS1Y);
		eyePosition[1] = 3*sin(2*3.14159265/360*(viewAngleY+deltaAngleY));
		eyePosition[2] = upVector[1]*3*cos(2*3.14159265/360*(viewAngleY+deltaAngleY));
		radiusXZ = abs(eyePosition[2]);
		eyePosition[2] = upVector[1]*radiusXZ*sin(2*3.14159265/360*(viewAngleX+deltaAngleX));
		eyePosition[0] = upVector[1]*radiusXZ*cos(2*3.14159265/360*(viewAngleX+deltaAngleX));

		if(viewAngleY+deltaAngleY>=90 && viewAngleY+deltaAngleY <=270)
			upVector[1] = -1;
		else
			upVector[1] = 1;

	}
}

void mouseBarPressHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP){
			if ((float)y/h/2>=0.018 && (float)y/h/2<=0.047)
				zoomScale *= 1.1;
			else if ((float)y/h/2>=0.063 && (float)y/h/2<=0.092)
				zoomScale *= 0.9;
			else if ((float)y/h/2>=0.108 && (float)y/h/2<=0.136)
				animate = true;
			else if ((float)y/h/2>=0.151 && (float)y/h/2<=0.181)
				animate = false;
		}
	}
}

void mouseBarMoveHandler(int x, int y) {

}

void keyboardPressHandler(unsigned char key, int x, int y) {
	int delta = 4;
	switch (key) {
	case '1': postureChangeMode = 1;
		break;
	case '2': postureChangeMode = 2;
		break;
	case '3': postureChangeMode = 3;
		break;
	case '4': postureChangeMode = 4;
		break;
	case '5': postureChangeMode = 5;
		break;
	case '6': postureChangeMode = 6;
		break;
	case 'q':
		if (postureChangeMode == 1)
			_robot.handLeft.turnShoulder(delta, 0);
		if (postureChangeMode == 2)
			_robot.handRight.turnShoulder(delta, 0);
		if (postureChangeMode == 3)
			_robot.legLeft.turnHip(delta, 0);
		if (postureChangeMode == 4)
			_robot.legRight.turnHip(delta, 0);
		if (postureChangeMode == 5)
			_robot.turnTorso(delta, 0, 0);
		if (postureChangeMode == 6)
			_robot.head.turnHead(delta, 0, 0);
		break;
	case 'a':
		if (postureChangeMode == 1)
			_robot.handLeft.turnShoulder(-delta, 0);
		if (postureChangeMode == 2)
			_robot.handRight.turnShoulder(-delta, 0);
		if (postureChangeMode == 3)
			_robot.legLeft.turnHip(-delta, 0);
		if (postureChangeMode == 4)
			_robot.legRight.turnHip(-delta, 0);
		if (postureChangeMode == 5)
			_robot.turnTorso(-delta, 0, 0);
		if (postureChangeMode == 6)
			_robot.head.turnHead(-delta, 0, 0);
		break;
	case 'w':
		if (postureChangeMode == 1)
			_robot.handLeft.turnShoulder(0, delta);
		if (postureChangeMode == 2)
			_robot.handRight.turnShoulder(0, delta);
		if (postureChangeMode == 3)
			_robot.legLeft.turnHip(0, delta);
		if (postureChangeMode == 4)
			_robot.legRight.turnHip(0, delta);
		if (postureChangeMode == 5)
			_robot.turnTorso(0, delta, 0);
		if (postureChangeMode == 6)
			_robot.head.turnHead(0, delta, 0);
		break;
	case 's':
		if (postureChangeMode == 1)
			_robot.handLeft.turnShoulder(0, -delta);
		if (postureChangeMode == 2)
			_robot.handRight.turnShoulder(0, -delta);
		if (postureChangeMode == 3)
			_robot.legLeft.turnHip(0, -delta);
		if (postureChangeMode == 4)
			_robot.legRight.turnHip(0, -delta);
		if (postureChangeMode == 5)
			_robot.turnTorso(0, -delta, 0);
		if (postureChangeMode == 6)
			_robot.head.turnHead(0, -delta, 0);
		break;
	case 'e':
		if (postureChangeMode == 1)
			_robot.handLeft.turnForeHand(delta, 0);
		if (postureChangeMode == 2)
			_robot.handRight.turnForeHand(delta, 0);
		if (postureChangeMode == 3)
			_robot.legLeft.turnShin(delta, 0);
		if (postureChangeMode == 4)
			_robot.legRight.turnShin(delta, 0);
		if (postureChangeMode == 5)
			_robot.turnTorso(0, 0, delta);
		if (postureChangeMode == 6)
			_robot.head.turnHead(0, 0, delta);
		break;
	case 'd':
		if (postureChangeMode == 1)
			_robot.handLeft.turnForeHand(-delta, 0);
		if (postureChangeMode == 2)
			_robot.handRight.turnForeHand(-delta, 0);
		if (postureChangeMode == 3)
			_robot.legLeft.turnShin(-delta, 0);
		if (postureChangeMode == 4)
			_robot.legRight.turnShin(-delta, 0);
		if (postureChangeMode == 5)
			_robot.turnTorso(0, 0, -delta);
		if (postureChangeMode == 6)
			_robot.head.turnHead(0, 0, -delta);
		break;
	case 'r':
		if (postureChangeMode == 1)
			_robot.handLeft.turnForeHand(0, delta);
		if (postureChangeMode == 2)
			_robot.handRight.turnForeHand(0, delta);
		if (postureChangeMode == 3)
			_robot.legLeft.turnShin(0, delta);
		if (postureChangeMode == 4)
			_robot.legRight.turnShin(0, delta);
		break;
	case 'f':
		if (postureChangeMode == 1)
			_robot.handLeft.turnForeHand(0, -delta);
		if (postureChangeMode == 2)
			_robot.handRight.turnForeHand(0, -delta);
		if (postureChangeMode == 3)
			_robot.legLeft.turnShin(0, -delta);
		if (postureChangeMode == 4)
			_robot.legRight.turnShin(0, -delta);
		break;
	case 't':
		if (postureChangeMode == 1)
			_robot.handLeft.turnWrist(delta, 0);
		if (postureChangeMode == 2)
			_robot.handRight.turnWrist(delta, 0);
		if (postureChangeMode == 3)
			_robot.legLeft.turnFoot(delta, 0);
		if (postureChangeMode == 4)
			_robot.legRight.turnFoot(delta, 0);
		break;
	case 'g':
		if (postureChangeMode == 1)
			_robot.handLeft.turnWrist(-delta, 0);
		if (postureChangeMode == 2)
			_robot.handRight.turnWrist(-delta, 0);
		if (postureChangeMode == 3)
			_robot.legLeft.turnFoot(-delta, 0);
		if (postureChangeMode == 4)
			_robot.legRight.turnFoot(-delta, 0);
		break;
	case 'y':
		if (postureChangeMode == 1)
			_robot.handLeft.turnWrist(0, delta);
		if (postureChangeMode == 2)
			_robot.handRight.turnWrist(0, delta);
		if (postureChangeMode == 3)
			_robot.legLeft.turnFoot(0, delta);
		if (postureChangeMode == 4)
			_robot.legRight.turnFoot(0, delta);
		break;
	case 'h':
		if (postureChangeMode == 1)
			_robot.handLeft.turnWrist(0, -delta);
		if (postureChangeMode == 2)
			_robot.handRight.turnWrist(0, -delta);
		if (postureChangeMode == 3)
			_robot.legLeft.turnFoot(0, -delta);
		if (postureChangeMode == 4)
			_robot.legRight.turnFoot(0, -delta);
		break;
	}
	if (key == 27) {
		glutDestroyWindow(subwindow1);
		glutDestroyWindow(subwindow2);
		glutDestroyWindow(mainWindow);
		exit(0);
	}
}