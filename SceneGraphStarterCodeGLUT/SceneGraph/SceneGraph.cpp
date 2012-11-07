#include "SceneGraph.h"

SceneGraph::SceneGraph()
{
}

SceneGraph::~SceneGraph()
{
	m_Nodes.clear();
}

void SceneGraph::addNode(Node* node)
{
	if(node != NULL)
	{
		updateNode(node);
		m_Nodes.push_back(node);
	}
}

Node* SceneGraph::nextNodeTraverse(Node* node)
{
	if(!node->IsRootNode())
	{
		for(unsigned int i = 0; i < node->GetParentNode()->CountChildNodes(0); ++i)
		{
			if(node == node->GetParentNode()->m_Children[i])
			{
				if(node->GetParentNode()->CountChildNodes(0) > (i + 1))
					return node->GetParentNode()->m_Children[i + 1];
				else
					return nextNodeTraverse(node->GetParentNode());
			}
		}
	}
	return m_Nodes[0];
}

Node* SceneGraph::nextNode(Node* node)
{
	if(!node->IsLeafNode())
		return node->m_Children[0];
	else
		return nextNodeTraverse(node);
}

void SceneGraph::updateNode(Node* node)
{
	if(!node->IsRootNode())
	{
		for(unsigned int i = 1; i < m_Nodes.size(); ++i)
		{
			if((m_Nodes[i]->xTranslation == node->xTranslation) && (m_Nodes[i]->zTranslation == node->zTranslation))
			{
				Node* newParent = m_Nodes[i];
				while(!newParent->IsLeafNode())
					newParent = newParent->m_Children[0];
				node->SetParentNode(newParent);
				newParent->AddChildNode(node);
				node->yTranslation = newParent->yTranslation + newParent->m_Geometry->height * newParent->yScale;
				//node->m_Transformation = translate(node->m_Transformation, vec3(0, node->yTranslation, 0));
			}
		}
	}
}

void SceneGraph::deleteNodeTraverse(Node* node)
{
	if(node != NULL)
	{
		if(!node->IsLeafNode())
		{
			for(unsigned int i = 0; i < node->CountChildNodes(0); ++i)
			{
				deleteNodeTraverse(node->m_Children[i]);
				node->RemoveChildNode(node->m_Children[i]);
			}
			for(unsigned int i = 0; i < m_Nodes.size(); ++i)
			{
				if(m_Nodes[i] == node)
					m_Nodes.erase(m_Nodes.begin() + i);
			}
		}
		for(unsigned int i = 0; i < m_Nodes.size(); ++i)
		{
			if(m_Nodes[i] == node)
				m_Nodes.erase(m_Nodes.begin() + i);
		}
	}
}

void SceneGraph::deleteNode(Node* node)
{
	if(!node->IsRootNode())
		node->GetParentNode()->RemoveChildNode(node);
	deleteNodeTraverse(node);
}

Node* SceneGraph::getRootNode()
{
	return m_Nodes[0];
}

void SceneGraph::draw(DisplayClass* displayClass)
{
	Node* node;
	for(unsigned int i = 0; i < m_Nodes.size(); ++i)
	{
		node = m_Nodes[i];
		node->m_Transformation = mat4(1.0f);
		node->m_Transformation = translate(node->m_Transformation, vec3(node->xTranslation, node->yTranslation, node->zTranslation));
		node->m_Transformation = rotate(node->m_Transformation, node->yRotation, vec3(0, 1, 0));
		node->m_Transformation = scale(node->m_Transformation, vec3(node->xScale, node->yScale, node->zScale));

		node->m_Geometry->setColor(node->color);

		node->m_Geometry->draw(node->m_Transformation, displayClass);
	}
}