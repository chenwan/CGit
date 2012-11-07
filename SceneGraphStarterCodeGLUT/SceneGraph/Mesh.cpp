#include "Mesh.h"

vec3 Face::getNormal()
{
	vec3 normal = glm::cross((p2 - p1), (p3 - p1));
	// hardcoded the eye position and light position
	if((dot(vec3(0, 5, 10), p1) < 0) && (dot(vec3(2.0, 0.0, 2.0), p1) < 0))
		normal = -normal;
	normal = glm::normalize(normal);
	return normal;
}

Mesh::Mesh()
{}

Mesh::~Mesh()
{}

void Mesh::generateNormals()
{
	for(unsigned int i = 0; i < nVertices; ++i)
	{
		normals[i * 4 + 0] = faces[i].getNormal().x;
		normals[i * 4 + 1] = faces[i].getNormal().y;
		normals[i * 4 + 2] = faces[i].getNormal().z;
		normals[i * 4 + 3] = 0.0f;
	}
}

void Mesh::setColor(float red, float green, float blue)
{
	for(unsigned int i = 0; i < nVertices * 3; i += 3)
	{
		colors[i] = red;
		colors[i+1] = green;
		colors[i+2] = blue;
	}
}

void Mesh::draw(mat4 modelMatrix, DisplayClass* displayClass)
{
	modelMatrix = translate(modelMatrix, vec3(0.0f, height, 0.0f));
	glBindBuffer(GL_ARRAY_BUFFER, displayClass->vbo);
	glBufferData(GL_ARRAY_BUFFER, nVertices * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, displayClass->cbo);
	glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), colors, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, displayClass->nbo);
	glBufferData(GL_ARRAY_BUFFER, nVertices * 4 * sizeof(float), normals, GL_STATIC_DRAW); 

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

Extrusion::Extrusion(float length, int numPoints, vec3* basePoints)
{
	this->length = length;
	height = length;
	this->nPoints = numPoints;
	// should check if the basePoints are clockwise, if not reverse the order
	this->basePoints = basePoints;

	if(isConvex())
	{
		nVertices = nPoints * 2 + 2;
		nFaces = nPoints * 4;
	}
	else
	{
		nVertices = nPoints * 2;
		nFaces = nPoints * 2;
	}

	vertices = new float[nVertices * 4];
	colors = new float[nVertices * 3];
	faces = new Face[nFaces];
	normals = new float[nVertices * 4];
	indices = new unsigned short[nFaces * 3];

	generateSides();

	if(isConvex())
		generateEndcaps();

	//generateNormals();
	setColor(0.6f, 0.6f, 0.6f);
}

Extrusion::~Extrusion()
{}

bool Extrusion::isConvex()
{
	for(unsigned int i = 0; i < nPoints - 1; ++i)
	{
		if(basePoints[i].x * basePoints[i + 1].y - basePoints[i].y * basePoints[i + 1].x > 0)
		{
			// basePoints are counter-clockwise
			// reverse the order
			vec3* tempPoints;
			for(unsigned int j = 0; j < nPoints; ++j)
			{
				tempPoints[j] = basePoints[nPoints - 1 - j];
			}
			basePoints = tempPoints;
		}

		if(basePoints[i].x * basePoints[i + 1].y - basePoints[i].y * basePoints[i + 1].x < 0)
		{
			// basePoints are clockwise
			for(unsigned int j = i + 1; j < nPoints - 1; ++j)
			{
				if(basePoints[j].x * basePoints[j + 1].y - basePoints[j].y * basePoints[j + 1].x > 0)
					return false;
			}
		}
	}
	return true;
}

