#include "Primitive.h"

Primitive::Primitive()
{}

Primitive::~Primitive()
{}

void Primitive::setColor(float red, float green, float blue)
{
	for(int i = 0; i < nVertices * 3; i += 3)
	{
		colors[i] = red;
		colors[i+1] = green;
		colors[i+2] = blue;
	}
}

void Primitive::draw(mat4 modelMatrix, DisplayClass* displayClass)
{
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned short), indices, GL_STATIC_DRAW);

	glUniformMatrix4fv(displayClass->u_modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(displayClass->positionLocation);
	glDisableVertexAttribArray(displayClass->colorLocation);
	glDisableVertexAttribArray(displayClass->normalLocation);	
}

Box::Box()
{
	nVertices = 24;
	nIndices = 36;
	vertices = new float[96];
	normals = new float[96];
	indices = new unsigned short[36];
	colors = new float[72];

	initVertices();
	initNormals();
	initIndices();
	setColor(1.0f, 1.0f, 1.0f);
}

Box::~Box()
{
	delete [] vertices;
	delete [] normals;
	delete [] indices;
	delete [] colors;
}

void Box::initVertices()
{
	// the actual 8 vertices
	vec4 v0 = vec4(-0.5f, -0.5f, -0.5f, 1.0f);
	vec4 v1 = vec4(-0.5f, -0.5f, 0.5f, 1.0f);
	vec4 v2 = vec4(-0.5f, 0.5f, -0.5f, 1.0f);
	vec4 v3 = vec4(-0.5f, 0.5f, 0.5f, 1.0f);
	vec4 v4 = vec4(0.5f, -0.5f, -0.5f, 1.0f);
	vec4 v5 = vec4(0.5f, -0.5f, 0.5f, 1.0f);
	vec4 v6 = vec4(0.5f, 0.5f, -0.5f, 1.0f);
	vec4 v7 = vec4(0.5f, 0.5f, 0.5f, 1.0f);

	// 24 vertices, 4 for each face
	vertices[0] = v0.x; vertices[1] = v0.y; vertices[2] = v0.z; vertices[3] = 1.0f; // v0
	vertices[4] = v1.x; vertices[5] = v1.y; vertices[6] = v1.z; vertices[7] = 1.0f; // v1
	vertices[8] = v2.x; vertices[9] = v2.y; vertices[10] = v2.z; vertices[11] = 1.0f; // v2
	vertices[12] = v3.x; vertices[13] = v3.y; vertices[14] = v3.z; vertices[15] = 1.0f; // v3
	vertices[16] = v4.x; vertices[17] = v4.y; vertices[18] = v4.z; vertices[19] = 1.0f; // v4
	vertices[20] = v5.x; vertices[21] = v5.y; vertices[22] = v5.z; vertices[23] = 1.0f; // v5
	vertices[24] = v6.x; vertices[25] = v6.y; vertices[26] = v6.z; vertices[27] = 1.0f; // v6
	vertices[28] = v7.x; vertices[29] = v7.y; vertices[30] = v7.z; vertices[31] = 1.0f; // v7
	vertices[32] = v0.x; vertices[33] = v0.y; vertices[34] = v0.z; vertices[35] = 1.0f; // v8
	vertices[36] = v2.x; vertices[37] = v2.y; vertices[38] = v2.z; vertices[39] = 1.0f; // v9
	vertices[40] = v4.x; vertices[41] = v4.y; vertices[42] = v4.z; vertices[43] = 1.0f; // v10
	vertices[44] = v6.x; vertices[45] = v6.y; vertices[46] = v6.z; vertices[47] = 1.0f; // v11
	vertices[48] = v1.x; vertices[49] = v1.y; vertices[50] = v1.z; vertices[51] = 1.0f; // v12
	vertices[52] = v3.x; vertices[53] = v3.y; vertices[54] = v3.z; vertices[55] = 1.0f; // v13
	vertices[56] = v5.x; vertices[57] = v5.y; vertices[58] = v5.z; vertices[59] = 1.0f; // v14
	vertices[60] = v7.x; vertices[61] = v7.y; vertices[62] = v7.z; vertices[63] = 1.0f; // v15
	vertices[64] = v0.x; vertices[65] = v0.y; vertices[66] = v0.z; vertices[67] = 1.0f; // v16
	vertices[68] = v1.x; vertices[69] = v1.y; vertices[70] = v1.z; vertices[71] = 1.0f; // v17
	vertices[72] = v4.x; vertices[73] = v4.y; vertices[74] = v4.z; vertices[75] = 1.0f; // v18
	vertices[76] = v5.x; vertices[77] = v5.y; vertices[78] = v5.z; vertices[79] = 1.0f; // v19
	vertices[80] = v2.x; vertices[81] = v2.y; vertices[82] = v2.z; vertices[83] = 1.0f; // v20
	vertices[84] = v3.x; vertices[85] = v3.y; vertices[86] = v3.z; vertices[87] = 1.0f; // v21
	vertices[88] = v6.x; vertices[89] = v6.y; vertices[90] = v6.z; vertices[91] = 1.0f; // v22
	vertices[92] = v7.x; vertices[93] = v7.y; vertices[94] = v7.z; vertices[95] = 1.0f; // v23
}

