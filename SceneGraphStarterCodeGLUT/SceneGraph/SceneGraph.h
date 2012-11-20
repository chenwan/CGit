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

	double RayIntersect(vec3 const& P0, vec3 const& V0, Geometry& j);

public:
	std::vector<Node*> m_Nodes;
	Node* currentNode;
	Node* previousNode;
	Geometry* geometry;
	int numItems;
};

#endif