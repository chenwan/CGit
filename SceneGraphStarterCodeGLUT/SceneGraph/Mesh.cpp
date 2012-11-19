#include "Mesh.h"
#include <iostream>

vec3 Face::getNormal()
{
	vec3 normal = glm::cross((p2 - p1), (p3 - p1));
	// hardcoded the eye position and light position
	if((dot(normal, vec3(0, 5, 10) - p1) < 0) && (dot(normal, vec3(2.0, 0.0, 2.0) - p1) < 0))
		normal = -normal;
	normal = glm::normalize(normal);
	return normal;
}

Mesh::Mesh()
{}

Mesh::~Mesh()
{
	delete [] normals;
	delete [] vertices;
	delete [] colors;
	delete [] points;
	delete [] faces;
}

void Mesh::generateNormals()
{
	for(unsigned int i = 0; i < nFaces; ++i)
	{
		normals[i * 12 + 0] = normals[i * 12 + 4] = normals[i * 12 + 8] = faces[i].getNormal().x;
		normals[i * 12 + 1] = normals[i * 12 + 5] = normals[i * 12 + 9] = faces[i].getNormal().y;
		normals[i * 12 + 2] = normals[i * 12 + 6] = normals[i * 12 + 10] = faces[i].getNormal().z;
		normals[i * 12 + 3] = normals[i * 12 + 7] = normals[i * 12 + 11] = 0.0f;
	}
}

void Mesh::generateIndices()
{
	for(unsigned int i = 0; i < nFaces * 3; ++i)
	{
		indices[i] = i;
	}
}

void Mesh::setColor(vec3 color)
{
	red = color.r;
	green = color.g;
	blue = color.b;
	for(unsigned int i = 0; i < nFaces * 3; ++i)
	{
		colors[i * 3 + 0] = color.r;
		colors[i * 3 + 1] = color.g;
		colors[i * 3 + 2] = color.b;
	}
}

