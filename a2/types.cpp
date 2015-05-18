#include "types.h"
#include <stdlib.h>
#include <math.h>

t_Point t_Point::operator+(t_Point &other) {
	return t_Point(x + other.x, y + other.y, z + other.z);
}

t_Point t_Point::operator*(float t) {
	return t_Point(x*t, y*t, z*t);
}

float t_Point::X() {
	return x;
}

float t_Point::Y() {
	return y;
}

float t_Point::Z() {
	return z;
}

t_Point t_Point::randomPoint() {
	float x, y, z;
	x = rand()%1000 - 500;
	y = rand()%1000 - 500;
	z = rand()%1000 - 500;

	return t_Point(x/1000, y/1000, z/1000);
}

t_Point t_Point::randomPoint2D() {
	float x, y;
	x = rand()%1000 - 500;
	y = rand()%1000 - 500;

	return t_Point(x/1000, y/1000, 0);
}

t_Point t_Point::cross(t_Point p1, t_Point p2) {
	float x = p1.Y()*p2.Z() - p2.Y()*p1.Z();
	float y = p2.X()*p1.Z() - p1.X()*p2.Z();
	float z = p1.X()*p2.Y() - p2.X()*p1.Y();
	return t_Point(x, y, z);
}

float t_Point::inner(t_Point p1, t_Point p2) {
	return (p1.X()*p2.X() + p1.Y()*p2.Y() + p1.Z()*p2.Z());
}