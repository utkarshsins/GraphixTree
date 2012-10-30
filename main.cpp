#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "treeskeleton.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "data_structures.h"

using namespace std;

double eye_radius = 8;
triplet eye = {0,0,eye_radius};
double upangle = 0;
double sideangle = 0;
double upangle_inc = 10*M_PI/180;
double sideangle_inc = 10*M_PI/180;
double global_time;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

TreeSkeleton tree(4,3);
Wind wind(2,5,2.5);

int TreeWindow, DirectionWindow;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glutSetWindow(TreeWindow);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

    glutSetWindow(DirectionWindow);
    glutPositionWindow(0, height-150);
    glViewport(0, 0, 150, 150);

}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0],eye[1],eye[2],0,0,0,0,1,0);

    //global_time += 1;
    glPushMatrix();
        tree.paint(global_time, wind);
    glPopMatrix();

    glutSwapBuffers();
}

static void xyz()
{
    glColor3f(1,0,0);
    glRasterPos2f(0.8,-0.9);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X');
    glColor3f(0,1,0);
    glRasterPos2f(0.15,-0.9);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Y');
    glColor3f(0,0,1);
    glRasterPos2f(-0.5,-0.9);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Z');
}

static void displaydir(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,1,3);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,-1,0,0,0,0,1,0);
    xyz();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1.0, 1.0, 3.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye[0]/2,eye[1]/2,eye[2]/2,0,0,0,0,1,0);

    glPushMatrix();
        glEnable(GL_COLOR_MATERIAL);
        glBegin(GL_LINES);
            glColor3f(1,0,0);
            glVertex3f(0,0,0);
            glVertex3f(1,0,0);
        glEnd();
        glBegin(GL_LINES);
            glColor3f(0,1,0);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
        glEnd();
        glBegin(GL_LINES);
            glColor3f(0,0,1);
            glVertex3f(0,0,0);
            glVertex3f(0,0,1);
        glEnd();
        glDisable(GL_COLOR_MATERIAL);
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

    glutSetWindow(TreeWindow);
    glutPostRedisplay();
    glutSetWindow(DirectionWindow);
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

void idle(void)
{
    global_time += 1;
    glutSetWindow(TreeWindow);
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT) - glutGet(GLUT_SCREEN_HEIGHT_MM));
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    TreeWindow = glutCreateWindow("Tree Animation");

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

    // glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glDisable(GL_COLOR_MATERIAL);

    DirectionWindow = glutCreateSubWindow(TreeWindow, 0, glutGet(GLUT_WINDOW_HEIGHT)-150, 150, 150);
    glutDisplayFunc(displaydir);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKey);

    glutSetWindow(TreeWindow);

    srand((unsigned) time(NULL));
    tree.makeTree();

    glutMainLoop();

    return EXIT_SUCCESS;
}
