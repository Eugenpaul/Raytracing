#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <fstream>
#include <iostream>
#ifndef STRUCTURE_H
#include "structure.h"
#define STRUCTURE_H
#endif
#include "vectormath.h"
#include "io.h"
#include "tracing.h"


#ifdef WIN32
#include <windows.h>
#include <Shellapi.h>
#endif

using namespace std;



#ifdef WIN32
void openpgm()
{
	ShellExecute(NULL,L"Open",L"D:\\Program Files\\IrfanView\\i_view32.exe",L"H:\\raytracing\\Raytracing\\Raytracing\\img.pgm",NULL,SW_SHOWMAXIMIZED);
}
#endif




//model file 
//x y z of lightspot
//x y z of camera
//x y z of cameratarget
//number of triangles
//x1 y1 z1 x2 y2 z2 x3 y3 z3 Color (-1 == mirror)
//...






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
	int g = 0;
	int goal = width*height/10;
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
    //double g;
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
					color diffuse;
					diffuse.g = 0;
					vector tempray;
					tempray.x = ray.x;
					tempray.y = ray.y;
					tempray.z = ray.z;
					raytrace( Color, Model.camera, ray, -1, 0, -1);
					if (Color->g < DIFFUSECOLORTHRESHOLD)
					{diffusetrace(&diffuse, Model.camera, tempray, -1, 0 , -1);
					Color->g+= diffuse.g;}
					if (Color->g>1) Color->g = 1;
			pictureArray[i][j] = (unsigned char)(Color->g * 255);
			//if ((((i*(j-1) +j)*100)%(goal*3)) == 0) cout << ".";
			int subgoal = ((i*(height) + j));
			if (subgoal >= g)
			{
				g += goal;
				cout << int((subgoal*10)/goal) << "%..";
			}
        }

	//##########################################################
/*			di = 35;
            dj = 40;
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
					color diffuse;
					diffuse.g = 0;
					vector tempray;
					tempray.x = ray.x;
					tempray.y = ray.y;
					tempray.z = ray.z;
					raytrace( Color, Model.camera, ray, -1, 0, -1);
					if (Color->g < 0.1)
					{diffusetrace(&diffuse, Model.camera, tempray, -1, 0 , -1);
					Color->g+= diffuse.g;}
					if (Color->g>1) Color->g = 1;
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
