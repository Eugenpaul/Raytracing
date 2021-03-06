#include "tracing.h"
#include <math.h>
#include <iostream>

using namespace std;

bool lighttrace(const vector &point, vector &ray, int itri, int &closesti, int objind)//objind sph = 0, tr = 1
{
	int i = 0;
	vector hitpoint;
	vector temp;
	double t;
	bool hitexists = false;
	double min = 1;
	closesti = -1;
	bool hittr = false;
	bool hitsph = false;
	
	for (i = 0; i<Model.spheresnum; i++)
	{
		if (/*(i!=itri)||(objind!=0)*/1)
			if (sphereIntersect(t,min,point,ray,Model.spheres[i]))
			{
				hitexists = true;
				hitsph = true;
				hittr = false;
				closesti = i;
			}
	}

	for (i = 0; i<Model.trianglesnum; i++)
		{
			if ((i!=itri)||(objind!=1))
			if (triangleIntersect(hitpoint,t,min,point,ray,Model.triangles[i],false))
			{
				hitexists = true;
				hitsph = false;
				hittr = true;
				closesti = i;
			}
		}
	/*if (hitexists)
	{
		if (Model.triangles[closesti].material.reflectfactor == 1.0)
		{
			mirror = true;
		}
		else 
		{
			mirror = false;
		}
		return false;
	}*/
		if (hitexists) return false;
		else return true;
}


