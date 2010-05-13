#include "structure.h"
const char* mode = "P";
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

const char *widthstring = "width";
const char *heightstring = "height";
const char *colorsstring = "colors";
const char *reflectdepthstring = "refdepth";
const char *diffusedepthstring = "difdepth";
const char *diffusedetailstring = "difdetail";
const char *lightdetailstring = "lightdetail";
const char *k0string = "k0";
const char *k1string = "k1";
const char *k2string = "k2";
Material materials[MATERIALSNUM];
model Model;
rend renderv; 
unsigned char pictureArray[MAXWIDTH][MAXHEIGHT];
