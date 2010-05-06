#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
//#include <conio.h>
#include <fstream>
#include <iostream>
//#include <windows.h>
//#include <Shellapi.h>
#include "vectormath.h"

using namespace std;

#define WIDTH 640
#define HEIGHT 480
#define COLORS 255
#define DEFAULTCOLOR 0.3
#define K0 0.1
#define K1 0.2
#define K2 0.5
#define LIGHT 150
const char* mode = "P";
#define P_FORMAT 2


const char *pgmname = "./img.pgm";
const char *modelfile = "./model.model";




typedef struct colorstruct
{
	double g;
}color;

typedef struct trianglestruct
{
	vector A;
	vector B;
	vector C;
	color Color;
}triangle;

typedef struct modelstruct
{
	vector lightspot;
	vector camera;
	vector cameratarget;
	int trianglesnum;
	triangle triangles[20];

}model;

model Model;

unsigned char pictureArray[WIDTH][HEIGHT];




void savepgm(const char *filename, int format, int width, int height, int maximum)
{
	ofstream out(filename);
	if (!out)
	{
		cout << "Cannot open file\n";
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
}
/*void openpgm()
{
	ShellExecute(NULL,L"Open",L"D:\\Program Files\\IrfanView\\i_view32.exe",L"H:\\raytracing\\Raytracing\\Raytracing\\img.pgm",NULL,SW_SHOWNORMAL);
}*/




//model file 
//x y z of lightspot
//x y z of camera
//x y z of cameratarget
//number of triangles
//x1 y1 z1 x2 y2 z2 x3 y3 z3
//...

void openmodel(const char *filename, model *ModelPoint)
{
	int n = 0;
	ifstream in(filename);
	if (!in)
	{
		cout <<"Cannot open file\n";
	}
    in >> ModelPoint->lightspot.x;
	in >> ModelPoint->lightspot.y;
	in >> ModelPoint->lightspot.z;
	in >> ModelPoint->camera.x;
	in >> ModelPoint->camera.y;
	in >> ModelPoint->camera.z;
	in >> ModelPoint->cameratarget.x;
	in >> ModelPoint->cameratarget.y;
	in >> ModelPoint->cameratarget.z;
	in >> ModelPoint->trianglesnum;
	int i = 0;
	for (i = 0; i<ModelPoint->trianglesnum; i++)
	{
		in >> ModelPoint->triangles[i].A.x;
		in >> ModelPoint->triangles[i].A.y;
		in >> ModelPoint->triangles[i].A.z;
		in >> ModelPoint->triangles[i].B.x;
		in >> ModelPoint->triangles[i].B.y;
		in >> ModelPoint->triangles[i].B.z;
		in >> ModelPoint->triangles[i].C.x;
		in >> ModelPoint->triangles[i].C.y;
		in >> ModelPoint->triangles[i].C.z;
		ModelPoint->triangles[i].Color.g = DEFAULTCOLOR;
	}
	in.close();
    
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
			if((abs(normal.z)>=abs(normal.y))&&(abs(normal.z)>=abs(normal.x)))
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
				if((abs(normal.y)>=abs(normal.z))&&(abs(normal.y)>=abs(normal.x)))
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
					if((abs(normal.x)>=abs(normal.y))&&(abs(normal.x)>=abs(normal.z)))
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

bool lighttrace(const vector &point, vector &ray, int itri)
{
	int i = 0;
	vector hitpoint;
	vector temp;
	double t;
	double min = 1;
	
	for (i = 0; i<Model.trianglesnum; i++)
		{
			if (i!=itri)
			if (triangleIntersect(hitpoint,t,min,point,ray,Model.triangles[i],false))
			{
				return false;
			}
		}
	
	return true;
}

void raytrace( color* final, const vector &cam, vector &ray)
{
	
	int i,k;
   
    
    vector hit;
    double p;
        
    p = 0.0;
    k = 0;
	vector hitpoint;
	vector finalhitpoint;
	bool hitexists = false;
	double t;
	double min = 9999999.9;
	int itri = -1;
    //while( 1 )
    {
        
		final->g = 0.0;
		for (i = 0; i<Model.trianglesnum; i++)
		{
			if (triangleIntersect(hitpoint,t,min,cam,ray,Model.triangles[i],false))
			{
				itri = i;
				final->g = Model.triangles[i].Color.g;
				hitexists = true;
				finalhitpoint.x = hitpoint.x;
				finalhitpoint.y = hitpoint.y;
				finalhitpoint.z = hitpoint.z;
			}
		}
        if(hitexists)
        {
            vector beam;
            color light;
                            
				light.g = 0.0;
				subtract( beam, Model.lightspot, finalhitpoint );
               
				if (lighttrace(finalhitpoint, beam, itri)) // hit the lightspot
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
					double factor = /*scalarproduct(N,beam) sqrt(1-cosang*cosang)*/C0 * abs(cosang)/(k0+k1*d + k2*d*d);
					//if (factor<0) factor = 0;
                light.g = (factor);
				if (light.g >0.5) 
				{
					int trololo = 0;
				}
                //final->g = 0.7;
				final->g += light.g;
				}
				else
				{
					//final->g = 0.2;
				}
		}
		else{
		int trololo =-1;}
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

					raytrace( Color, Model.camera, ray);
			pictureArray[i][j] = (unsigned char)(Color->g * 255);
			if ((((i*(j-1) +j)*100)%(goal*3)) == 0) cout << ".";
        }

	//##########################################################
			/*di = 197;
            dj = 94;
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

					raytrace( Color, Model.camera, ray);*/

	//#######################################################
}

int main()
{
	cout<<"opening file "<<modelfile<<"...";
	openmodel(modelfile,&Model);
	cout<<"done"<<endl;
	
	cout<<"raytracing...";
	render(WIDTH,HEIGHT);
	cout<<"done\n";

	cout<<"saving file "<<pgmname<<"...";
	savepgm(pgmname,P_FORMAT,WIDTH,HEIGHT,COLORS);
	cout<<"done"<<endl;
	//openpgm();
	return 0;
}