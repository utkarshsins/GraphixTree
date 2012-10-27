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
#include <iostream>
#include <utility>
#include "data_structures.h"

using namespace std;

class Branch
{
    public:
        pair<vec3, vec3> end_points;
        double thickness, length;
        vec3 end_boundary[8][2];

        Branch();
        Branch(vec3 , vec3 );
        Branch(vec3 , vec3 , double );
        void paint();
        void set(vec3 , vec3 , double );
};

#endif // BRANCH_H
