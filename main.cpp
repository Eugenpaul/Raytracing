#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <fstream>
#include <iostream>
#include "vectormath.h"

#ifdef WIN32
#include <windows.h>
#include <Shellapi.h>
#endif

using namespace std;

#define WIDTH 1024
#define HEIGHT 600
#define COLORS 255
#define DEFAULTCOLOR 0.0
#define MATERIALNAMESIZE 30
#define MATERIALSNUM 5
#define MAXDEPTH 3
#define K0 0.1
#define K1 0.3
#define K2 0.5
#define LIGHT 500
const char* mode = "P";
#define P_FORMAT 2


const char *pgmname = "./img.pgm";
const char *modelfile = "./model.model";
const char *paramstrm = "-m";
const char *paramstri = "-i";

const char *lightstring = "light";
const char *camerastring = "camera";
const char *targetstring = "target";
const char *materialstring = "material";
const char *reflectstring = "reflect";
const char *scatterstring = "scatter";
const char *colorstring = "color";
const char *trianglestring = "triangle";
const char *spherestring = "sphere";


typedef struct colorstruct
{
	double g;
}color;


typedef struct materialstruct
{
	char name[MATERIALNAMESIZE];
	color Color;
	double reflectfactor;
	double scatterfactor;
}Material;

typedef struct trianglestruct
{
	vector A;
	vector B;
	vector C;
	Material material;
}triangle;

typedef struct sphere
{
	vector C;
	double R;
	Material material;
}sphere;

typedef struct modelstruct
{
	vector lightspot;
	vector camera;
	vector cameratarget;
	int trianglesnum;
	triangle triangles[100];
	int spheresnum;
	sphere spheres[10];

}model;

Material materials[MATERIALSNUM];
model Model;

unsigned char pictureArray[WIDTH][HEIGHT];




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
#ifdef WIN32
void openpgm()
{
	ShellExecute(NULL,L"Open",L"D:\\Program Files\\IrfanView\\i_view32.exe",L"H:\\raytracing\\Raytracing\\Raytracing\\img.pgm",NULL,SW_SHOWNORMAL);
}
#endif




//model file 
//x y z of lightspot
//x y z of camera
//x y z of cameratarget
//number of triangles
//x1 y1 z1 x2 y2 z2 x3 y3 z3 Color (-1 == mirror)
//...

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
		in >> ModelPoint->lightspot.x;
		in >> ModelPoint->lightspot.y;
		in >> ModelPoint->lightspot.z;
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
	else 
	{
		end = true;
	}
	
	}
	in.close();
	return 1;
    
}

