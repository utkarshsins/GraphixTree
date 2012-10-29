#ifndef DATA_STRUCTURES_H_INCLUDED
#define DATA_STRUCTURES_H_INCLUDED

#include <iostream>
#include <algorithm>
#include <math.h>

typedef int pair_int[2];
typedef double triplet[3];
typedef double quad[4];

#ifndef M_PI
#define M_PI 3.14
#endif

#ifndef min3
#define min3(a,b,c) ((a<=b?(a<=c?a:c):(b<=c?b:c)))
#endif

#ifndef max3
#define max3(a,b,c) ((a>b?(a>c?a:c):(b>c?b:c)))
#endif

#ifndef min
#define min(a,b) (a<=b?a:b)
#endif

#ifndef max
#define max(a,b) (a>b?a:b)
#endif

class vec2
{
    public:
        pair_int values;
        vec2();
        vec2(int a, int b);
        vec2(pair_int a);
        void copy(vec2 a);

};

class vec3
{
    public:
        triplet values;
        vec3();
        vec3(double a, double b, double c);
        vec3(triplet a);
        void copy(vec3 a);
        void set(double , double , double);
        void printvec();
        vec3 operator + (const vec3 &c) const;
        vec3 operator - (const vec3 &c) const;
        vec3 operator * (const vec3 &c) const;
        const double& operator [] (int index) const;
        double& operator [] (int index);



};

class vec4
{
    public:
        quad values;
        vec4();
        vec4(double a, double b, double c, double d);
        vec4(quad a);
        void copy(vec4 a);
};

double dist(vec3 , vec3 );
int round_int(double );

#endif // DATA_STRUCTURES_H_INCLUDED
