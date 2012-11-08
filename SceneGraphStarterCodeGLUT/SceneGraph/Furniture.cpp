#include "Furniture.h"

Furniture::Furniture()
{
}

Furniture::~Furniture()
{
	primitives.clear();
	transformations.clear();
}

void Furniture::draw(mat4 modelMatrix, DisplayClass* displayClass)
{
	for(unsigned int i = 0; i < primitives.size(); ++i)
	{
		primitives[i]->setColor(red, green, blue);
		primitives[i]->draw(modelMatrix * transformations[i], displayClass);
	}
}

void Furniture::setColor(vec3 color)
{
	red = color.r;
	green = color.g;
	blue = color.b;
}

Floor::Floor()
{
	Box* floor = new Box();
	primitives.push_back(floor);

	mat4 view = mat4(1.0f);
	transformations.push_back(view);
}

Floor::Floor(int x, int z)
{
	xSize = x;
	zSize = z;

	height = 0.0f;
	initPrimitives();
	initTransformations();
}

Floor::~Floor()
{
	primitives.clear();
	transformations.clear();
}

void Floor::initPrimitives()
{
	Box* floor = new Box();
	primitives.push_back(floor);
}

void Floor::initTransformations()
{
	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.0f, -0.1f, 0.0f));
	view = scale(view, vec3((float)xSize * 2.0f, 0.2f, (float)zSize * 2.0f));
	transformations.push_back(view);	
}

Chair::Chair()
{
	height = 0.6f;
	initPrimitives();
	initTransformations();
}

Chair::~Chair()
{
	primitives.clear();
	transformations.clear();
}

void Chair::initPrimitives()
{
	Box* box = new Box();
	primitives.push_back(box);
	primitives.push_back(box);
	primitives.push_back(box);
	primitives.push_back(box);
	primitives.push_back(box);
	primitives.push_back(box);
}

void Chair::initTransformations()
{
	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.4f, 0.3f, 0.4f));
	view = scale(view, vec3(0.1f, 0.6f, 0.1f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(-0.4f, 0.3f, 0.4f));
	view = scale(view, vec3(0.1f, 0.6f, 0.1f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(0.4f, 0.3f, -0.4f));
	view = scale(view, vec3(0.1f, 0.6f, 0.1f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(-0.4f, 0.3f, -0.4f));
	view = scale(view, vec3(0.1f, 0.6f, 0.1f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.6f, 0.0f));
	view = scale(view, vec3(1.0f, 0.1f, 1.0f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(0.4f, 0.8f, 0.0f));
	view = scale(view, vec3(0.1f, 0.6f, 1.0f));
	transformations.push_back(view);	
}

Table::Table()
{
	height = 1.0f;
	initPrimitives();
	initTransformations();
}

Table::~Table()
{
	primitives.clear();
	transformations.clear();
}

void Table::initPrimitives()
{
	Box* box = new Box();
	Cylinder* cylinder = new Cylinder();
	primitives.push_back(cylinder);
	primitives.push_back(cylinder);
	primitives.push_back(cylinder);
	primitives.push_back(cylinder);
	primitives.push_back(box);
}

void Table::initTransformations()
{
	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.8f, 0.4f, 0.4f));
	view = scale(view, vec3(0.1f, 0.8f, 0.1f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(0.8f, 0.4f, -0.4f));
	view = scale(view, vec3(0.1f, 0.8f, 0.1f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(-0.8f, 0.4f, 0.4f));
	view = scale(view, vec3(0.1f, 0.8f, 0.1f));
	transformations.push_back(view);
		
	view = mat4(1.0f);
	view = translate(view, vec3(-0.8f, 0.4f, -0.4f));
	view = scale(view, vec3(0.1f, 0.8f, 0.1f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.9f, 0.0f));
	view = scale(view,vec3(2.0f, 0.2f, 1.0f));
	transformations.push_back(view);
}

Cabinet::Cabinet()
{
	height = 1.2f;
	initPrimitives();
	initTransformations();
}

Cabinet::~Cabinet()
{
	primitives.clear();
	transformations.clear();
}

void Cabinet::initPrimitives()
{
	Box* box = new Box();
	primitives.push_back(box);
	primitives.push_back(box);
	primitives.push_back(box);
	primitives.push_back(box);
	primitives.push_back(box);
}

void Cabinet::initTransformations()
{
	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.1f, 0.0f));
	view = scale(view, vec3(1.0f, 0.2f, 0.5f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.9f, 0.0f));
	view = scale(view, vec3(1.0f, 0.2f, 0.5f));
	transformations.push_back(view);
	
	view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.5f, 0.0f));
	view = scale(view, vec3(1.0f, 0.2f, 0.5f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(-0.4f, 0.5f, 0.0f));
	view = scale(view, vec3(0.2f, 1.0f, 0.5f));
	transformations.push_back(view);
		
	view = mat4(1.0f);
	view = translate(view, vec3(0.4f, 0.5f, 0.0f));
	view = scale(view, vec3(0.2f, 1.0f, 0.5f));
	transformations.push_back(view);
}

Lamp::Lamp()
{
	height = 1.0f;
	initPrimitives();
	initTransformations();
}

Lamp::~Lamp()
{
	primitives.clear();
	transformations.clear();
}

void Lamp::initPrimitives()
{
	Cylinder* cylinder = new Cylinder();
	Sphere* sphere = new Sphere();
	primitives.push_back(cylinder);
	primitives.push_back(cylinder);
	primitives.push_back(sphere);
	primitives.push_back(sphere);
	primitives.push_back(sphere);
}

void Lamp::initTransformations()
{
	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.0f, 0.0f));
	view = scale(view, vec3(0.5f, 0.1f, 0.5f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.3f, 0.0f));
	view = scale(view, vec3(0.1f, 0.6f, 0.1f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.8f, 0.0f));
	view = scale(view, vec3(0.4f, 0.4f, 0.4f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(0.4f, 1.0f, 0.0f));
	view = scale(view, vec3(0.2f, 0.2f, 0.2f));
	transformations.push_back(view);

	view = mat4(1.0f);
	view = translate(view, vec3(-0.4f, 1.0f, 0.0f));
	view = scale(view, vec3(0.2f, 0.2f, 0.2f));
	transformations.push_back(view);
}