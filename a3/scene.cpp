// -----------------------------------------------------------
// scene.cpp
// 2004 - Jacco Bikker - jacco@bik5.com - www.bik5.com -   <><
// -----------------------------------------------------------

#include "common.h"
#include "string.h"
#include "scene.h"
#include "raytracer.h"
#include <iostream>

using namespace std;

#define EPS 0.0001

namespace Raytracer {

// -----------------------------------------------------------
// Primitive class implementation
// -----------------------------------------------------------

void Primitive::SetName( char* a_Name )
{
	delete m_Name; 
	m_Name = new char[strlen( a_Name ) + 1]; 
	strcpy( m_Name, a_Name ); 
}

// -----------------------------------------------------------
// Material class implementation
// -----------------------------------------------------------

Material::Material() :
	m_Color( Color( 0.2f, 0.2f, 0.2f ) ),
	m_Refl( 0 ), m_Diff( 0.2f ), m_Spec( 0.8f ), m_RIndex( 1.5f )
{
}

// -----------------------------------------------------------
// Sphere primitive methods
// -----------------------------------------------------------

int Sphere::Intersect( Ray& a_Ray, float& a_Dist )
{
	vector3 v = a_Ray.GetOrigin() - m_Centre;
	float b = -DOT( v, a_Ray.GetDirection() );
	float det = (b * b) - DOT( v, v ) + m_SqRadius;
	int retval = MISS;
	if (det > 0)
	{
		det = sqrtf( det );
		float i1 = b - det;
		float i2 = b + det;
		if (i2 > 0)
		{
			if (i1 < 0) 
			{
				if (i2 < a_Dist) 
				{
					a_Dist = i2;
					retval = INPRIM;
				}
			}
			else
			{
				if (i1 < a_Dist)
				{
					a_Dist = i1;
					retval = HIT;
				}
			}
		}
	}
	return retval;
}

// -----------------------------------------------------------
// Cube primitive methods
// -----------------------------------------------------------

Cube::Cube(){
	m_Center = vector3(0, 0, 0);
	m_Side_Length = 1.0;
	m_Cube_Vertices[0] = vector3(-0.5, 0.5, 0.5);
	m_Cube_Vertices[1] = vector3(0.5, 0.5, 0.5);
	m_Cube_Vertices[2] = vector3(0.5, -0.5, 0.5);
	m_Cube_Vertices[3] = vector3(-0.5, -0.5, 0.5);
	
	m_Cube_Vertices[4] = vector3(-0.5, 0.5, -0.5);
	m_Cube_Vertices[5] = vector3(0.5, 0.5, -0.5);
	m_Cube_Vertices[6] = vector3(0.5, -0.5, -0.5);
	m_Cube_Vertices[7] = vector3(-0.5, -0.5, -0.5);
	
	this->CalculateNormalsAndFreeTerms();
}

void Cube::setTransformationMatrix(float trMatrix[3][3]) {
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			m_Tr_Matrix[i][j] = trMatrix[i][j];

	for (int i = 0; i<8; i++)
		m_Cube_Vertices[i] = transformPoint(m_Cube_Vertices[i]);

	this->CalculateNormalsAndFreeTerms();
}

vector3 Cube::transformPoint(vector3 point) {
	vector3 newPoint;
	vector3 oldPoint = vector3(point.x, point.y, point.z);
	oldPoint.x -= m_Center.x;
	oldPoint.y -= m_Center.y;
	oldPoint.z -= m_Center.z;

	vector3 col;
	col = vector3(m_Tr_Matrix[0][0], m_Tr_Matrix[1][0], m_Tr_Matrix[2][0]);
	newPoint.x = col.Dot(oldPoint);

	col = vector3(m_Tr_Matrix[0][1], m_Tr_Matrix[1][1], m_Tr_Matrix[2][1]);
	newPoint.y = col.Dot(oldPoint);

	col = vector3(m_Tr_Matrix[0][2], m_Tr_Matrix[1][2], m_Tr_Matrix[2][2]);
	newPoint.z = col.Dot(oldPoint);

	newPoint.x += m_Center.x;
	newPoint.y += m_Center.y;
	newPoint.z += m_Center.z;

	return newPoint;
}

Cube::Cube(vector3& center, float sideLength) {
	m_Center = vector3(center.x, center.y, center.z);
	m_Side_Length = sideLength;

	float q = sqrt(2.0)/2.0 * sideLength;

	//m_Cube_Vertices[0] = vector3(center.x, center.y + sideLength/2, center.z + q);
	//m_Cube_Vertices[1] = vector3(center.x + q, center.y + sideLength/2, center.z);
	//m_Cube_Vertices[2] = vector3(center.x + q, center.y - sideLength/2, center.z);
	//m_Cube_Vertices[3] = vector3(center.x, center.y - sideLength/2, center.z + q);

	//m_Cube_Vertices[4] = vector3(center.x - q, center.y + sideLength/2, center.z);
	//m_Cube_Vertices[5] = vector3(center.x, center.y + sideLength/2, center.z - q);
	//m_Cube_Vertices[6] = vector3(center.x, center.y - sideLength/2, center.z - q);
	//m_Cube_Vertices[7] = vector3(center.x - q, center.y - sideLength/2, center.z);

	m_Cube_Vertices[0] = vector3(center.x - sideLength/2, center.y + sideLength/2, center.z + sideLength/2);
	m_Cube_Vertices[1] = vector3(center.x + sideLength/2, center.y + sideLength/2, center.z + sideLength/2);
	m_Cube_Vertices[2] = vector3(center.x + sideLength/2, center.y - sideLength/2, center.z + sideLength/2);
	m_Cube_Vertices[3] = vector3(center.x - sideLength/2, center.y - sideLength/2, center.z + sideLength/2);

	m_Cube_Vertices[4] = vector3(center.x - sideLength/2, center.y + sideLength/2, center.z - sideLength/2);
	m_Cube_Vertices[5] = vector3(center.x + sideLength/2, center.y + sideLength/2, center.z - sideLength/2);
	m_Cube_Vertices[6] = vector3(center.x + sideLength/2, center.y - sideLength/2, center.z - sideLength/2);
	m_Cube_Vertices[7] = vector3(center.x - sideLength/2, center.y - sideLength/2, center.z - sideLength/2);
	
	this->CalculateNormalsAndFreeTerms();
}

void Cube::CalculateNormalsAndFreeTerms() {
	vector3 V1, V2;

	//V1 = m_Cube_Vertices[3] - m_Cube_Vertices[0];
	//V2 = m_Cube_Vertices[1] - m_Cube_Vertices[0];
	//m_Face_Normals[0] = V1.Cross(V2);
	m_Face_Normals[0] = m_Cube_Vertices[0] - m_Cube_Vertices[4];
	m_Face_Free_Terms[0] = - m_Face_Normals[0].Dot(m_Cube_Vertices[0]);

	//V1 = m_Cube_Vertices[2] - m_Cube_Vertices[1];
	//V2 = m_Cube_Vertices[5] - m_Cube_Vertices[1];
	//m_Face_Normals[1] = V1.Cross(V2);
	m_Face_Normals[1] = m_Cube_Vertices[1] - m_Cube_Vertices[0];
	m_Face_Free_Terms[1] = - m_Face_Normals[1].Dot(m_Cube_Vertices[1]);

	//V1 = m_Cube_Vertices[6] - m_Cube_Vertices[5];
	//V2 = m_Cube_Vertices[4] - m_Cube_Vertices[5];
	//m_Face_Normals[2] = V1.Cross(V2);
	m_Face_Normals[2] = m_Cube_Vertices[5] - m_Cube_Vertices[1];
	m_Face_Free_Terms[2] = - m_Face_Normals[2].Dot(m_Cube_Vertices[5]);

	//V1 = m_Cube_Vertices[7] - m_Cube_Vertices[4];
	//V2 = m_Cube_Vertices[0] - m_Cube_Vertices[4];
	//m_Face_Normals[3] = V1.Cross(V2);
	m_Face_Normals[3] = m_Cube_Vertices[0] - m_Cube_Vertices[1];
	m_Face_Free_Terms[3] = - m_Face_Normals[3].Dot(m_Cube_Vertices[4]);

	//V1 = m_Cube_Vertices[1] - m_Cube_Vertices[0];
	//V2 = m_Cube_Vertices[4] - m_Cube_Vertices[0];
	//m_Face_Normals[4] = V1.Cross(V2);
	m_Face_Normals[4] = m_Cube_Vertices[1] - m_Cube_Vertices[2];
	m_Face_Free_Terms[4] = - m_Face_Normals[4].Dot(m_Cube_Vertices[0]);

	//V1 = m_Cube_Vertices[3] - m_Cube_Vertices[2];
	//V2 = m_Cube_Vertices[6] - m_Cube_Vertices[2];
	//m_Face_Normals[5] = V1.Cross(V2);
	m_Face_Normals[5] = m_Cube_Vertices[2] - m_Cube_Vertices[1];
	m_Face_Free_Terms[5] = - m_Face_Normals[5].Dot(m_Cube_Vertices[2]);
}

int Cube::Intersect(Ray& a_Ray, float& a_Dist) {
	//vector3 V1, V3, V4, V5, P;
	vector3 E1, E2, T;
	float t;
	float u, v;
	int retVal = MISS;
	float uv[6][2] = {0};

	int hitFaces[6] = {0};

	//Check Intersection with ABCD face
	t = -(m_Face_Normals[0].Dot(a_Ray.GetOrigin()) + m_Face_Free_Terms[0])/(m_Face_Normals[0].Dot(a_Ray.GetDirection()));
	E1 = m_Cube_Vertices[3] - m_Cube_Vertices[0];
	E2 = m_Cube_Vertices[1] - m_Cube_Vertices[0];
	T = a_Ray.GetOrigin() - m_Cube_Vertices[0];
	u = a_Ray.GetDirection().Cross(E2).Dot(T)/a_Ray.GetDirection().Cross(E2).Dot(E1);
	v = T.Cross(E1).Dot(a_Ray.GetDirection())/a_Ray.GetDirection().Cross(E2).Dot(E1);
	uv[0][0] = u;
	uv[0][1] = v;
	if ((u>=0 && u<=1) && (v>=0 && v<=1)) {
		hitFaces[0] = 1;
		if (t < a_Dist && t>0) {
			retVal = HIT;
			a_Dist = t;
		}
		else if (t<0) {
			hitFaces[0] = -1;
		}
	}
	//End ABCD

	//Check Intersection with BFGC
	t = -(m_Face_Normals[1].Dot(a_Ray.GetOrigin()) + m_Face_Free_Terms[1])/(m_Face_Normals[1].Dot(a_Ray.GetDirection()));
	E1 = m_Cube_Vertices[2] - m_Cube_Vertices[1];
	E2 = m_Cube_Vertices[5] - m_Cube_Vertices[1];
	T = a_Ray.GetOrigin() - m_Cube_Vertices[1];
	u = a_Ray.GetDirection().Cross(E2).Dot(T)/a_Ray.GetDirection().Cross(E2).Dot(E1);
	v = T.Cross(E1).Dot(a_Ray.GetDirection())/a_Ray.GetDirection().Cross(E2).Dot(E1);
	uv[1][0] = u;
	uv[1][1] = v;
	if ((u>=0 && u<=1) && (v>=0 && v<=1)) {
		hitFaces[1] = 1;
		if (t < a_Dist && t>0) {
			retVal = HIT;
			a_Dist = t;
		}
		else if (t<0) {
			hitFaces[1] = -1;
		}
	}
	//End BFGC

	//Check Intersection with FEHG
	t = -(m_Face_Normals[2].Dot(a_Ray.GetOrigin()) + m_Face_Free_Terms[2])/(m_Face_Normals[2].Dot(a_Ray.GetDirection()));
	E1 = m_Cube_Vertices[6] - m_Cube_Vertices[5];
	E2 = m_Cube_Vertices[4] - m_Cube_Vertices[5];
	T = a_Ray.GetOrigin() - m_Cube_Vertices[5];
	u = a_Ray.GetDirection().Cross(E2).Dot(T)/a_Ray.GetDirection().Cross(E2).Dot(E1);
	v = T.Cross(E1).Dot(a_Ray.GetDirection())/a_Ray.GetDirection().Cross(E2).Dot(E1);
	uv[2][0] = u;
	uv[2][1] = v;
	if ((u>=0 && u<=1) && (v>=0 && v<=1)) {
		hitFaces[2] = 1;
		if (t < a_Dist && t>0) {
			retVal = HIT;
			a_Dist = t;
		}
		else if (t<0) {
			hitFaces[2] = -1;
		}
	}
	//End FEHG

	//Check Intersection with EADH
	t = -(m_Face_Normals[3].Dot(a_Ray.GetOrigin()) + m_Face_Free_Terms[3])/(m_Face_Normals[3].Dot(a_Ray.GetDirection()));
	E1 = m_Cube_Vertices[7] - m_Cube_Vertices[4];
	E2 = m_Cube_Vertices[0] - m_Cube_Vertices[4];
	T = a_Ray.GetOrigin() - m_Cube_Vertices[4];
	u = a_Ray.GetDirection().Cross(E2).Dot(T)/a_Ray.GetDirection().Cross(E2).Dot(E1);
	v = T.Cross(E1).Dot(a_Ray.GetDirection())/a_Ray.GetDirection().Cross(E2).Dot(E1);
	uv[3][0] = u;
	uv[3][1] = v;
	if ((u>=0 && u<=1) && (v>=0 && v<=1)) {
		hitFaces[3] = 1;
		if (t < a_Dist && t>0) {
			retVal = HIT;
			a_Dist = t;
		}
		else if (t<0) {
			hitFaces[3] = -1;
		}
	}
	//End EADH

	//Check intersection with AEFB
	t = -(m_Face_Normals[4].Dot(a_Ray.GetOrigin()) + m_Face_Free_Terms[4])/(m_Face_Normals[4].Dot(a_Ray.GetDirection()));
	E1 = m_Cube_Vertices[1] - m_Cube_Vertices[0];
	E2 = m_Cube_Vertices[4] - m_Cube_Vertices[0];
	T = a_Ray.GetOrigin() - m_Cube_Vertices[0];
	u = a_Ray.GetDirection().Cross(E2).Dot(T)/a_Ray.GetDirection().Cross(E2).Dot(E1);
	v = T.Cross(E1).Dot(a_Ray.GetDirection())/a_Ray.GetDirection().Cross(E2).Dot(E1);
	uv[4][0] = u;
	uv[4][1] = v;
	if ((u>=0 && u<=1) && (v>=0 && v<=1)) {
		hitFaces[4] = 1;
		if (t < a_Dist && t>0) {
			retVal = HIT;
			a_Dist = t;
		}
		else if (t<0) {
			hitFaces[4] = -1;
		}
	}
	//End AEFB

	//Check intersection with CGDH
	t = -(m_Face_Normals[5].Dot(a_Ray.GetOrigin()) + m_Face_Free_Terms[5])/(m_Face_Normals[5].Dot(a_Ray.GetDirection()));
	E1 = m_Cube_Vertices[3] - m_Cube_Vertices[2];
	E2 = m_Cube_Vertices[6] - m_Cube_Vertices[2];
	T = a_Ray.GetOrigin() - m_Cube_Vertices[2];
	u = a_Ray.GetDirection().Cross(E2).Dot(T)/a_Ray.GetDirection().Cross(E2).Dot(E1);
	v = T.Cross(E1).Dot(a_Ray.GetDirection())/a_Ray.GetDirection().Cross(E2).Dot(E1);
	uv[5][0] = u;
	uv[5][1] = v;
	if ((u>=0 && u<=1) && (v>=0 && v<=1)) {
		hitFaces[5] = 1;
		if (t < a_Dist && t>0) {
			retVal = HIT;
			a_Dist = t;
		}
		else if (t<0) {
			hitFaces[5] = -1;
		}
	}
	//End CGDH

	int posHit = 0, negHit = 0;
	for (int i=0; i<6; i++) {
		if (hitFaces[i] > 0) {
			posHit++;
		}
		else if (hitFaces[i] < 0)
			negHit++;
		else
			int tt = 0;
	}
	


	if (posHit > 0 && negHit > 0) {
		retVal = INPRIM;
		a_Ray;
	}

	return retVal;
}

vector3 Cube::GetNormal(vector3& a_Pos) {
	int k=0;
	bool hitFaces[6] = {false};
	for (int i=0; i<6; i++) {
		if (fabs(m_Face_Normals[i].Dot(a_Pos) + m_Face_Free_Terms[i]) < (float)0.0001) {
			k = i;
			hitFaces[i] = true;
		}
	}
	
	//vector3 N = vector3(0, 0, 0) - m_Face_Normals[k];
	vector3 N = m_Face_Normals[k];
	NORMALIZE(N);
	return N;
}

// -----------------------------------------------------------
// Plane primitive class implementation
// -----------------------------------------------------------

int PlanePrim::Intersect( Ray& a_Ray, float& a_Dist )
{
	float d = DOT( m_Plane.N, a_Ray.GetDirection() );
	if (d != 0)
	{
		float dist = -(DOT( m_Plane.N, a_Ray.GetOrigin() ) + m_Plane.D) / d;
		if (dist > 0)
		{
			if (dist < a_Dist) 
			{
				a_Dist = dist;
				return HIT;
			}
		}
	}
	return MISS;
}

vector3 PlanePrim::GetNormal( vector3& a_Pos )
{
	return m_Plane.N;
}

// -----------------------------------------------------------
// Scene class implementation
// -----------------------------------------------------------

Scene::~Scene()
{
	delete m_Primitive;
}

void Scene::InitScene()
{
	float trMatrix[3][3] = {{cos(PI/6), -sin(PI/6)*sin(PI/4), -sin(PI/6)*cos(PI/4)}, {0, cos(PI/4), -sin(PI/4)}, {sin(PI/6), cos(PI/6)*sin(PI/4), cos(PI/6)*cos(PI/4)}};
	float trMatrix2[3][3] = {{cos(PI/4), -sin(PI/4)*sin(PI/12), -sin(PI/4)*cos(PI/12)}, {0, cos(PI/12), -sin(PI/12)}, {sin(PI/4), cos(PI/4)*sin(PI/12), cos(PI/4)*cos(PI/12)}};
	m_Primitive = new Primitive*[500];
	// ground plane
	m_Primitive[0] = new PlanePrim( vector3( 0, 1, 0 ), 4.4f );
	m_Primitive[0]->SetName( "plane" );
	m_Primitive[0]->GetMaterial()->SetReflection( 0.0f );
	m_Primitive[0]->GetMaterial()->SetRefraction( 0.0f );
	m_Primitive[0]->GetMaterial()->SetDiffuse( 0.5f );
	m_Primitive[0]->GetMaterial()->SetColor( Color( 0.4f, 0.3f, 0.3f ) );
	
	//reflective cube
	m_Primitive[1] = new Cube( vector3( -1.5, 0.0, 7), 2.5);
	m_Primitive[1]->setTransformationMatrix(trMatrix2);
	m_Primitive[1]->SetName( "big cube" );
	m_Primitive[1]->GetMaterial()->SetReflection( 0.8f );
	m_Primitive[1]->GetMaterial()->SetRefraction( 0.0f );
	m_Primitive[1]->GetMaterial()->SetColor( Color( 0.7f, 0.7f, 1.0f ) );
	
	// small reflective sphere on the left
	m_Primitive[2] = new Sphere( vector3( -5.5f, -0.5, 7 ), 1.5 );
	m_Primitive[2]->SetName( "small sphere" );
	m_Primitive[2]->GetMaterial()->SetReflection( 0.5f );
	m_Primitive[2]->GetMaterial()->SetRefraction( 0.0f );
	//m_Primitive[2]->GetMaterial()->SetRefrIndex( 1.5f );
	m_Primitive[2]->GetMaterial()->SetDiffuse( 0.1f );
	m_Primitive[2]->GetMaterial()->SetColor( Color( 0.7f, 0.7f, 1.0f ) );
	
	// light source 1
	m_Primitive[3] = new Sphere( vector3( 0, 5, 5 ), 0.1f );
	m_Primitive[3]->Light( true );
	m_Primitive[3]->GetMaterial()->SetColor( Color( 0.4f, 0.4f, 0.4f ) );
	// light source 2
	m_Primitive[4] = new Sphere( vector3( -3, 5, 1 ), 0.1f );
	m_Primitive[4]->Light( true );
	m_Primitive[4]->GetMaterial()->SetColor( Color( 0.6f, 0.6f, 0.8f ) );

	//refractive cube
	m_Primitive[5] = new Cube( vector3(1.5, -0.2, 3.0), 3.5);
	m_Primitive[5]->setTransformationMatrix(trMatrix);
	m_Primitive[5]->SetName("refractive cube");
	m_Primitive[5]->GetMaterial()->SetReflection(0.2);
	m_Primitive[5]->GetMaterial()->SetRefraction(0.8);
	m_Primitive[5]->GetMaterial()->SetRefrIndex(1.1);
	m_Primitive[5]->GetMaterial()->SetColor( Color( 1, 1, 0.0));

	//diffuse cube
	m_Primitive[6] = new Cube( vector3( -1.5f, -3.8f, 1 ), 1.5f );
	m_Primitive[6]->SetName( "extra sphere" );
	m_Primitive[6]->GetMaterial()->SetReflection( 0.0f );
	m_Primitive[6]->GetMaterial()->SetRefraction( 0.0f );
	m_Primitive[6]->GetMaterial()->SetDiffuse(0.5);
	m_Primitive[6]->GetMaterial()->SetColor( Color( 1.0f, 0.4f, 0.4f ) );
	// back plane
	m_Primitive[7] = new PlanePrim( vector3( 0.4f, 0, -1 ), 12 );
	m_Primitive[7]->SetName( "back plane" );
	m_Primitive[7]->GetMaterial()->SetReflection( 0.0f );
	m_Primitive[7]->GetMaterial()->SetRefraction( 0.0f );
	m_Primitive[7]->GetMaterial()->SetSpecular( 0 );
	m_Primitive[7]->GetMaterial()->SetDiffuse( 0.6f );
	m_Primitive[7]->GetMaterial()->SetColor( Color( 0.5f, 0.3f, 0.5f ) );
	// ceiling plane
	m_Primitive[8] = new PlanePrim( vector3( 0, -1, 0 ), 7.4f );
	m_Primitive[8]->SetName( "ceiling plane" );
	m_Primitive[8]->GetMaterial()->SetReflection( 0.0f );
	m_Primitive[8]->GetMaterial()->SetRefraction( 0.0f );
	m_Primitive[8]->GetMaterial()->SetSpecular( 0 );
	m_Primitive[8]->GetMaterial()->SetDiffuse( 0.5f );
	m_Primitive[8]->GetMaterial()->SetColor( Color( 0.4f, 0.7f, 0.7f ) );
	// grid
	int prim = 9;
	for ( int x = 0; x < 8; x++ ) for ( int y = 0; y < 7; y++ )
	{
		m_Primitive[prim] = new Sphere( vector3( -4.5f + x * 1.5f, -4.3f + y * 1.5f, 10 ), 0.3f );
		m_Primitive[prim]->SetName( "grid sphere" );
		m_Primitive[prim]->GetMaterial()->SetReflection( 0 );
		m_Primitive[prim]->GetMaterial()->SetRefraction( 0 );
		m_Primitive[prim]->GetMaterial()->SetSpecular( 0.6f );
		m_Primitive[prim]->GetMaterial()->SetDiffuse( 0.6f );
		m_Primitive[prim]->GetMaterial()->SetColor( Color( 0.3f, 1.0f, 0.4f ) );
		prim++;
	}
	// set number of primitives
	m_Primitives = prim;
}

}; // namespace Raytracer

