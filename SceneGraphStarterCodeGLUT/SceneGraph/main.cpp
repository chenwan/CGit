// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
// cpp libraries
#include <time.h>
#include <string>

#include "Define.h"
#include "DisplayClass.h"
#include "SceneGraph.h"

// glut libraries: make sure glut is included AFTER your glew.h
#include "../freeglut/include/GL/glut.h"

using namespace glm;
using namespace std;

// main display class;
DisplayClass *displayClass;

// scene graph instance
SceneGraph* sceneGraph;
// geometry instance
Geometry* geometry;

//Animation/transformation stuff
clock_t old;

//standard glut-based program functions
void resize(int, int);
void display(void);
void keypress(unsigned char, int, int);
void cleanup(void);

void parseConfigFile(string fileName)
{
	ifstream readFile;
	ifstream readDatFile;
	readFile.open(fileName);
	if(!readFile.is_open())
	{
		cerr<<"Cannot open configuration file."<<endl;
		readFile.close();
	}

	cout<<"Start parsing configuration file."<<endl;

	sceneGraph = new SceneGraph();

	int xSize, zSize, numItems;
	readFile>>xSize>>zSize>>numItems;

	Floor* floor = new Floor(xSize, zSize);
	floor->red = 0.8f;
	floor->green = 0.8f;
	floor->blue = 0.5f;
	Node* root = new Node(floor);
	sceneGraph->addNode(root);
	sceneGraph->currentNode = root;

	string geometryType;
	string datFile;
	string textureFile;
	string meshType;
	float extrusion_length;
	int surfrev_numSlices;
	int numPoints;
	vec3* extrusion_basePoints;
	vec3* surfrev_polylinePoints;

	for(int i = 0; i < numItems; ++i)
	{
		readFile>>geometryType;
		if(!strcmp(geometryType.c_str(), "chair"))
		{
			geometry = new Chair();
			readFile>>geometry->red>>geometry->green>>geometry->blue;
		}
		else if(!strcmp(geometryType.c_str(), "table"))
		{
			geometry = new Table();
			readFile>>geometry->red>>geometry->green>>geometry->blue;
		}
		else if(!strcmp(geometryType.c_str(), "cabinet"))
		{
			geometry = new Cabinet();
			readFile>>geometry->red>>geometry->green>>geometry->blue;
		}
		else if(!strcmp(geometryType.c_str(), "lamp"))
		{
			geometry = new Lamp();
			readFile>>geometry->red>>geometry->green>>geometry->blue;
		}
		else if (!strcmp(geometryType.c_str(), "mesh"))
		{
			readFile>>datFile;
			readDatFile.open(datFile);
			if(!readDatFile.is_open())
			{
				cerr<<"Cannot open "<<datFile<<"."<<endl;
				readDatFile.close();
			}
			readDatFile>>meshType;
			if(!strcmp(meshType.c_str(), "extrusion"))
			{
				readDatFile>>extrusion_length;
				readDatFile>>numPoints;
				extrusion_basePoints = new vec3[numPoints];
				for(unsigned int i = 0; i < numPoints; ++i)
				{
					readDatFile>>extrusion_basePoints[i].x>>extrusion_basePoints[i].z;
					extrusion_basePoints[i].y = 0.0f;
				}
				geometry = new Extrusion(extrusion_length, numPoints, extrusion_basePoints);
				readDatFile.clear();
				readDatFile.close();
			}
			else if(!strcmp(meshType.c_str(), "surfrev"))
			{
				readDatFile>>surfrev_numSlices;
				readDatFile>>numPoints;
				if(numPoints < 3)
				{
					cerr<<"Number of radical sliced in the revolved mesh should be at least 3."<<endl;
					exit(1);
				}
				surfrev_polylinePoints = new vec3[numPoints];
				for(unsigned int i = 0; i < numPoints; ++i)
				{
					readDatFile>>surfrev_polylinePoints[i].x>>surfrev_polylinePoints[i].y;
					surfrev_polylinePoints[i].z = 0.0f;
					if(surfrev_polylinePoints[i].x < 0)
					{
						cerr<<"x-coordinate of the points of the polyline cannot be negative."<<endl;
						exit(1);
					}
				}
				geometry = new Surfrev(surfrev_numSlices, numPoints, surfrev_polylinePoints);
				readDatFile.clear();
				readDatFile.close();
			}
			readFile>>textureFile;
		}
		else
		{
			cerr<<"Cannot recognize the geometry type!"<<endl;
			exit(1);
		}
		readFile>>geometry->xIndex>>geometry->zIndex;
		readFile>>geometry->rotation;
		readFile>>geometry->xScale>>geometry->yScale>>geometry->zScale;
		Node* geometryNode = new Node(sceneGraph->getRootNode(), geometry);
		sceneGraph->addNode(geometryNode);
	}

	cout<<"Finish parsing configuration file."<<endl;
	readFile.clear();
	readFile.close();
}

