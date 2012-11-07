#ifndef NODE_H
#define NODE_H

#include "Furniture.h"
#include "Mesh.h"

class Node
{
public:
	Node();
	Node(Geometry* Geometry);
	Node(Node* Parent, Geometry* Geometry);
	virtual ~Node(void);

	void InitNode(Geometry* Geometry);
	virtual void Update(int type, float n);

	Node* GetParentNode(void) const;
	void SetParentNode(Node* NewParent);

	void AddChildNode(Node* ChildNode);
	void RemoveChildNode(Node* ChildNode);

	const unsigned int CountChildNodes(const bool& RecursiveCount = false) const;

	const bool IsRootNode(void) const;
	const bool IsLeafNode(void) const;

public:
	Node* m_Parent;
	std::vector<Node*> m_Children;
	mat4 m_Transformation;
	Geometry* m_Geometry;

	float xTranslation;
	float yTranslation;
	float zTranslation;
	float yRotation;
	float xScale;
	float yScale;
	float zScale;

	vec3 color;
	vec3 tColor;
};

#endif