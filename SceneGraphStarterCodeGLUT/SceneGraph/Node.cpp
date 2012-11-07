#include "Node.h"

Node::Node()
{
	m_Parent = NULL;
	m_Geometry = NULL;
} // Constructor

Node::Node(Geometry* Geometry)
	:m_Geometry(Geometry)
{
	m_Parent = NULL;
	InitNode(Geometry);
} // Constructor

Node::Node(Node* Parent, Geometry* Geometry)
	:m_Parent(Parent), m_Geometry(Geometry)
{
	if(Parent != NULL)
		Parent->AddChildNode(this);
	InitNode(Geometry);
} // Constructor

Node::~Node(void)
{
	m_Parent = NULL;
	m_Children.clear();
} // Destructor

void Node::Update(int type, float n)
{
	switch(type)
	{
	case 1: // xTranslation
		xTranslation += n;
		break;
	case 2: // zTranslation
		zTranslation += n;
		break;
	case 3: // yRotation
		yRotation += n;
		break;
	case 4: // yTranslation
		yTranslation = m_Parent->yTranslation + m_Parent->m_Geometry->height * m_Parent->yScale;
		break;
	}
	if(!m_Children.empty())
	{
		for(unsigned int i = 0; i < m_Children.size(); ++i)
		{
			if(NULL != m_Children[i])
			{
				m_Children[i]->Update(type, n);
			}
		}
	}
} // Update()

void Node::InitNode(Geometry* Geometry)
{
	xTranslation = (float)Geometry->xIndex;
	yTranslation = 0.0f;
	zTranslation = (float)Geometry->zIndex;
	yRotation = Geometry->rotation;
	xScale = Geometry->xScale;
	yScale = Geometry->yScale;
	zScale = Geometry->zScale;

	m_Transformation = mat4(1.0f);
	//m_Transformation = translate(m_Transformation, vec3(xTranslation, yTranslation, zTranslation));
	//m_Transformation = rotate(m_Transformation, yRotation, vec3(0, 1, 0));
	//m_Transformation = scale(m_Transformation, vec3(xScale, yScale, zScale));

	tColor.r = color.r = Geometry->red;
	tColor.g = color.g = Geometry->green;
	tColor.b = color.b = Geometry->blue;
} // InitNode()

Node* Node::GetParentNode(void) const
{
	return(m_Parent);
} // GetParentNode()

void Node::SetParentNode(Node* NewParent)
{
	if(NULL != m_Parent)
	{
		m_Parent->RemoveChildNode(this);
	}
	m_Parent = NewParent;
} // SetParentNode()

void Node::AddChildNode(Node* ChildNode)
{
	if(NULL != ChildNode)
	{
		if(NULL != ChildNode->GetParentNode())
		{
			ChildNode->SetParentNode(this);
		}
		m_Children.push_back(ChildNode);
	}
} // AddChildNode()

void Node::RemoveChildNode(Node* ChildNode)
{
	if(NULL != ChildNode && !m_Children.empty())
	{
		for(unsigned int i = 0; i < m_Children.size(); ++i)
		{
			if(m_Children[i] == ChildNode)
			{
				m_Children.erase(m_Children.begin() + i);
				//break; // break the for loop
			}
		}
	}
} // RemoveChildNode()

const unsigned int Node::CountChildNodes(const bool &RecursiveCount) const
{
	if(!RecursiveCount)
	{
		return(m_Children.size());
	}
	else
	{
		unsigned int Retval = m_Children.size();
		for(unsigned int i = 0; i < m_Children.size(); ++i)
		{
			Retval += m_Children[i]->CountChildNodes(true);
		}
		return(Retval);
	}
} // CountChildNodes()

const bool Node::IsRootNode(void) const
{
	return (m_Parent == NULL);
}

const bool Node::IsLeafNode(void) const
{
	return (m_Children.size() == 0);
}