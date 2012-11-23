#ifndef BRANCH_H
#define BRANCH_H

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <utility>
#include "data_structures.h"
#include "leaf.h"
#include "wind.h"
#include <time.h>
#include <cmath>

#define MAX_BENT_X 25
#define MAX_BENT_Y 25
#define MAX_BENT_Z 25
#define absd(x) (abs((long) x))
#define E_MODULUS 20000

using namespace std;

class Branch
{
    public:
		vec3 init_angle;
		vec3 current_angle;
        vec3 bent_angle;
        vec3 max_bent_angle;
        double maxLoad;
        double elastic_modulus;
        double startthickness, endthickness, length;
        vec3 end_boundary[8][2];

        vector<Leaf> leaves;
        void addleaves(int);

        Branch();
        Branch(vec3 , double, int, double = 0.2, double = 0.2);
        void paint();
        void set(vec3 , double, int, double , double );
        void copy(Branch );
        void wind_listener(Wind , double );

        static const GLfloat mat_ambient[];
        static const GLfloat mat_diffuse[];
        static const GLfloat mat_specular[];
        static const GLfloat high_shininess[];

};


#endif // BRANCH_H
