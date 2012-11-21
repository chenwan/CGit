#ifndef MATERIAL_H
#define MATERIAL_H

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

using namespace glm;

class Material
{
public:
	Material();
	~Material();
public:
	vec3 color;
	float Kd; // diffuse reflectivity factor
	float Ks; // specular reflectivity factor
	float Kn; // specular Phong exponent
	float Kt; // transmittance
	float Ka; // ambient factor
	float Kr; // index of refraction
	float reflectivity_parameter;
};

#endif