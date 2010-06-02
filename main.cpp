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


/*#ifdef WIN32
#include <windows.h>
#include <Shellapi.h>
#endif*/

using namespace std;



/*#ifdef WIN32
void openpgm()
{
	ShellExecute(NULL,L"Open",L"D:\\Program Files\\IrfanView\\i_view32.exe",L"H:\\raytracing\\Raytracing\\Raytracing\\img.pgm",NULL,SW_SHOWMAXIMIZED);
}
#endif*/


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
	color diffuse;
	diffuse.g = 0;
    double di,dj;
    //double g;
    for(i=0; i<width; i++)
       for(j=0; j<height; j++)
        {
            di = (double)i;
            dj = (double)j;
			if ((i == 100)&&(j == 67))
			{
				int trololo = 0;
			}
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
					
					vector tempray;
					tempray.x = ray.x;
					tempray.y = ray.y;
					tempray.z = ray.z;
					raytrace( Color, Model.camera, ray, -1, 0, -1);
					if (Color->g < DIFFUSECOLORTHRESHOLD)
						//if ( (i*(height)+j)%20 == 0)
						{diffusetrace(&diffuse, Model.camera, tempray, -1, 0 , -1);}
					Color->g+= diffuse.g;
					if (Color->g>1) Color->g = 1;
			pictureArray[i][j] = (unsigned char)(Color->g * 255);
			//if ((((i*(j-1) +j)*100)%(goal*3)) == 0) cout << ".";
			int subgoal = ((i*(height) + j));
			if (subgoal >= g)
			{
				g += goal;
				cout << int((subgoal*10)/goal) << "%.." << flush;
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
	char mfile[90];
	char ifile[90];
	memset(mfile,0,90);
	memset(ifile,0,90);
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
	if (strcmp(mfile,"") == 0)
	{
		cout<<"Usage: -m <model_filename> -i <image_filename>"<<endl;
		return 0;
	} 
	//strncpy(mfile,modelfile,strlen(modelfile));
	if (strcmp(ifile,"") ==0)
	{
	 strncpy(ifile,mfile,strlen(mfile));
	 strncat(ifile,".pgm\0",6);
	}
	cout<<"opening file "<<mfile<<"...";
	if (!openmodel(mfile,&Model)) return 0;
	cout<<"done"<<endl;
	
	cout<<"raytracing...";
	render(renderv.width,renderv.height);
	cout<<"done\n";


	cout<<"saving file "<<ifile<<"...";
	if (!savepgm(ifile,P_FORMAT,renderv.width,renderv.height,renderv.colors)) return 0;
	cout<<"done"<<endl;
	#ifdef WIN32
	//openpgm();
	#endif
	return 0;
}
