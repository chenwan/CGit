#include "RayTrace.h"

RayTrace::RayTrace()
{
	camera = new Camera();
	image = new Image();
	light = new Light();
}

RayTrace::~RayTrace()
{
	if(camera)
	{
		delete camera;
		camera = 0;
	}
	if(image)
	{
		delete image;
		image = 0;
	}
	if(light)
	{
		delete light;
		light = 0;
	}
}

void RayTrace::ParseRayTraceFile(string rayTraceFile)
{
	ifstream readFile;
	readFile.open(rayTraceFile);
	if(!readFile.is_open())
	{
		cerr<<"Cannot open configuration file."<<endl;
		readFile.close();
	}

	cout<<"Parsing ray trace configuration file..."<<endl;

	string temp;
	Camera* camera;

	readFile>>temp>>image->file;
	readFile>>temp>>image->reso[0]>>image->reso[1];
	readFile>>temp>>camera->eye.x>>camera->eye.y>>camera->eye.z;
	readFile>>temp>>camera->vdir.x>>camera->vdir.y>>camera->vdir.z;
	readFile>>temp>>camera->up.x>>camera->up.y>>camera->up.z;
	readFile>>temp>>camera->fovy;
	readFile>>temp>>light->position.x>>light->position.y>>light->position.z;
	readFile>>temp>>light->color.r>>light->color.g>>light->color.b;
	readFile>>temp>>light->acol.r>>light->acol.g>>light->acol.b;
	// read materials left

	cout<<"FILE "<<image->file<<endl;
	cout<<"RESO "<<image->reso[0]<<" "<<image->reso[1]<<endl;
	cout<<"EYEP "<<camera->eye.x<<" "<<camera->eye.y<<" "<<camera->eye.z<<endl;
	cout<<"VDIR "<<camera->vdir.x<<" "<<camera->vdir.y<<" "<<camera->vdir.z<<endl;
	cout<<"UVEC "<<camera->up.x<<" "<<camera->up.y<<" "<<camera->up.z<<endl;
	cout<<"FOVY "<<camera->fovy<<endl;
	cout<<"LPOS "<<light->position.x<<" "<<light->position.y<<" "<<light->position.z<<endl;
	cout<<"LCOL "<<light->color.r<<" "<<light->color.g<<" "<<light->color.b<<endl;
	cout<<"ACOL "<<light->acol.r<<" "<<light->acol.g<<" "<<light->acol.b<<endl;
}