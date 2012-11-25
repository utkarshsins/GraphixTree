#ifndef __LEAF_H__
#define __LEAF_H__

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include "betanoise.h"
#include "Debug.h"

#define LEAF_SCALE 0.05
#define LEAF_THICKNESS 0.005
#define LEAF_PETIOLE 0.16
#define LEAF_ANGLE 60

class Leaf
{
    public:
        double length_fraction,
            thetabranch,
            thetabranch360;

		double thetax, thetay, thetaz;

		#ifdef DEBUG_SINGLE_LEAF
		void paint(double,bool);
		#else
        void paint(double);
		#endif

        void set(double, bool);
        static void setMaterial();

        Leaf();

        static const float mat_ambient[];
        static const float mat_diffuse[];
        static const float mat_specular[];
        static const float high_shininess[];

		static bool render_leaves;

		std::vector<double> onefbeta;
};
#endif /* __LEAF_H__ */


