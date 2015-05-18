#pragma once
#include "types.h"
#include <vector>

using namespace std;

class BezierCurve
{
	int curveOrder;
	float curveLength;
	int Binomial (int n, int k);
	void EvaluateCurveLength();
public:
	vector<Point> controlPoints;
	BezierCurve(void);
	BezierCurve(int order, Point* points);
	~BezierCurve(void);

	Point pointForT(float t);
	
	Point TangentVectorAtT(float t);
	float TangentValueAtT(float t);

	Point AccelerationVectorAtT(float t);
	float AccelerationValueAtT(float t);

	float TForArcLength(float s, float tPrev);

	Point PointForArcLength(float s, float tPrev);

	float CurveLength();
};

