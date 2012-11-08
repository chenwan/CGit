// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
#ifndef _DISPLAYCLASS_H_
#define _DISPLAYCLASS_H_

// c++ libraries
#include <fstream>
#include <iostream>
// glew
#include "glew.h"
// glm
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "Camera.h"

using namespace glm;

class DisplayClass
{
public: // public members
	Camera *camera;
	// for rotating the 2 cubes;
	float rotation;

public: // public methods
	DisplayClass(void);
	~DisplayClass(void);

	void resizeWindow(int w, int h);
	void redraw();
	void createRedSquare(mat4 modelMatrix);
	void createBlueSquare(mat4 modelMatrix);

public: // private members
	//vertex arrays needed for drawing
	unsigned int vbo;
	unsigned int cbo;
	unsigned int nbo;
	unsigned int ibo;

	//attributes
	unsigned int positionLocation;
	unsigned int colorLocation;
	unsigned int normalLocation;

	//uniforms
	unsigned int u_modelMatrixLocation;
	unsigned int u_projMatrixLocation;
	unsigned int u_lightPosition;
	unsigned int u_lightColor;

	//needed to compile and link and use the shaders
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;

private: // private methods
	//helper function to read shader source and put it in a char array
	//thanks to Swiftless
	char* textFileRead(const char*);

	//some other helper functions from CIS 565
	void printLinkInfoLog(int);
	void printShaderInfoLog(int);
};

#endif