bool triangleIntersect(vector &resultpoint,double &resultt, double &mint, const vector &camera,  vector &ray, const triangle &tr, bool light)
{
	vector normal;
	vector temp1;
	vector temp2;
	cross(normal, subtract(temp1,tr.B,tr.A),subtract(temp2,tr.C,tr.B));
	double A = normal.x;
	double B = normal.y;
	double C = normal.z;
	double D = - A*tr.A.x - B*tr.A.y - C*tr.A.z;
	if (light) scale(normal,-1.0);
	double denominator = scalarproduct(normal,ray);
	if (denominator == 0)
	{
		return false;
	}
	else
	{
		resultt = -(D + scalarproduct(normal,camera))/denominator;
		
		if ((resultt>0)&&(resultt<1)&&(mint == 1)) 
		{
			int trololo = 0;
		}
		if ((resultt<mint)&&(resultt>0))
		{
			vector AB,BC,CA;
			double j1=0,j2=0,j3=0;
			subtract(AB, tr.A,tr.B);
			subtract(BC,tr.B,tr.C);
			subtract(CA,tr.C,tr.A);
			vector cloneray;
			cloneray.x = ray.x;
			cloneray.y = ray.y;
			cloneray.z = ray.z;
			add(resultpoint,camera,scale(cloneray,resultt));
			//scale(ray,1/resultt);
			if((fabs(normal.z)>=fabs(normal.y))&&(fabs(normal.z)>=fabs(normal.x)))
			{
			j1 = (resultpoint.y - tr.A.y)*(tr.B.x - tr.A.x) - (resultpoint.x - tr.A.x)*(tr.B.y - tr.A.y);
			j2 = (resultpoint.y - tr.B.y)*(tr.C.x - tr.B.x) - (resultpoint.x - tr.B.x)*(tr.C.y - tr.B.y);
			j3 = (resultpoint.y - tr.C.y)*(tr.A.x - tr.C.x) - (resultpoint.x - tr.C.x)*(tr.A.y - tr.C.y);
			}
			if (((j1>0)&&(j2>0)&&(j3>0))||((j1<0)&&(j2<0)&&(j3<0)))
			{
				mint = resultt;
			return true;
			}
			else 
			{
				if((fabs(normal.y)>=fabs(normal.z))&&(fabs(normal.y)>=fabs(normal.x)))
				{
				j1 = (resultpoint.z - tr.A.z)*(tr.B.x - tr.A.x) - (resultpoint.x - tr.A.x)*(tr.B.z - tr.A.z);
				j2 = (resultpoint.z - tr.B.z)*(tr.C.x - tr.B.x) - (resultpoint.x - tr.B.x)*(tr.C.z - tr.B.z);
				j3 = (resultpoint.z - tr.C.z)*(tr.A.x - tr.C.x) - (resultpoint.x - tr.C.x)*(tr.A.z - tr.C.z);
				}
				if (((j1>0)&&(j2>0)&&(j3>0))||((j1<0)&&(j2<0)&&(j3<0)))
				{
					mint = resultt;
					return true;
				}
				else
				{
					if((fabs(normal.x)>=fabs(normal.y))&&(fabs(normal.x)>=fabs(normal.z)))
					{
					j1 = (resultpoint.z - tr.A.z)*(tr.B.y - tr.A.y) - (resultpoint.y - tr.A.y)*(tr.B.z - tr.A.z);
					j2 = (resultpoint.z - tr.B.z)*(tr.C.y - tr.B.y) - (resultpoint.y - tr.B.y)*(tr.C.z - tr.B.z);
					j3 = (resultpoint.z - tr.C.z)*(tr.A.y - tr.C.y) - (resultpoint.y - tr.C.y)*(tr.A.z - tr.C.z);
					}
					if (((j1>0)&&(j2>0)&&(j3>0))||((j1<0)&&(j2<0)&&(j3<0)))
					{
						mint = resultt;
						return true;
					}
					else return false;
				}
			}
		}
		else return false;
	}

}

bool sphereIntersect(double &resultt, double &mint, const vector &camera, vector &ray,const sphere &sph)
{
	double det,b;
	vector camcent;
	subtract(camcent,camera,sph.C);
	b = -scalarproduct(camcent,ray);
	det = b*b - scalarproduct(camcent,camcent)*scalarproduct(ray,ray) + sph.R*sph.R*scalarproduct(ray,ray);
	if (det<0)
	{
		return false;
	}
	det = sqrt(det);
	double temp1, temp2;
	temp1 = (b + det)/scalarproduct(ray,ray);
	if (temp1 < 0) return false;
	else
	{
		double temp2 = (b - det)/scalarproduct(ray,ray);
		if (temp2>0)
		{
			if ((temp2*temp2*(ray.x*ray.x + ray.y*ray.y + ray.z*ray.z) > 0.01))
			temp1 = temp2;
		}
		if (temp1<mint)
		{
			//mint = temp1;
			resultt = temp1;
			if ((resultt*resultt*(ray.x*ray.x + ray.y*ray.y + ray.z*ray.z) > 0.01))
			{
				mint = temp1;
				return true;
			}
			else return false;
		}
		else return false;
	}
	
	
}

