#ifndef RAYTRACE_H
#define RAYTRACE_H

#include<fstream>
#include<iostream>
#include<string>

#include "EasyBMP.h"
#include "Camera.h"
#include "Image.h"
#include "Light.h"

using namespace std;

class RayTrace
{
public:
	RayTrace();
	~RayTrace();
	void ParseRayTraceFile(string rayTraceFile);

public:
	Camera* camera;
	Image* image;
	Light* light;
};

#endif