void Mesh::draw(mat4 modelMatrix, DisplayClass* displayClass)
{
	glBindBuffer(GL_ARRAY_BUFFER, displayClass->vbo);
	glBufferData(GL_ARRAY_BUFFER, nFaces * 3 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, displayClass->cbo);
	glBufferData(GL_ARRAY_BUFFER, nFaces * 3 * 3 * sizeof(float), colors, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, displayClass->nbo);
	glBufferData(GL_ARRAY_BUFFER, nFaces * 3 * 4 * sizeof(float), normals, GL_STATIC_DRAW); 

	glEnableVertexAttribArray(displayClass->positionLocation);
	glEnableVertexAttribArray(displayClass->colorLocation);
	glEnableVertexAttribArray(displayClass->normalLocation);

	glBindBuffer(GL_ARRAY_BUFFER, displayClass->vbo);
	glVertexAttribPointer(displayClass->positionLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, displayClass->cbo);
	glVertexAttribPointer(displayClass->colorLocation, 3, GL_FLOAT, 0, 0, static_cast<char*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, displayClass->nbo);
	glVertexAttribPointer(displayClass->normalLocation, 4, GL_FLOAT, 0, 0, static_cast<char*>(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, displayClass->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nFaces * 3 * sizeof(unsigned short), indices, GL_STATIC_DRAW);

	glUniformMatrix4fv(displayClass->u_modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, nFaces * 3, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(displayClass->positionLocation);
	glDisableVertexAttribArray(displayClass->colorLocation);
	glDisableVertexAttribArray(displayClass->normalLocation);	
}

Extrusion::Extrusion()
{}

Extrusion::Extrusion(float length, int numPoints, vec3* base)
{
	this->length = length;
	height = length;
	this->nPoints = numPoints;
	// should check if the basePoints are clockwise, if not reverse the order
	basePoints = new vec3[nPoints];
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		basePoints[i] = base[i];
	}

	if(isConvex())
	{
		nVertices = nPoints * 2 + 2;
		nFaces = (nPoints - 1) * 4;
	}
	else
	{
		nVertices = nPoints * 2;
		nFaces = (nPoints - 1) * 2;
	}
	points = new vec3[nVertices];
	vertices = new float[nFaces * 3 * 4];
	colors = new float[nFaces * 3 * 3];
	faces = new Face[nFaces];
	normals = new float[nFaces * 3 * 4];
	indices = new unsigned short[nFaces * 3];

	generateSides();

	if(isConvex())
		generateEndcaps();

	generateNormals();
	generateIndices();
	setColor(vec3(1.0f, 0.0f, 1.0f));
}

Extrusion::~Extrusion()
{
	delete [] normals;
	delete [] vertices;
	delete [] colors;
	delete [] points;
	delete [] faces;
}

bool Extrusion::isConvex()
{
	if(nPoints > 3)
	{
		for(unsigned int i = 0; i < nPoints - 2; ++i)
		{
			vec3 u = basePoints[i] - basePoints[i + 1];
			vec3 v = basePoints[i + 2] - basePoints[i + 1];
			if(v.x * u.z - u.x * v.z > 0)
			{
				// basePoints are counter-clockwise
				vec3* tempPoints = new vec3[nPoints];
				for(unsigned int j = 0; j < nPoints; ++j)
				{
					tempPoints[j] = basePoints[nPoints - 1 - j];
				}
				for(unsigned int j = 0; j < nPoints; ++j)
				{
					basePoints[j] = tempPoints[j];
				}
				u = basePoints[i] - basePoints[i + 1];
				v = basePoints[i + 2] - basePoints[i + 1];
			}
			if(v.x * u.z - u.x * v.z < 0)
			{
				// basePoints are clockwise
				vec3 uu = basePoints[nPoints - 2] - basePoints[0];
				vec3 vv = basePoints[1] - basePoints[0];
				if(vv.x * uu.z - uu.x * vv.z > 0)
					return false;
				for(unsigned int j = i + 1; j < nPoints - 2; ++j)
				{
					u = basePoints[j] - basePoints[j + 1];
					v = basePoints[j + 2] - basePoints[j + 1];

					if(v.x * u.z - u.x * v.z > 0)
						return false;
				}
			}
		}
	}
	return true;
}

void Extrusion::generateSides()
{
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		points[i] = basePoints[i];
	}
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		points[i + nPoints] = basePoints[i] + vec3(0.0f, length, 0.0f);
	}
	// if basePoints are clockwise
	for(unsigned int i = 0; i < nPoints - 1; ++i)
	{
		// side faces
		faces[i * 2 + 0].p1 = points[i];
		faces[i * 2 + 0].p2 = points[i + nPoints];
		faces[i * 2 + 0].p3 = points[i + 1];

		faces[i * 2 + 1].p1 = points[i + 1];
		faces[i * 2 + 1].p2 = points[i + nPoints];
		faces[i * 2 + 1].p3 = points[i + nPoints + 1];

		// side face vertices
		vertices[i * 24 + 0] = faces[i * 2].p1.x;
		vertices[i * 24 + 1] = faces[i * 2].p1.y;
		vertices[i * 24 + 2] = faces[i * 2].p1.z;
		vertices[i * 24 + 3] = 1.0f;
		vertices[i * 24 + 4] = faces[i * 2].p2.x;
		vertices[i * 24 + 5] = faces[i * 2].p2.y;
		vertices[i * 24 + 6] = faces[i * 2].p2.z;
		vertices[i * 24 + 7] = 1.0f;
		vertices[i * 24 + 8] = faces[i * 2].p3.x;
		vertices[i * 24 + 9] = faces[i * 2].p3.y;
		vertices[i * 24 + 10] = faces[i * 2].p3.z;
		vertices[i * 24 + 11] = 1.0f;
		vertices[i * 24 + 12] = faces[i * 2 + 1].p1.x;
		vertices[i * 24 + 13] = faces[i * 2 + 1].p1.y;
		vertices[i * 24 + 14] = faces[i * 2 + 1].p1.z;
		vertices[i * 24 + 15] = 1.0f;
		vertices[i * 24 + 16] = faces[i * 2 + 1].p2.x;
		vertices[i * 24 + 17] = faces[i * 2 + 1].p2.y;
		vertices[i * 24 + 18] = faces[i * 2 + 1].p2.z;
		vertices[i * 24 + 19] = 1.0f;
		vertices[i * 24 + 20] = faces[i * 2 + 1].p3.x;
		vertices[i * 24 + 21] = faces[i * 2 + 1].p3.y;
		vertices[i * 24 + 22] = faces[i * 2 + 1].p3.z;
		vertices[i * 24 + 23] = 1.0f;
	}
}

