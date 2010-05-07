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


vector &mirror( vector &out, const vector &ray, const vector &normal )
{
	vector cross1;
	vector cross2;
	cross( cross1, ray, normal );
	cross( cross2, cross1, normal );
	double n, m;
	if (normal.x == 0)
	{
		if (normal.z==0)
		{
			out.x = -ray.x;
            out.y =  ray.y;
			out.z = -ray.z;
			return out;
		}
		else
		{
			n = (ray.y - (normal.y*ray.z)/normal.z)/(cross2.y - (normal.y*cross2.z)/normal.z);
			m = (ray.z - (cross2.z*n))/normal.z;
		}
	}
	else
	{
		n = (ray.y - (normal.y*ray.x)/normal.x)/(cross2.y - (normal.y*cross2.x)/normal.x);
		m = (ray.x - (cross2.x*n))/normal.x;
	}

	vector temp1, temp2;
    temp1 = normal;
    temp2 = cross2;

	subtract( out, scale(temp1,m), scale(temp2,n) );
    return out;
}


vector &normal(vector &A, vector &B, vector &C)
{
	vector temp1;
	vector temp2;
	vector temp3;
	return cross(temp3,subtract(temp1,B,A),subtract(temp2,C,B));
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