bool lighttrace(const vector &point, vector &ray, int itri, int &closesti, int objind)//objind sph = 0, tr = 1
{
	int i = 0;
	vector hitpoint;
	vector temp;
	double t;
	bool hitexists = false;
	double min = 1;
	closesti = -1;
	bool hittr = false;
	bool hitsph = false;
	for (i = 0; i<Model.spheresnum; i++)
	{
		if (/*(i!=itri)||(objind!=0)*/1)
			if (sphereIntersect(t,min,point,ray,Model.spheres[i]))
			{
				hitexists = true;
				hitsph = true;
				hittr = false;
				closesti = i;
			}
	}

	for (i = 0; i<Model.trianglesnum; i++)
		{
			if ((i!=itri)||(objind!=1))
			if (triangleIntersect(hitpoint,t,min,point,ray,Model.triangles[i],false))
			{
				hitexists = true;
				hitsph = false;
				hittr = true;
				closesti = i;
			}
		}
	/*if (hitexists)
	{
		if (Model.triangles[closesti].material.reflectfactor == 1.0)
		{
			mirror = true;
		}
		else 
		{
			mirror = false;
		}
		return false;
	}*/
		if (hitexists) return false;
		else return true;
}


/*bool lightIntersect(const vector &cam,vector &ray, double R)
{
	vector temp;
	add(temp,cam,ray);
	if ((temp.x - Model.lightspot.x)*(temp.x - Model.lightspot.x)+
		(temp.y - Model.lightspot.y)*(temp.y - Model.lightspot.y)+
		(temp.z - Model.lightspot.z)*(temp.z - Model.lightspot.z) <=R*R)
		return true;
	else return false;
}*/

