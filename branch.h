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
#include "betanoise.h"
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
        pair<vec3, vec3> end_points;
        //pair<vec3, vec3> new_end_points;
        vec3 bent_angle;
        bool bent_angle_changed;
        vec3 current_angle;
        //vec3 total_bent_angle;
        vec3 max_bent_angle;
        double maxLoad;
        double elastic_modulus;
        double startthickness, endthickness, length;
        vec3 end_boundary[8][2];

        vector<Leaf> leaves;
        void addleaves(int,bool);

		vector<double> betanoise;
		int betasize;

        Branch();
        Branch(vec3 , vec3 , int );
        Branch(vec3 , vec3 , double, double , int );

		#ifdef DEBUG_SINGLE_LEAF
		void paint(double,bool);
		#else
        void paint(double);
		#endif

		#ifdef DEBUG_LEAF_BETA
        void set(vec3 , vec3 , double, double , int , bool , Branch* );
		#else
		void set(vec3 , vec3 , double, double , int , Branch* );
		#endif

        void wind_listener(Wind , long long);

        static const GLfloat mat_ambient[];
        static const GLfloat mat_diffuse[];
        static const GLfloat mat_specular[];
        static const GLfloat high_shininess[];


};


#endif // BRANCH_H