void Extrusion::generateEndcaps()
{
	// base faces
	for(unsigned int i = 0; i < nPoints - 1; ++i)
	{
		faces[(nPoints - 1) * 2 + i].p1 = vec3(basePoints[i].x, basePoints[i].y, basePoints[i].z);
		faces[(nPoints - 1) * 2 + i].p2 = vec3(basePoints[i + 1].x, basePoints[i + 1].y, basePoints[i + 1].z);
		faces[(nPoints - 1) * 2 + i].p3 = vec3(0.0f, 0.0f, 0.0f);

		vertices[(nPoints - 1) * 24 + i * 12 + 0] = faces[(nPoints - 1) * 2 + i].p1.x;
		vertices[(nPoints - 1) * 24 + i * 12 + 1] = faces[(nPoints - 1) * 2 + i].p1.y;
		vertices[(nPoints - 1) * 24 + i * 12 + 2] = faces[(nPoints - 1) * 2 + i].p1.z;
		vertices[(nPoints - 1) * 24 + i * 12 + 3] = 1.0f;
		vertices[(nPoints - 1) * 24 + i * 12 + 4] = faces[(nPoints - 1) * 2 + i].p2.x;
		vertices[(nPoints - 1) * 24 + i * 12 + 5] = faces[(nPoints - 1) * 2 + i].p2.y;
		vertices[(nPoints - 1) * 24 + i * 12 + 6] = faces[(nPoints - 1) * 2 + i].p2.z;
		vertices[(nPoints - 1) * 24 + i * 12 + 7] = 1.0f;
		vertices[(nPoints - 1) * 24 + i * 12 + 8] = faces[(nPoints - 1) * 2 + i].p3.x;
		vertices[(nPoints - 1) * 24 + i * 12 + 9] = faces[(nPoints - 1) * 2 + i].p3.y;
		vertices[(nPoints - 1) * 24 + i * 12 + 10] = faces[(nPoints - 1) * 2 + i].p3.z;
		vertices[(nPoints - 1) * 24 + i * 12 + 11] = 1.0f;
	}
	// top faces
	for(unsigned int i = 0; i < nPoints - 1; ++i)
	{
		faces[(nPoints - 1) * 3 + i].p1 = vec3(basePoints[i].x, length, basePoints[i].z);
		faces[(nPoints - 1) * 3 + i].p2 = vec3(0.0f, length, 0.0f);
		faces[(nPoints - 1) * 3 + i].p3 = vec3(basePoints[i + 1].x, length, basePoints[i + 1].z);

		vertices[(nPoints - 1) * 36 + i * 12 + 0] = faces[(nPoints - 1) * 3 + i].p1.x;
		vertices[(nPoints - 1) * 36 + i * 12 + 1] = faces[(nPoints - 1) * 3 + i].p1.y;
		vertices[(nPoints - 1) * 36 + i * 12 + 2] = faces[(nPoints - 1) * 3 + i].p1.z;
		vertices[(nPoints - 1) * 36 + i * 12 + 3] = 1.0f;
		vertices[(nPoints - 1) * 36 + i * 12 + 4] = faces[(nPoints - 1) * 3 + i].p2.x;
		vertices[(nPoints - 1) * 36 + i * 12 + 5] = faces[(nPoints - 1) * 3 + i].p2.y;
		vertices[(nPoints - 1) * 36 + i * 12 + 6] = faces[(nPoints - 1) * 3 + i].p2.z;
		vertices[(nPoints - 1) * 36 + i * 12 + 7] = 1.0f;
		vertices[(nPoints - 1) * 36 + i * 12 + 8] = faces[(nPoints - 1) * 3 + i].p3.x;
		vertices[(nPoints - 1) * 36 + i * 12 + 9] = faces[(nPoints - 1) * 3 + i].p3.y;
		vertices[(nPoints - 1) * 36 + i * 12 + 10] = faces[(nPoints - 1) * 3 + i].p3.z;
		vertices[(nPoints - 1) * 36 + i * 12 + 11] = 1.0f;
	}
}

Surfrev::Surfrev()
{}

Surfrev::Surfrev(int numSlices, int numPoints, vec3* polylinePoints)
{
	this->numSlices = numSlices;
	this->polylinePoints = polylinePoints;
	this->nPoints = numPoints;
	// temp height
	height = 1.0f;

	nVertices = numSlices * nPoints;
	nFaces = nVertices * 2;
	/*
	if(polylinePoints[0].y != 0)
	{
		nVertices++;
		nFaces += numSlices;
	}
	if(polylinePoints[nPoints - 1].y != 0)
	{
		nVertices++;
		nFaces += numSlices;
	}
	*/
	points = new vec3[numSlices * nPoints + nPoints];
	vertices = new float[nFaces * 3 * 4];
	colors = new float[nFaces * 3 * 3];
	faces = new Face[nFaces];
	normals = new float[nFaces * 3 * 4];
	indices = new unsigned short[nFaces * 3];

	generateSides();
	// if hasEndcap
	//generateEndcaps();
	generateNormals();
	generateIndices();
	setColor(vec3(1.0f, 0.0f, 0.0f));
}

