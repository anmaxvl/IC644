#include "Loop.h"

Loop::Loop(void){}

Loop::~Loop(void){}

Loop::Loop(int numberOfCurves, int curveOrder) {
	numberOfBezierCurves = numberOfCurves;
	bezierCurveOrder = curveOrder;

	bcLoop = vector<BezierCurve>();

	RandomLoop();
}

void Loop::ChangeCurveProperties(int numberOfCurves, int curveOrder) {
	updating = true;
	if (numberOfCurves>=2 && curveOrder>=3) {
		bcLoop.clear();
		bezierCurveOrder = curveOrder;
		numberOfBezierCurves = numberOfCurves;
		RandomLoop();
	}
	updating = false;
}

void Loop::RandomLoop() {
	updating = true;

	if (bcLoop.size() != 0) {
		for (int i=0; i<numberOfBezierCurves; i++)
			bcLoop.pop_back();
	}

	BezierCurve bc1;
	Point *points = new Point[bezierCurveOrder+1];
	Point *prevPoints = new Point[bezierCurveOrder+1];

	for (int i=0; i<bezierCurveOrder+1; i++) {
		points[i] = RandomPoint();
		prevPoints[i] = points[i];
	}

	bc1 = BezierCurve(bezierCurveOrder, points);
	bcLoop.push_back(bc1);

	for (int i=1; i<numberOfBezierCurves - 1; i++) {
		points[0] = prevPoints[bezierCurveOrder];
		//C1 continuity
		points[1] = prevPoints[bezierCurveOrder-1]*(-1) + points[0]*2;
		//G1 continuity
		//points[1] = RandomPointForLineWithPoints(prevPoints[bezierCurveOrder-1], points[0]);
		//C2 continuity
		//points[2] = points[1]*2 + prevPoints[bezierCurveOrder - 2] + prevPoints[bezierCurveOrder - 1]*(-2);
		for (int j=2; j<bezierCurveOrder+1; j++)
			points[j] = RandomPoint();
		BezierCurve bc = BezierCurve(bezierCurveOrder, points);
		bcLoop.push_back(bc);
		for (int j=0; j<bezierCurveOrder + 1; j++)
			prevPoints[j] = points[j];
	}

	BezierCurve bcn;
	points[0] = prevPoints[bezierCurveOrder];
	//C1 continuity
	points[1] = prevPoints[bezierCurveOrder - 1]*(-1) + points[0]*2;
	//G1 continuity
	//points[1] = RandomPointForLineWithPoints(prevPoints[bezierCurveOrder-1], points[0]);
	points[bezierCurveOrder-1] = bcLoop[0].controlPoints[1]*(-1) + bcLoop[0].controlPoints[0]*2;
	points[bezierCurveOrder] = bcLoop[0].controlPoints[0];
	for (int i=2; i<bezierCurveOrder-1; i++)
		points[i] = RandomPoint();

	bcn = BezierCurve(bezierCurveOrder, points);
	bcLoop.push_back(bcn);
	delete(points);
	delete(prevPoints);

	EvaluateLoopLength();

	updating = false;
}

Point Loop::RandomPoint() {
	int x, y, z;
	x = rand()%2000 - 1000;
	y = rand()%2000 - 1000;
	z = rand()%2000 - 1000;

	return Point((float)x/1000, (float)y/1000, (float)z/1000);
//	return Point((float)x/1000, (float)y/1000, 0);
}

//this is for G1 continuty
Point Loop::RandomPointForLineWithPoints(Point p1, Point p2) {
	float t;
	Point p;

	t = (rand()%1000)/1000.0;
	p = p1*(-t) + p2*(1+t);

//	p = p1*(-1) + p2*2;
	return p;
}

Point Loop::PointForT(float t) {
	if (updating)
		return Point(0, 0, 0);
	else {
		int i = floor(t*numberOfBezierCurves);
		if (i>=numberOfBezierCurves)
			i--;
		t-=1.0/numberOfBezierCurves*i;
		return bcLoop[i].pointForT(numberOfBezierCurves*t);
	}
}

Point Loop::PointForArcLength(float s, float tPrev) {
	int i = floor(tPrev*numberOfBezierCurves);
	if (i>=numberOfBezierCurves)
		i++;
	tPrev -= 1.0/numberOfBezierCurves*i;
	tPrev *= numberOfBezierCurves;
	float tNext;
	tNext = tPrev + s/bcLoop[i].TangentValueAtT(tPrev);
	return bcLoop[i].pointForT(tNext);
}

float Loop::TForArcLength(float s, float tPrev) {
	int i = floor(tPrev*numberOfBezierCurves);
	if (i>=numberOfBezierCurves)
		i++;
	tPrev -= 1.0/numberOfBezierCurves*i;
	tPrev *= numberOfBezierCurves;
	float tNext;
	tNext = tPrev + s/bcLoop[i].TangentValueAtT(tPrev);
	tNext /= numberOfBezierCurves;
	tNext += 1.0/numberOfBezierCurves*i;
	if (tNext>1)
		tNext-=1;

	return tNext;
}

//T(t) = P'(t)/||P'(t)||
Point Loop::TangentAtT(float t) {
	int i = floor(t*numberOfBezierCurves);
	if (i>= numberOfBezierCurves)
		i--;
	float tt = t - 1.0/numberOfBezierCurves*i;
	tt *= numberOfBezierCurves;
	Point tangent = bcLoop[i].TangentVectorAtT(tt);
	tangent = tangent*(1/sqrt(Point::inner(tangent, tangent)));
	return tangent;
}

//N(t) = P'(t) x P''(t)/||P'(t)||^3
Point Loop::NormalAtT(float t) {
	int i = floor(t*numberOfBezierCurves);
	if (i>=numberOfBezierCurves)
		i--;
	float tt = t - 1.0/numberOfBezierCurves*i;
	tt *= numberOfBezierCurves;
	Point tanVector = bcLoop[i].TangentVectorAtT(tt);
	Point accVector = bcLoop[i].AccelerationVectorAtT(tt);
	float tanValue = bcLoop[i].TangentValueAtT(tt);

	Point normal = Point::cross(tanVector, accVector);
	float n = Point::inner(normal, normal);

	normal = normal * (1/sqrt(n));
	return normal;
}

//B(t) = T(t) x N(t)
Point Loop::BiNormalAtT(float t) {
	Point tangent = TangentAtT(t);
	Point normal = NormalAtT(t);
	Point binormal = Point::cross(normal, tangent);
	float bi = Point::inner(binormal, binormal);
	binormal = binormal*(1/sqrt(bi));

	return binormal;
}

void Loop::EvaluateLoopLength() {
	float l = 0;
	for (int i=0; i<numberOfBezierCurves; i++)
		l += bcLoop[i].CurveLength();
	loopLength = l;
}