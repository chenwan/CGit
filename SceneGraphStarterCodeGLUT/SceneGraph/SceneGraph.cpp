#include "SceneGraph.h"

SceneGraph::SceneGraph()
{
	geometry = new Geometry;
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

void SceneGraph::ParseSceneFile(string sceneFile)
{
	ifstream readFile;
	ifstream readDatFile;
	readFile.open(sceneFile);
	if(!readFile.is_open())
	{
		cerr<<"Cannot open configuration file."<<endl;
		readFile.close();
	}

	cout<<"Start parsing configuration file."<<endl;

	int xSize, zSize;
	readFile>>xSize>>zSize>>numItems;

	Floor* floor = new Floor(xSize, zSize);
	floor->red = 0.0f;
	floor->green = 1.0f;
	floor->blue = 1.0f;
	Node* root = new Node(floor);
	this->addNode(root);
	this->currentNode = root;

	string geometryType;
	string datFile;
	string meshType;
	float extrusion_length;
	int surfrev_numSlices;
	int numPoints;
	vec3* extrusion_basePoints;
	vec3* surfrev_polylinePoints;
	int material;

	for(int i = 0; i < numItems; ++i)
	{
		readFile>>geometryType;
		if(!strcmp(geometryType.c_str(), "chair"))
		{
			geometry = new Chair();
			//readFile>>geometry->red>>geometry->green>>geometry->blue;
		}
		else if(!strcmp(geometryType.c_str(), "table"))
		{
			geometry = new Table();
			//readFile>>geometry->red>>geometry->green>>geometry->blue;
		}
		else if(!strcmp(geometryType.c_str(), "cabinet"))
		{
			geometry = new Cabinet();
			//readFile>>geometry->red>>geometry->green>>geometry->blue;
		}
		else if(!strcmp(geometryType.c_str(), "lamp"))
		{
			geometry = new Lamp();
			//readFile>>geometry->red>>geometry->green>>geometry->blue;
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
		}
		else
		{
			cerr<<"Cannot recognize the geometry type!"<<endl;
			exit(1);
		}
		readFile>>material;
		readFile>>geometry->xIndex>>geometry->zIndex;
		readFile>>geometry->rotation;
		readFile>>geometry->xScale>>geometry->yScale>>geometry->zScale;
		Node* geometryNode = new Node(this->getRootNode(), geometry);
		this->addNode(geometryNode);
	}

	cout<<"Finish parsing configuration file."<<endl;
	readFile.clear();
	readFile.close();
}

// need to check the smallest t for mesh
double SceneGraph::RayIntersect(vec3 const& P0, vec3 const& V0)
{
	double t = -1;
	double t_min = -1;
	for(unsigned int i = 0; i < m_Nodes.size(); ++i)
	{
		t = m_Nodes[i]->m_Geometry->RayIntersect(P0, V0);
		if(t > 0)
		{
			t_min = t;
			for(unsigned int j = i + 1; j < m_Nodes.size(); ++j)
			{
				t = m_Nodes[i]->m_Geometry->RayIntersect(P0, V0);
				if((t > 0) && (t < t_min))
					t_min = t;
			}
		}
		return t_min;
	}
	return -1;
}