void Box::initNormals()
{
	// normals of v0, v1, v2, v3
	for(int i = 0; i < 4; ++i)
	{
		normals[0 + i * 4] = -1;
		normals[1 + i * 4] = 0;
		normals[2 + i * 4] = 0;
		normals[3 + i * 4] = 0;
	}
	// normals of v4, v5, v6, v7
	for(int i = 0; i < 4; ++i)
	{
		normals[16 + i * 4] = 1;
		normals[17 + i * 4] = 0;
		normals[18 + i * 4] = 0;
		normals[19 + i * 4] = 0;
	}
	// normals of v8, v9, v10, v11
	for(int i = 0; i < 4; ++i)
	{
		normals[32 + i * 4] = 0;
		normals[33 + i * 4] = 0;
		normals[34 + i * 4] = -1;
		normals[35 + i * 4] = 0;
	}
	// normals of v12, v13, v14, v15
	for(int i = 0; i < 4; ++i)
	{
		normals[48 + i * 4] = 0;
		normals[49 + i * 4] = 0;
		normals[50 + i * 4] = 1;
		normals[51 + i * 4] = 0;
	}
	// normals of v16, v17, v18, v19
	for(int i = 0; i < 4; ++i)
	{
		normals[64 + i * 4] = 0;
		normals[65 + i * 4] = -1;
		normals[66 + i * 4] = 0;
		normals[67 + i * 4] = 0;
	}
	// normals of v20, v21, v22, v23
	for(int i = 0; i < 4; ++i)
	{
		normals[80 + i * 4] = 0;
		normals[81 + i * 4] = 1;
		normals[82 + i * 4] = 0;
		normals[83 + i * 4] = 0;
	}
}

void Box::initIndices()
{
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 1; indices[4] = 3; indices[5] = 2;
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 5; indices[10] = 6; indices[11] = 7;
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 9; indices[16] = 11; indices[17] = 10;
	indices[18] = 12; indices[19] = 14; indices[20] = 13;
	indices[21] = 13; indices[22] = 14; indices[23] = 15;
	indices[24] = 16; indices[25] = 18; indices[26] = 17;
	indices[27] = 17; indices[28] = 18; indices[29] = 19;
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 21; indices[34] = 23; indices[35] = 22;
}

void swap(double &a, double &b)
{
	double t = a;
	a = b;
	b = t;
}
double RayIntersect(vec3 const& P0, vec3 const& V0)
{
	vec3 V = normalize(V0);
	double Tnear = -INFINITY;
	double Tfar = INFINITY;
	double Xl = -0.5, Xh = 0.5;
	double Yl = -0.5, Yh = 0.5;
	double Zl = -0.5, Zh = 0.5;
	if((V.x < EPSILON) && (V.x > -EPSILON))
	{
		if((P0.x < Xl) || (P0.x > Xh))
			return -1;
	}
	else
	{
		double T1 = (Xl - P0.x) / V.x;
		double T2 = (Xh - P0.x) / V.x;
		if(T1 > T2)
			swap(T1, T2);
		if(T1 > Tnear)
			Tnear = T1;
		if(T2 < Tfar)
			Tfar = T2;
		if(Tnear > Tfar)
			return -1;
		if(Tfar < 0)
			return -1;
	}
	if((V.y < EPSILON) && (V.y > -EPSILON))
	{
		if((P0.y < Yl) || (P0.y > Yh))
			return -1;
	}
	else
	{
		double T1 = (Yl - P0.y) / V.y;
		double T2 = (Yh - P0.y) / V.y;
		if(T1 > T2)
			swap(T1, T2);
		if(T1 > Tnear)
			Tnear = T1;
		if(T2 < Tfar)
			Tfar = T2;
		if(Tnear > Tfar)
			return -1;
		if(Tfar < 0)
			return -1;
	}
	if((V.z < EPSILON) && (V.z > -EPSILON))
	{
		if((P0.z < Zl) || (P0.z > Zh))
			return -1;
	}
	else
	{
		double T1 = (Zl - P0.z) / V.z;
		double T2 = (Zh - P0.z) / V.z;
		if(T1 > T2)
			swap(T1, T2);
		if(T1 > Tnear)
			Tnear = T1;
		if(T2 < Tfar)
			Tfar = T2;
		if(Tnear > Tfar)
			return -1;
		if(Tfar < 0)
			return -1;
	}
	return Tnear;
}

