#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "data_structures.h"
#include "treeskeleton.h"

using namespace std;

double eye_radius = 8;
triplet eye = {0,0,eye_radius};
double upangle = 0;
double sideangle = 0;
double upangle_inc = 10*M_PI/180;
double sideangle_inc = 10*M_PI/180;


const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

TreeSkeleton tree(4,3);

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    glLoadIdentity();
    gluLookAt(eye[0],eye[1],eye[2],0,0,0,0,1,0);

    glPushMatrix();
        tree.paint();
    glPopMatrix();

    glutSwapBuffers();
}

static void specialKey(int key, int x, int y)
{
    bool validKey = false;
    switch (key)
    {
        case GLUT_KEY_LEFT :
            sideangle += sideangle_inc;
            validKey = true;
            break;
        case GLUT_KEY_RIGHT :
            sideangle -= sideangle_inc;
            validKey = true;
            break;
        case GLUT_KEY_UP :
            upangle -= upangle_inc;
            validKey = true;
            break;
        case GLUT_KEY_DOWN :
            upangle += upangle_inc;
            validKey = true;
            break;
    }
    if(validKey)
    {
        double eye_height = eye_radius*sin(upangle);
        double smaller_radius = sqrt(eye_radius*eye_radius - eye_height*eye_height);
        eye[0] = smaller_radius*sin(sideangle);
        eye[1] = eye_height;
        eye[2] = smaller_radius*cos(sideangle);
    }

    glutPostRedisplay();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Tree Animation");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutSpecialFunc(specialKey);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    tree.makeTree();

    glutMainLoop();

    return EXIT_SUCCESS;
}
