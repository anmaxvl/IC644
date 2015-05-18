#pragma once;

struct t_Point{
	t_Point(float xx, float yy, float zz): x(xx), y(yy), z(zz){}
	t_Point operator + (t_Point &other);
	t_Point operator * (float t);
	t_Point():x(0), y(0), z(0){};
	static t_Point randomPoint();
	static t_Point randomPoint2D();
	float X();
	float Y();
	float Z();

	static t_Point cross(t_Point p1, t_Point p2);
	static float inner(t_Point p1, t_Point p2);
private:
	float x, y, z;
};

typedef struct t_Point Point;

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

typedef struct Image Image;