int main(int argc, char** argv) {

	// File Input
	char fileName[64];
	for(int i = 1; i < argc; ++i)
	{
		if(i == 1)
			strcpy_s(fileName, argv[1]);
		else
			strcat_s(fileName, argv[i]);
		if(i != argc - 1)
			strcat_s(fileName, " ");
	}
	cout<<fileName<<endl;
	parseConfigFile(fileName);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glutCreateWindow("Scene Graph Starter Code GLUT");

	// initialize the display class.
	displayClass = new DisplayClass();
	// start timer
	old = clock();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keypress);
	glutIdleFunc(display);

	glutMainLoop();
	return 0;
}

void keypress(unsigned char key, int x, int y) {
	switch(key) {
	case 'q': 
	case 'Q':
		exit(0);
		break;
	case 'n': // select the next node in the preorder traversal
		sceneGraph->previousNode = sceneGraph->currentNode;
		sceneGraph->previousNode->color = sceneGraph->previousNode->tColor;
		sceneGraph->currentNode = sceneGraph->nextNode(sceneGraph->currentNode);
		sceneGraph->currentNode->tColor = sceneGraph->currentNode->color;
		sceneGraph->currentNode->color = vec3(0.8, 0.8, 0.8);
		break;
	case 'a': // translate the selected node (and by nature of the scene graph, its children) 
		      // along the negative x-axis half a unit
		sceneGraph->currentNode->Update(1, -0.5);
		break;
	case 'd': // translate the selected node along the positive x-axis by half a unit
		sceneGraph->currentNode->Update(1, 0.5);
		break;
	case 'w': // translate the selected node along the positive z-axis half a unit, and I've changed it to negative z-axis
		sceneGraph->currentNode->Update(2, -0.5);
		break;
	case 's': // translate the selected node along the negative z-axis half a unit, and I've changed it to positive z-axis
		sceneGraph->currentNode->Update(2, 0.5);
		break;
	case 'x': // increase scale in the x-axis by 0.5
		sceneGraph->currentNode->xScale += 0.5f;
		break;
	case 'X': // decrease scale in the x-axis by 0.5
		sceneGraph->currentNode->xScale -= 0.5f;
		break;
	case 'y': // increase scale in the y-axis by 0.5
		sceneGraph->currentNode->yScale += 0.5f;
		sceneGraph->currentNode->Update(4, 0);
		break;
	case 'Y': // decrease scale in the y-axis by 0.5
		sceneGraph->currentNode->yScale -= 0.5f;
		sceneGraph->currentNode->Update(4, 0);
		break;
	case 'z': // increase scale in the z-axis by 0.5
		sceneGraph->currentNode->zScale += 0.5f;
		break;
	case 'Z': // decrease scale in the z-axis by 0.5
		sceneGraph->currentNode->zScale -= 0.5f;
		break;
	case 'r': // increase the rotation of the selected node by 10 degrees
		sceneGraph->currentNode->Update(3, 10);
		break;
	case 'R': // decrease the rotation of the selected node by 10 degrees
		sceneGraph->currentNode->Update(3, -10);
		break;
	case 'e': // remove this node and all its children from the graph,
		      // then select the next node in the preorder traversal
		if(!sceneGraph->currentNode->IsRootNode())
		{
			sceneGraph->deleteNode(sceneGraph->currentNode);
			// this is not preorder traversal
			sceneGraph->currentNode = sceneGraph->getRootNode();
		}
		break;
	case 27: // ascii code of esc key
		if (displayClass)
			delete displayClass;
		exit(0);
		break;
	}

	glutPostRedisplay();
}

void display() {
	//Always and only do this at the start of a frame, it wipes the slate clean
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	clock_t newTime = clock();

	//part of the animation
	displayClass->rotation += 150 * (static_cast<float>(newTime - old) / static_cast<float>(CLOCKS_PER_SEC));

	//displayClass->redraw();
	sceneGraph->draw(displayClass);

	glutSwapBuffers();
	old = newTime;
}

void resize(int width, int height) {
	//set the viewport, more boilerplate
	glViewport(0, 0, width, height);

	displayClass->resizeWindow(width, height);

	glutPostRedisplay();
}