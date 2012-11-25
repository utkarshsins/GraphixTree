#include "data_structures.h"

double dist(vec3 a, vec3 b)
{
    return sqrt((b[0]-a[0])*(b[0]-a[0]) + (b[1]-a[1])*(b[1]-a[1]) + (b[2]-a[2])*(b[2]-a[2]));
}

int round_int(double x)
{
    int fl = floor(x);
    int cl = ceil(x);
    if(x - fl <= .5)
    {
        return fl;
    }
    else
    {
        return cl;
    }
}

vec2 :: vec2()
{
    values[0] = 0;
    values[1] = 0;
}

vec2 :: vec2(int a, int b)
{
    values[0] = a;
    values[1] = b;
}

vec2 :: vec2(pair_int a)
{
    values[0] = a[0];
    values[1] = a[1];
}

void vec2 :: copy(vec2 a)
{
    values[0] = a.values[0];
    values[1] = a.values[1];
}

vec3 :: vec3()
{
    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
}

vec3 :: vec3(double a, double b, double c)
{
    values[0] = a;
    values[1] = b;
    values[2] = c;
}

vec3 :: vec3(triplet a)
{
    values[0] = a[0];
    values[1] = a[1];
    values[2] = a[2];
}

double vec3 :: value()
{
    return sqrt(values[0]*values[0] + values[1]*values[1] + values[2]*values[2]);
}

void vec3 :: copy(vec3 a)
{
    values[0] = a.values[0];
    values[1] = a.values[1];
    values[2] = a.values[2];
}

void vec3 :: tend_to(vec3 limit, vec3 inc)
{
    for(int i = 0; i < 3; i++)
    {
        if(fabs(values[i]) < fabs(limit[i]))
        {
            //if(values[i])
            values[i] += (limit[i] >= 0 ? inc[i] : -inc[i]);

        }
		else if(fabs(values[i]) > fabs(limit[i]) && (fabs(values[i]) - fabs(limit[i])) >= 2*inc[i] )
        {
            values[i] -= (limit[i] >= 0 ? inc[i] : -inc[i]);

        }


    }
}
/*
void vec3 :: decrease(vec3 limit, vec3 dec)
{
    for(int i = 0; i < 3; i++)
    {
        if(fabs(values[i]) > fabs(limit[i]))
        {
            //if(values[i])
            values[i] -= (limit[i] >= 0 ? dec[i] : -dec[i]);
        }


    }
}
*/
void vec3 :: set(double a, double b, double c)
{
    values[0] = a;
    values[1] = b;
    values[2] = c;
}

void vec3 :: printvec()
{
	std::cout << values[0] << "," << values[1] << "," << values[2];
}

const double& vec3 :: operator [] (int index) const
{
    return values[index];
}

double& vec3 :: operator [] (int index)
{
    return values[index];
}

vec3 vec3 :: operator + (const vec3 &v) const
{
	return vec3(values[0] + v.values[0], values[1] + v.values[1], values[2] + v.values[2]);
}

vec3 vec3 :: operator - (const vec3 &v) const
{
	return vec3(values[0] - v.values[0], values[1] - v.values[1], values[2] - v.values[2]);
}

vec3 vec3 :: operator * (const vec3 &v) const
{
	return vec3(values[0] * v.values[0], values[1] * v.values[1], values[2] * v.values[2]);
}

vec4 :: vec4()
{
    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
}

vec4 :: vec4(quad a)
{
    values[0] = a[0];
    values[1] = a[1];
    values[2] = a[2];
    values[3] = a[3];
}

vec4 :: vec4(double a, double b, double c, double d)
{
    values[0] = a;
    values[1] = b;
    values[2] = c;
    values[3] = d;
}

void vec4 :: copy(vec4 a)
{
    values[0] = a.values[0];
    values[1] = a.values[1];
    values[2] = a.values[2];
    values[3] = a.values[3];

}
