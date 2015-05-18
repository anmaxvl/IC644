#include "functions.h"

#include <iostream>
#include <GL/glut.h>
#include <math.h>

using namespace std;

//float pi = 3.14159265;
//const int slices = 10;

void drawBox (float lenX, float lenY, float lenZ) {
	//glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
		glScalef(lenX, lenY, lenZ);
		glutSolidCube(1.0);
	glPopMatrix();

	////front
	////glColor3f(0.4, 0.4, 0.0);
	//glBegin(GL_QUADS);
	//	glVertex3f(-lenX/2, lenY/2, lenZ/2);
	//	glVertex3f(-lenX/2, -lenY/2, lenZ/2);
	//	glVertex3f(lenX/2, -lenY/2, lenZ/2);
	//	glVertex3f(lenX/2, lenY/2, lenZ/2);
	//glEnd();

	////right
	////glColor3f(0.4, 0.0, 0.4);
	//glBegin(GL_QUADS);
	//	glVertex3f(lenX/2, lenY/2, lenZ/2);
	//	glVertex3f(lenX/2, -lenY/2, lenZ/2);
	//	glVertex3f(lenX/2, -lenY/2, -lenZ/2);
	//	glVertex3f(lenX/2, lenY/2, -lenZ/2);
	//glEnd();

	////back
	////glColor3f(0.0, 0.4, 0.4);
	//glBegin(GL_QUADS);
	//	glVertex3f(-lenX/2, lenY/2, -lenZ/2);
	//	glVertex3f(-lenX/2, -lenY/2, -lenZ/2);
	//	glVertex3f(lenX/2, -lenY/2, -lenZ/2);
	//	glVertex3f(lenX/2, lenY/2, -lenZ/2);
	//glEnd();

	////left
	////glColor3f(0.4, 0.0, 0.0);
	//glBegin(GL_QUADS);
	//	glVertex3f(-lenX/2, lenY/2, lenZ/2);
	//	glVertex3f(-lenX/2, -lenY/2, lenZ/2);
	//	glVertex3f(-lenX/2, -lenY/2, -lenZ/2);
	//	glVertex3f(-lenX/2, lenY/2, -lenZ/2);
	//glEnd();

	////top
	////glColor3f(0.0, 0.4, 0.0);
	//glBegin(GL_QUADS);
	//	glVertex3f(-lenX/2, lenY/2, lenZ/2);
	//	glVertex3f(lenX/2, lenY/2, lenZ/2);
	//	glVertex3f(lenX/2, lenY/2, -lenZ/2);
	//	glVertex3f(-lenX/2, lenY/2, -lenZ/2);
	//glEnd();

	////bottom
	////glColor3f(0.0, 0.0, 0.4);
	//glBegin(GL_QUADS);
	//	glVertex3f(-lenX/2, -lenY/2, lenZ/2);
	//	glVertex3f(lenX/2, -lenY/2, lenZ/2);
	//	glVertex3f(lenX/2, -lenY/2, -lenZ/2);
	//	glVertex3f(-lenX/2, -lenY/2, -lenZ/2);
	//glEnd();

}



void mouseWheelHandler(int button, int direction, int x, int y) {
	cout<<"Wheel scroll\n";
}