Surfrev::~Surfrev()
{
	delete [] normals;
	delete [] vertices;
	delete [] colors;
	delete [] points;
	delete [] faces;
}

void Surfrev::generateSides()
{
	// side vertices
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		for(unsigned int j = 0; j < numSlices + 1; ++j)
		{
			points[i * (numSlices + 1) + j] = vec3(cos(j * (360.0f / (float)numSlices) * PI / 180.0f) * polylinePoints[i].x, polylinePoints[i].y, sin(j * (360.0f / (float)numSlices) * PI / 180.0f) * polylinePoints[i].x);
		}
	}
	// side faces
	for(unsigned int i = 0; i < nPoints - 1; ++i)
	{
		for(unsigned int j = 0; j < numSlices; ++j)
		{
			faces[(i * numSlices + j) * 2].p1 = points[i * numSlices + j + 0];
			faces[(i * numSlices + j) * 2].p2 = points[i * numSlices + j + 1];
			faces[(i * numSlices + j) * 2].p3 = points[i * numSlices + j + 0 + numSlices + 1];

			faces[(i * numSlices + j) * 2 + 1].p1 = points[i * numSlices + j + 1];
			faces[(i * numSlices + j) * 2 + 1].p2 = points[i * numSlices + j + 1 + numSlices + 1];
			faces[(i * numSlices + j) * 2 + 1].p3 = points[i * numSlices + j + 0 + numSlices + 1];

			vertices[(i * numSlices + j) * 24 + 0] = faces[(i * numSlices + j) * 2].p1.x;
			vertices[(i * numSlices + j) * 24 + 1] = faces[(i * numSlices + j) * 2].p1.y;
			vertices[(i * numSlices + j) * 24 + 2] = faces[(i * numSlices + j) * 2].p1.z;
			vertices[(i * numSlices + j) * 24 + 3] = 1.0f;
			vertices[(i * numSlices + j) * 24 + 4] = faces[(i * numSlices + j) * 2].p2.x;
			vertices[(i * numSlices + j) * 24 + 5] = faces[(i * numSlices + j) * 2].p2.y;
			vertices[(i * numSlices + j) * 24 + 6] = faces[(i * numSlices + j) * 2].p2.z;
			vertices[(i * numSlices + j) * 24 + 7] = 1.0f;
			vertices[(i * numSlices + j) * 24 + 8] = faces[(i * numSlices + j) * 2].p3.x;
			vertices[(i * numSlices + j) * 24 + 9] = faces[(i * numSlices + j) * 2].p3.y;
			vertices[(i * numSlices + j) * 24 + 10] = faces[(i * numSlices + j) * 2].p3.z;
			vertices[(i * numSlices + j) * 24 + 11] = 1.0f;
			vertices[(i * numSlices + j) * 24 + 12] = faces[(i * numSlices + j) * 2 + 1].p1.x;
			vertices[(i * numSlices + j) * 24 + 13] = faces[(i * numSlices + j) * 2 + 1].p1.y;
			vertices[(i * numSlices + j) * 24 + 14] = faces[(i * numSlices + j) * 2 + 1].p1.z;
			vertices[(i * numSlices + j) * 24 + 15] = 1.0f;
			vertices[(i * numSlices + j) * 24 + 16] = faces[(i * numSlices + j) * 2 + 1].p2.x;
			vertices[(i * numSlices + j) * 24 + 17] = faces[(i * numSlices + j) * 2 + 1].p2.y;
			vertices[(i * numSlices + j) * 24 + 18] = faces[(i * numSlices + j) * 2 + 1].p2.z;
			vertices[(i * numSlices + j) * 24 + 19] = 1.0f;
			vertices[(i * numSlices + j) * 24 + 20] = faces[(i * numSlices + j) * 2 + 1].p3.x;
			vertices[(i * numSlices + j) * 24 + 21] = faces[(i * numSlices + j) * 2 + 1].p3.y;
			vertices[(i * numSlices + j) * 24 + 22] = faces[(i * numSlices + j) * 2 + 1].p3.z;
			vertices[(i * numSlices + j) * 24 + 23] = 1.0f;
		}
	}
}

