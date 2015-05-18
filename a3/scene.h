// -----------------------------------------------------------
// scene.h
// 2004 - Jacco Bikker - jacco@bik5.com - www.bik5.com -   <><
// -----------------------------------------------------------

#ifndef I_SCENE_H
#define I_SCENE_H

#include "raytracer.h"

#include <vector>

using namespace std;

namespace Raytracer {

// Intersection method return values
#define HIT		 1		// Ray hit primitive
#define MISS	 0		// Ray missed primitive
#define INPRIM	-1		// Ray started inside primitive

// -----------------------------------------------------------
// Material class definition
// -----------------------------------------------------------

class Material
{
public:
	Material();
	void SetColor( Color& a_Color ) { m_Color = a_Color; }
	Color GetColor() { return m_Color; }
	void SetDiffuse( float a_Diff ) { m_Diff = a_Diff; }
	void SetSpecular( float a_Spec ) { m_Spec = a_Spec; }
	void SetReflection( float a_Refl ) { m_Refl = a_Refl; }
	void SetRefraction( float a_Refr ) { m_Refr = a_Refr; }

	float GetSpecular() { return m_Spec; }
	float GetDiffuse() { return m_Diff; }
	float GetReflection() { return m_Refl; }
	float GetRefraction() { return m_Refr; }

	void SetRefrIndex( float a_Refr ) { m_RIndex = a_Refr; }
	float GetRefrIndex() { return m_RIndex; }
private:
	Color m_Color;
	float m_Refl, m_Refr;
	float m_Diff, m_Spec;
	float m_RIndex;
};

// -----------------------------------------------------------
// Primitive class definition
// -----------------------------------------------------------

class Primitive
{
public:
	enum
	{
		SPHERE = 1,
		PLANE,
		CUBE
	};
	Primitive() : m_Name( 0 ), m_Light( false ) {};
	Material* GetMaterial() { return &m_Material; }
	void SetMaterial( Material& a_Mat ) { m_Material = a_Mat; }
	virtual int GetType() = 0;
	virtual int Intersect( Ray& a_Ray, float& a_Dist ) = 0;
	virtual vector3 GetNormal( vector3& a_Pos ) = 0;
	virtual Color GetColor( vector3& ) { return m_Material.GetColor(); }
	virtual void Light( bool a_Light ) { m_Light = a_Light; }
	bool IsLight() { return m_Light; }
	void SetName( char* a_Name );
	char* GetName() { return m_Name; }

	virtual void setTransformationMatrix(float trMatrix[3][3])=0;
protected:
	Material m_Material;
	char* m_Name;
	bool m_Light;
};

// -----------------------------------------------------------
// Sphere primitive class definition
// -----------------------------------------------------------

class Sphere : public Primitive
{
public:
	int GetType() { return SPHERE; }
	Sphere( vector3& a_Centre, float a_Radius ) : 
		m_Centre( a_Centre ), m_SqRadius( a_Radius * a_Radius ), 
		m_Radius( a_Radius ), m_RRadius( 1.0f / a_Radius ) {};
	vector3& GetCentre() { return m_Centre; }
	float GetSqRadius() { return m_SqRadius; }
	int Intersect( Ray& a_Ray, float& a_Dist );
	vector3 GetNormal( vector3& a_Pos ) { return (a_Pos - m_Centre) * m_RRadius; }
	void setTransformationMatrix(float trMatrix[3][3]){}
private:
	vector3 m_Centre;
	float m_SqRadius, m_Radius, m_RRadius;
};

// -----------------------------------------------------------
// PlanePrim primitive class definition
// -----------------------------------------------------------

class PlanePrim : public Primitive
{
public:
	int GetType() { return PLANE; }
	PlanePrim( vector3& a_Normal, float a_D ) : m_Plane( plane( a_Normal, a_D ) ) {};
	vector3& GetNormal() { return m_Plane.N; }
	float GetD() { return m_Plane.D; }
	int Intersect( Ray& a_Ray, float& a_Dist );
	vector3 GetNormal( vector3& a_Pos );
	void setTransformationMatrix(float trMatrix[3][3]){}
private:
	plane m_Plane;
};

//------------------------------------------------------------
// CubePrim primitive class definition
//------------------------------------------------------------

class Cube: public Primitive {
public:
	Cube();
	Cube(vector3& center, float sideLength);
	Cube(Cube& otherCube);
	~Cube();

	int GetType() {return CUBE;}
	
	vector3& GetCentre() {return m_Center;}
	vector3 GetNormal(vector3& a_Pos);
	int Intersect(Ray& a_Ray, float& a_Dist);
	float GetSideLength() {return m_Side_Length;}

	void setTransformationMatrix(float a_Matrix[3][3]);
	vector3 transformPoint(vector3 point);
private:
	void CalculateNormalsAndFreeTerms();
	vector3 m_Center;
	float m_Side_Length;
	vector3 m_Cube_Vertices[8];
	vector3 m_Face_Normals[6];
	float m_Face_Free_Terms[6];

	float m_Tr_Matrix[3][3];
};

// -----------------------------------------------------------
// Scene class definition
// -----------------------------------------------------------

class Scene
{
public:
	Scene() : m_Primitives( 0 ), m_Primitive( 0 ) {};
	~Scene();
	void SphereTree( int& a_Prim, float a_Radius, vector3 a_Pos, int a_Depth );
	void InitScene();
	int GetNrPrimitives() { return m_Primitives; }
	Primitive* GetPrimitive( int a_Idx ) { return m_Primitive[a_Idx]; }
private:
	int m_Primitives;
	Primitive** m_Primitive;
};

}; // namespace Raytracer

#endif