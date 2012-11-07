// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
#include "Camera.h"

#pragma region constructors and destructors
Camera::Camera(void)
{
	// setup default camera parameters
	this->eye = vec3(0, 5, 10);
	this->center = vec3(0, 0, 0);
	this->up = vec3(0, 1, 0);
	this->fovy = 60.0f;
	this->width = DEFAULT_WIDTH;
	this->height = DEFAULT_HEIGHT;
	this->zNear = 0.1f;
	this->zFar = 30.0f;
}

Camera::~Camera(void)
{

}
#pragma endregion

// return projection * view matrix
mat4 Camera::getCameraMatrix()
{
	// LOOKAT: view is the transformation matrix for the CAMERA
	mat4 projection = perspective(fovy, static_cast<float>(width) / static_cast<float>(height), zNear, zFar);
	mat4 view = lookAt(eye, center, up);

	return (projection * view);
}

// resize
void Camera::resizeWindow(int w, int h)
{
	this->width = w;
	this->height = h;
}