void Surfrev::generateEndcaps()
{
	// if polylinePoints start from bottom
	// first center vertex
	vertices[numSlices * nPoints * 4 + 0] = 0.0f;
	vertices[numSlices * nPoints * 4 + 1] = polylinePoints[0].y;
	vertices[numSlices * nPoints * 4 + 2] = 0.0f;
	vertices[numSlices * nPoints * 4 + 3] = 1.0f;

	points[numSlices * nPoints] = vec3(0.0f, polylinePoints[0].y, 0.0f);

	// second center vertex
	vertices[numSlices * nPoints * 4 + 4] = 0.0f;
	vertices[numSlices * nPoints * 4 + 5] = polylinePoints[nPoints - 1].y;
	vertices[numSlices * nPoints * 4 + 6] = 0.0f;
	vertices[numSlices * nPoints * 4 + 7] = 1.0f;

	points[numSlices * nPoints + 1] = vec3(0.0f, polylinePoints[nPoints - 1].y, 0.0f);

	// first endcap faces
	for(unsigned int i = 0; i < numSlices; ++i)
	{
		faces[numSlices * nPoints * 2 + i].p1 = points[i + 0];
		faces[numSlices * nPoints * 2 + i].p2 = points[numSlices * nPoints];
		faces[numSlices * nPoints * 2 + i].p3 = points[i + 1];

		indices[numSlices * nPoints * 6 + i * 3 + 0] = i;
		indices[numSlices * nPoints * 6 + i * 3 + 1] = numSlices * nPoints;
		indices[numSlices * nPoints * 6 + i * 3 + 2] = i + 1;
	}

	// second endcap faces
	for(unsigned int i = 0; i < numSlices; ++i)
	{
		faces[(numSlices + 1) * nPoints * 2 + i].p1 = points[(numSlices - 1) * nPoints + 0];
		faces[(numSlices + 1) * nPoints * 2 + i].p2 = points[(numSlices - 1) * nPoints + 1];
		faces[(numSlices + 1) * nPoints * 2 + i].p3 = points[numSlices * nPoints + 1];

		indices[(numSlices + 1) * nPoints * 6 + i * 3 + 0] = (numSlices - 1) * nPoints + 0;
		indices[(numSlices + 1) * nPoints * 6 + i * 3 + 1] = (numSlices - 1) * nPoints + 1;
		indices[(numSlices + 1) * nPoints * 6 + i * 3 + 2] = numSlices * nPoints + 1;
	}
}

double triangleArea(vec3 const& P1, vec3 const& P2, vec3 const& P3)
{
	double det1 = determinant(mat3(vec3(P1.y, P1.z, 1), vec3(P2.y, P2.z, 1), vec3(P3.y, P3.z, 1)));
	double det2 = determinant(mat3(vec3(P1.z, P1.x, 1), vec3(P2.z, P2.x, 1), vec3(P3.z, P3.x, 1)));
	double det3 = determinant(mat3(vec3(P1.x, P1.y, 1), vec3(P2.x, P2.y, 1), vec3(P3.x, P3.y, 1)));
	double area = 0.5 * sqrt(det1 * det1 + det2 * det2  + det3 * det3);
	return area;
}

double Mesh::RayIntersect(vec3 const& P0, vec3 const& V0)
{
	for(unsigned int i = 0; i < nFaces; ++i)
	{
		vec3 P1 = faces[i].p1;
		vec3 P2 = faces[i].p2;
		vec3 P3 = faces[i].p3;

		vec3 V = normalize(V0);
		vec3 normal = normalize(cross(P2 - P1, P3 - P1));
		if(dot(normal, V) < 0)
			normal = -normal;
		double denominator = dot(normal, V);
		double t = -1;
		if((denominator < EPSILON) && (denominator > -EPSILON))
			t = -1;
		else
		{
			t = dot(normal, P1 - P0) / denominator;
			if(t < 0)
				t = -1;
			else
			{
				vec3 P = P0 + vec3(t * V.x, t * V.y, t * V.z);
				double s = triangleArea(P1, P2, P3);
				double s1 = triangleArea(P, P2, P3) / s;
				double s2 = triangleArea(P, P3, P1) / s;
				double s3 = triangleArea(P, P1, P2) / s;
				// better solution?
				if((s1 >= 0) && (s1 <= 1) && (s2 >= 0) && (s2 <= 1) && (s3 >= 0) && (s3 <= 1) && (s1 + s2 + s3 - 1 < EPSILON) && (s1 + s2 + s3 - 1 > -EPSILON))
					return t;
				else
					t = -1;
			}
		}
	}
	return -1;
}