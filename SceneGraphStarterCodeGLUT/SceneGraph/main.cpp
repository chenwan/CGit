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
SceneGraph* sceneGraph;

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
	sceneGraph = new SceneGraph();
	sceneGraph->ParseSceneFile(argv[1]);

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
		sceneGraph->currentNode->color = vec3(1.0, 1.0, 1.0);
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
	case 'p':
		//RayTrace* rayTrace = new RayTrace();
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