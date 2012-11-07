#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "DisplayClass.h"
#include "../glm/gtc/matrix_transform.hpp"

#include<vector>

using namespace glm;

class Geometry
{
public:
	Geometry();
	~Geometry();
	void setColor(vec3 color);

	virtual void draw(mat4 modelMatrix, DisplayClass* displayClass){};

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
};

#endif