Sphere::Sphere()
{
	nIterations = 4;
	nVertices = 6144;
	nIndices = 6144;
	//faces = new Face[2048];
	vertices = new float[nVertices * 4];
	normals = new float[nVertices * 4];
	indices = new unsigned short[nVertices];
	colors = new float[nVertices * 3];
	//CreateNSphere(4);
	initVertices();
	initNormals();
	initIndices();
	setColor(1.0f, 1.0f, 1.0f);
}

Sphere::~Sphere()
{
	delete [] vertices;
	delete [] normals;
	delete [] indices;
	delete [] colors;
}
/*
   Create a triangular facet approximation to a sphere
   Return the number of facets created.
   The number of facets will be (4^iterations) * 8
*/
/*
int Sphere::CreateNSphere(int iterations)
{
	// Create the level 0 object
	vec3 p[6];
	p[0] = vec3(0,0,1);
	p[1] = vec3(0,0,-1);
	p[2] = vec3(-1,-1,0);
	p[3] = vec3(1,-1,0);
	p[4] = vec3(1,1,0);
	p[5] = vec3(-1,1,0);

	float a = 1.0f / sqrt(2.0f);
	for (int i = 0; i < 6; i++) {
		p[i].x *= a;
		p[i].y *= a;
	}
	
	// Set Vertices
	int numOfVertices;
	for(int i = 0; i < 6; ++i)
	{
		vertices[0 + 4 * i] = p[i].x;
		vertices[1 + 4 * i] = p[i].y;
		vertices[2 + 4 * i] = p[i].z;
		vertices[3 + 4 * i] = 1.0f;
	}
	numOfVertices = 6;

	// Set Faces
	int numOfFacets;
	faces[0].p1 = p[0]; faces[0].p2 = p[3]; faces[0].p3 = p[4];
	faces[1].p1 = p[0]; faces[1].p2 = p[4]; faces[1].p3 = p[5];
	faces[2].p1 = p[0]; faces[2].p2 = p[5]; faces[2].p3 = p[2];
	faces[3].p1 = p[0]; faces[3].p2 = p[2]; faces[3].p3 = p[3];
	faces[4].p1 = p[1]; faces[4].p2 = p[4]; faces[4].p3 = p[3];
	faces[5].p1 = p[1]; faces[5].p2 = p[5]; faces[5].p3 = p[4];
	faces[6].p1 = p[1]; faces[6].p2 = p[2]; faces[6].p3 = p[5];
	faces[7].p1 = p[1]; faces[7].p2 = p[3]; faces[7].p3 = p[2];
	numOfFacets = 8;

	// Set Indices
	indices[0] = 0; indices[1] = 3; indices[2] = 4;
	indices[3] = 0; indices[4] = 4; indices[5] = 5;
	indices[6] = 0; indices[7] = 5; indices[8] = 2;
	indices[9] = 0; indices[10] = 2; indices[11] = 3;
	indices[12] = 1; indices[13] = 4; indices[14] = 3;
	indices[15] = 1; indices[16] = 5; indices[17] = 4;
	indices[18] = 1; indices[19] = 2; indices[20] = 5;
	indices[21] = 1; indices[22] = 3; indices[23] = 2;

	if (iterations < 1)
		return(numOfFacets);

	// Bisect each edge and move to the surface of a unit sphere
	int n;
	vec3 newPoint1, newPoint2, newPoint3;
	for (int it=0;it<iterations;it++) {
		n = numOfFacets;
		for (int i=0;i<n;i++) {
			newPoint1.x = (faces[i].p1.x + faces[i].p2.x) / 2.0f;
			newPoint1.y = (faces[i].p1.y + faces[i].p2.y) / 2.0f;
			newPoint1.z = (faces[i].p1.z + faces[i].p2.z) / 2.0f;
			newPoint2.x = (faces[i].p2.x + faces[i].p3.x) / 2.0f;
			newPoint2.y = (faces[i].p2.y + faces[i].p3.y) / 2.0f;
			newPoint2.z = (faces[i].p2.z + faces[i].p3.z) / 2.0f;
			newPoint3.x = (faces[i].p3.x + faces[i].p1.x) / 2.0f;
			newPoint3.y = (faces[i].p3.y + faces[i].p1.y) / 2.0f;
			newPoint3.z = (faces[i].p3.z + faces[i].p1.z) / 2.0f;
			newPoint1 = glm::normalize(newPoint1);
			newPoint2 = glm::normalize(newPoint2);
			newPoint3 = glm::normalize(newPoint3);

			vertices[numOfVertices * 4] = newPoint1.x; vertices[numOfVertices * 4 + 1] = newPoint1.y; vertices[numOfVertices * 4 + 2] = newPoint1.z; vertices[numOfVertices * 4 + 3] = 1.0f; numOfVertices++;
			vertices[numOfVertices * 4] = newPoint2.x; vertices[numOfVertices * 4 + 1] = newPoint2.y; vertices[numOfVertices * 4 + 2] = newPoint2.z; vertices[numOfVertices * 4 + 3] = 1.0f; numOfVertices++;
			vertices[numOfVertices * 4] = newPoint3.x; vertices[numOfVertices * 4 + 1] = newPoint3.y; vertices[numOfVertices * 4 + 2] = newPoint3.z; vertices[numOfVertices * 4 + 3] = 1.0f; numOfVertices++;

			faces[numOfFacets].p1 = faces[i].p1; faces[numOfFacets].p2 = newPoint1; faces[numOfFacets].p3 = newPoint3; numOfFacets++;
			faces[numOfFacets].p1 = newPoint1; faces[numOfFacets].p2 = faces[i].p2; faces[numOfFacets].p3 = newPoint2; numOfFacets++;
			faces[numOfFacets].p1 = newPoint2; faces[numOfFacets].p2 = faces[i].p3; faces[numOfFacets].p3 = newPoint3; numOfFacets++;
			faces[i].p1 = newPoint1; faces[i].p2 = newPoint2; faces[i].p3 = newPoint3;
		}
	}

	return(numOfFacets);
}
*/
void Sphere::initVertices()
{
	vec3 v1, v2, v3;
	float *v = new float[nVertices / 2];
	
	vertices[0] = 1.0f; vertices[1] = 0.0f; vertices[2] = 0.0f; vertices[3] = 1.0f;
	vertices[4] = 0.0f; vertices[5] = 1.0f; vertices[6] = 0.0f; vertices[7] = 1.0f;
	vertices[8] = 0.0f; vertices[9] = 0.0f; vertices[10] = 1.0f; vertices[11] = 1.0f;
	vertices[12] = -1.0f; vertices[13] = 0.0f; vertices[14] = 0.0f; vertices[15] = 1.0f;
	vertices[16] = 0.0f; vertices[17] = -1.0f; vertices[18] = 0.0f; vertices[19] = 1.0f;
	vertices[20] = 0.0f; vertices[21] = 0.0f; vertices[22] = -1.0f; vertices[23] = 1.0f;

	for(int i = 0; i < nIterations; i++)
	{
		for(int n = 0; n < (3 * (int)pow(4.0f,(float)i)) * 4; n += 1)
		{
			v[n] = vertices[n];
		}
		for(int n = 0; n < (3 * (int)pow(4.0f,(float)i)) * 4; n += 3 * 4)
		{
			v1.x = (v[n] + v[n + 4]) / 2.0f; v1.y = (v[n + 1] + v[n + 5]) / 2.0f; v1.z = (v[n + 2] + v[n + 6]) / 2.0f;
			v1 = glm::normalize(v1);

			v2.x = (v[n] + v[n + 8]) / 2.0f; v2.y = (v[n + 1] + v[n + 9]) / 2.0f; v2.z = (v[n + 2] + v[n + 10]) / 2.0f;
			v2 = glm::normalize(v2);

			v3.x = (v[n + 4] + v[n + 8]) / 2.0f; v3.y = (v[n + 5] + v[n + 9]) / 2.0f; v3.z = (v[n + 6] + v[n + 10]) / 2.0f;
			v3 = glm::normalize(v3);

			vertices[n * 4 + 0] = v[n];	vertices[n * 4 + 1] = v[n + 1]; vertices[n * 4 + 2] = v[n + 2]; vertices[n * 4 + 3] = 1.0f;
			vertices[n * 4 + 4] = v1.x; vertices[n * 4 + 5] = v1.y; vertices[n * 4 + 6] = v1.z; vertices[n * 4 + 7] = 1.0f;
			vertices[n * 4 + 8] = v2.x; vertices[n * 4 + 9] = v2.y; vertices[n * 4 + 10] = v2.z; vertices[n * 4 + 11] = 1.0f;
			vertices[n * 4 + 12] = v[n + 4]; vertices[n * 4 + 13] = v[n + 5]; vertices[n * 4 + 14] = v[n + 6]; vertices[n * 4 + 15] = 1.0f;
			vertices[n * 4 + 16] = v1.x; vertices[n * 4 + 17] = v1.y; vertices[n * 4 + 18] = v1.z; vertices[n * 4 + 19] = 1.0f;
			vertices[n * 4 + 20] = v3.x; vertices[n * 4 + 21] = v3.y; vertices[n * 4 + 22] = v3.z; vertices[n * 4 + 23] = 1.0f;
			vertices[n * 4 + 24] = v[n + 8]; vertices[n * 4 + 25] = v[n + 9]; vertices[n * 4 + 26] = v[n + 10]; vertices[n * 4 + 27] = 1.0f;
			vertices[n * 4 + 28] = v2.x; vertices[n * 4 + 29] = v2.y; vertices[n * 4 + 30] = v2.z; vertices[n * 4 + 31] = 1.0f;
			vertices[n * 4 + 32] = v3.x; vertices[n * 4 + 33] = v3.y; vertices[n * 4 + 34] = v3.z; vertices[n * 4 + 35] = 1.0f;
			vertices[n * 4 + 36] = v1.x; vertices[n * 4 + 37] = v1.y; vertices[n * 4 + 38] = v1.z; vertices[n * 4 + 39] = 1.0f;
			vertices[n * 4 + 40] = v2.x; vertices[n * 4 + 41] = v2.y; vertices[n * 4 + 42] = v2.z; vertices[n * 4 + 43] = 1.0f;
			vertices[n * 4 + 44] = v3.x; vertices[n * 4 + 45] = v3.y; vertices[n * 4 + 46] = v3.z; vertices[n * 4 + 47] = 1.0f;
		}
	}

	for(int i = 0; i < nVertices / 8 * 4; i += 4)
	{
		vertices[nVertices / 8 * 4 + i] = -vertices[i];
		vertices[nVertices / 8 * 4 + i + 1] = vertices[i + 1];
		vertices[nVertices / 8 * 4 + i + 2] = vertices[i + 2];
		vertices[nVertices / 8 * 4 + i + 3] = 1;

		vertices[nVertices / 8 * 8 + i] = -vertices[i];
		vertices[nVertices / 8 * 8 + i + 1] = vertices[i + 1];
		vertices[nVertices / 8 * 8 + i + 2] = -vertices[i + 2];
		vertices[nVertices / 8 * 8 + i + 3] = 1;

		vertices[nVertices / 8 * 12 + i] = vertices[i];
		vertices[nVertices / 8 * 12 + i + 1] = vertices[i + 1];
		vertices[nVertices / 8 * 12 + i + 2] = -vertices[i + 2];
		vertices[nVertices / 8 * 12 + i + 3] = 1;

		vertices[nVertices / 8 * 16 + i] = vertices[i];
		vertices[nVertices / 8 * 16 + i + 1] = -vertices[i + 1];
		vertices[nVertices / 8 * 16 + i + 2] = vertices[i + 2];
		vertices[nVertices / 8 * 16 + i + 3] = 1;

		vertices[nVertices / 8 * 20 + i] = -vertices[i];
		vertices[nVertices / 8 * 20 + i + 1] = -vertices[i + 1];
		vertices[nVertices / 8 * 20 + i + 2] = vertices[i + 2];
		vertices[nVertices / 8 * 20 + i + 3] = 1;

		vertices[nVertices / 8 * 24 + i] = -vertices[i];
		vertices[nVertices / 8 * 24 + i + 1] = -vertices[i + 1];
		vertices[nVertices / 8 * 24 + i + 2] = -vertices[i + 2];
		vertices[nVertices / 8 * 24 + i + 3] = 1;

		vertices[nVertices / 8 * 28 + i] = vertices[i];
		vertices[nVertices / 8 * 28 + i + 1] = -vertices[i + 1];
		vertices[nVertices / 8 * 28 + i + 2] = -vertices[i + 2];
		vertices[nVertices / 8 * 28 + i + 3] = 1;
	}
}