void raytrace( color* final, const vector &cam, vector &ray, int it, int depth, int objind)
{
	
	int i,k;
    k = 0;
	vector hitpoint;
	vector finalhitpoint;
	vector temphitpoint;
	bool hitexists = false;
	double t;
	double min = 9999999.9;
	int itri = -1;
	final->g = -1.0;
	finalhitpoint.x = cam.x;
	finalhitpoint.y = cam.y;
	finalhitpoint.z = cam.z;
    {
		min = 9999999.9;
		hitexists = false;
        itri = -1;
		final->g = DEFAULTCOLOR;
		bool hittr = false;
		bool hitsph = false;
		for (i = 0; i<Model.spheresnum; i++)
		{
			if ((i!=itri)||(objind!=0))
				if (sphereIntersect(t,min,finalhitpoint,ray,Model.spheres[i]))
				{
					hitexists = true;
					hitsph = true;
					hittr = false;
					itri = i;
					final->g = Model.spheres[i].material.Color.g;
					temphitpoint.x = finalhitpoint.x + ray.x*t;
					temphitpoint.y = finalhitpoint.y + ray.y*t;
					temphitpoint.z = finalhitpoint.z + ray.z*t;
				}
		}

		for (i = 0; i<Model.trianglesnum; i++)
		{	if (((itri!=i)&&(it!=i))||objind!=1)
			if (triangleIntersect(hitpoint,t,min,finalhitpoint,ray,Model.triangles[i],false))
			{
				itri = i;
				hitsph = false;
				hittr = true;
				final->g = Model.triangles[i].material.Color.g;
				hitexists = true;
				temphitpoint.x = hitpoint.x;
				temphitpoint.y = hitpoint.y;
				temphitpoint.z = hitpoint.z;
			}
		}
		if (hitexists)
		if  (1)//(Model.triangles[itri].material.reflectfactor > 0.0)
		{
			finalhitpoint.x = temphitpoint.x;
			finalhitpoint.y = temphitpoint.y;
			finalhitpoint.z = temphitpoint.z;
			vector N;
			if (hittr)
			N = normal(Model.triangles[itri].A,Model.triangles[itri].B,Model.triangles[itri].C);
			else if (hitsph)
			{
			subtract(N,finalhitpoint,Model.spheres[itri].C);
			}
			//normalize(N,1.0);
			vector temp;
			mirror(temp,ray,N);
			if (1)//(rand()%100 > (1-Model.triangles[itri].material.scatterfactor)*100)
			{
				double randomk = 1500;
				int kk;
				if (hittr)
				kk = Model.triangles[itri].material.scatterfactor*1000;
				else if (hitsph)
				{
					kk = Model.spheres[itri].material.scatterfactor*1000;
				}
				if (kk!=0)
				{
					ray.x = -temp.x*(randomk+rand()%kk)/(randomk+rand()%kk);
					ray.y = -temp.y*(randomk+rand()%kk)/(randomk+rand()%kk);
					ray.z = -temp.z*(randomk+rand()%kk)/(randomk+rand()%kk);
				}
				else
				{
					ray.x = -temp.x;
					ray.y = -temp.y;
					ray.z = -temp.z;
				
				}
			}
			finalhitpoint.x = temphitpoint.x;
			finalhitpoint.y = temphitpoint.y;
			finalhitpoint.z = temphitpoint.z;
            vector beam;
            color light;
            bool mirror = false;  
			
			{
				light.g = 0.0;
				subtract( beam, Model.lightspot, finalhitpoint );
                
				int closesti;
				if (hittr)
				{
				if (lighttrace(finalhitpoint, beam, itri,closesti,1)) // hit the lightspot
				{
					vector temp1;
					vector temp2;
					vector temp3;
					vector temp4;
					vector N = cross(temp3,subtract(temp1,Model.triangles[itri].B,Model.triangles[itri].A),subtract(temp2,Model.triangles[itri].C,Model.triangles[itri].B));
					double cosang = scalarproduct(beam,N)/(sqrt(scalarproduct(beam,beam))*sqrt(scalarproduct(N,N)));
					//if (cosang<-0.51) cosang = 1 + cosang;
					double d = sqrt(beam.x*beam.x + beam.y*beam.y + beam.z*beam.z);
					double k0 = K0;
					double k1 = K1;
					double k2 = K2;
					double C0 = LIGHT;
					if (1)//(rand()%100 > (1-Model.triangles[itri].material.scatterfactor)*100)
					{
					double randomk = 1500;
					int kk = Model.triangles[itri].material.scatterfactor*10000;
					if (kk!=0)
					{
					//beam.x *= (randomk+rand()%kk)/(randomk+rand()%kk);
					//beam.y *= (randomk+rand()%kk)/(randomk+rand()%kk);
					//beam.z *= (randomk+rand()%kk)/(randomk+rand()%kk);
					cosang *= (randomk+rand()%kk)/(randomk+rand()%kk);
					
					}
					}
					double factor = C0 * sqrt(sqrt(fabs(cosang)))/(k0+k1*d + k2*d*d);
					light.g = (factor);
					//final->g += light.g;
				}
				}
				else if (hitsph)
				{
					if (lighttrace(finalhitpoint, beam, itri,closesti,0)) // hit the lightspot
					{
					vector temp1;
					vector temp2;
					vector temp3;
					vector temp4;
					vector N;
					subtract(N,finalhitpoint,Model.spheres[itri].C);
					double cosang = scalarproduct(beam,N)/(sqrt(scalarproduct(beam,beam))*sqrt(scalarproduct(N,N)));
					//if (cosang< -0.51) cosang = 1+cosang; 
					double d = sqrt(beam.x*beam.x + beam.y*beam.y + beam.z*beam.z);
					double k0 = K0;
					double k1 = K1;
					double k2 = K2;
					double C0 = LIGHT;
					if (1)//(rand()%100 > (1-Model.triangles[itri].material.scatterfactor)*100)
					{
					double randomk = 1500;
					int kk = Model.spheres[itri].material.scatterfactor*10000;
					if (kk!=0)
					{
					//beam.x *= (randomk+rand()%kk)/(randomk+rand()%kk);
					//beam.y *= (randomk+rand()%kk)/(randomk+rand()%kk);
					//beam.z *= (randomk+rand()%kk)/(randomk+rand()%kk);
					cosang *= (randomk+rand()%kk)/(randomk+rand()%kk);
					
					}
					}
					double factor = C0 * sqrt(sqrt(fabs(cosang)))/(k0+k1*d + k2*d*d);
					light.g = (factor);
					//final->g += light.g;
					}
				}
			}
			final->g = 0;
			color tempc;
			tempc.g = final->g;
			if (depth<MAXDEPTH)
			{
				if (hitsph)
				raytrace(&tempc,finalhitpoint,ray,itri,depth+1,0);
				else if (hittr)
				raytrace(&tempc,finalhitpoint,ray,itri,depth+1,1);
			}
			if (hitsph)
			final->g+= Model.spheres[itri].material.reflectfactor*tempc.g + light.g*(1 - Model.spheres[itri].material.reflectfactor);
			else if (hittr)
				final->g+= Model.triangles[itri].material.reflectfactor*tempc.g + light.g;
			if (final->g>1) final->g = 1;
			return;
		}
		////
	}{
		////
        if(hitexists)
        {
			finalhitpoint.x = temphitpoint.x;
			finalhitpoint.y = temphitpoint.y;
			finalhitpoint.z = temphitpoint.z;
            vector beam;
            color light;
			{
				light.g = 0.0;
				subtract( beam, Model.lightspot, finalhitpoint );
                
				int closesti;
				if (lighttrace(finalhitpoint, beam, itri,closesti,0)) // hit the lightspot
				{
					vector temp1;
					vector temp2;
					vector temp3;
					vector temp4;
					vector N = cross(temp3,subtract(temp1,Model.triangles[itri].B,Model.triangles[itri].A),subtract(temp2,Model.triangles[itri].C,Model.triangles[itri].B));
				
				
			
					
					double cosang = scalarproduct(beam,N)/(sqrt(scalarproduct(beam,beam))*sqrt(scalarproduct(N,N)));
					//if (cosang<-0.51) cosang = 1 + cosang;
					//random  for scatter		
					if (1)//(rand()%100 > (1-Model.triangles[itri].material.scatterfactor)*100)
					{
					double randomk = 1500;
					int kk = Model.triangles[itri].material.scatterfactor*10000;
					if (kk!=0)
					{
					//beam.x *= (randomk+rand()%kk)/(randomk+rand()%kk);
					//beam.y *= (randomk+rand()%kk)/(randomk+rand()%kk);
					//beam.z *= (randomk+rand()%kk)/(randomk+rand()%kk);
					cosang *= (randomk+rand()%kk)/(randomk+rand()%kk);
					
					}
					}
			
					
					double d = sqrt(beam.x*beam.x + beam.y*beam.y + beam.z*beam.z);
					double k0 = K0;
					double k1 = K1;
					double k2 = K2;
					double C0 = LIGHT;
					double factor = C0 * sqrt(sqrt(fabs(cosang)))/(k0+k1*d + k2*d*d);
					light.g = (factor);
					final->g += (light.g);///((depth+1)*3);
				}
				else
				{
					/*if (mirror)
					{
						vector temp1;
						vector temp2;
						vector temp3;
						vector temp4;
						vector N = cross(temp3,subtract(temp1,Model.triangles[itri].B,Model.triangles[itri].A),subtract(temp2,Model.triangles[itri].C,Model.triangles[itri].B));
						double cosang = scalarproduct(beam,N)/(sqrt(scalarproduct(beam,beam))*sqrt(scalarproduct(N,N)));
						double d = sqrt(beam.x*beam.x + beam.y*beam.y + beam.z*beam.z);
						double k0 = K0;
						double k1 = K1;
						double k2 = K2;
						double C0 = LIGHT;
						double factor = C0 * sqrt(sqrt(fabs(cosang)))/(k0+k1*d + k2*d*d);
						light.g = (factor);
						final->g += light.g/((depth+1)*5);
						
					}*/
				}
			}
		}
}
    if( final->g>1.0 ) final->g = 1.0;
    if( final->g<0.0 ) final->g = 0.0;
    return;
}

