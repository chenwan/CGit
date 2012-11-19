#include "Geometry.h"

Geometry::Geometry()
{	
	red = 1.0f;
	green = 1.0f;
	blue = 1.0f;

	xIndex = 0;
	zIndex = 0;
	rotation = 0.0f;
	xScale = 1.0f;
	yScale = 1.0f;
	zScale = 1.0f;

	height = 1.0f;
	material = new Material();
}

Geometry::~Geometry()
{
}