void Sphere::initNormals()
{
	for(int i = 0; i < nVertices * 4; i += 4)
	{
		normals[i] = vertices[i];
		normals[i + 1] = vertices[i + 1];
		normals[i + 2] = vertices[i + 2];
		normals[i + 3] = 0.0f;
	}
}

void Sphere::initIndices()
{
	for(int i = 0; i < nVertices; ++i)
	{
		indices[i] = i;
	}
}

double Sphere::RayIntersect(vec3 const& P0, vec3 const& V0)
{
	vec3 V = normalize(V0);
	double b = 2.0 * dot(V, P0);
	double c = dot(P0, P0) - 1.0;
	double t0, t1;
	double discriminant = b * b - 4.0 * c;
	if(discriminant < 0)
		return -1;
	else
	{
		t0 = (-b - sqrt(discriminant)) / 2.0;
		if(t0 > 0)
			return t0;
		else
		{
			t1 = (-b + sqrt(discriminant)) / 2.0;
			if(t1 > 0)
				return t1;
			else
				return -1;
		}
	}
	return -1;
}

Cylinder::Cylinder()
{
	nEdges = 30;
	nVertices = 62;
	nIndices = 360;
	vertices = new float[nVertices * 4];
	normals = new float[nVertices * 4];
	indices = new unsigned short[nEdges * 12];
	colors = new float[nVertices * 3];

	initVertices();
	initNormals();
	initIndices();
	setColor(1.0f, 1.0f, 1.0f);
}