void render(int width, int height)
{
	int i,j,k,m,n;
	int goal = width*height;
    color *Color;
    Color = (color*)malloc(sizeof(color));
    memset(pictureArray,0,sizeof(pictureArray));

	subtract(Model.cameratarget, Model.cameratarget, Model.camera);
	normalize(Model.cameratarget, 1.0 );
	add( Model.cameratarget, Model.cameratarget, Model.camera );

    double tarw = 1.0;
    double pixpitch = tarw/width;

    vector mainvector;
	mainvector.x = Model.cameratarget.x - Model.camera.x;
    mainvector.y = Model.cameratarget.y - Model.camera.y;
    mainvector.z = Model.cameratarget.z - Model.camera.z;

    double di,dj;
    double g;
    for(i=0; i<width; i++)
        for(j=0; j<height; j++)
        {
            di = (double)i;
            dj = (double)j;
                    vector side;
                    side.x =  mainvector.y;
                    side.y = -mainvector.x;
                    side.z = 0.0;

                    vector up;
                    cross( up, mainvector, side );

                    normalize (side, (di - (double)width/2.0)*pixpitch);
                    normalize( up, (dj-(double)height/2.0)*pixpitch);

                    vector ray;
					ray.x = Model.cameratarget.x+side.x+up.x - Model.camera.x;
                    ray.y = Model.cameratarget.y+side.y+up.y - Model.camera.y;
                    ray.z = Model.cameratarget.z+side.z+up.z - Model.camera.z;

					raytrace( Color, Model.camera, ray, -1, 0, -1);
			pictureArray[i][j] = (unsigned char)(Color->g * 255);
			if ((((i*(j-1) +j)*100)%(goal*3)) == 0) cout << ".";
        }

	//##########################################################
/*			di = 453;
            dj = 289;
                    vector side;
                    side.x =  mainvector.y;
                    side.y = -mainvector.x;
                    side.z = 0.0;

                    vector up;
                    cross( up, mainvector, side );

                    normalize (side, (di - (double)width/2.0)*pixpitch);
                    normalize( up, (dj-(double)height/2.0)*pixpitch);

                    vector ray;
					ray.x = Model.cameratarget.x+side.x+up.x - Model.camera.x;
                    ray.y = Model.cameratarget.y+side.y+up.y - Model.camera.y;
                    ray.z = Model.cameratarget.z+side.z+up.z - Model.camera.z;

					raytrace( Color, Model.camera, ray,-1,0,-1);
*/
	//#######################################################
}

