
#include <fstream>
#include <iostream>
#include "io.h"
using namespace std;

int savepgm(const char *filename, int format, int width, int height, int maximum)
{
	ofstream out(filename);
	if (!out)
	{
		cout << "Cannot open file "<<filename<<"\n";
		return 0;
	}
	out << mode << format << endl;
	out << width << " " << height << endl;
	out << maximum << endl;
	int i = 0, j = 0;
	for(i=0; i<height; i++)
    {
		for (j = 0; j<width - 1; j++)
		{
			out << (int)pictureArray[j][i] << " ";
			//out << rand()%maximum << " ";
		}
		out << (int)pictureArray[j][i]<<endl;
		//out << rand()%maximum << endl;
	}
	out.close();
	return 1;
}

int openmodel(const char *filename, model *ModelPoint)
{
	int n = 0, i = 0;
	int materialnum = 0;
	char temp[30];
	ifstream in(filename);
	if (!in)
	{
		cout <<"Cannot open file "<<filename<<"\n";
		return 0;
	}
	
	bool end = false;
	ModelPoint->trianglesnum = 0;
	ModelPoint->spheresnum = 0;
	ModelPoint->lightsnum = 0;
	while (!end)
	{
	in >> temp;
	if (strcmp(temp,trianglestring)==0)
	{
		char m[30];
		in >> m;
		for (i = 0; i < materialnum; i++)
		{
			if (strcmp(m,materials[i].name) ==0)
			{
				ModelPoint->triangles[ModelPoint->trianglesnum].material = materials[i];
			}
		}
		in >> ModelPoint->triangles[ModelPoint->trianglesnum].A.x;
		in >> ModelPoint->triangles[ModelPoint->trianglesnum].A.y;
		in >> ModelPoint->triangles[ModelPoint->trianglesnum].A.z;
		in >> ModelPoint->triangles[ModelPoint->trianglesnum].B.x;
		in >> ModelPoint->triangles[ModelPoint->trianglesnum].B.y;
		in >> ModelPoint->triangles[ModelPoint->trianglesnum].B.z;
		in >> ModelPoint->triangles[ModelPoint->trianglesnum].C.x;
		in >> ModelPoint->triangles[ModelPoint->trianglesnum].C.y;
		in >> ModelPoint->triangles[ModelPoint->trianglesnum].C.z;
		ModelPoint->trianglesnum ++;
	}
	else if (strcmp(temp,spherestring) == 0)
	{
		char m[30];
		in >>m;
		for (i = 0; i < materialnum; i++)
		{
			if (strcmp(m,materials[i].name) == 0)
			{
			ModelPoint->spheres[ModelPoint->spheresnum].material = materials[i];
			}
		}
		in >> ModelPoint->spheres[ModelPoint->spheresnum].R;
		in >> ModelPoint->spheres[ModelPoint->spheresnum].C.x;
		in >> ModelPoint->spheres[ModelPoint->spheresnum].C.y;
		in >> ModelPoint->spheres[ModelPoint->spheresnum].C.z;
		ModelPoint->spheresnum ++;
	}
	else if(strcmp(temp,materialstring)==0)
	{
		in>>materials[materialnum].name;
		in>>materials[materialnum].reflectfactor;
		in>>materials[materialnum].scatterfactor;
		in>>materials[materialnum].Color.g;
		materialnum++;
	}
	else if (strcmp(temp,lightstring)==0)
	{
		in >> ModelPoint->lightspot[ModelPoint->lightsnum].Intensity;
		in >> ModelPoint->lightspot[ModelPoint->lightsnum].R;
		in >> ModelPoint->lightspot[ModelPoint->lightsnum].C.x;
		in >> ModelPoint->lightspot[ModelPoint->lightsnum].C.y;
		in >> ModelPoint->lightspot[ModelPoint->lightsnum].C.z;
		ModelPoint->lightsnum++;
	}
	else if(strcmp(temp,camerastring)==0)
	{
		in >> ModelPoint->camera.x;
		in >> ModelPoint->camera.y;
		in >> ModelPoint->camera.z;
	}
	else if(strcmp(temp,targetstring)==0)
	{
		in >> ModelPoint->cameratarget.x;
		in >> ModelPoint->cameratarget.y;
		in >> ModelPoint->cameratarget.z;
	}
	else if (strcmp(temp,widthstring) == 0)
	{
		in >>renderv.width;
	}
	else if (strcmp(temp,heightstring) ==0)
	{
		in >> renderv.height;
	}
	else if (strcmp(temp,colorsstring) ==0)
	{
		in >> renderv.colors;
	}
	else if (strcmp(temp,reflectdepthstring) ==0)
	{
		in >>renderv.refdepth;
	}
	else if (strcmp(temp,diffusedepthstring) ==0)
	{
		in >> renderv.difdepth;
	}
	else if (strcmp(temp,diffusedetailstring) ==0)
	{
		in >> renderv.difdetail;
	}
	else if (strcmp(temp,lightdetailstring) ==0)
	{
		in >> renderv.lightdetail;
	}
	else if (strcmp(temp,k0string) ==0)
	{
		in >> renderv.K0;
	}
	else if (strcmp(temp,k1string) ==0)
	{
		in >> renderv.K1;
	}
	else if (strcmp(temp,k2string) ==0)
	{
		in >> renderv.K2;
	}
	else 
	{
		end = true;
	}
	
	}
	in.close();
	return 1;
    
}