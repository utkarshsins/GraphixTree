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
#include <iostream>
#include <utility>
#include "data_structures.h"
#include "leaf.h"

#define MAX_LEAVES 8

using namespace std;

class Branch
{
    public:
        pair<vec3, vec3> end_points;
        double startthickness, endthickness, length;
        vec3 end_boundary[8][2];

        vector<Leaf> leaves;
        // Leaf* leaves;
        void addleaves(int);

        Branch();
        Branch(vec3 , vec3 , int );
        Branch(vec3 , vec3 , double, double , int );
        void paint();
        void set(vec3 , vec3 , double, double , int );

        static const GLfloat mat_ambient[];
        static const GLfloat mat_diffuse[]; 
        static const GLfloat mat_specular[]; 
        static const GLfloat high_shininess[]; 

};


#endif // BRANCH_H