int main(int argc, char *argv[])
{
	char mfile[30];
	char ifile[30];
	memset(mfile,0,30);
	memset(ifile,0,30);
	int i = 1;
			
	if (argc >=2)

	{
		while (i<argc-1)
		{
			if(strcmp(argv[i],paramstrm) == 0)
			{
				strncpy(mfile,argv[i+1],strlen(argv[i+1]));
				i+=2;
			}
			else if (strcmp(argv[i],paramstri) ==0)
			{
				strncpy(ifile,argv[i+1],strlen(argv[i+1]));
				i+=2;
			}
			else 
			{
				cout<<"Usage: -m <model_filename> -i <image_filename>"<<endl;
				return 0;
			}
				
		}
	}
	if (strcmp(mfile,"") == 0) strncpy(mfile,modelfile,strlen(modelfile));
	if (strcmp(ifile,"") ==0) strncpy(ifile,pgmname,strlen(pgmname));
	cout<<"opening file "<<mfile<<"...";
	if (!openmodel(mfile,&Model)) return 0;
	cout<<"done"<<endl;
	
	cout<<"raytracing...";
	render(WIDTH,HEIGHT);
	cout<<"done\n";

	cout<<"saving file "<<ifile<<"...";
	if (!savepgm(ifile,P_FORMAT,WIDTH,HEIGHT,COLORS)) return 0;
	cout<<"done"<<endl;
	#ifdef WIN32
	openpgm();
	#endif
	return 0;
}
