#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "Node.h"

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

public:
	std::vector<Node*> m_Nodes;
	Node* currentNode;
	Node* previousNode;
};

#endif