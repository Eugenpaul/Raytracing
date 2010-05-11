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
Material materials[MATERIALSNUM];
model Model;
unsigned char pictureArray[WIDTH][HEIGHT];
