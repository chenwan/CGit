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
	readFile>>temp>>light->ambientColor.r>>light->ambientColor.g>>light->ambientColor.b;
	// read materials left

	cout<<"FILE "<<image->file<<endl;
	cout<<"RESO "<<image->reso[0]<<" "<<image->reso[1]<<endl;
	cout<<"EYEP "<<camera->eye.x<<" "<<camera->eye.y<<" "<<camera->eye.z<<endl;
	cout<<"VDIR "<<camera->vdir.x<<" "<<camera->vdir.y<<" "<<camera->vdir.z<<endl;
	cout<<"UVEC "<<camera->up.x<<" "<<camera->up.y<<" "<<camera->up.z<<endl;
	cout<<"FOVY "<<camera->fovy<<endl;
	cout<<"LPOS "<<light->position.x<<" "<<light->position.y<<" "<<light->position.z<<endl;
	cout<<"LCOL "<<light->color.r<<" "<<light->color.g<<" "<<light->color.b<<endl;
	cout<<"ACOL "<<light->ambientColor.r<<" "<<light->ambientColor.g<<" "<<light->ambientColor.b<<endl;
}

void RayTrace::Main()
{
	float H_length = image->reso[0] / 2.0f;
	float V_length = image->reso[1] / 2.0f;
	float C_length = V_length / tan(camera->fovy / 180.0f * PI);
	float tangent_theta = H_length / C_length;
	vec3 C = camera->vdir * C_length;
	vec3 M = camera->eye + C;
	vec3 A = cross(C, camera->up);
	vec3 B = cross(A, C);
	vec3 H = (A * C_length * tangent_theta) * (1.0f / length(A));
	vec3 V = (B * C_length * tan(camera->fovy / 180.0f * 3.14159f)) * (1.0f / length(B));
	vec3 origin = vec3(M.x - H_length, M.y - V_length, M.z);

	cout<<"M: ("<<M.x<<" "<<M.y<<" "<<M.z<<")"<<endl;
	cout<<"H: ("<<H.x<<" "<<H.y<<" "<<H.z<<")"<<endl;
	cout<<"V: ("<<V.x<<" "<<V.y<<" "<<V.z<<")"<<endl;
	cout<<"Origin: ("<<origin.x<<" "<<origin.y<<" "<<origin.z<<")"<<endl;

	BMP output;
	output.SetSize(image->reso[0], image->reso[1]);
	output.SetBitDepth(24);

	vec3 pixel = origin + vec3(0.5f, 0.5f, 0);
	for(int y = 0; y < image->reso[1]; ++y)
	{
		pixel.x = origin.x + 0.5f;
		for(int x = 0; x < image->reso[0]; ++x)
		{
			P = M + float(2.0 * x / float(image->reso[0] - 1) - 1) * H + float(2.0 * y / float(image->reso[1] - 1) - 1) * V;
			D = normalize(P - camera->eye);
			TraceRay(camera->eye, D, 0, color);
			output(x, image->reso[1] - 1 - y)->Red = color.r;
			output(x, image->reso[1] - 1 - y)->Green = color.g;
			output(x, image->reso[1] - 1 - y)->Blue = color.b;
		}
	}
}

// I don't know what this is, just using it temporarily
extern SceneGraph* sceneGraph;
void RayTrace::TraceRay(vec3 start, vec3 direction, int depth, vec3 color)
{
	vec3 ReflectedDirection;
	vec3 RefractedDirection;
	vec3 TransmittedDirection;
	vec3 spec;
	vec3 ReflectedColor;
	vec3 refr;
	vec3 RefractedColor;
	vec3 trans;
	vec3 TransmittedColor;

	if(depth > MAXDEPTH)
	{
		color = image->backgroundColor;
		return;
	}
	// intersect ray with all objects and find intersection point(if any)
	// on object j that is closest to start of ray; else return nil
	float t = sceneGraph->RayIntersect(start, direction);
	if(t < 0)
	{
		// if ray is parallel to any of the *light directions*?
		// color = light_color
		// else
		// color = background_color
	}
	else
	{
		vec3 IntersectionPoint = start + t * direction;
	}
}