// codebit written by Tiantian Liu @ University of Pennsylvania, 2012
// cpp libraries
#include <time.h>
#include <string>

#include "Define.h"
#include "DisplayClass.h"
#include "SceneGraph.h"
#include "RayTrace.h"

// glut libraries: make sure glut is included AFTER your glew.h
#include "../freeglut/include/GL/glut.h"

using namespace glm;
using namespace std;

// main display class;
DisplayClass *displayClass;

// scene graph instance
SceneGraph* sceneGraph_global;

// ray trace file
string rayTraceFile;

// ray trace
RayTrace* rayTrace;

//Animation/transformation stuff
clock_t old;

//standard glut-based program functions
void resize(int, int);
void display(void);
void keypress(unsigned char, int, int);
void cleanup(void);

int main(int argc, char** argv) {

	/*char fileName[64];
	for(int i = 1; i < argc; ++i)
	{
		if(i == 1)
			strcpy_s(fileName, argv[1]);
		else
			strcat_s(fileName, argv[i]);
		if(i != argc - 1)
			strcat_s(fileName, " ");
	}
	cout<<fileName<<endl;*/
	sceneGraph_global = new SceneGraph();
	sceneGraph_global->ParseSceneFile(argv[1]);
	rayTraceFile = argv[2];

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
		sceneGraph_global->previousNode = sceneGraph_global->currentNode;
		sceneGraph_global->previousNode->color = sceneGraph_global->previousNode->tColor;
		sceneGraph_global->currentNode = sceneGraph_global->nextNode(sceneGraph_global->currentNode);
		sceneGraph_global->currentNode->tColor = sceneGraph_global->currentNode->color;
		sceneGraph_global->currentNode->color = vec3(1.0, 1.0, 1.0);
		break;
	case 'a': // translate the selected node (and by nature of the scene graph, its children) 
		      // along the negative x-axis half a unit
		sceneGraph_global->currentNode->Update(1, -0.5);
		break;
	case 'd': // translate the selected node along the positive x-axis by half a unit
		sceneGraph_global->currentNode->Update(1, 0.5);
		break;
	case 'w': // translate the selected node along the positive z-axis half a unit, and I've changed it to negative z-axis
		sceneGraph_global->currentNode->Update(2, -0.5);
		break;
	case 's': // translate the selected node along the negative z-axis half a unit, and I've changed it to positive z-axis
		sceneGraph_global->currentNode->Update(2, 0.5);
		break;
	case 'x': // increase scale in the x-axis by 0.5
		sceneGraph_global->currentNode->xScale += 0.5f;
		break;
	case 'X': // decrease scale in the x-axis by 0.5
		sceneGraph_global->currentNode->xScale -= 0.5f;
		break;
	case 'y': // increase scale in the y-axis by 0.5
		sceneGraph_global->currentNode->yScale += 0.5f;
		sceneGraph_global->currentNode->Update(4, 0);
		break;
	case 'Y': // decrease scale in the y-axis by 0.5
		sceneGraph_global->currentNode->yScale -= 0.5f;
		sceneGraph_global->currentNode->Update(4, 0);
		break;
	case 'z': // increase scale in the z-axis by 0.5
		sceneGraph_global->currentNode->zScale += 0.5f;
		break;
	case 'Z': // decrease scale in the z-axis by 0.5
		sceneGraph_global->currentNode->zScale -= 0.5f;
		break;
	case 'r': // increase the rotation of the selected node by 10 degrees
		sceneGraph_global->currentNode->Update(3, 10);
		break;
	case 'R': // decrease the rotation of the selected node by 10 degrees
		sceneGraph_global->currentNode->Update(3, -10);
		break;
	case 'e': // remove this node and all its children from the graph,
		      // then select the next node in the preorder traversal
		if(!sceneGraph_global->currentNode->IsRootNode())
		{
			sceneGraph_global->deleteNode(sceneGraph_global->currentNode);
			// this is not preorder traversal
			sceneGraph_global->currentNode = sceneGraph_global->getRootNode();
		}
		break;
	case 'p':
		rayTrace = new RayTrace(rayTraceFile);
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
	sceneGraph_global->draw(displayClass);

	glutSwapBuffers();
	old = newTime;
}

void resize(int width, int height) {
	//set the viewport, more boilerplate
	glViewport(0, 0, width, height);

	displayClass->resizeWindow(width, height);

	glutPostRedisplay();
}