void Extrusion::generateSides()
{
	// base circle vertices
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		vertices[i * 4 + 0] = basePoints[i].x;
		vertices[i * 4 + 1] = 0.0f;
		vertices[i * 4 + 2] = basePoints[i].z;
		vertices[i * 4 + 3] = 1.0f;

		vec3 normal = vec3(basePoints[i].x, 0.0f, basePoints[i].z);
		normal = normalize(normal);
		normals[i * 4 + 0] = normal.x;
		normals[i * 4 + 1] = normal.y;
		normals[i * 4 + 2] = normal.z;
		normals[i * 4 + 3] = 0.0f;
	}
	// top circle vertices
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		vertices[nPoints * 4 + i * 4 + 0] = basePoints[i].x;
		vertices[nPoints * 4 + i * 4 + 1] = length;
		vertices[nPoints * 4 + i * 4 + 2] = basePoints[i].z;
		vertices[nPoints * 4 + i * 4 + 3] = 1.0f;

		normals[nPoints * 4 + i * 4 + 0] = basePoints[i].x;
		normals[nPoints * 4 + i * 4 + 1] = length;
		normals[nPoints * 4 + i * 4 + 2] = basePoints[i].z;
		normals[nPoints * 4 + i * 4 + 3] = 0.0f;
	}
	// if basePoints are clockwise
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		// side faces
		faces[i * 2].p1 = basePoints[i];
		faces[i * 2].p2 = basePoints[i + 1];
		faces[i * 2].p3 = vec3(basePoints[i].x, length, basePoints[i].z);

		faces[i * 2 + 1].p1 = vec3(basePoints[i].x, length, basePoints[i].z);
		faces[i * 2 + 1].p2 = basePoints[i + 1];
		faces[i * 2 + 1].p3 = vec3(basePoints[i + 1].x, length, basePoints[i + 1].z);

		// side face indices
		indices[i * 6 + 0] = i;
		indices[i * 6 + 1] = i + 1;
		indices[i * 6 + 2] = nPoints + 1 + i;

		indices[i * 6 + 3] = nPoints + 1 + i;
		indices[i * 6 + 4] = i + 1;
		indices[i * 6 + 5] = nPoints + 2 + i;
	}
}

void Extrusion::generateEndcaps()
{
	// base center vertex
	vertices[nPoints * 8 + 0] = 0.0f;
	vertices[nPoints * 8 + 1] = 0.0f;
	vertices[nPoints * 8 + 2] = 0.0f;
	vertices[nPoints * 8 + 3] = 1.0f;
	// top center vertex
	vertices[nPoints * 8 + 4] = 0.0f;
	vertices[nPoints * 8 + 5] = length;
	vertices[nPoints * 8 + 6] = 0.0f;
	vertices[nPoints * 8 + 7] = 1.0f;

	normals[nPoints * 8 + 0] = 0.0f;
	normals[nPoints * 8 + 1] = -1.0f;
	normals[nPoints * 8 + 2] = 0.0f;
	normals[nPoints * 8 + 3] = 0.0f;
	
	normals[nPoints * 8 + 4] = 0.0f;
	normals[nPoints * 8 + 5] = 1.0;
	normals[nPoints * 8 + 6] = 0.0f;
	normals[nPoints * 8 + 7] = 0.0f;

	// base faces
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		faces[nPoints * 2 + i].p1 = basePoints[i];
		faces[nPoints * 2 + i].p2 = basePoints[i + i];
		faces[nPoints * 2 + i].p3 = vec3(0.0f, 0.0f, 0.0f);

		indices[nPoints * 6 + i * 3 + 0] = i;
		indices[nPoints * 6 + i * 3 + 1] = i + 1;
		indices[nPoints * 6 + i * 3 + 2] = nPoints;
	}
	// top faces
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		faces[nPoints * 3 + i].p1 = vec3(basePoints[i].x, length, basePoints[i].z);
		faces[nPoints * 3 + i].p2 = vec3(basePoints[i + 1].x, length, basePoints[i + 1].z);
		faces[nPoints * 3 + i].p3 = vec3(0.0f, length, 0.0f);

		indices[nPoints * 9 + i * 3 + 0] = nPoints + 1 + i;
		indices[nPoints * 9 + i * 3 + 1] = nPoints + 2 + i;
		indices[nPoints * 9 + i * 3 + 2] = nPoints * 2 + 1;
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
	points = new vec3[nVertices];
	vertices = new float[nVertices * 4];
	colors = new float[nVertices * 3];
	faces = new Face[nFaces];
	normals = new float[nVertices * 4];
	indices = new unsigned short[nFaces * 3];

	generateSides();
	// if hasEndcap
	//generateEndcaps();
	generateNormals();
	setColor(1.0f, 0.2f, 0.2f);
}

