#pragma once
#include "vectormath.h"
//#include "structure.h"
#include <math.h>



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
			if((fabs(normal.z)>=fabs(normal.y))&&(fabs(normal.z)>=fabs(normal.x)))
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
				if((fabs(normal.y)>=fabs(normal.z))&&(fabs(normal.y)>=fabs(normal.x)))
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
					if((fabs(normal.x)>=fabs(normal.y))&&(fabs(normal.x)>=fabs(normal.z)))
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

bool sphereIntersect(double &resultt, double &mint, const vector &camera, vector &ray,const sphere &sph)
{
	double det,b;
	vector camcent;
	subtract(camcent,camera,sph.C);
	b = -scalarproduct(camcent,ray);
	det = b*b - scalarproduct(camcent,camcent)*scalarproduct(ray,ray) + sph.R*sph.R*scalarproduct(ray,ray);
	if (det<0)
	{
		return false;
	}
	det = sqrt(det);
	double temp1, temp2;
	temp1 = (b + det)/scalarproduct(ray,ray);
	if (temp1 < 0) return false;
	else
	{
		double temp2 = (b - det)/scalarproduct(ray,ray);
		if (temp2>0)
		{
			if ((temp2*temp2*(ray.x*ray.x + ray.y*ray.y + ray.z*ray.z) > 0.01))
			temp1 = temp2;
		}
		if (temp1<mint)
		{
			//mint = temp1;
			resultt = temp1;
			if ((resultt*resultt*(ray.x*ray.x + ray.y*ray.y + ray.z*ray.z) > 0.01))
			{
				mint = temp1;
				return true;
			}
			else return false;
		}
		else return false;
	}
	
	
}

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