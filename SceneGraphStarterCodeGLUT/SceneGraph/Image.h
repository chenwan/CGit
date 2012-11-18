#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image
{
public:
	Image();
	~Image();
public:
	std::string file;
	int reso[2];
};

#endif