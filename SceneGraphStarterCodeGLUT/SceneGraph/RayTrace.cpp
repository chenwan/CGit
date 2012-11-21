#include "RayTrace.h"

extern SceneGraph* sceneGraph_global;

RayTrace::RayTrace()
{
	camera = new Camera();
	image = new Image();
	light = new Light();
}

RayTrace::RayTrace(string rayTraceFile)
{
	camera = new Camera();
	image = new Image();
	light = new Light();
	ParseRayTraceFile(rayTraceFile);
	Main();
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
	materials.clear();
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

	cout<<"Start parsing ray trace configuration file."<<endl;

	string temp;
	readFile>>temp>>image->file;
	readFile>>temp>>image->reso[0]>>image->reso[1];
	readFile>>temp>>camera->eye.x>>camera->eye.y>>camera->eye.z;
	readFile>>temp>>camera->center.x>>camera->center.y>>camera->center.z;
	readFile>>temp>>camera->up.x>>camera->up.y>>camera->up.z;
	readFile>>temp>>camera->fovy;
	readFile>>temp>>light->position.x>>light->position.y>>light->position.z;
	readFile>>temp>>light->color.r>>light->color.g>>light->color.b;
	readFile>>temp>>light->ambientColor.r>>light->ambientColor.g>>light->ambientColor.b;
	// read materials left
	while(readFile>>temp)
	{
		Material* material = new Material();
		readFile>>material->color.r;
		readFile>>material->color.g;
		readFile>>material->color.b;
		readFile>>material->Kn;
		readFile>>material->reflectivity_parameter;
		readFile>>material->Kt;
		readFile>>material->Kr;
		readFile>>temp;
		materials.push_back(material);
	}

	/*cout<<"FILE "<<image->file<<endl;
	cout<<"RESO "<<image->reso[0]<<" "<<image->reso[1]<<endl;
	cout<<"EYEP "<<camera->eye.x<<" "<<camera->eye.y<<" "<<camera->eye.z<<endl;
	cout<<"VDIR "<<camera->center.x<<" "<<camera->center.y<<" "<<camera->center.z<<endl;
	cout<<"UVEC "<<camera->up.x<<" "<<camera->up.y<<" "<<camera->up.z<<endl;
	cout<<"FOVY "<<camera->fovy<<endl;
	cout<<"LPOS "<<light->position.x<<" "<<light->position.y<<" "<<light->position.z<<endl;
	cout<<"LCOL "<<light->color.r<<" "<<light->color.g<<" "<<light->color.b<<endl;
	cout<<"ACOL "<<light->ambientColor.r<<" "<<light->ambientColor.g<<" "<<light->ambientColor.b<<endl;
	for(unsigned int i = 0; i < materials.size(); ++i)
	{
		cout<<"MAT"<<i + 1<<" COLOR "<<materials[i]->color.r<<" "<<materials[i]->color.g<<" "<<materials[i]->color.b<<" Kn "<<materials[i]->Kn<<" Ks "<<materials[i]->Ks<<" Kt "<<materials[i]->Kt<<" Kr "<<materials[i]->Kr<<endl;
	}*/
	cout<<"Finish parsing ray trace configuration file."<<endl;
}

void RayTrace::Main()
{
	float H_length = image->reso[0] / 2.0f;
	float V_length = image->reso[1] / 2.0f;
	float C_length = V_length / tan(camera->fovy / 180.0f * PI);
	float tangent_theta = H_length / C_length;
	vec3 C = camera->center * C_length;
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

	// set ray tracing materials to scenegraph geometries
	Geometry* floor = sceneGraph_global->m_Nodes[0]->m_Geometry;
	floor->material->color = vec3(0.8f, 0.8f, 0.2f);
	for(unsigned int i = 1; i < sceneGraph_global->m_Nodes.size(); ++i)
	{
		sceneGraph_global->m_Nodes[i]->m_Geometry->material = materials[sceneGraph_global->m_Nodes[i]->m_Geometry->material_index - 1];
	}

	vec3 pixel = origin + vec3(0.5f, 0.5f, 0);
	for(int y = 0; y < image->reso[1]; ++y)
	{
		pixel.x = origin.x + 0.5f;
		for(int x = 0; x < image->reso[0]; ++x)
		{
			P = M + float(2.0 * x / float(image->reso[0] - 1) - 1) * H + float(2.0 * y / float(image->reso[1] - 1) - 1) * V;
			D = normalize(P - camera->eye);
			TraceRay(camera->eye, D, 0, color);
			output(x, image->reso[1] - 1 - y)->Red = color.r * 255.0f;
			output(x, image->reso[1] - 1 - y)->Green = color.g * 255.0f;
			output(x, image->reso[1] - 1 - y)->Blue = color.b * 255.0f;
		}
		pixel.y += 1.0f;
		cout<<"Moving to line"<< y + 1 <<endl;
	}
	output.WriteToFile(image->file.c_str());
}