Cylinder::~Cylinder()
{
	delete [] vertices;
	delete [] normals;
	delete [] indices;
	delete [] colors;
}

void Cylinder::initVertices()
{
	vertices[0] = 0.0f; vertices[1] = 0.5f; vertices[2] = 0.0f; vertices[3] = 1.0f;
	vertices[4] = 0.0f; vertices[5] = -0.5f; vertices[6] = 0.0f; vertices[7] = 1.0f;

	vec3 point;
	for(int i = 0; i < nEdges; ++i)
	{
		point.x = cos((float)(i + 1) * (360.0f / (float)nEdges) * PI / 180.0f);
		point.z = sin((float)(i + 1) * (360.0f / (float)nEdges) * PI / 180.0f);

		vertices[8 + i * 4] = point.x; vertices[8 + i * 4 + 1] = 0.5f; vertices[8 + i * 4 + 2] = point.z; vertices[8 + i * 4 + 3] = 1.0f;
		vertices[(nEdges + 2) * 4 + i * 4] = point.x; vertices[(nEdges + 2) * 4 + i * 4 + 1] = -0.5f; vertices[(nEdges + 2) * 4 + i * 4 + 2] = point.z; vertices[(nEdges + 2) * 4 + i * 4 + 3] = 1.0f;
	}
}

