#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "Node.h"
#include<string>

using namespace std;

// Scene Graph
class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	void addNode(Node* node);

	Node* nextNodeTraverse(Node* node);
	Node* nextNode(Node* node);

	void updateNode(Node* node);

	void deleteNodeTraverse(Node* node);
	void deleteNode(Node* node);

	Node* getRootNode();

	void draw(DisplayClass* displayClass);
	void ParseSceneFile(string sceneFile);

public:
	std::vector<Node*> m_Nodes;
	Node* currentNode;
	Node* previousNode;
	Geometry* geometry;
	int numItems;
};

#endif