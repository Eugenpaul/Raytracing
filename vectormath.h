#ifndef STRUCTURE_H
#include "structure.h"
#define STRUCTURE_H
#endif

vector &add( vector &out, const vector &a, const vector &b );
vector &subtract( vector &out, const vector &a, const vector &b );
vector &cross( vector &out, const vector &a, const vector &b );
vector &scale( vector &out, double n );
vector &normalize( vector &out, double n );
vector &normal(vector &A, vector &B, vector &C);
vector &mirror( vector &out, const vector &ray, const vector &normal );
double scalarproduct( const vector &a, const vector &b);
bool sphereIntersect(double &resultt, double &mint, const vector &camera, vector &ray,const sphere &sph);
bool triangleIntersect(vector &resultpoint,double &resultt, double &mint, const vector &camera,  vector &ray, const triangle &tr, bool light);