void Cylinder::initNormals()
{
	normals[0] = 0.0f; normals[1] = 1.0f; normals[2] = 0.0f; normals[3] = 0.0f;
	normals[4] = 0.0f; normals[5] = -1.0f; normals[6] = 0.0f; normals[7] = 0.0f;

	vec3 norm;
	for(int i = 0; i < nEdges; ++i)
	{
		norm.x = cos((float)(i + 1) * (360.0f / (float)nEdges) * PI / 180.0f) / 1.414f;
		norm.z = sin((float)(i + 1) * (360.0f / (float)nEdges) * PI / 180.0f) / 1.414f;

		normals[8 + i * 4] = norm.x; normals[8 + i * 4 + 1] = 0.707f; normals[8 + i * 4 + 2] = norm.z; normals[8 + i * 4 + 3] = 0.0f;
		normals[(nEdges + 2) * 4 + i * 4] = norm.x; normals[(nEdges + 2) * 4 + i * 4 + 1] = -0.707f; normals[(nEdges + 2) * 4 + i * 4 + 2] = norm.z; normals[(nEdges + 2) * 4 + i * 4 + 3] = 0.0f;
	}
}

void Cylinder::initIndices()
{
	for(int i = 0; i < nEdges; ++i)
	{
		indices[i * 3] = 0; indices[i * 3 + 1] = i + 2; indices[i * 3 + 2] = i + 3;
		indices[nEdges * 3 + i * 3] = 1; indices[nEdges * 3 + i * 3 + 1] = nEdges + i + 2; indices[nEdges * 3 + i * 3 + 2] = nEdges + i + 3;
		indices[nEdges * 6 + i * 6] = i + 2; indices[nEdges * 6 + i * 6 + 1] = nEdges + i + 2; indices[nEdges * 6 + i * 6 + 2] = nEdges + i + 3;
		indices[nEdges * 6 + i * 6 + 3] = nEdges + i + 3; indices[nEdges * 6 + i * 6 + 4] = i + 2; indices[nEdges * 6 + i * 6 + 5] = i + 3;
	}

	indices[nEdges * 6 + (nEdges - 1) * 6 + 2] = nEdges + 2;
	indices[nEdges * 6 + (nEdges - 1) * 6 + 3] = nEdges + 2;
	indices[nEdges * 6 + (nEdges - 1) * 6 + 5] = 2;
	indices[nEdges * 3 + (nEdges - 1) * 3 + 2] = nEdges + 2;
	indices[(nEdges - 1) * 3 + 2] = 2;
}

