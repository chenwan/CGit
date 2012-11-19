#ifndef RAYTRACE_H
#define RAYTRACE_H

#include<fstream>
#include<iostream>
#include<string>

#include "EasyBMP.h"
#include "Camera.h"
#include "Image.h"
#include "Light.h"
#include "SceneGraph.h"

using namespace std;

class RayTrace
{
public:
	RayTrace();
	~RayTrace();
	void ParseRayTraceFile(string rayTraceFile);
	void Main();
	void TraceRay(vec3 start, vec3 direction, int depth, vec3 color);

public:
	Camera* camera;
	Image* image;
	Light* light;
	vec3 color; // returned color
	vec3 P; // ray start position
	vec3 D; // ray direction
};

#endif