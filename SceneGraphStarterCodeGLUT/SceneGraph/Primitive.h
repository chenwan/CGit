#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "../glm/glm.hpp"
#include "Define.h"
#include "DisplayClass.h"

using namespace glm;

class Primitive
{
public:
	Primitive();
	~Primitive();

	void initVertices();
	void initNormals();
	void initIndices();
	void setColor(float red, float green, float blue);

	void draw(mat4 modelMatrix, DisplayClass* displayClass);
	double RayIntersect(vec3 const& P0, vec3 const& V0);

public:
	int nVertices;
	int nIndices;
	float* vertices;
	float* normals;
	unsigned short* indices;
	float* colors;
};

class Box: public Primitive
{
public:
	Box();
	~Box();

	void initVertices();
	void initNormals();
	void initIndices();

	double RayIntersect(vec3 const& P0, vec3 const& V0);
};

class Sphere: public Primitive
{
public:
	Sphere();
	~Sphere();

	void initVertices();
	void initNormals();
	void initIndices();
	int CreateNSphere(int iterations);

	double RayIntersect(vec3 const& P0, vec3 const& V0);

public:
	int nIterations;
};

class Cylinder: public Primitive
{
public:
	Cylinder();
	~Cylinder();

	void initVertices();
	void initNormals();
	void initIndices();

	double RayIntersect(vec3 const& P0, vec3 const& V0);

public:
	int nEdges;
};

#endif