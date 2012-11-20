#ifndef FURNITURE_H
#define FURNITURE_H

#include "Primitive.h"
#include "Geometry.h"

class Furniture: public Geometry
{
public:
	Furniture();
	~Furniture();
	void initPrimitives();
	void initTransformations();

	void draw(mat4 modelMatrix, DisplayClass* displayClass);
	void setColor(vec3 color);
	double RayIntersect(vec3 const& P0, vec3 const& V0, vec3& N0);

public:
	std::vector<Primitive*> primitives;
	std::vector<mat4> transformations;
};

class Floor: public Furniture
{
public:
	Floor();
	Floor(int x, int z);
	~Floor();

	void initPrimitives();
	void initTransformations();

public:
	int xSize;
	int zSize;
};

class Chair: public Furniture
{
public:
	Chair();
	~Chair();

	void initPrimitives();
	void initTransformations();
};

class Table: public Furniture
{
public:
	Table();
	~Table();

	void initPrimitives();
	void initTransformations();
};

class Cabinet: public Furniture
{
public:
	Cabinet();
	~Cabinet();

	void initPrimitives();
	void initTransformations();
};

class Lamp: public Furniture
{
public:
	Lamp();
	~Lamp();

	void initPrimitives();
	void initTransformations();
};

#endif