#include "vectormath.h"
#include <math.h>


vector &add( vector &out, const vector &a, const vector &b )
{
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    out.z = a.z + b.z;

    return out;
}


vector &subtract( vector &out, const vector &a, const vector &b )
{
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    out.z = a.z - b.z;

    return out;
}

double scalarproduct( const vector &a, const vector &b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}



vector &cross( vector &out, const vector &a, const vector &b )
{
    out.x = a.y*b.z - a.z*b.y;
    out.y = a.z*b.x - a.x*b.z;
    out.z = a.x*b.y - a.y*b.x;

    return out;
}


vector &scale( vector &out, double n )
{
    out.x *= n;
    out.y *= n;
    out.z *= n;

    return out;
}


vector &normalize( vector &out, double n )
{
    double l = sqrt(out.x*out.x + out.y*out.y + out.z*out.z);

    out.x *= n/l;
    out.y *= n/l;
    out.z *= n/l;

    return out;
}