#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "../glm/glm.hpp"

class Image
{
public:
	Image();
	~Image();
public:
	std::string file;
	int reso[2];
	glm::vec3 backgroundColor;
};

#endif