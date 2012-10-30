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

#define LEAF_SCALE 0.05
#define LEAF_THICKNESS 0.005
#define LEAF_PETIOLE 0.16
#define LEAF_ANGLE 60

class Leaf
{
    public:
        double length_fraction,
            thetabranch,
            thetabranch360,
            theta360;

        void paint();
        void set(double);
        static void setMaterial();

        Leaf();

        static const float mat_ambient[];
        static const float mat_diffuse[];
        static const float mat_specular[];
        static const float high_shininess[];

};
#endif /* __LEAF_H__ */