void diffusetrace(color* final, const vector &cam, vector &ray, int it, int depth, int objind)
{
	int i,k;
    k = 0;
	vector hitpoint;
	vector finalhitpoint;
	vector temphitpoint;
	vector N;
	N.x = 0;
	N.y = 0;
	N.z = 0;
	
	bool hitexists = false;
	double t;
	double min = 9999999.9;
	int itri = -1;
	final->g = 0.0;
	color lightfinal;
	lightfinal.g = 0;
	color light;
	light.g = 0;
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
            vector beam;
            
			int lighti = 0;
			if (depth >0)
			for (lighti = 0; lighti < Model.lightsnum; lighti ++)
			{
				//light.g = 0.0;
				subtract( beam, Model.lightspot[lighti].C, finalhitpoint );
				int closesti;
				if (hittr)
				{
				if (lighttrace(finalhitpoint, beam, itri,closesti,1)) // hit the lightspot
				{
					vector temp1;
					vector temp2;
					vector temp3;
					vector temp4;
					N = cross(temp3,subtract(temp1,Model.triangles[itri].B,Model.triangles[itri].A),subtract(temp2,Model.triangles[itri].C,Model.triangles[itri].B));
					double cosang = scalarproduct(beam,N)/(sqrt(scalarproduct(beam,beam))*sqrt(scalarproduct(N,N)));
					if (cosang <0)
					{
						scale(N,-1.0);
					}
					double d = sqrt(beam.x*beam.x + beam.y*beam.y + beam.z*beam.z);
					double k0 = renderv.K0;
					double k1 = renderv.K1;
					double k2 = renderv.K2;
					double C0 = Model.lightspot[lighti].Intensity;
					if (1)//(rand()%100 > (1-Model.triangles[itri].material.scatterfactor)*100)
					{
					double randomk = 1500;
					int kk = Model.triangles[itri].material.scatterfactor*10000;
					if (kk!=0)
					{
						cosang *= (randomk+rand()%kk)/(randomk+rand()%kk);
					}
					}
					double factor = C0 * /*sqrt(sqrt(*/fabs(cosang)/(k0+k1*d + k2*d*d);
					light.g += (factor);
					/////////////////////
					if (depth > 0)//diffusefactor
					{
						vector temp1;
					vector temp2;
					vector temp3;
					vector temp4;
					vector NN;
					if(objind == 1)
						NN = cross(temp3,subtract(temp1,Model.triangles[it].B,Model.triangles[it].A),subtract(temp2,Model.triangles[it].C,Model.triangles[it].B));
					else
						NN = subtract(NN,cam,Model.spheres[it].C); 
					double cosang = scalarproduct(ray,NN)/(sqrt(scalarproduct(ray,ray))*sqrt(scalarproduct(NN,NN)));
					if (cosang <0)
					{
						scale(NN,-1.0);
					}
					cosang = scalarproduct(N,NN)/(sqrt(scalarproduct(N,N))*sqrt(scalarproduct(NN,NN)));
					d = sqrt((finalhitpoint.x - cam.x)*(finalhitpoint.x - cam.x)+
									(finalhitpoint.y - cam.y)*(finalhitpoint.y - cam.y)+
									(finalhitpoint.z - cam.z)*(finalhitpoint.z - cam.z));
					
					if (1)//(rand()%100 > (1-Model.triangles[itri].material.scatterfactor)*100)
					{
					double randomk = 1500;
					int kk;
					if (objind == 1)
						kk = Model.triangles[it].material.scatterfactor*100;
					else
						kk = Model.spheres[it].material.scatterfactor*100;
					if (kk!=0)
					{
						cosang *= (randomk+rand()%kk)/(randomk+rand()%kk);
					}
					}
					factor = /*sqrt(sqrt(*/fabs(cosang)*DIFFUSEADDITION/(k0+k1*d + k2*d*d);
					light.g += (factor);


					}
					/////////////////////
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
				
					subtract(N,finalhitpoint,Model.spheres[itri].C);
					double cosang = scalarproduct(beam,N)/(sqrt(scalarproduct(beam,beam))*sqrt(scalarproduct(N,N)));
					if (cosang< 0) scale(N,-1); 
					double d = sqrt(beam.x*beam.x + beam.y*beam.y + beam.z*beam.z);
					double k0 = renderv.K0;
					double k1 = renderv.K1;
					double k2 = renderv.K2;
					double C0 = Model.lightspot[lighti].Intensity;
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
					double factor = C0 * /*sqrt(sqrt(*/fabs(cosang)/(k0+k1*d + k2*d*d);
					light.g += (factor);
										/////////////////////
					if (depth > 0)//diffusefactor
					{
						vector temp1;
					vector temp2;
					vector temp3;
					vector temp4;
					vector NN;
					if(objind == 1)
						NN = cross(temp3,subtract(temp1,Model.triangles[it].B,Model.triangles[it].A),subtract(temp2,Model.triangles[it].C,Model.triangles[it].B));
					else
						NN = subtract(NN,cam,Model.spheres[it].C); 
					double cosang = scalarproduct(ray,NN)/(sqrt(scalarproduct(ray,ray))*sqrt(scalarproduct(NN,NN)));
					if (cosang <0)
					{
						scale(NN,-1.0);
					}
					cosang = scalarproduct(N,NN)/(sqrt(scalarproduct(N,N))*sqrt(scalarproduct(NN,NN)));
					d = sqrt((finalhitpoint.x - cam.x)*(finalhitpoint.x - cam.x)+
									(finalhitpoint.y - cam.y)*(finalhitpoint.y - cam.y)+
									(finalhitpoint.z - cam.z)*(finalhitpoint.z - cam.z));
					
					if (1)//(rand()%100 > (1-Model.triangles[itri].material.scatterfactor)*100)
					{
					double randomk = 1500;
					int kk;
					if (objind == 1)
						kk = Model.triangles[it].material.scatterfactor*100;
					else
						kk = Model.spheres[it].material.scatterfactor*100;
					if (kk!=0)
					{
						cosang *= (randomk+rand()%kk)/(randomk+rand()%kk);
					}
					}
					factor =/* sqrt(sqrt(*/fabs(cosang)*DIFFUSEADDITION/(k0+k1*d + k2*d*d);
					light.g += (factor);


					}
					/////////////////////
					//final->g += light.g;
					}
				}
			}
			light.g /= (Model.lightsnum);
			final->g = 0;
			color tempc;
			tempc.g = final->g;
			if (depth<renderv.difdepth)
			{
				int ii;
				vector randomray;
				
				lightfinal.g = 0.0;
				double cosangraynormal = scalarproduct(ray,N)/(sqrt(scalarproduct(ray,ray))*sqrt(scalarproduct(N,N)));
				if (cosangraynormal>0) 
				{
					scale(N,-1);
				}
				vector upN;
				vector sideN;
				int A,B,C;
				if ((N.y!=0)||(N.x!=0))
				{
				sideN.x = N.y;
				sideN.y = -N.x;
				sideN.z = 0;
				cross(upN,sideN,N);
				normalize(upN,1);
				normalize(sideN,1);
				normalize(N,1);
				}
				else
				{
					sideN.x = 1;
					sideN.y = 1;
					sideN.z = 0;
					cross(upN,sideN,N);
					normalize(upN,1);
					normalize(sideN,1);
					normalize(N,1);
				}
				for (ii = 1; ii<=renderv.difdetail; ii++)
				{
					//randomray.x =  sign(N.x)*(rand()%(ii*10)+1000 - ii*10)/(double)1000;
					//randomray.y =  sign(N.y)*(rand()%(ii*10)+1000 - ii*10)/(double)1000;
					//randomray.z = sign(N.z)*(rand()%(ii*10)+1000 - ii*10)/(double)1000;
					A = (rand()%500);
					B = (rand()%1000 - rand()%1000);
					C = (rand()%1000 - rand()%1000);
					randomray.x = A*N.x + B*upN.x+ C*sideN.x;
					randomray.y = A*N.y + B*upN.y+ C*sideN.y;
					randomray.z = A*N.z + B*upN.z+ C*sideN.z;
					normalize(randomray,1);
				if (hitsph)
				diffusetrace(&tempc,finalhitpoint,randomray,itri,depth+1,0);
				else if (hittr)
				diffusetrace(&tempc,finalhitpoint,randomray,itri,depth+1,1);
				lightfinal.g += tempc.g;
				}
				lightfinal.g = lightfinal.g/(double)renderv.difdetail;
				if (lightfinal.g != 0)
				{
					int trololo = 0;
				}
			}
			if (hitsph)
			final->g+= lightfinal.g*(1-Model.spheres[itri].material.reflectfactor) + light.g*(1 - Model.spheres[itri].material.reflectfactor);//Model.spheres[itri].material.reflectfactor*tempc.g + light.g*(1 - Model.spheres[itri].material.reflectfactor);
			else if (hittr)
				final->g+= lightfinal.g*(1 - Model.triangles[itri].material.reflectfactor) + light.g*(1 - Model.triangles[itri].material.reflectfactor);//Model.triangles[itri].material.reflectfactor*tempc.g + light.g;
			if (final->g>1) 
			{
				final->g = 1;
			}
			final->g;//sqrt(sqrt(final->g))/3;
			if (final->g > DIFFUSECOLORTHRESHOLD) final->g = DIFFUSECOLORTHRESHOLD;
			return;
		}
		
}
    if( final->g>1.0 ) final->g = 1.0;
    if( final->g<0.0 ) final->g = 0.0;
    return;
}

