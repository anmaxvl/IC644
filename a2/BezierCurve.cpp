#include "BezierCurve.h"
#include <math.h>

BezierCurve::BezierCurve(void){}

BezierCurve::~BezierCurve(void){}

BezierCurve::BezierCurve(int order, Point* points) {
	curveOrder = order;

	controlPoints = vector<Point>();

	for (int i=0; i<curveOrder+1; i++)
		controlPoints.push_back(points[i]);

	EvaluateCurveLength();
}

Point BezierCurve::pointForT(float t) {
	Point pt(0, 0, 0);
	for (int i=0; i<curveOrder+1; i++) {
		float oneMinusT = 1;
		float T = 1;
		for (int j=0; j<curveOrder - i; j++) {
			oneMinusT *= 1-t;
		}
		for (int j=0; j<i; j++)
			T *= t;
		pt = pt + controlPoints[i]*Binomial(curveOrder, i)*oneMinusT*T;
	}
	return pt;
}

int BezierCurve::Binomial(int n, int k) {
	int ans = 1;
	int kFactorial = 1;
	for (int i=1; i<k+1; i++)
		kFactorial*=i;
	
	int kObjects = 1;
	for (int i=n; i>=n-k+1; i--)
		kObjects *= i;
	
	return kObjects/kFactorial;
}

float BezierCurve::TangentValueAtT(float t) {
	Point tangent = TangentVectorAtT(t);
	float ans = sqrt(tangent.X()*tangent.X() + tangent.Y()*tangent.Y() + tangent.Z()*tangent.Z());
	return ans;
}

Point BezierCurve::TangentVectorAtT(float t) {
	Point tangent(0, 0, 0);
	for (int i=0; i<curveOrder; i++) {
		float oneMinusT = 1;
		float T = 1;
		for (int j=0; j<curveOrder - i - 1; j++) {
			oneMinusT *= 1-t;
		}
		for (int j=0; j<i; j++)
			T *= t;
		tangent = tangent + (controlPoints[i+1]+controlPoints[i]*(-1))*Binomial(curveOrder-1, i)*oneMinusT*T;
	}
	tangent = tangent*curveOrder;

	return tangent;
}

Point BezierCurve::AccelerationVectorAtT(float t) {
	Point accVector(0, 0, 0);
	for (int i=0; i<curveOrder - 1; i++) {
		float oneMinusT = 1;
		float T = 1;
		for (int j=0; j<curveOrder - 2 - i; j++)
			oneMinusT *= 1-t;
		for (int j=0; j<i; j++)
			T *= t;
		accVector = accVector + (controlPoints[i+2] + controlPoints[i+1]*(-2) + controlPoints[i])*Binomial(curveOrder - 2, i)*oneMinusT*T;
	}
	accVector = accVector*curveOrder*(curveOrder - 1);

	return accVector;
}

float BezierCurve::AccelerationValueAtT(float t) {
	Point acceleration = AccelerationVectorAtT(t);
	float ans = sqrt(acceleration.X()*acceleration.X() + acceleration.Y()*acceleration.Y() + acceleration.Z()*acceleration.Z());
	return ans;
}

float BezierCurve::TForArcLength(float s, float tPrev) {
	float tNext = tPrev + s/TangentValueAtT(tPrev);
	return tNext;
}

Point BezierCurve::PointForArcLength(float s, float tPrev) {
	float tNext = tPrev + s/TangentValueAtT(tPrev);
	return pointForT(tNext);
}

void BezierCurve::EvaluateCurveLength() {
	float s = 0;
	for (float i=0.0; i<1.0; i+=0.001) {
		s+=0.001*TangentValueAtT(i);
	}
	curveLength = s;
}

float BezierCurve::CurveLength() {
	return curveLength;
}