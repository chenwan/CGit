#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

using namespace glm;

class Light
{
public:
	Light();
	~Light();
	void setColor(vec3 color);
	void setPosition(vec3 position);
public:
	vec3 color;
	vec3 position;
	vec3 ambientColor;
};

#endif