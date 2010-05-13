#pragma once
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <fstream>
#include <iostream>



#define MAXWIDTH 3000
#define MAXHEIGHT 2500
//#define COLORS 255
#define DEFAULTCOLOR 0.0
#define MATERIALNAMESIZE 30
#define MATERIALSNUM 10
//#define MAXDEPTH 4
//#define MAXDIFFUSEDEPTH 1
//#define DIFFUSERAYSNUM 10
#define DIFFUSEADDITION 100
#define DIFFUSECOLORTHRESHOLD 1
//#define LIGHTDOTS	10
//#define K0 0.1
//#define K1 0.2
//#define K2 0.5
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
extern const char *widthstring;// = "width";
extern const char *heightstring;// = "height";
extern const char *colorsstring;// = "colors";
extern const char *reflectdepthstring;// = "refdepth";
extern const char *diffusedepthstring;// = "difdepth";
extern const char *diffusedetailstring; // = "difdetail";
extern const char *lightdetailstring;// = "lightdetail";
extern const char *k0string;// = "k0";
extern const char *k1string;// = "k1";
extern const char *k2string;// = "k2";


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

typedef struct lightstruct
{
	vector C;
	double R;
	double Intensity;
}lightr;

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
	int lightsnum;
	lightr lightspot[10];
	vector camera;
	vector cameratarget;
	int trianglesnum;
	triangle triangles[100];
	int spheresnum;
	sphere spheres[10];

}model;

typedef struct renderq
{
	int width;
	int height;
	int colors;
	int refdepth;
	int difdepth;
	int lightdetail;
	int difdetail;
	double K0;
	double K1;
	double K2;
}rend;



extern Material materials[MATERIALSNUM];
extern model Model;
extern rend renderv; 
extern unsigned char pictureArray[MAXWIDTH][MAXHEIGHT];