bool ShadowRayUnblocked(vec3 P1, vec3 P2) // ShadowRayUnblocked(IntersectionPoint, light->position)
{
	//return true;
	vec3 P0 = P1;
	vec3 V0 = normalize(P2 - P1);
	Geometry block_j;
	vec3 N;
	double t = sceneGraph_global->RayIntersect(P0, V0, block_j, N);
	// t should be within light distance
	if(t > 0)
		return false;
	else
		return true;
}
vec3 ReflectedRay(vec3 P1, vec3 PN, vec3 PO) // ReflectedRay(light->position, normalize(N), IntersectionPoint)
{
	vec3 Ri = normalize(PO - P1);
	vec3 Rr = normalize(Ri - 2.0f * normalize(PN) * dot(Ri, normalize(PN)));
	return Rr;
}

void RayTrace::TraceRay(vec3 start, vec3 direction, int depth, vec3& color)
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
	Geometry j;
	vec3 N;

	if(depth > MAXDEPTH)
	{
		color = image->backgroundColor;
		return;
	}
	// intersect ray with all objects and find intersection point(if any)
	// on object j that is closest to start of ray; else return nil
	double t = sceneGraph_global->RayIntersect(start, normalize(direction), j, N);
	if(t < 0)
	{
		if(cross(direction, vec3(light->position - camera->eye)) == vec3(0, 0, 0))
			color = light->color;
		else
			color = image->backgroundColor;
		return;
	}
	else
	{
		vec3 IntersectionPoint = start + (float)t * normalize(direction);
		if(dot(normalize(N), normalize(camera->eye - IntersectionPoint)) < 0)
			N = -N;
		if(j.material->Ks > 0)
		{
			ReflectedDirection = normalize(normalize(direction) - 2.0f * normalize(N) * dot(normalize(direction), normalize(N)));
			TraceRay(IntersectionPoint, ReflectedDirection, depth + 1, ReflectedColor);
			spec = j.material->Ks * ReflectedColor;
		}
		else
		{
			spec = vec3(0, 0, 0);
		}
		//if(j.material->Kt > 0)
		//{
		//	// TODO compute direction of refracted ray
		//	float eta_1 = AIR;
		//	float eta_2 = j.material->Kr;
		//	float eta_12 = eta_1 / eta_2;
		//	float discriminant = 1 - eta_12 * eta_12 * (1 - dot(N, direction) * dot(N, direction));
		//	if(discriminant > 0)
		//	{
		//		RefractedDirection = normalize((-eta_12 * dot(N, direction) - sqrt(discriminant)) * N + eta_12 * direction);
		//		TraceRay(IntersectionPoint, RefractedDirection, depth + 1, RefractedColor);
		//		refr = j.material->Kt * RefractedColor;
		//	}
		//	else if(discriminant = 0)
		//	{}
		//	else if(discriminant < 0)
		//	{
		//		// reflection
		//	}
		//}
		//else
		//{
		//	refr = vec3(0, 0, 0);
		//}

		// do shadow feelers
		// LOOKAT assume no light is inside geometry for now
		if(/*inside(start, j)*/false)
			color = vec3(0, 0, 0);
		else
		{
			color = j.material->Ka * light->ambientColor * j.material->color/* + refr*/;
			if(ShadowRayUnblocked(IntersectionPoint, light->position))
			{
				float diffuseTerm = clamp(dot(normalize(N), normalize(light->position - IntersectionPoint)), 0.0f, 1.0f);
				vec3 diffuseComponent = j.material->Kd * j.material->color * diffuseTerm;
				float specularTerm = pow(clamp(dot(ReflectedRay(light->position, normalize(N), IntersectionPoint), normalize(camera->eye - IntersectionPoint)), 0.0f, 1.0f), j.material->Kn);
				vec3 specularComponent = spec * specularTerm;
				color = color + light->color * (diffuseComponent/* + specularComponent*/);
			}
		}
	}
}