double Cylinder::RayIntersect(vec3 const& P0, vec3 const& V0)
{
	vec3 V = normalize(V0);
	double t = -1;
	if(P0.y > 0.5)
	{
		// check intersection with the top endcap
		vec3 normal = vec3(0.0, 1.0, 0.0);
		double denominator = dot(normal, V);
		if((denominator < EPSILON) && (denominator > -EPSILON))
			t = -1;
		t = dot(normal, vec3(0.0, 0.5, 0.0) - P0) / denominator;
		if(t < 0)
			t = -1;
		else
		{
			vec3 P = P0 + vec3(t * V.x, t * V.y, t * V.z);
			vec3 PO = P - vec3(0.0, 0.5, 0.0);
			if(PO.x * PO.x + PO.z * PO.z < 0.25)
				return t;
		}
	}
	if(P0.y < -0.5)
	{
		// check intersection with the bottom endcap
		vec3 normal = vec3(0.0, -1.0, 0.0);
		double denominator = dot(normal, V);
		if((denominator < EPSILON) && (denominator > -EPSILON))
			t = -1;
		double t = dot(normal, vec3(0.0, -0.5, 0.0) - P0) / denominator;
		if(t < 0)
			t = -1;
		else
		{
			vec3 P = P0 + vec3(t * V.x, t * V.y, t * V.z);
			vec3 PO = P - vec3(0.0, -0.5, 0.0);
			if(PO.x * PO.x + PO.z * PO.z < 0.25)
				return t;
		}
	}
	// check intersection with cylinder body
	double a = V.x * V.x + V.z * V.z;
	double b = 2.0 * (P0.x * V.x + P0.z * V.z);
	double c = P0.x * P0.x + P0.z * P0.z - 0.25;
	double discriminant = b * b - 4 * a * c;
	if((a < EPSILON) && (a > -EPSILON))
		t = -c / b;
	else
	{
		if(discriminant < 0)
			return -1;
		else
		{
			t = (-b - sqrt(discriminant)) / (2.0 * a);
			if(t > 0)
				;
			else
			{
				t = (-b + sqrt(discriminant)) / (2.0 * a);
				if(t > 0)
					;
				else
					return -1;
			}
		}
	}
	vec3 P = P0 + vec3(t * V.x, t * V.y, t * V.z);
	if((P.y > 0.5) || (P.y < -0.5))
		return -1;
	return t;
}