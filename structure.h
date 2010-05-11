#pragma once
//#include "vectormath.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <fstream>
#include <iostream>



#define WIDTH 400
#define HEIGHT 360
#define COLORS 255
#define DEFAULTCOLOR 0.0
#define MATERIALNAMESIZE 30
#define MATERIALSNUM 5
#define MAXDEPTH 4
#define MAXDIFFUSEDEPTH 1
#define DIFFUSERAYSNUM 30
#define DIFFUSEADDITION 100
#define DIFFUSECOLORTHRESHOLD 1
#define K0 0.1
#define K1 0.3
#define K2 0.5
#define LIGHT 500
extern const char* mode;
#define P_FORMAT 2



extern const char *pgmname;// = "./img.pgm";
extern const char *modelfile;// = "./model.model";
extern const char *paramstrm;// = "-m";
extern const char *paramstri;// = "-i";

extern const char *lightstring;// = "light";
extern const char *camerastring;// = "camera";
extern const char *targetstring;// = "target";
extern const char *materialstring;// = "material";
extern const char *reflectstring;// = "reflect";
extern const char *scatterstring;// = "scatter";
extern const char *colorstring;// = "color";
extern const char *trianglestring;// = "triangle";
extern const char *spherestring;// = "sphere";


typedef struct colorstruct
{
	double g;
}color;

typedef struct vectorstruct
{
	double x;
	double y;
	double z;
}vector;

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



extern Material materials[MATERIALSNUM];
extern model Model;
extern unsigned char pictureArray[WIDTH][HEIGHT];
