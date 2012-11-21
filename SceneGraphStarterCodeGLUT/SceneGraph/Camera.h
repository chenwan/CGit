// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "Define.h"

using namespace glm;

class Camera
{
public:
	vec3 eye;
	vec3 center; // vdir
	vec3 up;
	float fovy;
	int height;
	int width;
	float zNear;
	float zFar;

public:
	// constructor and destructor
	Camera(void);
	~Camera(void);
	
	// get & set properties
	// for example:
	// vec3 getEyePos(void);
	// void setEyePos(vec3 pos);
	// get camera matrices:
	mat4 getCameraMatrix();

	// resize
	void resizeWindow(int w, int h);

	// LOOKAT: Add your interactive camera methods here:
};

#endif