void raytrace( color* final, const vector &cam, vector &ray, int it, int depth, int objind)
{
	int i,k;
    k = 0;
	vector hitpoint;
	vector finalhitpoint;
	vector temphitpoint;
	color finallight;
	finallight.g = 0;
	color light;
	light.g = 0;
	bool hitexists = false;
	double t;
	double min = 9999999.9;
	int itri = -1;
	final->g = -1.0;
	finalhitpoint.x = cam.x;
	finalhitpoint.y = cam.y;
	finalhitpoint.z = cam.z;
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
	{	
		if (((itri!=i)&&(it!=i))||objind!=1)
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
		vector temp;
		mirror(temp,ray,N);	
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
		finalhitpoint.x = temphitpoint.x;
		finalhitpoint.y = temphitpoint.y;
		finalhitpoint.z = temphitpoint.z;
        vector beam;
        bool mirror = false;  
		int lighti = 0;
		vector lightball;
		for (lighti = 0; lighti < Model.lightsnum; lighti++)
		{
			int si = 0;
			int A = 0;
			int B = 0;
			int C = 0;
			for (si = 0; si < renderv.lightdetail; si ++)
			{
				lightball.x = Model.lightspot[lighti].C.x + (rand()%1000 - rand()%1000)/(double)1000 * Model.lightspot[lighti].R;
				lightball.y = Model.lightspot[lighti].C.y + (rand()%1000 - rand()%1000)/(double)1000 * Model.lightspot[lighti].R;
				lightball.z = Model.lightspot[lighti].C.z;//Model.lightspot[lighti].z + (rand()%1000 - rand()%1000)/(double)1000 * LIGHTRADIUS;
				subtract( beam, lightball, finalhitpoint );      
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
						double d = sqrt(beam.x*beam.x + beam.y*beam.y + beam.z*beam.z);
						double k0 = renderv.K0;
						double k1 = renderv.K1;
						double k2 = renderv.K2;
						double C0 = Model.lightspot[lighti].Intensity;
						double randomk = 1500;
						int kk = Model.triangles[itri].material.scatterfactor*10000;
						if (kk!=0)
						{
						cosang *= (randomk+rand()%kk)/(randomk+rand()%kk);
						}					
						double factor = C0 * fabs(cosang)/(k0+k1*d + k2*d*d);
						light.g += (factor);
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
						double d = sqrt(beam.x*beam.x + beam.y*beam.y + beam.z*beam.z);
						double k0 = renderv.K0;
						double k1 = renderv.K1;
						double k2 = renderv.K2;
						double C0 = Model.lightspot[lighti].Intensity;
						double randomk = 1500;
						int kk = Model.spheres[itri].material.scatterfactor*10000;
						if (kk!=0)
						{
							cosang *= (randomk+rand()%kk)/(randomk+rand()%kk);
						}
						double factor = C0 * fabs(cosang)/(k0+k1*d + k2*d*d);
						light.g +=(factor);
					}
				}
			}
			light.g /= renderv.lightdetail;
			finallight.g +=light.g;
		}
		light.g = finallight.g/(Model.lightsnum);
		final->g = 0;
		color tempc;
		tempc.g = final->g;
		if (depth<renderv.refdepth)
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
    if( final->g>1.0 ) final->g = 1.0;
    if( final->g<0.0 ) final->g = 0.0;
    return;
}

