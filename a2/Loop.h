#pragma once
#include <vector>
#include <iostream>

using namespace std;

#include "BezierCurve.h"
#include "types.h"

class Loop
{
	bool updating;
	float loopLength;

	Point RandomPoint();
	Point RandomPointForLineWithPoints(Point p1, Point p2);

	void EvaluateLoopLength();
public:
	vector<BezierCurve> bcLoop;
	int numberOfBezierCurves;
	int bezierCurveOrder;

	Loop(void);
	Loop(int numberOfCurves, int curveOrder);
	~Loop(void);

	void ChangeCurveProperties(int numberOfCurves, int curveOrder);
	void RandomLoop();
	Point PointForT(float t);

	Point PointForArcLength(float s, float tPrev);
	float TForArcLength(float s, float tPrev);

	float GetLoopLength() {return loopLength;}

	Point TangentAtT(float t);
	Point NormalAtT(float t);
	Point BiNormalAtT(float t);
};

