#ifndef MESH_H
#define MESH_H

#include "../glm/glm.hpp"
#include "Geometry.h"

class Face
{
public:
	vec3 getNormal();
public:
	vec3 p1;
	vec3 p2;
	vec3 p3;
};

class Mesh: public Geometry
{
public:
	Mesh();
	~Mesh();

	void generateSides();
	void generateEndcaps();
	void generateNormals();
	void generateIndices();
	void setColor(vec3 color);

	void draw(mat4 modelMatrix, DisplayClass* displayClass);

public:
	unsigned int nPoints; // the number of the input basePoints or polylinePoints
	unsigned int nVertices; // the number of unique vertices
	unsigned int nFaces; // the number of faces
	Face* faces;
	vec3* points; // unique vertices array
	float* vertices; // the actual vertices array that stores 3 vertices per face
	float* colors;
	float* normals;
	unsigned short* indices;
};

class Extrusion: public Mesh
{
public:
	Extrusion();
	Extrusion(float length, int numPoints, vec3* base);
	~Extrusion();

	bool isConvex(); // Check whether or not the base is a convex polygon.
	                 // If it is, the endcaps must be displayed,
	                 // otherwise only the sides whould appear on the screen.
	void generateSides();
	void generateEndcaps();

public:
	float length;
	vec3* basePoints;
};

class Surfrev: public Mesh
{
public:
	Surfrev();
	Surfrev(int numSlices, int numPoints, vec3* polylinePoints);
	~Surfrev();

	void generateSides();
	void generateEndcaps();

public:
	unsigned int numSlices;
	vec3* polylinePoints;
};

#endif