Surfrev::~Surfrev()
{}

void Surfrev::generateSides()
{
	// side vertices
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		for(unsigned int j = 0; j < numSlices; ++j)
		{
			vertices[(i * numSlices + j) * 4 + 0] = cos(j * (360.0f / (float)numSlices) * PI / 180.0f) * polylinePoints[i].x;
			vertices[(i * numSlices + j) * 4 + 1] = polylinePoints[i].y;
			vertices[(i * numSlices + j) * 4 + 2] = sin(j * (360.0f / (float)numSlices) * PI / 180.0f) * polylinePoints[i].x;
			vertices[(i * numSlices + j) * 4 + 3] = 1.0f;

			points[i * numSlices + j] = vec3(vertices[(i * numSlices + j) * 4 + 0], vertices[(i * numSlices + j) * 4 + 1], vertices[(i * numSlices + j) * 4 + 2]);
		}
	}
	// side faces
	for(unsigned int i = 0; i < nPoints; ++i)
	{
		for(unsigned int j = 0; j < numSlices - 1; ++j)
		{
			faces[(i * numSlices + j) * 2].p1 = points[i * numSlices + j + 0];
			faces[(i * numSlices + j) * 2].p2 = points[i * numSlices + j + 1];
			faces[(i * numSlices + j) * 2].p3 = points[i * (numSlices + 1) + j + 0];

			faces[(i * numSlices + j) * 2 + 1].p1 = points[i * numSlices + j + 1];
			faces[(i * numSlices + j) * 2 + 1].p2 = points[i * (numSlices + 1) + j + 1];
			faces[(i * numSlices + j) * 2 + 1].p3 = points[i * (numSlices + 1) + j + 0];

			indices[(i * numSlices + j) * 6 + 0] = i * numSlices + j + 0;
			indices[(i * numSlices + j) * 6 + 1] = i * numSlices + j + 1;
			indices[(i * numSlices + j) * 6 + 2] = i * (numSlices + 1) + j + 0;

			indices[(i * numSlices + j) * 6 + 3] = i * numSlices + j + 1;
			indices[(i * numSlices + j) * 6 + 4] = i * (numSlices + 1) + j + 1;
			indices[(i * numSlices + j) * 6 + 5] = i * (numSlices + 1) + j + 0;
		}
		faces[(i * numSlices + numSlices - 1) * 2].p1 = points[i * numSlices + numSlices - 1 + 0];
		faces[(i * numSlices + numSlices - 1) * 2].p2 = points[i * numSlices + 0];
		faces[(i * numSlices + numSlices - 1) * 2].p3 = points[i * (numSlices + 1) + 0];

		faces[(i * numSlices + numSlices - 1) * 2 + 1].p1 = points[i * numSlices + numSlices - 1 + 1];
		faces[(i * numSlices + numSlices - 1) * 2 + 1].p2 = points[i * (numSlices + 1) + 1];
		faces[(i * numSlices + numSlices - 1) * 2 + 1].p3 = points[i * (numSlices + 1) + 0];

		indices[(i * numSlices + numSlices - 1) * 6 + 0] = i * numSlices + numSlices - 1 + 0;
		indices[(i * numSlices + numSlices - 1) * 6 + 1] = i * numSlices + 0;
		indices[(i * numSlices + numSlices - 1) * 6 + 2] = i * (numSlices + 1) + 0;

		indices[(i * numSlices + numSlices - 1) * 6 + 3] = i * numSlices + numSlices - 1 + 1;
		indices[(i * numSlices + numSlices - 1) * 6 + 4] = i * (numSlices + 1) + 1;
		indices[(i * numSlices + numSlices - 1) * 6 + 5] = i * (numSlices + 1) + 0;
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