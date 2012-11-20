#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "DisplayClass.h"
#include "../glm/gtc/matrix_transform.hpp"
#include "Material.h"

#include<vector>

using namespace glm;

class Geometry
{
public:
	Geometry();
	~Geometry();

	virtual void setColor(vec3 color){};
	virtual void draw(mat4 modelMatrix, DisplayClass* displayClass){};
	virtual double RayIntersect(vec3 const& P0, vec3 const& V0){return -1;};

public:
	float red;
	float green;
	float blue;
	int xIndex;
	int zIndex;
	float rotation;
	float xScale;
	float yScale;
	float zScale;

	float height;
	Material* material;
};

#endif